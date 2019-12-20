#include "event_definitions.h"
#include "event_manager.h"
#include "API/Core/Zip/zlib_compression.h"
#include "API/Core/Text/console_logger.h"


namespace clan
{
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

    return (build_events_long_string(labelEventUpdateEntityData, xmlZipBuffer, pEntity->getId(), eventVector));
  }

   void event_manager::handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
   {
     int i;
     int transactionId = e.get_argument(0); // contains entityId
     int nbBlocks = e.get_argument(1);
     int blocksIndex = e.get_argument(2);
     DataBuffer xmlZipBufferBlock = e.get_argument(3);

     if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
     {
       log_event("events", "Biotop update full entity: ERROR bad nbBlocks: %1", nbBlocks);
     }
     else if (nbBlocks == 1)
     {
       updateBiotopWithEntityZipBuffer(xmlZipBufferBlock, transactionId, pBiotop, setAsRemoteControl);
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
           updateBiotopWithEntityZipBuffer(fullXmlZipBuffer, transactionId, pBiotop, setAsRemoteControl);
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
    int reactionIndex = 0;
    if (pEntity->getBrain() != NULL)
      reactionIndex = pEntity->getBrain()->GetCurrentReactionIndex();
    newEvent.add_argument(pEntity->getBiotop()->getBiotopTime().seconds);
    newEvent.add_argument(pEntity->getId());
    newEvent.add_argument(pEntity->getLabel());
    newEvent.add_argument(pEntity->getStepCoord().x);
    newEvent.add_argument(pEntity->getStepCoord().y);
    newEvent.add_argument(pEntity->getLayer());
    newEvent.add_argument(pEntity->getStepDirection());
    newEvent.add_argument(pEntity->getCurrentSpeed());
    newEvent.add_argument(float(pEntity->getWeight()));
    newEvent.add_argument(reactionIndex);
    newEvent.add_argument(pEntity->getStatus());
    return (std::move(newEvent));
  }

  CBasicEntity* event_manager::handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    int index = e.get_argument(0);
    int entityId = e.get_argument(1);
    std::string  entityLabel = e.get_argument(2);
    Point_t position;
    position.x = e.get_argument(3);
    position.y = e.get_argument(4);
    int layer = e.get_argument(5);
    int direction = e.get_argument(6);
    int speed = e.get_argument(7);
    float weight = e.get_argument(8);
    int reactIndex = e.get_argument(9);
    int status = e.get_argument(10);

    // Check if entity exists
    CBasicEntity* pEntity = pBiotop->getEntityById(entityId);

    // If entity does not exist, create one with default parameters
    if (pEntity == NULL)
    {
      pEntity = new CBasicEntity();
      pEntity->setLabel(entityLabel);
      log_event("events", "Biotop add entity position: entityID %1 label %2 ", entityId, pEntity->getLabel());
      if (setAsRemoteControl)
      {
        if (!pBiotop->addRemoteCtrlEntity(entityId, pEntity, position, layer))
        {
          delete pEntity;
          return NULL;
        }
      }
      else
      {
        if (!pBiotop->addEntity(pEntity, position, layer))
        {
          delete pEntity;
          return NULL;
        }
      }
    }
    else // If entity exists, check name and update position
    {
      if (pEntity->getLabel() != entityLabel)
        log_event("events", "Biotop update entity position: entityID %1 label mistmatch %2 expected %3", entityId, pEntity->getLabel(), entityLabel);
      pEntity->forceWeight(weight);
      pEntity->setStatus((StatusType_e)status);
      if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
      {
        CAnimal* pAnimal = (CAnimal*)pEntity;
        pAnimal->forceCurrentSpeed(speed);
        pAnimal->getBrain()->SetCurrentReactionIndex(reactIndex);

        log_event("events", "Biotop update entity position: %1 action:%2 coord old x=%3 y=%4 new x=%5 y=%6", pEntity->getLabel(),
          pAnimal->getBrain()->GetReactionByIndex(reactIndex)->GetLabel(),
          pEntity->getStepCoord().x, pEntity->getStepCoord().y,
          position.x, position.y);
      }
    }

    pEntity->jumpToStepCoord(position, layer);
    pEntity->setStepDirection(direction);
    return pEntity;
  }

	NetGameEvent event_manager::buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId)
	{
    NetGameEvent newEvent(labelEventRemoveEntity);
    newEvent.add_argument(entityId);
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

  bool event_manager::build_events_long_string(const std::string event_label, DataBuffer data, int transactionId, std::vector<NetGameEvent>& eventVector)
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
      genericEvent.add_argument(dataBlock);
      eventVector.push_back(std::move(genericEvent));
    }

    NetGameEvent genericEvent(event_label);
    DataBuffer dataBlock(data, (nbBlocks - 1) * SENT_BUFFER_MAX_SIZE, (data.get_size() % SENT_BUFFER_MAX_SIZE));
    genericEvent.add_argument(transactionId);
    genericEvent.add_argument(nbBlocks);
    genericEvent.add_argument(nbBlocks - 1);
    genericEvent.add_argument(dataBlock);
    eventVector.push_back(std::move(genericEvent));
    return true;
  }

  void event_manager::updateBiotopWithEntityZipBuffer(DataBuffer xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop, bool setAsRemoteControl)
  {
    DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuffer.get_data());

    CBasicEntity* pNewEntity = pBiotop->createEntity(&xmlDoc, ".\\temp\\");
    log_event("events", "Biotop update full entity: %1 state %2", pNewEntity->getLabel(), pNewEntity->getStatus());

    CBasicEntity* pCurEntity;
    bool bFound = false;
    int curStepDirection;

    if (pNewEntity->getClass() < CLASS_ANIMAL_FIRST)
    {
      // Update all entities with same name
      for (int i = 0; i < pBiotop->getNbOfEntities(); i++)
      {
        pCurEntity = pBiotop->getEntityByIndex(i);
        if (pCurEntity->getLabel() == pNewEntity->getLabel())
        {
          curStepDirection = pCurEntity->getStepDirection();
          CBasicEntity* pClonedNewEntity = pBiotop->createCloneEntity(pNewEntity);
          pClonedNewEntity->setRemoteControlled(setAsRemoteControl);
          pClonedNewEntity->setStepDirection(curStepDirection);
          if (!pBiotop->replaceEntityByAnother(pCurEntity->getId(), pClonedNewEntity))
          {
            delete pClonedNewEntity;
          }
          bFound = true;
        }
      }
    }
    else
    {
      // Update entity with same Id
      pCurEntity = pBiotop->getEntityById(entityId);
      if (pCurEntity != NULL)
      {
        curStepDirection = pCurEntity->getStepDirection();
        CBasicEntity* pClonedNewEntity = pBiotop->createCloneEntity(pNewEntity);
        pClonedNewEntity->setRemoteControlled(setAsRemoteControl);
        pClonedNewEntity->setStepDirection(curStepDirection);
        if (!pBiotop->replaceEntityByAnother(pCurEntity->getId(), pClonedNewEntity))
        {
          delete pClonedNewEntity;
        }
        bFound = true;
      }
    }
  }
}
