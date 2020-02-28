#include "event_definitions.h"
#include "event_manager.h"
#include "API/Core/Zip/zlib_compression.h"
#include "API/Core/Text/console_logger.h"
#include "CMeasureFactory.h"

namespace clan
{
  bool event_manager::buildEventsAddEntity(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pEntity->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (build_events_long_string(labelEventAddEntity, xmlZipBuffer, pEntity->getId(),
                                     pEntity->getStepCoord().x, pEntity->getStepCoord().y, pEntity->getLayer(), pEntity->getStepDirection(),
                                     eventVector));
  }


  void event_manager::handleEventAddEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    int i;
    int transactionId = e.get_argument(0); // contains entityId
    int nbBlocks = e.get_argument(1);
    int blocksIndex = e.get_argument(2);
    int custom1 = e.get_argument(3);
    int custom2 = e.get_argument(4);
    int custom3 = e.get_argument(5);
    int custom4 = e.get_argument(6);
    DataBuffer xmlZipBufferBlock = e.get_argument(7);

    if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
    {
      log_event("events", "Biotop update full entity: ERROR bad nbBlocks: %1", nbBlocks);
    }
    else if (nbBlocks == 1)
    {
      addEntityWithZipBuffer(xmlZipBufferBlock, transactionId, custom1, custom2, custom3, custom4, pBiotop, setAsRemoteControl);
    }
    else
    {
      int storeIndex = -1;
      // Find current transaction if exists
      for (i = 0; i < (int)m_tEntityBufferEvent.size(); i++)
      {
        if (m_tEntityBufferEvent[i].transactionId == transactionId)
        {
          storeIndex = i;
          break;
        }
      }
      // if context exist, fill context 
      if (storeIndex > -1)
      {
        m_tEntityBufferEvent[storeIndex].nb_blocks_received++;
        m_tEntityBufferEvent[storeIndex].buffer[blocksIndex] = xmlZipBufferBlock;
        if (m_tEntityBufferEvent[storeIndex].nb_blocks_received == m_tEntityBufferEvent[storeIndex].nb_blocks)
        {
          DataBuffer fullXmlZipBuffer;
          int fullXmlZipBufferSize = 0;
          int curBufIndex = 0;
          // Process buffer size
          for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
          {
            fullXmlZipBufferSize += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
          }
          fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
          // Copy blocks in single buffer
          for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
          {
            memcpy(&fullXmlZipBuffer[curBufIndex], &(m_tEntityBufferEvent[storeIndex].buffer[i][0]), m_tEntityBufferEvent[storeIndex].buffer[i].get_size());
            curBufIndex += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
          }
          // Update entity
          addEntityWithZipBuffer(fullXmlZipBuffer, transactionId, custom1, custom2, custom3, custom4, pBiotop, setAsRemoteControl);
          // clean m_tEntityBufferEvent
          m_tEntityBufferEvent.erase(m_tEntityBufferEvent.begin() + storeIndex);
        }
      }
      else // (storeIndex==-1) new context creation needed
      {
        LongBufferEvent_t newBufferEvent;
        newBufferEvent.transactionId = transactionId;
        newBufferEvent.nb_blocks = nbBlocks;
        newBufferEvent.nb_blocks_received = 1;
        newBufferEvent.buffer[blocksIndex] = xmlZipBufferBlock;
        m_tEntityBufferEvent.insert(m_tEntityBufferEvent.end(), newBufferEvent);
      }
    }
  }

  bool event_manager::buildEventsAddCloneEntities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, std::vector<NetGameEvent>& eventVector)
  {
    static constexpr int maxNumEntitiesPerEvent = 500;
    int nbEvents = vectPositions.size() / maxNumEntitiesPerEvent + 1;
    int nbRemainingEntityPos = vectPositions.size();
    int index = 0;
    for (int indexEvent = 0; indexEvent < nbEvents; indexEvent++)
    {
      NetGameEvent newEvent(labelEventAddCloneEntity);
      int nbEntityPosInEvent = cybio_min(maxNumEntitiesPerEvent, nbRemainingEntityPos);
      newEvent.add_argument(nbEntityPosInEvent);
      newEvent.add_argument((int)modelEntityId);
      for (int i = 0; i < nbEntityPosInEvent;  i++)
      {
        BiotopEntityPosition_t entityPos{ vectPositions[index] };
        newEvent.add_argument((int)entityPos.entityId);
        newEvent.add_argument(entityPos.layer);
        newEvent.add_argument(entityPos.stepCoordX);
        newEvent.add_argument(entityPos.stepCoordY);
        newEvent.add_argument(entityPos.stepDirection);
        nbRemainingEntityPos--;
        index++;
      }
      eventVector.push_back(std::move(newEvent));
    }
    return true;
  }
  
  bool event_manager::handleEventAddCloneEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    bool resu = false;
    Point_t stepCoord;
    int nbEntityPosInEvent = e.get_argument(0);
    int modelEntityId = e.get_argument(1);
    int index = 2;
    CBasicEntity* pModelEntity = pBiotop->getEntityById(modelEntityId);
    log_event("events", "handleEventAddCloneEntity nbEntityPosInEvent: %1 modelId=%2", nbEntityPosInEvent, modelEntityId);
    if (pModelEntity == NULL)
    {
      return false;
    }

    for (int i = 0; i < nbEntityPosInEvent; i++)
    {
      int entityId = e.get_argument(index);
      int layer = e.get_argument(index + 1);
      stepCoord.x = e.get_argument(index + 2);
      stepCoord.y = e.get_argument(index + 3);
      int direction = e.get_argument(index + 4);
      index += 5;

      CBasicEntity* pClonedNewEntity = pBiotop->createCloneEntity(pModelEntity);
      pClonedNewEntity->setStepDirection(direction);
      if (setAsRemoteControl)
      {
        if (!pBiotop->addRemoteCtrlEntity(entityId, pClonedNewEntity, stepCoord, layer))
        {
          delete pClonedNewEntity;
          return false;
        }
      }
      else
      {
        if (!pBiotop->addEntity(pClonedNewEntity, CBasicEntity::getGridCoordFromStepCoord(stepCoord), layer))
        {
          delete pClonedNewEntity;
          return false;
        }
      }
    }
  }

   bool event_manager::buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    pEntity->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (build_events_long_string(labelEventUpdateEntityData, xmlZipBuffer, pEntity->getId(),
      pEntity->getStepCoord().x, pEntity->getStepCoord().y, pEntity->getLayer(), pEntity->getStepDirection(),
      eventVector));
  }

   void event_manager::handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
   {
     int i;
     int transactionId = e.get_argument(0); // contains entityId
     int nbBlocks = e.get_argument(1);
     int blocksIndex = e.get_argument(2);
     DataBuffer xmlZipBufferBlock = e.get_argument(7);

     if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
     {
       log_event("events", "Biotop update full entity: ERROR bad nbBlocks: %1", nbBlocks);
     }
     else if (nbBlocks == 1)
     {
       updateEntityWithZipBuffer(xmlZipBufferBlock, transactionId, pBiotop, setAsRemoteControl);
     }
     else
     {
       int storeIndex = -1;
       // Find current transaction if exists
       for (i = 0; i < (int)m_tEntityBufferEvent.size(); i++)
       {
         if (m_tEntityBufferEvent[i].transactionId == transactionId)
         {
           storeIndex = i;
           break;
         }
       }
       // if context exist, fill context 
       if (storeIndex > -1)
       {
         m_tEntityBufferEvent[storeIndex].nb_blocks_received++;
         m_tEntityBufferEvent[storeIndex].buffer[blocksIndex] = xmlZipBufferBlock;
         if (m_tEntityBufferEvent[storeIndex].nb_blocks_received == m_tEntityBufferEvent[storeIndex].nb_blocks)
         {
           DataBuffer fullXmlZipBuffer;
           int fullXmlZipBufferSize = 0;
           int curBufIndex = 0;
           // Process buffer size
           for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
           {
             fullXmlZipBufferSize += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
           }
           fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
           // Copy blocks in single buffer
           for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
           {
             memcpy(&fullXmlZipBuffer[curBufIndex], &(m_tEntityBufferEvent[storeIndex].buffer[i][0]), m_tEntityBufferEvent[storeIndex].buffer[i].get_size());
             curBufIndex += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
           }
           // Update entity
           updateEntityWithZipBuffer(fullXmlZipBuffer, transactionId, pBiotop, setAsRemoteControl);
           // clean m_tEntityBufferEvent
           m_tEntityBufferEvent.erase(m_tEntityBufferEvent.begin() + storeIndex);
         }
       }
       else // (storeIndex==-1) new context creation needed
       {
         LongBufferEvent_t newBufferEvent;
         newBufferEvent.transactionId = transactionId;
         newBufferEvent.nb_blocks = nbBlocks;
         newBufferEvent.nb_blocks_received = 1;
         newBufferEvent.buffer[blocksIndex] = xmlZipBufferBlock;
         m_tEntityBufferEvent.insert(m_tEntityBufferEvent.end(), newBufferEvent);
       }
     }
   }

  NetGameEvent event_manager::buildEventUpdateEntityPos(CBasicEntity* pEntity)
  {
    NetGameEvent newEvent(labelEventUpdateEntityPos);
    newEvent.add_argument((int)pEntity->getId());
    newEvent.add_argument(pEntity->getLabel());
    newEvent.add_argument(pEntity->getStatus());
    newEvent.add_argument(pEntity->getStepCoord().x);
    newEvent.add_argument(pEntity->getStepCoord().y);
    newEvent.add_argument(pEntity->getLayer());
    newEvent.add_argument(pEntity->getStepDirection());
    newEvent.add_argument((int)pEntity->isImmortal());
    // Add parameters
    for (int i = 0; i < pEntity->getNumParameter(); i++)
    {
      newEvent.add_argument((float)pEntity->getParameter(i)->getVal());
    }
    // Add other usefull info
    if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
    {
      CAnimal* pAnimal = (CAnimal*)pEntity;
      newEvent.add_argument(pAnimal->getBrain()->GetCurrentReactionIndex());
      newEvent.add_argument(pAnimal->getBrain()->GetCurrentPurposeIndex());
    }
    return (std::move(newEvent));
  }

  CBasicEntity* event_manager::handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    int entityId = e.get_argument(0);
    std::string  entityLabel = e.get_argument(1);
    int status = e.get_argument(2);
    Point_t position;
    position.x = e.get_argument(3);
    position.y = e.get_argument(4);
    int layer = e.get_argument(5);
    int direction = e.get_argument(6);
    int isImmortal = e.get_argument(7);

    // Check if entity exists
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);
    // If entity does not exist, create one with default parameters
    if (pEntity != NULL)
    {
      float paramValue = 0;
      int index = 8;
      if (pEntity->getLabel() != entityLabel)
        log_event("events", "Biotop update entity position: entityID %1 label mistmatch %2 expected %3", entityId, pEntity->getLabel(), entityLabel);
      pEntity->setStatus((StatusType_e)status);
      pEntity->jumpToStepCoord(position, layer);
      pEntity->setStepDirection(direction);
      pEntity->setImmortal(isImmortal);
      // Update parameters
      for (int i = 0; i < pEntity->getNumParameter(); i++)
      {
        paramValue = e.get_argument(index);
        pEntity->getParameter(i)->forceVal(paramValue);
        index++;
      }
      // Additional infos
      if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
      {
        CAnimal* pAnimal = (CAnimal*)pEntity;
        int reactIndex = e.get_argument(index);
        index++;
        int purposeIndex = e.get_argument(index);
        pAnimal->getBrain()->SetCurrentReactionIndex(reactIndex);
        pAnimal->getBrain()->ForceCurrentPurpose(purposeIndex);
      }
    }
    return pEntity;
  }

	NetGameEvent event_manager::buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId)
	{
    NetGameEvent newEvent(labelEventRemoveEntity);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument(pEntity->getLabel());
    return (std::move(newEvent));
	} 

  bool event_manager::handleEventRemoveEntity(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    std::string  entityLabel = e.get_argument(1);
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);

    if ((pEntity != NULL) && (entityLabel == pEntity->getLabel()))
    {
      log_event("events", "Biotop remove entity: entityID %1 label %2", entityId, pEntity->getLabel());
      pEntity->autoRemove();
      return true;
    }
    else
    {
      log_event("events", "Biotop remove entity: Error entityID %1 label expected %2", entityId, entityLabel);
      return false;
    }
  }

  NetGameEvent event_manager::buildEventChangeBiotopSpeed(const float newBiotopSpeed, const bool isManualMode)
  {
    NetGameEvent newEvent(labelEventChangeBiotopSpeed);
    newEvent.add_argument(newBiotopSpeed);
    newEvent.add_argument((int)isManualMode);
    return (std::move(newEvent));
  }

  bool event_manager::handleEventChangeBiotopSpeed(const NetGameEvent& e, float& newBiotopSpeed, bool& isManualMode)
  {
    newBiotopSpeed = e.get_argument(0);
    int manualMode = e.get_argument(1);
    isManualMode = manualMode;
    log_event("events", "Biotop speed change: %1 isManual%2", newBiotopSpeed, isManualMode);
    return true;
  }

  NetGameEvent event_manager::buildEventForceEntityAction(const entityIdType entityId, const int actionIndex)
  {
    NetGameEvent newEvent(labelEventForceEntityAction);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument(actionIndex);
    return (std::move(newEvent));
  }

  bool event_manager::handleEventForceEntityAction(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    int actionIndex = e.get_argument(1);
    log_event("events", "ForceEntityAction: entity%1 actionIndex%2", entityId, actionIndex);
    return pBiotop->forceEntityAction(entityId, actionIndex);
  }

  bool event_manager::buildEventsCreateMeasure(CMeasure* pMeasure, std::vector<NetGameEvent>& eventVector)
  {
    std::string dataString = pMeasure->buildStringDataFromMeasure();

    // Compress data string
    DataBuffer dataBuffer(dataString.c_str(), dataString.length());
    DataBuffer dataZipBuffer = ZLibCompression::compress(dataBuffer, false);

    int measureTypeSubType = pMeasure->GetType() + (pMeasure->GetSubTypeId() << 8);
    int entityId = -1;
    if (pMeasure->GetEntity() != NULL)
    {
      entityId = pMeasure->GetEntity()->getId();
    }
    return (build_events_long_string(labelEventCreateMeasure, dataZipBuffer, pMeasure->GetId(),
      pMeasure->GetPeriod(), measureTypeSubType, pMeasure->GetParameterIndex(), entityId, eventVector));
  }

  void event_manager::handleEventCreateMeasure(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int i;
    int transactionId = e.get_argument(0); // contains measureId
    int nbBlocks = e.get_argument(1);
    int blocksIndex = e.get_argument(2);
    int custom1 = e.get_argument(3);
    int custom2 = e.get_argument(4);
    int custom3 = e.get_argument(5);
    int custom4 = e.get_argument(6);
    DataBuffer dataZipBufferBlock = e.get_argument(7);

    if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
    {
      log_event("events", "Biotop create measure: ERROR bad nbBlocks: %1", nbBlocks);
    }
    else if (nbBlocks == 1)
    {
      createMeasureWithZipBuffer(dataZipBufferBlock, pBiotop, transactionId, custom1, custom2, custom3, custom4);
    }
    else
    {
      int storeIndex = -1;
      // Find current transaction if exists
      for (i = 0; i < (int)m_tMeasureBufferEvent.size(); i++)
      {
        if (m_tMeasureBufferEvent[i].transactionId == transactionId)
        {
          storeIndex = i;
          break;
        }
      }
      // if context exist, fill context 
      if (storeIndex > -1)
      {
        m_tMeasureBufferEvent[storeIndex].nb_blocks_received++;
        m_tMeasureBufferEvent[storeIndex].buffer[blocksIndex] = dataZipBufferBlock;
        if (m_tMeasureBufferEvent[storeIndex].nb_blocks_received == m_tMeasureBufferEvent[storeIndex].nb_blocks)
        {
          DataBuffer fullXmlZipBuffer;
          int fullXmlZipBufferSize = 0;
          int curBufIndex = 0;
          // Process buffer size
          for (i = 0; i < m_tMeasureBufferEvent[storeIndex].nb_blocks; i++)
          {
            fullXmlZipBufferSize += m_tMeasureBufferEvent[storeIndex].buffer[i].get_size();
          }
          fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
          // Copy blocks in single buffer
          for (i = 0; i < m_tMeasureBufferEvent[storeIndex].nb_blocks; i++)
          {
            memcpy(&fullXmlZipBuffer[curBufIndex], &(m_tMeasureBufferEvent[storeIndex].buffer[i][0]), m_tMeasureBufferEvent[storeIndex].buffer[i].get_size());
            curBufIndex += m_tMeasureBufferEvent[storeIndex].buffer[i].get_size();
          }
          // Create measure
          createMeasureWithZipBuffer(dataZipBufferBlock, pBiotop, transactionId, custom1, custom2, custom3, custom4);
          // clean m_tMeasureBufferEvent
          m_tMeasureBufferEvent.erase(m_tMeasureBufferEvent.begin() + storeIndex);
        }
      }
      else // (storeIndex==-1) new context creation needed
      {
        LongBufferEvent_t newBufferEvent;
        newBufferEvent.transactionId = transactionId;
        newBufferEvent.nb_blocks = nbBlocks;
        newBufferEvent.nb_blocks_received = 1;
        newBufferEvent.buffer[blocksIndex] = dataZipBufferBlock;
        m_tMeasureBufferEvent.insert(m_tMeasureBufferEvent.end(), newBufferEvent);
      }
    }
  }

  NetGameEvent event_manager::buildEventReqEntityRefresh(CBasicEntity* pEntity, entityIdType entityId)
  {
    NetGameEvent newEvent(labelEventReqEntityRefresh);
    newEvent.add_argument((int)entityId);
    newEvent.add_argument(pEntity->getLabel());
    return (std::move(newEvent));
  }

  CBasicEntity* event_manager::handleEventReqEntityRefresh(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int entityId = e.get_argument(0);
    string label = e.get_argument(1);
    log_event("events", "Reqest entity refresh: entity%1 label%2", entityId, label);
    return (pBiotop->getEntityById(entityId));
  }

  bool event_manager::buildEventsAddEntitySpawner(int index, BiotopRandomEntitiyGeneration_t& generator, std::vector<NetGameEvent>& eventVector)
  {
    TiXmlDocument xmlDoc;
    generator.pModelEntity->saveInXmlFile(&xmlDoc);
    TiXmlPrinter xmlPrinter;
    xmlDoc.Accept(&xmlPrinter);
    std::string xmlString = xmlPrinter.Str();

    // Compress xml string
    DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
    DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

    return (build_events_long_string(labelEventAddEntitySpawner, xmlZipBuffer, index,
      generator.intensity, generator.avaragePeriodicity, generator.IsProportionalToFertility, 0,
      eventVector));
  }

  void event_manager::handleEventAddEntitySpawner(const NetGameEvent& e, CBiotop* pBiotop)
  {
    int i;
    int transactionId = e.get_argument(0); // contains spawn index
    int nbBlocks = e.get_argument(1);
    int blocksIndex = e.get_argument(2);
    int intensity = e.get_argument(3);
    int period = e.get_argument(4);
    int isProportional = e.get_argument(5);
    DataBuffer xmlZipBufferBlock = e.get_argument(7);

    if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
    {
      log_event("events", "Biotop Add Entity Spawner: ERROR bad nbBlocks: %1", nbBlocks);
    }
    else if (nbBlocks == 1)
    {
      createSpawnerZipBuffer(xmlZipBufferBlock, pBiotop, transactionId, intensity, period, isProportional);
    }
    else
    {
      int storeIndex = -1;
      // Find current transaction if exists
      for (i = 0; i < (int)m_tEntityBufferEvent.size(); i++)
      {
        if (m_tEntityBufferEvent[i].transactionId == transactionId)
        {
          storeIndex = i;
          break;
        }
      }
      // if context exist, fill context 
      if (storeIndex > -1)
      {
        m_tEntityBufferEvent[storeIndex].nb_blocks_received++;
        m_tEntityBufferEvent[storeIndex].buffer[blocksIndex] = xmlZipBufferBlock;
        if (m_tEntityBufferEvent[storeIndex].nb_blocks_received == m_tEntityBufferEvent[storeIndex].nb_blocks)
        {
          DataBuffer fullXmlZipBuffer;
          int fullXmlZipBufferSize = 0;
          int curBufIndex = 0;
          // Process buffer size
          for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
          {
            fullXmlZipBufferSize += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
          }
          fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
          // Copy blocks in single buffer
          for (i = 0; i < m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
          {
            memcpy(&fullXmlZipBuffer[curBufIndex], &(m_tEntityBufferEvent[storeIndex].buffer[i][0]), m_tEntityBufferEvent[storeIndex].buffer[i].get_size());
            curBufIndex += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
          }
          // Create spawner
          createSpawnerZipBuffer(xmlZipBufferBlock, pBiotop, transactionId, intensity, period, isProportional);
          // clean m_tEntityBufferEvent
          m_tEntityBufferEvent.erase(m_tEntityBufferEvent.begin() + storeIndex);
        }
      }
      else // (storeIndex==-1) new context creation needed
      {
        LongBufferEvent_t newBufferEvent;
        newBufferEvent.transactionId = transactionId;
        newBufferEvent.nb_blocks = nbBlocks;
        newBufferEvent.nb_blocks_received = 1;
        newBufferEvent.buffer[blocksIndex] = xmlZipBufferBlock;
        m_tEntityBufferEvent.insert(m_tEntityBufferEvent.end(), newBufferEvent);
      }
    }
  }

  bool event_manager::build_events_long_string(const std::string event_label, const DataBuffer& data, const int transactionId,
                                               const int custom1, const int custom2, const int custom3, const int custom4,
                                               std::vector<NetGameEvent>& eventVector)
  {
    int nbBlocks = data.get_size() / SENT_BUFFER_MAX_SIZE + 1;
    if (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS)
    {
      log_event("events", "Biotop update full entity: ERROR bad nbBlocks: %1", nbBlocks);
      return false;
    }

    for (int i = 0; i < (nbBlocks - 1); i++)
    {
      NetGameEvent genericEvent(event_label);
      DataBuffer dataBlock(data, i * SENT_BUFFER_MAX_SIZE, SENT_BUFFER_MAX_SIZE);
      genericEvent.add_argument(transactionId);
      genericEvent.add_argument(nbBlocks);
      genericEvent.add_argument(i);
      genericEvent.add_argument(custom1);
      genericEvent.add_argument(custom2);
      genericEvent.add_argument(custom3);
      genericEvent.add_argument(custom4);
      genericEvent.add_argument(dataBlock);
      eventVector.push_back(std::move(genericEvent));
    }

    NetGameEvent genericEvent(event_label);
    DataBuffer dataBlock(data, (nbBlocks - 1) * SENT_BUFFER_MAX_SIZE, (data.get_size() % SENT_BUFFER_MAX_SIZE));
    genericEvent.add_argument(transactionId);
    genericEvent.add_argument(nbBlocks);
    genericEvent.add_argument(nbBlocks - 1);
    genericEvent.add_argument(custom1);
    genericEvent.add_argument(custom2);
    genericEvent.add_argument(custom3);
    genericEvent.add_argument(custom4);
    genericEvent.add_argument(dataBlock);
    eventVector.push_back(std::move(genericEvent));
    return true;
  }

  bool event_manager::addEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, const entityIdType entityId,
                                             const int stepCoordX, const int stepCoordY, const int layer, const int stepDirection,
                                             CBiotop* pBiotop, bool setAsRemoteControl)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    CBasicEntity* pNewEntity = pBiotop->createEntity(&xmlDoc, ".\\temp\\");
    log_event("events", "Biotop add entity: %1 state %2 stepCoordX %3 stepCoordY %4 ID %5", pNewEntity->getLabel(), pNewEntity->getStatus(), stepCoordX, stepCoordY, (int)entityId);

    Point_t stepCoord{ stepCoordX , stepCoordY };

    if (setAsRemoteControl)
    {
      if (!pBiotop->addRemoteCtrlEntity(entityId, pNewEntity, stepCoord, layer))
      {
        delete pNewEntity;
        return false;
      }
    }
    else
    {
      if (!pBiotop->addEntity(pNewEntity, CBasicEntity::getGridCoordFromStepCoord(stepCoord), layer))
      {
        delete pNewEntity;
        return false;
      }
    }
    pNewEntity->setStepDirection(stepDirection);
    return true;
  }


  bool event_manager::updateEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    CBasicEntity* pNewEntity = pBiotop->createEntity(&xmlDoc, ".\\temp\\");
    log_event("events", "Biotop update full entity: %1 state %2", pNewEntity->getLabel(), pNewEntity->getStatus());

    CBasicEntity* pCurEntity;
    bool bFound = false;
    int curStepDirection;

    // Update entity with same Id
    pCurEntity = pBiotop->getEntityById(entityId);
    if (pCurEntity != NULL)
    {
      curStepDirection = pCurEntity->getStepDirection();
      pNewEntity->setRemoteControlled(setAsRemoteControl);
      pNewEntity->setStepDirection(curStepDirection);
      if (!pBiotop->replaceEntityByAnother(pCurEntity->getId(), pNewEntity))
      {
        delete pNewEntity;
      }
      bFound = true;
    }
    return (bFound);
  }

  bool event_manager::createMeasureWithZipBuffer(const DataBuffer& dataZipBuffer, CBiotop* pBiotop, const int measureId,
                                                 const int period, const int typeSubType, const int paramId, const int entityId)
  {
    DataBuffer dataBuffer = ZLibCompression::decompress(dataZipBuffer, false);
    CMeasure* pNewMeasure = CMeasureFactory::createMeasure(measureId, period, typeSubType, pBiotop, paramId, entityId);
    if (pNewMeasure != NULL)
    {
      pNewMeasure->buildMeasureDataFromString(dataBuffer.get_data());
      log_event("events", "Biotop create measure: Id%1 period=%2 label=%3", measureId, period, pNewMeasure->GetLabel());
      pBiotop->replaceMeasure(measureId, pNewMeasure);
    }
    return true;
  }

  bool event_manager::createSpawnerZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop, const int spawnerId,
                                             const int intensity, const int period, const bool isProportional)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());
    CBasicEntity* pNewEntity = pBiotop->createEntity(&xmlDoc, ".\\temp\\");
    if (pNewEntity!=NULL)
    {
      log_event("events", "Add entity spawn: Id%1 period=%2 label=%3", spawnerId, period, pNewEntity->getLabel());
      return (pBiotop->addEntitySpawner(spawnerId, pNewEntity, intensity, period, isProportional));
    }
    else
    {
      log_event("events", "Add entity spawn error: Id%1 period=%2", spawnerId, period);
      return false;
    }
  }

}
