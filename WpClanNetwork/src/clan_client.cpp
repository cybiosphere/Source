
#include "clan_client.h"
#include "custom_type.h"
#include "API/Core/Zip/zlib_compression.h"
#include "CAnimal.h"
#ifdef USE_OGRE3D
#include "CybiOgre3D.h"
#endif
#include "CScenarioPlayer.h"

#define CLIENT_CMD_NUMBER 2
CommandHandler_t ClientCmdNameList[CLIENT_CMD_NUMBER] =
{
  // cmd name                     cmd function                       help string
  { "HELP",                      Client::CmdHelp,                   "HELP" },
  { "DISPLAY_BIOTOP",            Client::CmdDisplayBiotop,          "DISPLAY_BIOTOP" }
};

Client::Client(std::string serverAddr, std::string portId, CybiOgre3DApp* pCybiOgre3DApp)
{
  // Update attributes
  m_ServerAddr = serverAddr;
  m_PortId = portId;

	// Connect essential signals - connecting, disconnecting and receiving events
	cc.connect(network_client.sig_event_received(), clan::bind_member(this, &Client::on_event_received));
	cc.connect(network_client.sig_connected(), clan::bind_member(this, &Client::on_connected));
	cc.connect(network_client.sig_disconnected(), clan::bind_member(this, &Client::on_disconnected));

	// Set up event dispatchers to route incoming events to functions
	login_events.func_event("Login-Success") = clan::bind_member(this, &Client::on_event_login_success);
	login_events.func_event("Login-Fail") = clan::bind_member(this, &Client::on_event_login_fail);
	game_events.func_event("Game-LoadMap") = clan::bind_member(this, &Client::on_event_game_loadmap);
	game_events.func_event("Game-Start") = clan::bind_member(this, &Client::on_event_game_startgame);
  game_events.func_event("Biotop-Next second start") = clan::bind_member(this, &Client::on_event_biotop_nextsecond_start);
  game_events.func_event("Biotop-Next second end") = clan::bind_member(this, &Client::on_event_biotop_nextsecond_end);
  game_events.func_event("Biotop-Update entity data") = clan::bind_member(this, &Client::on_event_biotop_updatefullentity);
  game_events.func_event("Biotop-Update entity position") = clan::bind_member(this, &Client::on_event_biotop_updateentityposition);
  game_events.func_event("Biotop-Remove entity") = clan::bind_member(this, &Client::on_event_biotop_removeentity);

	quit = false;
	logged_in = false;
  m_pBiotop = new CBiotop(200,200,0); // Default empty biotop
  m_bBiotopConfigComplete = false;
  m_bEventNextSecondStart = false;
  m_bEventNextSecondEnd = false;
  m_biotopSpeed = 1.0;
  m_pCybiOgre3DApp = pCybiOgre3DApp;

}

Client::~Client()
{
}

void Client::exec()
{
	log_event("system", "CLIENT started");

  //std::string serverAddr = "192.168.1.14"; //"localhost"; //"192.168.1.67"
  //std::string portId     = "4556";

	connect_to_server();

	while (!quit)
	{
		System::sleep(10);
		network_client.process_events();
    std::string inputcommand;
    bool resu = log_get_console_input(inputcommand);
    if (resu)
    {
      int var1 = -1;
      int var2 = -1;
      log_event("Input command:", inputcommand);
      CScenarioPlayer::ExecuteCmd(m_pBiotop, inputcommand, "C:\\temp", var1, var2, ClientCmdNameList, CLIENT_CMD_NUMBER);
    }
	}
}

// Connect to localhost server
void Client::connect_to_server()
{
	try
	{
    network_client.connect(m_ServerAddr, m_PortId);
	}
	catch(const clan::Exception &e)
	{
		log_event("error", e.message);
	}
}

void Client::disconnect_from_server()
{
	try
	{
		network_client.disconnect();
	}
	catch(const clan::Exception &e)
	{
		log_event("error", e.message);
	}
}

void Client::process_new_events()
{
  network_client.process_events();
}

CBiotop* Client::get_pBiotop()
{
  return m_pBiotop;
}

bool Client::is_biotop_config_complete()
{
  return m_bBiotopConfigComplete;
}

float Client::get_biotop_speed()
{
  return m_biotopSpeed;
}

// Successfully connected to server
void Client::on_connected()
{
	log_event("network", "Connected to server");

	// Properly login
	network_client.send_event(NetGameEvent("Login", "my user name"));
}

// Disconnected from server
void Client::on_disconnected()
{
	log_event("network", "Disconnected from server");
	quit = true;
}

// An event was received from server
void Client::on_event_received(const NetGameEvent &e) 
{
	//log_event("events", "Server sent event: %1", e.to_string());
	bool handled_event = false;
	
	if(!logged_in)
	{
		// We haven't logged in, so route events to login dispatcher
		handled_event = login_events.dispatch(e);
	}
	else
	{
		// We have logged in, so route events to game dispatcher
		handled_event = game_events.dispatch(e);
	}

	if(!handled_event)
	{
		// We received an event which we didn't hook up
		log_event("events", "Unhandled event: %1", e.to_string());
	}
}

// "Login-Success" event was received
void Client::on_event_login_success(const NetGameEvent &e)
{
	log_event("events", "Login success");

	logged_in = true;

	network_client.send_event(NetGameEvent("Game-RequestStart"));
}

// "Login-Fail" event was received
void Client::on_event_login_fail(const NetGameEvent &e)
{
	std::string fail_reason = e.get_argument(0);

	log_event("events", "Login failed: %1", fail_reason);
}

// "Game-LoadMap" event was received
void Client::on_event_game_loadmap(const NetGameEvent &e) 
{
  DataBuffer xmlZipBuffer = e.get_argument(0);
  DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
  TiXmlDocument xmlDoc;
  xmlDoc.Parse(xmlBuffer.get_data());

  m_pBiotop->loadFromXmlFile(&xmlDoc, "");
  log_event("events", "Loading biotop: %1, Size %2,%3,%4", m_pBiotop->getLabel(), m_pBiotop->getDimension().x, m_pBiotop->getDimension().y, m_pBiotop->getNbLayer());
}

// "Game-Start" event was received
void Client::on_event_game_startgame(const NetGameEvent &e) 
{
  displayBiotopEntities();
  m_bBiotopConfigComplete = true;
	log_event("events", "Starting game!");
}

// "Biotop-Next second" event was received
void Client::on_event_biotop_nextsecond_start(const NetGameEvent &e) 
{
  CustomType biotopTime = e.get_argument(0);
  m_biotopSpeed = e.get_argument(1);
  m_bEventNextSecondStart = true;
  m_bEventNextSecondEnd = false;
	log_event("events", "Biotop next second start. Time: %1:%2:%3 day%4", biotopTime.get_y(), biotopTime.get_x()/60, biotopTime.get_x()%60 , biotopTime.get_z());
}

// "Biotop-Next second" event was received
void Client::on_event_biotop_nextsecond_end(const NetGameEvent &e) 
{
  CustomType biotopTime = e.get_argument(0);
  m_bEventNextSecondEnd = true;
  m_lastEventTimeStamp = biotopTime.get_x();
	log_event("events", "Biotop next second end. Time: %1:%2:%3 day%4", biotopTime.get_y(), biotopTime.get_x()/60, biotopTime.get_x()%60 , biotopTime.get_z());

#ifdef USE_OGRE3D
  // Prepare new movement and animation in case of Ogre3D Application
  if (m_pCybiOgre3DApp != NULL)
  {
    m_pCybiOgre3DApp->updateAllMeshEntityNewSecond();
  }
#endif // USE_OGRE3D
}

bool  Client::check_if_event_next_second_start_and_clean()
{
  bool resu = m_bEventNextSecondStart;
  m_bEventNextSecondStart = false;
  return resu;
}

bool  Client::check_if_event_next_second_end_and_clean()
{
  bool resu = m_bEventNextSecondEnd;
  m_bEventNextSecondEnd = false;
  return resu;
}

void Client::on_event_biotop_updatefullentity(const NetGameEvent &e) 
{
  int i;
  int transactionId = e.get_argument(0); // contains entityId
  int nbBlocks      = e.get_argument(1);
  int blocksIndex   = e.get_argument(2);
  DataBuffer xmlZipBufferBlock = e.get_argument(3);

  if ((nbBlocks == 0) || (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS))
  {
    log_event("events", "Biotop update full entity: ERROR bad nbBlocks: %1", nbBlocks);
  }
  else if (nbBlocks == 1)
  {
    updateBiotopWithEntityZipBuffer(xmlZipBufferBlock, transactionId);
  }
  else
  {
    int storeIndex = -1;
    // Find current transaction if exists
    for (i=0; i<(int)m_tEntityBufferEvent.size(); i++)
    {
      if (m_tEntityBufferEvent[i].transactionId == transactionId)
      {
        storeIndex = i;
        break;
      }
    }
    // if context exist, fill context 
    if (storeIndex>-1)
    {
      m_tEntityBufferEvent[storeIndex].nb_blocks_received++;
      m_tEntityBufferEvent[storeIndex].buffer[blocksIndex] = xmlZipBufferBlock;
      if (m_tEntityBufferEvent[storeIndex].nb_blocks_received == m_tEntityBufferEvent[storeIndex].nb_blocks)
      {
        DataBuffer fullXmlZipBuffer;
        int fullXmlZipBufferSize = 0;
        int curBufIndex = 0;
        // Process buffer size
        for (i=0; i<m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
        {
          fullXmlZipBufferSize += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
        }
        fullXmlZipBuffer.set_size(fullXmlZipBufferSize);
        // Copy blocks in single buffer
        for (i=0; i<m_tEntityBufferEvent[storeIndex].nb_blocks; i++)
        {
          memcpy(&fullXmlZipBuffer[curBufIndex], &(m_tEntityBufferEvent[storeIndex].buffer[i][0]), m_tEntityBufferEvent[storeIndex].buffer[i].get_size());
          curBufIndex += m_tEntityBufferEvent[storeIndex].buffer[i].get_size();
        }
        // Update entity
        updateBiotopWithEntityZipBuffer(fullXmlZipBuffer, transactionId);
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

void Client::on_event_biotop_updateentityposition(const NetGameEvent &e) 
{
  int index       = e.get_argument(0);
  int entityId    = e.get_argument(1);
  std::string  entityLabel = e.get_argument(2);
  Point_t position;
  position.x      = e.get_argument(3);
  position.y     = e.get_argument(4);
  int layer      = e.get_argument(5);
  int direction  = e.get_argument(6);
  int speed      = e.get_argument(7);
  float weight   = e.get_argument(8);
  int reactIndex = e.get_argument(9);
  int status     = e.get_argument(10);

  // Check if entity exists
  CBasicEntity* pEntity = m_pBiotop->getEntityById(entityId);

  // If entity does not exist, create one with default parameters
  if (pEntity == NULL)
  {
    pEntity = new CBasicEntity();
    pEntity->setLabel(entityLabel);
    log_event("events", "Biotop add entity position: entityID %1 label %2 ", entityId, pEntity->getLabel());
    if (!m_pBiotop->addRemoteCtrlEntity(entityId, pEntity, position, layer))
    {
      delete pEntity;
      return;
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

#ifdef USE_OGRE3D
  // Prepare new movement and animation in case of Ogre3D Application
  if (m_pCybiOgre3DApp != NULL)
  {
    m_pCybiOgre3DApp->setMeshEntityPreviousPosition(pEntity);
    //m_pCybiOgre3DApp->updateMeshEntityNewSecond(pEntity);
  }
#endif // USE_OGRE3D
}

void Client::on_event_biotop_removeentity(const NetGameEvent &e)
{
  int entityId = e.get_argument(0);
  std::string  entityLabel = e.get_argument(1);
  CBasicEntity* pEntity = m_pBiotop->getEntityById(entityId);

  if ((pEntity!=NULL) && (entityLabel == pEntity->getLabel()))
  {
    log_event("events", "Biotop remove entity: entityID %1 label %2", entityId, pEntity->getLabel());
    pEntity->autoRemove();
  }
  else
  {
    log_event("events", "Biotop remove entity: Error entityID %1 label expected %2", entityId, entityLabel);
  }
}

void Client::updateBiotopWithEntityZipBuffer(DataBuffer xmlZipBuffer, entityIdType entityId)
{
  DataBuffer xmlBuffer = ZLibCompression::decompress(xmlZipBuffer, false);
  TiXmlDocument xmlDoc;
  xmlDoc.Parse(xmlBuffer.get_data());

  CBasicEntity* pNewEntity = m_pBiotop->createEntity(&xmlDoc, ".\\temp\\");
	log_event("events", "Biotop update full entity: %1 state %2", pNewEntity->getLabel(), pNewEntity->getStatus());

  CBasicEntity* pCurEntity;
  bool bFound = false;
  int curStepDirection;

  if (pNewEntity->getClass() < CLASS_ANIMAL_FIRST)
  {
    // Update all entities with same name
    for (int i = 0; i < m_pBiotop->getNbOfEntities(); i++)
    {
      pCurEntity = m_pBiotop->getEntityByIndex(i);
      if (pCurEntity->getLabel() == pNewEntity->getLabel())
      {
        curStepDirection = pCurEntity->getStepDirection();
        CBasicEntity* pClonedNewEntity = m_pBiotop->createCloneEntity(pNewEntity);
        pClonedNewEntity->setRemoteControlled(true);
        pClonedNewEntity->setStepDirection(curStepDirection);
        if (!m_pBiotop->replaceEntityByAnother(pCurEntity->getId(), pClonedNewEntity))
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
    pCurEntity = m_pBiotop->getEntityById(entityId);
    if (pCurEntity != NULL)
    {
      curStepDirection = pCurEntity->getStepDirection();
      CBasicEntity* pClonedNewEntity = m_pBiotop->createCloneEntity(pNewEntity);
      pClonedNewEntity->setRemoteControlled(true);
      pClonedNewEntity->setStepDirection(curStepDirection);
      if (!m_pBiotop->replaceEntityByAnother(pCurEntity->getId(), pClonedNewEntity))
      {
        delete pClonedNewEntity;
      }
      bFound = true;
    }
  }

  // if not found, add it as new entity
  if (bFound == false)
  {
    CBasicEntity* pClonedNewEntity = m_pBiotop->createCloneEntity(pNewEntity);
    if (!m_pBiotop->addRemoteCtrlEntity(pClonedNewEntity->getId(), pClonedNewEntity, pClonedNewEntity->getStepCoord(), pClonedNewEntity->getLayer()))
    {
      delete pClonedNewEntity;
    }
    else
    {
      log_event("events", "Biotop add entity position: entityID %1 label %2 state %3", pClonedNewEntity->getId(), pClonedNewEntity->getLabel(), pClonedNewEntity->getStatus());
    }
  }

  delete (pNewEntity);
}

void Client::displayBiotopEntities()
{
  CBasicEntity* pEntity;
  log_event("INFO", "Biotop display:");
  for (int i=0; i<m_pBiotop->getNbOfEntities(); i++)
  {
    pEntity = m_pBiotop->getEntityByIndex(i);
    log_event("INFO", "%1 pos %2 %3 dir%4", pEntity->getLabel(), pEntity->getGridCoord().x, pEntity->getGridCoord().y, pEntity->getDirection());
  }
}

void Client::displayBiotopEntityDetail(entityIdType entityId)
{

}

bool Client::CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2)
{
  int i;
  for (i = 0; i<CLIENT_CMD_NUMBER; i++)
  {
    log_event("Client cmd", ClientCmdNameList[i].helpString);
  }
  return true;
}

bool Client::CmdDisplayBiotop(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2)
{
  CBasicEntity* pEntity;
  for (int i = 0; i<pBiotop->getNbOfEntities(); i++)
  {
    pEntity = pBiotop->getEntityByIndex(i);
    log_event("INFO", "%1 pos %2 %3 dir%4", pEntity->getLabel(), pEntity->getGridCoord().x, pEntity->getGridCoord().y, pEntity->getDirection());
  }
  return true;
}