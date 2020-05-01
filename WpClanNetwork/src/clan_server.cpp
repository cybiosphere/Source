#include "clan_server.h"
#include "clan_server_user.h"
#include "event_definitions.h"
#include "custom_type.h"
#include "API/Core/Zip/zlib_compression.h"
#include <chrono>
#include "CScenarioPlayer.h"

#define SERVER_CMD_NUMBER 4

CommandHandler_t ServerCmdNameList[SERVER_CMD_NUMBER] =
{
// cmd name                     cmd function                       help string
  {"HELP",                      Server::CmdHelp,                   "HELP"}, 
  {"STOP_BIOTOP",               Server::CmdStopBiotop,             "STOP_BIOTOP"}, 
  {"START_BIOTOP",              Server::CmdStartBiotop,            "START_BIOTOP"}, 
  {"SET_BIOTOP_SPEED",          Server::CmdSetBiotopSpeed,         "SET_BIOTOP_SPEED <speed int>"}
};


Server::Server(CBiotop* pBiotop)
: next_user_id(1)
{
	// Connect essential signals - connecting, disconnecting and receiving events
	cc.connect(network_server.sig_client_connected(), clan::bind_member(this, &Server::on_client_connected));
	cc.connect(network_server.sig_client_disconnected(), clan::bind_member(this, &Server::on_client_disconnected));
	cc.connect(network_server.sig_event_received(), clan::bind_member(this, &Server::on_event_received));

	// Set up event dispatchers to route incoming events to functions
	login_events.func_event(labelEventLogin) = clan::bind_member(this, &Server::on_event_login);
	game_events.func_event(labelEventRequestStart) = clan::bind_member(this, &Server::on_event_game_requeststart);
  game_events.func_event(labelEventAddEntity) = clan::bind_member(this, &Server::on_event_biotop_addentity);
  game_events.func_event(labelEventAddCloneEntity) = clan::bind_member(this, &Server::on_event_biotop_addcloneentity);
  game_events.func_event(labelEventUpdateEntityData) = clan::bind_member(this, &Server::on_event_biotop_updatefullentity);
  game_events.func_event(labelEventUpdateEntityPos) = clan::bind_member(this, &Server::on_event_biotop_updateentityposition);
  game_events.func_event(labelEventRemoveEntity) = clan::bind_member(this, &Server::on_event_biotop_removeentity);
  game_events.func_event(labelEventChangeBiotopSpeed) = clan::bind_member(this, &Server::on_event_biotop_changespeed);
  game_events.func_event(labelEventForceEntityAction) = clan::bind_member(this, &Server::on_event_biotop_forceentityaction);
  game_events.func_event(labelEventCreateMeasure) = clan::bind_member(this, &Server::on_event_biotop_createmeasure);
  game_events.func_event(labelEventReqEntityRefresh) = clan::bind_member(this, &Server::on_event_biotop_requestentityrefresh);
  game_events.func_event(labelEventAddEntitySpawner) = clan::bind_member(this, &Server::on_event_biotop_addEntitySpawner);
  game_events.func_event(labelEventCreateSpecieMap) = clan::bind_member(this, &Server::on_event_biotop_createspeciemap);

  nb_users_connected = 0;
  m_pBiotop = pBiotop;
  m_biotopSpeed = 1.0;
  m_bManualMode = false;
}

Server::~Server()
{
}

void Server::startServer()
{
  network_server.start("4556");
  log_event("System  ", "SERVER started");
}

void Server::ProcessEvents(bool isNewSec, float biotopSpeed)
{
  m_biotopSpeed = biotopSpeed;
  network_server.process_events();
  if (isNewSec)
  {
	// Update clients with biotop evolution
	  if (nb_users_connected > 0)
	  {
      // Send event next second start update
      NetGameEvent bioNextSecEventStart(labelEventNextSecStart);
      CustomType biotopTime(m_pBiotop->getBiotopTime().seconds, m_pBiotop->getBiotopTime().hours, m_pBiotop->getBiotopTime().days);
      bioNextSecEventStart.add_argument(biotopTime);
      bioNextSecEventStart.add_argument(m_biotopSpeed);
      bioNextSecEventStart.add_argument((float)m_pBiotop->getSunlight());
      bioNextSecEventStart.add_argument((float)m_pBiotop->getParamFertility()->getVal());
      bioNextSecEventStart.add_argument((float)m_pBiotop->getParamTemperature()->getVal());
      network_server.send_event(bioNextSecEventStart);

      // Update all entities
      BiotopEvent_t bioEvent;
      for (int i = 0; i<m_pBiotop->getNbOfBiotopEvents(); i++)
      {
        bioEvent = m_pBiotop->getBiotopEvent(i);
        switch (bioEvent.eventType)
        {
        //case BIOTOP_EVENT_ENTITY_MOVED:  // Move is reserved for studio 
        case BIOTOP_EVENT_ENTITY_CHANGED:  // Include generic move
          send_event_update_entity_position(bioEvent.pEntity);
          break;
        case BIOTOP_EVENT_ENTITY_MODIFIED:
          send_event_update_entity_data(bioEvent.pEntity);
          break;
        case BIOTOP_EVENT_ENTITY_ADDED:
          send_event_add_entity(bioEvent.pEntity);
          break;
        case BIOTOP_EVENT_ENTITY_REMOVED:
          send_event_remove_entity(bioEvent.pEntity, bioEvent.entityId);
          break;
        default:
          break;
        }
      }
      m_pBiotop->resetBiotopEvents();

		  // Send event next second end
		  NetGameEvent bioNextSecEventEnd(labelEventNextSecEnd);
		  CustomType biotopTimeEnd(m_pBiotop->getBiotopTime().seconds, m_pBiotop->getBiotopTime().hours, m_pBiotop->getBiotopTime().days);
      //TODO: missing years
		  bioNextSecEventEnd.add_argument(biotopTimeEnd);
		  network_server.send_event(bioNextSecEventEnd);
	  }
    else
    {
      m_pBiotop->resetBiotopEvents();
    }
  }
}

// Server main loop
void Server::exec()
{
  int timeCount = 0;
  std::chrono::time_point<std::chrono::system_clock> curTick;
  std::chrono::time_point<std::chrono::system_clock> lastRunTick = std::chrono::system_clock::now();

	network_server.start("4556");

	log_event("System  ", "SERVER started");
	while (true)
	{
		// Lets not worry about exiting this function!
		System::sleep(10);
		network_server.process_events();

    // Run biotop every sec
    //
    timeCount += (int)round(10.0 * m_biotopSpeed);
    curTick = std::chrono::system_clock::now();
    std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTick - lastRunTick);

    if ((elapsedTime.count() * m_biotopSpeed) >= 1000)
    {
      if (!m_bManualMode)
      {
        // Next second in biotop
        m_pBiotop->nextSecond();
      }
      // Reset timer
      timeCount = 0;
      lastRunTick = std::chrono::system_clock::now();

      // Update clients with biotop evolution
      ProcessEvents(true, m_biotopSpeed);
    }
    else
    {
      std::string inputcommand;
      bool resu = log_get_console_input(inputcommand);
      if (resu)
      {
        log_event("User cmd", inputcommand);
        process_cmd_line(inputcommand);
      }
    }
	}

	network_server.stop();
}

float Server::get_biotop_speed()
{
  return m_biotopSpeed;
}

bool Server::get_manual_mode()
{
  return m_bManualMode;
}

void Server::set_manual_mode(bool newManualMode)
{
  m_bManualMode = newManualMode;
}

bool Server::process_cmd_line(const std::string input_cmd_string)
{
  int var1=-1;
  int var2=-1;
  bool resu;

  if (input_cmd_string == "")
    return false;

  // Check command for biotop
  resu = CScenarioPlayer::ExecuteCmd(m_pBiotop, input_cmd_string, "C:\\temp", var1, var2);
  if (resu == false)
  {
    // Check command for server
    resu = CScenarioPlayer::ExecuteCmd(m_pBiotop, input_cmd_string, "C:\\temp", var1, var2, ServerCmdNameList, SERVER_CMD_NUMBER);
    if (resu == true)
    {
      // Var1 is used to set speed
      if (var1>=0)
      {
        m_biotopSpeed = (float)var1;
        log_event("User cmd", "Biotop speed set to %1", var1);
      }
    }
    else
    {
      log_event("User cmd", "Invalid command");
    }
  }
    
  return resu;
}


// A new client is connecting
void Server::on_client_connected(NetGameConnection *connection)
{
	log_event("Network ", "Client connected");

	// Create user and attach it to connection
	ServerUser *user = new ServerUser();
	user->attach_to_connection(connection);
}

// A client disconnected
void Server::on_client_disconnected(NetGameConnection *connection, const std::string &message)
{
	log_event("Network ", "Client disconnected");

	ServerUser *user = ServerUser::get_user(connection);
	if(user)
		delete user;

  if (nb_users_connected > 0)
    nb_users_connected--;

}

// An event was received from a client
void Server::on_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	//log_event("Events  ", "Client sent event: %1", e.to_string());
	ServerUser *user = ServerUser::get_user(connection);
	if(user)
	{
		bool handled_event = false;

		if (user->id == 0)	// User has not logged in, so route events to login dispatcher
		{
			// Route login events
			handled_event = login_events.dispatch(e, user);
		}
		else
		{
			// Route game events
			handled_event = game_events.dispatch(e, user);
		}

		if (!handled_event)
		{
			// We received an event which we didn't hook up
			log_event("Events  ", "Unhandled event: %1", e.to_string());
		}
	}
}

// "Login" event was received
void Server::on_event_login(const NetGameEvent &e, ServerUser *user)
{
	log_event("Events  ", "Client requested login");

	std::string user_name = e.get_argument(0);

	if(user_name.length() == 0)
	{
		user->send_event(NetGameEvent(labelEventLoginKo,  "Missing user name"));
	}
	else
	{
		// Assign name and id to User object (created when user connected earlier)
		user->user_name = user_name;
		user->id = next_user_id++;

		user->send_event(NetGameEvent(labelEventLoginOk));
	}
}

// "Game-RequestStartGame" event was received
void Server::on_event_game_requeststart(const NetGameEvent &e, ServerUser *user)
{
	log_event("Events  ", "Client requested game start");
  nb_users_connected++;

  // Freeze biotop time during new player configuration
  double biotopSpeed = m_biotopSpeed;
  m_biotopSpeed = 0;
  int i;

  TiXmlDocument xmlDoc;
  m_pBiotop->saveInXmlFile(&xmlDoc,"",false);
  TiXmlPrinter xmlPrinter;
  xmlDoc.Accept(&xmlPrinter);
  std::string xmlString = xmlPrinter.Str();

  // Compress xml string
  DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
  DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

  if (xmlZipBuffer.get_size() > SENT_BUFFER_MAX_SIZE)
  {
    log_event("-ERROR- ", "Client requested game error: Biotop to big to be sent");
  }
  else
  {
    // Send empty map without entities
	  NetGameEvent loadMapEvent(labelEventLoadMap);
    loadMapEvent.add_argument(xmlZipBuffer);
	  user->send_event(loadMapEvent);

    // Update entitiy data. Send info only 1 time if several entities has same name
    std::map<entityIdType, std::vector<BiotopEntityPosition_t>> cloneEntitiesMap;
    CBasicEntity* pCurEntity = NULL;
    std::string prevEntityLabel = "";
    int prevEntityId = -1;
    cloneEntitiesMap.clear();
    for (i=0; i<m_pBiotop->getNbOfEntities(); i++)
    {
      pCurEntity = m_pBiotop->getEntityByIndex(i);
      if (pCurEntity->getLabel() != prevEntityLabel)
      {
        // Send only to new user
        send_event_add_entity(pCurEntity, user);
        prevEntityLabel = pCurEntity->getLabel();
        prevEntityId = pCurEntity->getId();
      }
      else
      {
        // Store in clone entity map
        BiotopEntityPosition_t entityPos{ pCurEntity->getId(),
                                          pCurEntity->getLayer(), 
                                          pCurEntity->getStepCoord().x, 
                                          pCurEntity->getStepCoord().y,
                                          pCurEntity->getStepDirection() };
        cloneEntitiesMap[prevEntityId].push_back(entityPos);
      }
    }

    for (auto cloneEntity : cloneEntitiesMap)
    {
      send_events_add_clone_entities(cloneEntity.first, cloneEntity.second, user);
    }

    for (i = 0; i < m_pBiotop->getNbOfMeasures(); i++)
    {
      send_event_create_measure(m_pBiotop->getMeasureByIndex(i), user);
    }

    for (i = 0; i < m_pBiotop->getNumberOfRandomEntitiyGeneration(); i++)
    {
      send_event_add_entity_spawner(i, m_pBiotop->getRandomEntitiyGeneration(i), user);
    }

    for (i = 0; i < m_pBiotop->getNbOfGeoMapSpecie(); i++)
    {
      send_event_create_specie_map(m_pBiotop->getGeoMapSpecieByIndex(i), user);
    }

	  user->send_event(NetGameEvent(labelEventStart));
  }

  // Restore biotop time
  m_biotopSpeed = biotopSpeed;
}

void Server::on_event_biotop_addentity(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventAddEntity(e, m_pBiotop, false);
}

void Server::on_event_biotop_addcloneentity(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventAddCloneEntity(e, m_pBiotop, false);
}

void Server::on_event_biotop_updatefullentity(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEvenUpdateEntityData(e, m_pBiotop, false);
}

void Server::on_event_biotop_updateentityposition(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventUpdateEntityPosition(e, m_pBiotop, false);
}

void Server::on_event_biotop_removeentity(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventRemoveEntity(e, m_pBiotop);
}

void Server::on_event_biotop_changespeed(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventChangeBiotopSpeed(e, m_biotopSpeed, m_bManualMode);
}

void Server::on_event_biotop_forceentityaction(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventForceEntityAction(e, m_pBiotop);
}

void Server::on_event_biotop_createmeasure(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventCreateMeasure(e, m_pBiotop);
}

void Server::on_event_biotop_addEntitySpawner(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventAddEntitySpawner(e, m_pBiotop);
}

void Server::on_event_biotop_requestentityrefresh(const NetGameEvent& e, ServerUser* user)
{
  CBasicEntity* pEntity = event_manager::handleEventReqEntityRefresh(e, m_pBiotop);
  send_event_update_entity_data(pEntity, user);
}

void Server::on_event_biotop_createspeciemap(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventCreateGeoMapSpecie(e, m_pBiotop);
}

void Server::send_event_add_entity(CBasicEntity* pEntity, ServerUser* user)
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

  log_event("Events  ", "Add entity: %1", pEntity->getLabel());
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsAddEntity(pEntity, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_add_entity: Event not sent");
  }
}

void Server::send_events_add_clone_entities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, ServerUser* user)
{
  //log_event("Events  ", "Add clone entites");
  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsAddCloneEntities(modelEntityId, vectPositions, eventVector))
  {
    for (NetGameEvent eventToSend : eventVector)
    {
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_events_add_clone_entities: Event not sent");
  }
}

void Server::send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user)
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
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_update_entity_data: Event not sent");
  }
}

void Server::send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user)
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
  NetGameEvent bioUpdateEntityPosEvent { event_manager::buildEventUpdateEntityPos(pEntity) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioUpdateEntityPosEvent);
  else
    user->send_event(bioUpdateEntityPosEvent);
}

void Server::send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId, ServerUser *user)
{
  log_event("Events  ", "Remove entity: %1", pEntity->getLabel());
  NetGameEvent bioRemoveEntityEvent{ event_manager::buildEventRemoveEntity(pEntity, entityId) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioRemoveEntityEvent);
  else
    user->send_event(bioRemoveEntityEvent);
}

void Server::send_event_create_measure(CMeasure* pMeasure, ServerUser* user)
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
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_create_measure: Event not sent");
  }
}

void Server::send_event_add_entity_spawner(int index, BiotopRandomEntitiyGeneration_t& generator, ServerUser* user)
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
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_add_entity_spawner: Event not sent");
  }
}

void Server::send_event_create_specie_map(CGeoMapPopulation* pGeoMapSpecie, ServerUser* user)
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
      if (user == NULL) // If user not define, broadcast info to all
        network_server.send_event(eventToSend);
      else
        user->send_event(eventToSend);
    }
  }
  else
  {
    log_event("-ERROR- ", "send_event_create_specie_map: Event not sent");
  }
}


bool Server::CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2)
{
  int i;
  for (i=0; i<SERVER_CMD_NUMBER; i++)
  {
    log_event("Server cmd", ServerCmdNameList[i].helpString);
  }
  string bioCmdStr = "";
  for (i=0; i<100; i++)
  {
    bioCmdStr = CScenarioPlayer::GetHelpCmdString(i);
    if (bioCmdStr != "")
    {
      log_event("Biotop cmd", bioCmdStr);
    }
  }
  return true;
}

bool Server::CmdStopBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused)
{
  *pBiotopSpeed = 0;
  return true;
}

bool Server::CmdStartBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused)
{
  *pBiotopSpeed = 1;
  return true;
}

bool Server::CmdSetBiotopSpeed(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused)
{
  int newSpeed = atoi(CScenarioPlayer::GetParamFromString(commandParam,0).c_str());
  *pBiotopSpeed = newSpeed;
  return true;
}
