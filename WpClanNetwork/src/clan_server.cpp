#include "clan_server.h"
#include "clan_server_user.h"
#include "custom_type.h"
#include "API/Core/Zip/zlib_compression.h"
#include <time.h>
#include "CScenarioPlayer.h"

#define SERVER_CMD_NUMBER 4

CommandHandler_t ScenarioCmdNameList[SERVER_CMD_NUMBER] =
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
	login_events.func_event("Login") = clan::bind_member(this, &Server::on_event_login);
	game_events.func_event("Game-RequestStart") = clan::bind_member(this, &Server::on_event_game_requeststart);

	users_connected = false;
  m_pBiotop = pBiotop;
  m_biotopSpeed = 1.0;
  m_LastRunTick = clock();
}

Server::~Server()
{
}

// Server main loop
void Server::exec()
{
  int timeCount = 0;
  clock_t curTick;
	network_server.start("4556");

	log_event("System  ", "SERVER started");
	while (true)
	{
		// Lets not worry about exiting this function!
		System::sleep(10);
		network_server.process_events();

    // Run biotop every sec
    //
    timeCount += round(10.0 * m_biotopSpeed);
    curTick = clock();

    if (((curTick - m_LastRunTick) * m_biotopSpeed * 1000 / CLOCKS_PER_SEC) >= 1000) // temporaire
    {
      CBasicEntity* pCurEntity = NULL;
      if (users_connected)
      {
        // Send event next second start update
        NetGameEvent bioNextSecEventStart("Biotop-Next second start");
        CustomType biotopTime(m_pBiotop->getBiotopTime().seconds, m_pBiotop->getBiotopTime().hours, m_pBiotop->getBiotopTime().days);
        bioNextSecEventStart.add_argument(biotopTime);
        bioNextSecEventStart.add_argument(m_biotopSpeed);
        network_server.send_event(bioNextSecEventStart);
      }

      //System::sleep(20); // temporary . wait to ensure bioNextSecEventStart alone 
      // Next second in biotop
      m_pBiotop->nextSecond();
      // Reset timer
      timeCount = 0;
      m_LastRunTick = clock();

      // Update clients with biotop evolution
      if (users_connected)
      {
        // Update all entities
        for (int i=0; i<m_pBiotop->getNbOfEntities(); i++)
        {
          pCurEntity = m_pBiotop->getEntityByIndex(i);
          if (pCurEntity->checkIfhasChangedAndClear()) 
            send_event_update_entity_position(pCurEntity);
          // FRED TBC: Check if new entity arrived and advise clients
        }
        // Send event next second end
        NetGameEvent bioNextSecEventEnd("Biotop-Next second end");
        CustomType biotopTimeEnd(m_pBiotop->getBiotopTime().seconds, m_pBiotop->getBiotopTime().hours, m_pBiotop->getBiotopTime().days);
        bioNextSecEventEnd.add_argument(biotopTimeEnd);
        network_server.send_event(bioNextSecEventEnd);
      }
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
    resu = CScenarioPlayer::ExecuteCmd(m_pBiotop, input_cmd_string, "C:\\temp", var1, var2, ScenarioCmdNameList, SERVER_CMD_NUMBER);
    if (resu == true)
    {
      // Var1 is used to set speed
      if (var1>=0)
      {
        m_biotopSpeed = var1;
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

	// Shut down running game for test-purposes
	//if(users_connected)
	//	users_connected = false;
}

// An event was received from a client
void Server::on_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	log_event("Events  ", "Client sent event: %1", e.to_string());

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
		user->send_event(NetGameEvent("Login-Fail",  "Missing user name"));
	}
	else
	{
		// Assign name and id to User object (created when user connected earlier)
		user->user_name = user_name;
		user->id = next_user_id++;

		user->send_event(NetGameEvent("Login-Success"));
	}
}

// "Game-RequestStartGame" event was received
void Server::on_event_game_requeststart(const NetGameEvent &e, ServerUser *user)
{
	log_event("Events  ", "Client requested game start");

	if(users_connected == false)
	{
		users_connected = true;
  }

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
	  NetGameEvent loadMapEvent("Game-LoadMap");
    loadMapEvent.add_argument(xmlZipBuffer);
	  user->send_event(loadMapEvent);

    // Provide all entities position
    for (i=0; i<m_pBiotop->getNbOfEntities(); i++)
    {
      // Send only to new user
      send_event_update_entity_position(m_pBiotop->getEntityByIndex(i), user);
    }

    // Update entitiy data. Send info only 1 time if several entities has same name
    CBasicEntity* pCurEntity = NULL;
    std::string prevEntityLabel = "";
    for (i=0; i<m_pBiotop->getNbOfEntities(); i++)
    {
      pCurEntity = m_pBiotop->getEntityByIndex(i);
      if (pCurEntity->getLabel() != prevEntityLabel)
      {
        // Send only to new user
        send_event_update_entity_data(pCurEntity, user);
        prevEntityLabel = pCurEntity->getLabel();
      }
    }

	  user->send_event(NetGameEvent("Game-Start"));
  }

  // Update again all animal to set action and status
  for (i=0; i<m_pBiotop->getNbOfAnimals(); i++)
  {
    // Send only to new user
    send_event_update_entity_position(m_pBiotop->getEntityByIndex(i), user);
  }

  // Restore biotop time
  m_biotopSpeed = biotopSpeed;
}

void Server::send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user)
{
  if (pEntity == NULL)
  {
  	log_event("Events  ", "Update entity data: NULL entity");
    return;
  }

	log_event("Events  ", "Update entity data: %1", pEntity->getLabel());

  TiXmlDocument xmlDoc;
  pEntity->saveInXmlFile(&xmlDoc);
  TiXmlPrinter xmlPrinter;
  xmlDoc.Accept(&xmlPrinter);
  std::string xmlString = xmlPrinter.Str();
 
    // Compress xml string
  DataBuffer xmlBuffer(xmlString.c_str(), xmlString.length());
  DataBuffer xmlZipBuffer = ZLibCompression::compress(xmlBuffer, false);

  send_generic_event_long_string("Biotop-Update entity data", xmlZipBuffer, user);
}

void Server::send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user)
{
  if (pEntity == NULL)
  {
  	log_event("Events  ", "Update entity position: NULL");
    return;
  }

  int reactionIndex = 0;
  if (pEntity->getBrain() != NULL)
    reactionIndex = pEntity->getBrain()->GetCurrentReactionIndex();

  NetGameEvent bioUpdateEntityPosEvent("Biotop-Update entity position");
  bioUpdateEntityPosEvent.add_argument(pEntity->getId());
  bioUpdateEntityPosEvent.add_argument(pEntity->getLabel());
  bioUpdateEntityPosEvent.add_argument(pEntity->getStepCoord().x);
  bioUpdateEntityPosEvent.add_argument(pEntity->getStepCoord().y);
  bioUpdateEntityPosEvent.add_argument(pEntity->getLayer());
  bioUpdateEntityPosEvent.add_argument(pEntity->getStepDirection());
  bioUpdateEntityPosEvent.add_argument(pEntity->getCurrentSpeed());
  bioUpdateEntityPosEvent.add_argument(float(pEntity->getWeight()));
  bioUpdateEntityPosEvent.add_argument(reactionIndex);
  bioUpdateEntityPosEvent.add_argument(pEntity->getStatus());
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(bioUpdateEntityPosEvent);
  else
    user->send_event(bioUpdateEntityPosEvent);
}

void Server::send_generic_event_long_string(const std::string event_label, DataBuffer data, ServerUser *user)
{
  int nbBlocks = data.get_size()/SENT_BUFFER_MAX_SIZE + 1;
  int transactionId = data.get_size(); // temporary

  if (nbBlocks > SENT_BUFFER_MAX_NB_BLOCKS)
  {
    log_event("-ERROR- ", "Generic event: Event too big to be sent");
    return;
  }

  for (int i=0; i<(nbBlocks-1); i++)
  {
    NetGameEvent genericEvent(event_label);
    DataBuffer dataBlock(data, i*SENT_BUFFER_MAX_SIZE, SENT_BUFFER_MAX_SIZE);
    genericEvent.add_argument(transactionId);
    genericEvent.add_argument(nbBlocks);
    genericEvent.add_argument(i);
    genericEvent.add_argument(dataBlock);
    if (user == NULL) // If user not define, broadcast info to all
      network_server.send_event(genericEvent);
    else
      user->send_event(genericEvent);
  }

  NetGameEvent genericEvent(event_label);
  DataBuffer dataBlock(data, (nbBlocks-1)*SENT_BUFFER_MAX_SIZE, (data.get_size()%SENT_BUFFER_MAX_SIZE));
  genericEvent.add_argument(transactionId);
  genericEvent.add_argument(nbBlocks);
  genericEvent.add_argument(nbBlocks-1);
  genericEvent.add_argument(dataBlock);
  if (user == NULL) // If user not define, broadcast info to all
    network_server.send_event(genericEvent);
  else
    user->send_event(genericEvent);
}

bool Server::CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2)
{
  int i;
  for (i=0; i<SERVER_CMD_NUMBER; i++)
  {
    log_event("Server cmd", ScenarioCmdNameList[i].helpString);
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
