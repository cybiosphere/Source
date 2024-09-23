#include "clan_server.h"
#include "clan_server_user.h"
#include "event_definitions.h"
#include "server_coprocessor.h"
#include "custom_type.h"
#include "API/Core/Zip/zlib_compression.h"
#include <chrono>
#include "CScenarioPlayer.h"
#include "StartupHelpers.h"

#define SERVER_CMD_NUMBER 4

CommandHandler_t ServerCmdNameList[SERVER_CMD_NUMBER] =
{
// cmd name                     cmd function                       help string
  {"HELP",                      Server::CmdHelp,                   "HELP"},
  {"STOP_BIOTOP",               Server::CmdStopBiotop,             "STOP_BIOTOP"},
  {"START_BIOTOP",              Server::CmdStartBiotop,            "START_BIOTOP"},
  {"SET_BIOTOP_SPEED",          Server::CmdSetBiotopSpeed,         "SET_BIOTOP_SPEED <speed int>"}
};


Server::Server(std::string portStr, CBiotop* pBiotop, CScenarioPlayer* pScenarioPlayer, double defaultSpeed)
: serverPortStr{ portStr },
m_pBiotop{ pBiotop },
m_pScenarioPlayer{ pScenarioPlayer },
next_user_id(1),
nb_users_connected(0),
m_biotopSpeed(defaultSpeed),
m_bManualMode(false),
m_bMaxSpeedMode(false),
m_MaxSpeedStepfactor(1)
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
  game_events.func_event(labelEventUpdateEntityPhy) = clan::bind_member(this, &Server::on_event_biotop_updateentityphysic);
  game_events.func_event(labelEventRemoveEntity) = clan::bind_member(this, &Server::on_event_biotop_removeentity);
  game_events.func_event(labelEventChangeBiotopSpeed) = clan::bind_member(this, &Server::on_event_biotop_changespeed);
  game_events.func_event(labelEventForceEntityAction) = clan::bind_member(this, &Server::on_event_biotop_forceentityaction);
  game_events.func_event(labelEventCreateMeasure) = clan::bind_member(this, &Server::on_event_biotop_createmeasure);
  game_events.func_event(labelEventReqEntityRefresh) = clan::bind_member(this, &Server::on_event_biotop_requestentityrefresh);
  game_events.func_event(labelEventAddEntitySpawner) = clan::bind_member(this, &Server::on_event_biotop_addEntitySpawner);
  game_events.func_event(labelEventCreateSpecieMap) = clan::bind_member(this, &Server::on_event_biotop_createspeciemap);
  game_events.func_event(labelEventMarkEntities) = clan::bind_member(this, &Server::on_event_biotop_markEntitiesWithGene);
  game_events.func_event(labelEventUpdateClimate) = clan::bind_member(this, &Server::on_event_biotop_updateClimate);
  game_events.func_event(labelEventNextSecEnd) = clan::bind_member(this, &Server::on_event_biotop_nextsecond_end);
}

Server::~Server()
{
}

void Server::startServer()
{
  network_server.start(serverPortStr);
  log_event(labelServer, "SERVER started on port " + serverPortStr);
}

void Server::processBiotopEvents()
{
	// Update clients with biotop evolution
	if (nb_users_connected > 0)
	{
    BiotopEventPair eventPair = m_pBiotop->getNextUnreadNetworkBiotopEvent();
    while (eventPair.first != ENTITY_ID_INVALID)
    {
      BiotopEvent_t* bioEvent{ &eventPair.second };
      entityIdType entityId = eventPair.first;

      if (bioEvent->eventList.test(BIOTOP_EVENT_ENTITY_REMOVED))
      {
        send_event_remove_entity(bioEvent->pEntity, entityId);
      }
      else if (bioEvent->eventList.test(BIOTOP_EVENT_ENTITY_MODIFIED))
      {
        send_event_update_entity_data(bioEvent->pEntity);
      }
      else if (bioEvent->eventList.test(BIOTOP_EVENT_ENTITY_ADDED))
      {
        send_event_add_entity(bioEvent->pEntity);
      }
      else if (bioEvent->eventList.test(BIOTOP_EVENT_ENTITY_PHYSICAL_CHANGE))
      {
        send_event_update_entity_physic(bioEvent->pEntity);
      }
      else
      {
        send_event_update_entity_position(bioEvent->pEntity);
      }
      eventPair = m_pBiotop->getNextUnreadNetworkBiotopEvent();
    }
  }
}

// Server main loop
void Server::exec()
{
  std::chrono::time_point<std::chrono::system_clock> curTick;
  std::chrono::time_point<std::chrono::system_clock> lastRunTick = std::chrono::system_clock::now();
  int currentScenarioScore = 0;

	network_server.start(serverPortStr);

	log_event(labelServer, "SERVER started on port " + serverPortStr);
	while (true)
	{
		// Lets not worry about exiting this function!
		System::sleep(1);
		network_server.process_events();
    processBiotopEvents();

    // Run biotop every sec
    curTick = std::chrono::system_clock::now();
    std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTick - lastRunTick);

    if (((elapsedTime.count() * m_biotopSpeed) >= 1000) && checkAllCoprocessorCompleteSecond())
    {
      if (!m_bManualMode)
      {
        for (int i = 0; i < m_MaxSpeedStepfactor; i++)
        {
          send_event_new_second_start();
          // Next second in biotop
          if ((m_pScenarioPlayer != nullptr) && m_pScenarioPlayer->NextCmdNextSecond())
          {
            if (m_pScenarioPlayer->m_totalScore > currentScenarioScore)
            {
              currentScenarioScore = m_pScenarioPlayer->m_totalScore;
              log_event(labelServer, "Scenario score success: %1 / %2", m_pScenarioPlayer->m_successScore, m_pScenarioPlayer->m_totalScore);
            }
          }
          else
          {
            m_pBiotop->nextSecond();
          }
          send_event_new_second_end();
        }
      }
      // Reset timer
      lastRunTick = std::chrono::system_clock::now();
    }
    /*else
    {
      std::string inputcommand;
      bool resu = log_get_console_input(inputcommand);
      if (resu)
      {
        log_event(labelInput, inputcommand);
        process_cmd_line(inputcommand);
      }
    }*/
	}

	network_server.stop();
}

void Server::process_new_events()
{
  network_server.process_events();
}

bool Server::checkAllCoprocessorCompleteSecond()
{
  for (auto coprocess : m_tCoprocessors)
  {
    if (coprocess.checkNextSecondComplete() == false)
    {
      //log_event(labelServer, "Latency: waiting for coprocessor");
      return false;
    }
  }
  // Every coprocess is done
  for (auto coprocess : m_tCoprocessors)
  {
    coprocess.forceNextSecondComplete(false);
    //log_event(labelServer, "Latency: coprocessor complete OK");
  }
  return true;
}

ServerUser* Server::getCoprocessorOwnerUser(CBasicEntity* pEntity)
{
  ServerUser* ownerUser = NULL;
  for (auto coprocess : m_tCoprocessors)
  {
    if (coprocess.checkIfEntityOwner(pEntity))
    {
      ownerUser = coprocess.getUser();
    }
  }
  return ownerUser;
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
  if (m_bManualMode == false)
  {
    // Need to call new_second_end before next new_second_start to avoid to loose events raised during manual mode
    send_event_new_second_end();
  }
  m_bManualMode = newManualMode;
}

void Server::set_maxSpeed_mode(bool newMaxSpeedMode)
{
  m_bMaxSpeedMode = newMaxSpeedMode;
}

bool Server::get_maxSpeed_mode()
{
  return m_bMaxSpeedMode;
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
        log_event(labelInput, "Biotop speed set to %1", var1);
      }
    }
    else
    {
      log_event(labelInput, "Invalid command");
    }
  }

  return resu;
}


// A new client is connecting
void Server::on_client_connected(NetGameConnection *connection)
{
	log_event(labelServer, "Client connected");

	// Create user and attach it to connection
	ServerUser *user = new ServerUser();
	user->attach_to_connection(connection);
}

// A client disconnected
void Server::on_client_disconnected(NetGameConnection *connection, const std::string &message)
{
	log_event(labelServer, "Client disconnected");

	ServerUser *user = ServerUser::get_user(connection);
  if (user)
  {
    if (user->isCoprocessor)
    {
      std::vector<ServerCoprocessor>::iterator iter;
      for (iter = m_tCoprocessors.begin(); iter != m_tCoprocessors.end(); iter++)
      {
        if (iter->getUser() == user)
        {
          m_tCoprocessors.erase(iter);
          break;
        }
      }
      log_event(labelServer, "Coprocessor removed. Update control");
      ServerCoprocessor::reset_all_entities_control(m_pBiotop);
      for (auto coprocess : m_tCoprocessors)
      {
        coprocess.assign_all_entities_control();
      }
    }
    delete user;
    nb_users_connected--;
  }
}

// An event was received from a client
void Server::on_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	//log_event(labelServer, "Client sent event: %1", e.to_string());
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
			log_event(labelServer, "Unhandled event: %1", e.to_string());
		}
	}
}

// "Login" event was received
void Server::on_event_login(const NetGameEvent &e, ServerUser *user)
{
	std::string user_name = e.get_argument(0);

	if(user_name.length() == 0)
	{
    log_event(labelServer, "Client requested login error : Missing user name");
		user->send_event(NetGameEvent(labelEventLoginKo,  "Missing user name"));
	}
	else
	{
		// Assign name and id to User object (created when user connected earlier)
		user->user_name = user_name;
		user->id = next_user_id++;
    if ((user->user_name.find("Coprocess") != std::string::npos) || (user->user_name.find("coprocess") != std::string::npos))
    {
      if (m_tCoprocessors.size() < MaxNumberCoprocessor)
      {
        user->isCoprocessor = true;
        if (m_tCoprocessors.empty())
        {
          ServerCoprocessor newCoprocessor(this, user, m_pBiotop, 2 * m_pBiotop->getDimension().x / 3, m_pBiotop->getDimension().x);
          m_tCoprocessors.push_back(std::move(newCoprocessor));
        }
        else
        {
          ServerCoprocessor newCoprocessor(this, user, m_pBiotop, 0, m_pBiotop->getDimension().x / 3);
          m_tCoprocessors.push_back(std::move(newCoprocessor));
        }
      }
      else
      {
        log_event(labelServer, "Too many coprocessors. Set user as simple client");
        user->isCoprocessor = false;
      }
    }
    else
    {
      user->isCoprocessor = false;
    }
    log_event(labelServer, "Client requested login: " + user_name);
		user->send_event(NetGameEvent(labelEventLoginOk));
	}
}

// "Game-RequestStartGame" event was received
void Server::on_event_game_requeststart(const NetGameEvent &e, ServerUser *user)
{
	log_event(labelServer, "Client requested game start");
  nb_users_connected++;

  // Freeze biotop time during new player configuration
  double biotopSpeed = m_biotopSpeed;
  m_biotopSpeed = 0;
  size_t i;

  std::vector<NetGameEvent> eventVector;
  if (event_manager::buildEventsCreateBiotop(m_pBiotop, eventVector))
  {
    // Send biotop file without entities
    for (NetGameEvent eventToSend : eventVector)
    {
      user->send_event(eventToSend);
    }

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
                                          pCurEntity->getGlobalStepCoord().x,
                                          pCurEntity->getGlobalStepCoord().y,
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
      send_event_add_entity_spawner((int)i, m_pBiotop->getRandomEntitiyGeneration(i), user);
    }

    for (i = 0; i < m_pBiotop->getNbOfGeoMapSpecie(); i++)
    {
      send_event_create_specie_map(m_pBiotop->getGeoMapSpecieByIndex(i), user);
    }

    int nextHourTimeOffsetForClient = 0;
    // If new coprocessor arrives, update all entities control
    if (user->isCoprocessor)
    {
      //ServerCoprocessor::reset_all_entities_control(m_pBiotop);
      nextHourTimeOffsetForClient = m_tCoprocessors.size() * 10;
      log_event(labelServer, "New coprocessor added. Update control. Time offset = %1", nextHourTimeOffsetForClient);
      for (auto coprocess : m_tCoprocessors)
      {
        coprocess.assign_all_entities_control();
      }
    }

	  user->send_event(NetGameEvent(labelEventStart, nextHourTimeOffsetForClient));

    if (m_pBiotop->getGeneToMark().getGeneType() != GENE_GENERIC)
    {
      send_event_mark_entities_with_gene(m_pBiotop->getGeneToMark(), m_pBiotop->getMarkDominantAlleleOnly());
    }
  }

  // Restore biotop time
  m_biotopSpeed = biotopSpeed;
}

void Server::on_event_biotop_addentity(const NetGameEvent& e, ServerUser* user)
{
  CBasicEntity* pEntity = m_EventManager.handleEventAddEntity(e, m_pBiotop, false);
}

void Server::on_event_biotop_addcloneentity(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventAddCloneEntity(e, m_pBiotop, false);
}

void Server::on_event_biotop_updatefullentity(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEvenUpdateEntityData(e, m_pBiotop);
}

void Server::on_event_biotop_updateentityposition(const NetGameEvent& e, ServerUser* user)
{
  CBasicEntity* pEntity = event_manager::handleEventUpdateEntityPosition(e, m_pBiotop, m_bManualMode, false);
  if (pEntity && (m_tCoprocessors.size() > 0))
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.update_entity_control(pEntity);
    }
  }
}

void Server::on_event_biotop_updateentityphysic(const NetGameEvent& e, ServerUser* user)
{
  CBasicEntity* pEntity = event_manager::handleEventUpdateEntityPosition(e, m_pBiotop, m_bManualMode, true);
  if (pEntity && (m_tCoprocessors.size() > 0))
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.update_entity_control(pEntity);
    }
  }
}


void Server::on_event_biotop_removeentity(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventRemoveEntity(e, m_pBiotop);
}

void Server::on_event_biotop_changespeed(const NetGameEvent& e, ServerUser* user)
{
  event_manager::handleEventChangeBiotopSpeed(e, m_biotopSpeed, m_bManualMode, m_bMaxSpeedMode);
  m_MaxSpeedStepfactor = computeMaxSpeedStepfactor(m_pBiotop, m_bMaxSpeedMode);
  // Broadcast to all client new speed
  send_event_change_biotop_speed(m_biotopSpeed, m_bManualMode, m_bMaxSpeedMode);
  // Reset coprocess synchro
  for (auto coprocess : m_tCoprocessors)
  {
    coprocess.forceNextSecondComplete(true);
  }
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
  ServerUser* pOwnerUser = getCoprocessorOwnerUser(pEntity);
  if (pOwnerUser != NULL)
  {
    send_event_request_entity_refresh(pEntity, pOwnerUser);
  }
  else
  {
    send_event_update_entity_data(pEntity, user);
  }
}

void Server::on_event_biotop_createspeciemap(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventCreateGeoMapSpecie(e, m_pBiotop);
}

void Server::on_event_biotop_markEntitiesWithGene(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventMarkEntitiesWithGene(e, m_pBiotop);
}

void Server::on_event_biotop_updateClimate(const NetGameEvent& e, ServerUser* user)
{
  m_EventManager.handleEventUpdateClimate(e, m_pBiotop);
  // Forward new climate parameters to all clients
  send_event_update_biotop_climate(*m_pBiotop->getParamFertility(), *m_pBiotop->getParamTemperature());
}

void Server::on_event_biotop_nextsecond_end(const NetGameEvent& e, ServerUser* user)
{
  user->isNextSecondCompleted = true;
}

void Server::send_event_add_entity(CBasicEntity* pEntity, ServerUser* user)
{
  if (pEntity == NULL)
  {
    log_event(labelError, "Send event Add entity: NULL entity");
    return;
  }
  if (pEntity->isToBeRemoved())
  {
    log_event(labelServer, "Send event Add removed entity: %1", pEntity->getLabel());
    return;
  }
  if (pEntity->isAnimal())
  {
    log_event(labelServer, "Send event Add entity: %1", pEntity->getLabel());
  }
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
    log_event(labelError, "send_event_add_entity: Event not sent");
  }

  if (pEntity && (m_tCoprocessors.size() > 0))
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.assign_entity_control(pEntity);
    }
  }
}

void Server::send_events_add_clone_entities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, ServerUser* user)
{
  //log_event(labelServer, "Add clone entites");
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
    log_event(labelError, "send_events_add_clone_entities: Event not sent");
  }
}

void Server::send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user)
{
  if (pEntity == NULL)
  {
  	log_event(labelServer, "Update entity data: NULL entity");
    return;
  }
  if (pEntity->isToBeRemoved())
  {
    log_event(labelServer, "Update removed entity: %1", pEntity->getLabel());
    return;
  }

	log_event(labelServer, "Update entity data: %1", pEntity->getLabel());
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
    log_event(labelError, "send_event_update_entity_data: Event not sent");
  }

  if (m_tCoprocessors.size() > 0)
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.assign_entity_control(pEntity);
    }
  }
}

void Server::send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user)
{
  if ((pEntity == NULL) || (pEntity->isToBeRemoved()))
  {
  	log_event(labelServer, "Update entity position: NULL or removed");
    return;
  }

  NetGameEvent bioUpdateEntityPosEvent { event_manager::buildEventUpdateEntityPos(pEntity, labelEventUpdateEntityPos) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioUpdateEntityPosEvent);
  else
    user->send_event(bioUpdateEntityPosEvent);

  if (pEntity && (m_tCoprocessors.size() > 0))
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.update_entity_control(pEntity);
    }
  }
}

void Server::send_event_update_entity_physic(CBasicEntity* pEntity, ServerUser* user)
{
  if ((pEntity == NULL) || (pEntity->isToBeRemoved()))
  {
    log_event(labelServer, "Update entity physic: NULL or removed");
    return;
  }
  NetGameEvent bioUpdateEntityPosEvent{ event_manager::buildEventUpdateEntityPos(pEntity, labelEventUpdateEntityPhy) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioUpdateEntityPosEvent);
  else
    user->send_event(bioUpdateEntityPosEvent);

  if (pEntity && (m_tCoprocessors.size() > 0))
  {
    for (auto coprocess : m_tCoprocessors)
    {
      coprocess.update_entity_control(pEntity);
    }
  }
}

void Server::send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId, ServerUser *user)
{
  if (pEntity->isAnimal())
  {
    log_event(labelServer, "Remove entity: %1 ID %2", pEntity->getLabel(), (int)entityId);
  }
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
    log_event(labelServer, "Create measure: NULL measure");
    return;
  }

  log_event(labelServer, "Create measure: %1", pMeasure->GetLabel());
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
    log_event(labelError, "send_event_create_measure: Event not sent");
  }
}

void Server::send_event_add_entity_spawner(int index, BiotopRandomEntitiyGeneration_t& generator, ServerUser* user)
{
  if (generator.pModelEntity == NULL)
  {
    log_event(labelServer, "Add entity spwaner: NULL entity");
    return;
  }

  log_event(labelServer, "Add entity spwaner: %1", generator.pModelEntity->getLabel());
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
    log_event(labelError, "send_event_add_entity_spawner: Event not sent");
  }
}

void Server::send_event_create_specie_map(CGeoMapPopulation* pGeoMapSpecie, ServerUser* user)
{
  if (pGeoMapSpecie == NULL)
  {
    log_event(labelServer, "Create specie Map : NULL map");
    return;
  }

  log_event(labelServer, "Create map specie: %1", pGeoMapSpecie->GetSpecieName());
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
    log_event(labelError, "send_event_create_specie_map: Event not sent");
  }
}

void Server::send_event_change_remote_control(CBasicEntity* pEntity, bool setRemoteControl, ServerUser* user)
{
  //log_event(labelServer, "Change entity remote control: %1 setControl: %2", pEntity->getLabel(), setRemoteControl);
  NetGameEvent bioChangeCtrlEntityEvent{ event_manager::buildEventChangeEntityRemoteControl(pEntity->getId(), setRemoteControl) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioChangeCtrlEntityEvent);
  else
    user->send_event(bioChangeCtrlEntityEvent);
}

void Server::send_event_change_biotop_speed(const float newBiotopSpeed, const bool isManualMode, const bool isMaxSpeedMode, ServerUser* user)
{
  //log_event(labelServer, "Change biotop speed: %1 manualMode: %2 isMaxSpeedMode: %3", newBiotopSpeed, isManualMode, isMaxSpeedMode);
  m_biotopSpeed = newBiotopSpeed;
  m_bManualMode = isManualMode;
  m_bMaxSpeedMode = isMaxSpeedMode;
  NetGameEvent bioChangeSpeedEvent{ event_manager::buildEventChangeBiotopSpeed(newBiotopSpeed, isManualMode, isMaxSpeedMode) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioChangeSpeedEvent);
  else
    user->send_event(bioChangeSpeedEvent);
}

void Server::send_event_new_second_start(ServerUser* user)
{
  NetGameEvent bioNextSecEventStart(labelEventNextSecStart);
  bioNextSecEventStart.add_argument(m_biotopSpeed);
  bioNextSecEventStart.add_argument((float)m_pBiotop->getSunlight());
  bioNextSecEventStart.add_argument((float)m_pBiotop->getParamFertility()->getVal());
  bioNextSecEventStart.add_argument((float)m_pBiotop->getParamTemperature()->getVal());
  network_server.send_event(bioNextSecEventStart);
}

void Server::send_event_new_second_end(ServerUser* user)
{
  NetGameEvent bioNextSecEventEnd(labelEventNextSecEnd);
  CustomType biotopTimeEnd(m_pBiotop->getBiotopTime().seconds, m_pBiotop->getBiotopTime().hours, m_pBiotop->getBiotopTime().days);
  bioNextSecEventEnd.add_argument(biotopTimeEnd);
  bioNextSecEventEnd.add_argument(m_pBiotop->getBiotopTime().years);
  network_server.send_event(bioNextSecEventEnd);
}

void Server::send_event_request_entity_refresh(CBasicEntity* pEntity, ServerUser* user)
{
  log_event(labelServer, "Request entity refresh: label %1", pEntity->getLabel());
  NetGameEvent bioReqActionRefresh{ event_manager::buildEventReqEntityRefresh(pEntity) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioReqActionRefresh);
  else
    user->send_event(bioReqActionRefresh);
}

void Server::send_event_mark_entities_with_gene(CGene& modelGene, bool markDominantAlleleOnly, ServerUser* user)
{
  log_event(labelServer, "Mark entities with gene");
  NetGameEvent bioMarkEntities{ event_manager::buildEventMarkEntitiesWithGene(modelGene, markDominantAlleleOnly) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioMarkEntities);
  else
    user->send_event(bioMarkEntities);
}

void Server::send_event_update_biotop_climate(CCyclicParam& fertility, CCyclicParam& temperature, ServerUser* user)
{
  log_event(labelServer, "Update biotop climate");
  NetGameEvent bioUpdateClimate{ event_manager::buildEventUpdateClimate(fertility, temperature) };
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioUpdateClimate);
  else
    user->send_event(bioUpdateClimate);
}

bool Server::CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2)
{
  int i;
  for (i=0; i<SERVER_CMD_NUMBER; i++)
  {
    log_event(labelInfo, ServerCmdNameList[i].helpString);
  }
  string bioCmdStr = "";
  for (i=0; i<100; i++)
  {
    bioCmdStr = CScenarioPlayer::GetHelpCmdString(i);
    if (bioCmdStr != "")
    {
      log_event(labelInfo, bioCmdStr);
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
