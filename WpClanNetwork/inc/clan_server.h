#pragma once

//FRED #include <ClanLib/core.h>
//FRED #include <ClanLib/network.h>

//#include "Network/precomp.h"
//#include "API/Network/NetGame/event.h"
//#include "API/Core/Text/string_format.h"
//#include "API/Network/NetGame/event_dispatcher.h"
//#include "API/Network/NetGame/server.h"
//#include "Network/NetGame/connection_impl.h"

#include "API/core.h"
#include "API/network.h"
#include "CBiotop.h"

using namespace clan;

class ServerUser;

#define SENT_BUFFER_MAX_SIZE        31000
#define SENT_BUFFER_MAX_NB_BLOCKS   10

class Server
{
public:
	Server(CBiotop* pBiotop);
	~Server();

	void exec();

private:
	void on_client_connected(NetGameConnection *connection);
	void on_client_disconnected(NetGameConnection *connection, const std::string &message);

	void on_event_received(NetGameConnection *connection, const NetGameEvent &e);

	void on_event_login(const NetGameEvent &e, ServerUser *user);
	void on_event_game_requeststart(const NetGameEvent &e, ServerUser *user);

  void send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user = NULL);
  void send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user = NULL);

  void send_generic_event_long_string(const std::string event_label, DataBuffer data, ServerUser *user = NULL);

  bool process_cmd_line(const std::string input_cmd_string);

private:
	NetGameServer network_server;
	SlotContainer cc;

	NetGameEventDispatcher<ServerUser*> login_events;
	NetGameEventDispatcher<ServerUser*> game_events;

	int next_user_id;
	bool users_connected;
  CBiotop* m_pBiotop;
  float m_biotopSpeed; // set 1.0 for real time speed. Biotp update every 1sec
  clock_t m_LastRunTick;

public:
  // Functions for CMD line
  static bool CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2);
  static bool CmdStopBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdStartBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdSetBiotopSpeed(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
};
