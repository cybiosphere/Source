
#include "clan_client.h"
#include "event_definitions.h"
#include "event_manager.h"
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
	login_events.func_event(labelEventLoginOk) = clan::bind_member(this, &Client::on_event_login_success);
	login_events.func_event(labelEventLoginKo) = clan::bind_member(this, &Client::on_event_login_fail);
	game_events.func_event(labelEventLoadMap) = clan::bind_member(this, &Client::on_event_game_loadmap);
	game_events.func_event(labelEventStart) = clan::bind_member(this, &Client::on_event_game_startgame);
  game_events.func_event(labelEventNextSecStart) = clan::bind_member(this, &Client::on_event_biotop_nextsecond_start);
  game_events.func_event(labelEventNextSecEnd) = clan::bind_member(this, &Client::on_event_biotop_nextsecond_end);
  game_events.func_event(labelEventAddEntity) = clan::bind_member(this, &Client::on_event_biotop_addentity);
  game_events.func_event(labelEventAddCloneEntity) = clan::bind_member(this, &Client::on_event_biotop_addcloneentity);
  game_events.func_event(labelEventUpdateEntityData) = clan::bind_member(this, &Client::on_event_biotop_updatefullentity);
  game_events.func_event(labelEventUpdateEntityPos) = clan::bind_member(this, &Client::on_event_biotop_updateentityposition);
  game_events.func_event(labelEventRemoveEntity) = clan::bind_member(this, &Client::on_event_biotop_removeentity);
  game_events.func_event(labelEventCreateMeasure) = clan::bind_member(this, &Client::on_event_biotop_createmeasure);
  game_events.func_event(labelEventAddEntitySpawner) = clan::bind_member(this, &Client::on_event_biotop_addEntitySpawner);
  game_events.func_event(labelEventCreateSpecieMap) = clan::bind_member(this, &Client::on_event_biotop_createspeciemap);

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
	log_event("system", "CLIENT started. Connect to server " + m_ServerAddr + " port:" + m_PortId);

  //std::string serverAddr = "192.168.1.14"; //"localhost"; //"192.168.1.67"
  //std::string portId     = "4556";

	connect_to_server();
  for (int i = 0; i < 1000; i++)
  {
    System::sleep(10);
    network_client.process_events();
  }
  if (logged_in != true)
  {
    log_event("Server connection:", "FAILED");
    System::sleep(1000);
    return;
  }

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

// Connect to server
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

bool Client::is_logged_in()
{
  return logged_in;
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
	network_client.send_event(NetGameEvent(labelEventLogin, "my user name"));
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
  m_EventManager.handleEventCreateBiotop(e, m_pBiotop);
}

// "Game-Start" event was received
void Client::on_event_game_startgame(const NetGameEvent &e) 
{
  //displayBiotopEntities();
  m_bBiotopConfigComplete = true;
	log_event("events", "Starting game!");
}

// "Biotop-Next second" event was received
void Client::on_event_biotop_nextsecond_start(const NetGameEvent &e) 
{
  CustomType biotopTime = e.get_argument(0);
  m_biotopSpeed = e.get_argument(1);
  float sunlight = e.get_argument(2);
  float fertility = e.get_argument(3);
  float temperature = e.get_argument(4);
  m_bEventNextSecondStart = true;
  m_bEventNextSecondEnd = false;
  m_pBiotop->getParamSunlight()->forceVal(sunlight);
  m_pBiotop->getParamFertility()->forceVal(fertility);
  m_pBiotop->getParamTemperature()->forceVal(temperature);
	//log_event("events", "Biotop next second start. Time: %1:%2:%3 day%4", biotopTime.get_y(), biotopTime.get_x()/60, biotopTime.get_x()%60 , biotopTime.get_z());
}

// "Biotop-Next second" event was received
void Client::on_event_biotop_nextsecond_end(const NetGameEvent &e) 
{
  CustomType biotopTime = e.get_argument(0);
  m_bEventNextSecondEnd = true;
  m_lastEventTimeStamp = biotopTime.get_x();
	//log_event("events", "Biotop next second end. Time: %1:%2:%3 day%4", biotopTime.get_y(), biotopTime.get_x()/60, biotopTime.get_x()%60 , biotopTime.get_z());
  m_pBiotop->setBiotopTime(biotopTime.get_x(), biotopTime.get_y(), biotopTime.get_z(), 0);  //TODO: missing year
  m_pBiotop->triggerMeasuresNextSecond();

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

void Client::on_event_biotop_addentity(const NetGameEvent& e)
{
  m_EventManager.handleEventAddEntity(e, m_pBiotop, true);
}

void Client::on_event_biotop_addcloneentity(const NetGameEvent& e)
{
  m_EventManager.handleEventAddCloneEntity(e, m_pBiotop, true);
}

void Client::on_event_biotop_updatefullentity(const NetGameEvent &e) 
{
  m_EventManager.handleEvenUpdateEntityData(e, m_pBiotop, true);
}

void Client::on_event_biotop_updateentityposition(const NetGameEvent &e) 
{
  CBasicEntity* pEntity = event_manager::handleEventUpdateEntityPosition(e, m_pBiotop, true);

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
  event_manager::handleEventRemoveEntity(e, m_pBiotop);
}

void Client::on_event_biotop_createmeasure(const NetGameEvent& e)
{
  m_EventManager.handleEventCreateMeasure(e, m_pBiotop);
}

void Client::on_event_biotop_addEntitySpawner(const NetGameEvent& e)
{
  m_EventManager.handleEventAddEntitySpawner(e, m_pBiotop);
}

void Client::on_event_biotop_createspeciemap(const NetGameEvent& e)
{
  m_EventManager.handleEventCreateGeoMapSpecie(e, m_pBiotop);
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
    log_event("INFO", "%1 pos %2 %3 dir%4 ID=%5", pEntity->getLabel(), pEntity->getGridCoord().x, pEntity->getGridCoord().y, pEntity->getDirection(), (int)(pEntity->getId()));
  }
  return true;
}

void Client::send_event_add_entity(CBasicEntity* pEntity)
{
  if (pEntity == NULL)
  {
    log_event("Events  ", "Add entity: NULL entity");
    return;
  }
  if (pEntity->isToBeRemoved())
  {
    log_event("Events  ", "Add removed entity: %1", pEntity->getLabel());
    return;
  }

  log_event("Events  ", "Add entity: %1 grid coordX %2 coordY %3", pEntity->getLabel(), pEntity->getGridCoord().x, pEntity->getGridCoord().y);
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsAddEntity(pEntity, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_update_entity_data: Event not sent");
  }
}

void Client::send_events_add_clone_entities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions)
{
  //log_event("Events  ", "Add clone entities");
  std::vector<NetGameEvent> eventVector;
  if (event_manager::event_manager::buildEventsAddCloneEntities(modelEntityId, vectPositions, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_events_add_clone_entities: Event not sent");
  }
}

void Client::send_event_update_entity_data(CBasicEntity* pEntity)
{
  if (pEntity == NULL)
  {
    log_event("Events  ", "Update entity data: NULL entity");
    return;
  }
  if (pEntity->isToBeRemoved())
  {
    log_event("Events  ", "Update removed entity: %1", pEntity->getLabel());
    return;
  }

  log_event("Events  ", "Update entity data: %1", pEntity->getLabel());

  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsUpdateEntityData(pEntity, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_update_entity_data: Event not sent");
  }
}

void Client::send_event_update_entity_position(CBasicEntity* pEntity)
{
  if (pEntity == NULL)
  {
    log_event("Events  ", "Update entity position: NULL");
    return;
  }
  if (pEntity->isToBeRemoved())
  {
    log_event("Events  ", "Update entity position: removed");
    return;
  }
  NetGameEvent bioUpdateEntityPosEvent{ event_manager::buildEventUpdateEntityPos(pEntity) };
  network_client.send_event(bioUpdateEntityPosEvent);
}

void Client::send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId)
{
  log_event("Events  ", "Remove entity: %1", pEntity->getLabel());
  NetGameEvent bioRemoveEntityEvent{ event_manager::buildEventRemoveEntity(pEntity, entityId) };
  network_client.send_event(bioRemoveEntityEvent);
}

void Client::send_event_change_biotop_speed(const float newBiotopSpeed, const bool isManualMode)
{
  log_event("Events  ", "Change biotop speed: %1 manualMode: %2", newBiotopSpeed, isManualMode);
  m_biotopSpeed = newBiotopSpeed;
  NetGameEvent bioChangeSpeedEvent{ event_manager::buildEventChangeBiotopSpeed(newBiotopSpeed, isManualMode) };
  network_client.send_event(bioChangeSpeedEvent);
}

void Client::send_event_force_entity_action(const entityIdType entityId, const int actionIndex)
{
  log_event("Events  ", "Force entity action: Id %1 reaction: %2", (int)entityId, actionIndex);
  NetGameEvent bioForceActionEvent{ event_manager::buildEventForceEntityAction(entityId, actionIndex) };
  network_client.send_event(bioForceActionEvent);
}

void Client::send_event_create_measure(CMeasure* pMeasure)
{
  if (pMeasure == NULL)
  {
    log_event("Events  ", "Create measure: NULL measure");
    return;
  }

  log_event("Events  ", "Create measure: %1", pMeasure->GetLabel());
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsCreateMeasure(pMeasure, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_create_measure: Event not sent");
  }
}

void Client::send_event_request_entity_refresh(CBasicEntity* pEntity, const entityIdType entityId)
{
  log_event("Events  ", "Request entity refresh: Id %1 label: %2", (int)entityId, pEntity->getLabel());
  NetGameEvent bioReqActionRefresh{ event_manager::buildEventReqEntityRefresh(pEntity, entityId) };
  network_client.send_event(bioReqActionRefresh);
}

void Client::send_event_add_entity_spawner(int index, BiotopRandomEntitiyGeneration_t& generator)
{
  if (generator.pModelEntity == NULL)
  {
    log_event("Events  ", "Add entity spwaner: NULL entity");
    return;
  }

  log_event("Events  ", "Add entity spwaner: %1", generator.pModelEntity->getLabel());
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsAddEntitySpawner(index, generator, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_add_entity_spawner: Event not sent");
  }
}

void Client::send_event_create_specie_map(CGeoMapPopulation* pGeoMapSpecie)
{
  if (pGeoMapSpecie == NULL)
  {
    log_event("Events  ", "Create specie Map : NULL map");
    return;
  }

  log_event("Events  ", "Create map specie: %1", pGeoMapSpecie->GetSpecieName());
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsCreateGeoMapSpecie(pGeoMapSpecie, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      network_client.send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_create_specie_map: Event not sent");
  }
}