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
#include "event_manager.h"

using namespace clan;

class ServerUser;

class Server
{
public:
	Server(CBiotop* pBiotop);
	~Server();

	// start only
	void startServer();
	void ProcessEvents(bool isNewSec, float biotopSpeed);

	// Start and loop on ProcessEvents
	void exec();
	float get_biotop_speed();
	bool get_manual_mode();
	void set_manual_mode(bool newManualMode);

private:
	void on_client_connected(NetGameConnection *connection);
	void on_client_disconnected(NetGameConnection *connection, const std::string &message);

	void on_event_received(NetGameConnection *connection, const NetGameEvent &e);

	void on_event_login(const NetGameEvent &e, ServerUser *user);
	void on_event_game_requeststart(const NetGameEvent &e, ServerUser *user);
	void on_event_biotop_addentity(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_addcloneentity(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_updatefullentity(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_updateentityposition(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_removeentity(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_changespeed(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_forceentityaction(const NetGameEvent& e, ServerUser* user);

	void send_event_add_entity(CBasicEntity* pEntity, ServerUser* user = NULL);
	void send_event_add_clone_entity(CBasicEntity* pEntity, entityIdType modelEntityId, ServerUser* user = NULL);
  void send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user = NULL);
  void send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user = NULL);
  void send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId, ServerUser *user = NULL);

  bool process_cmd_line(const std::string input_cmd_string);

private:
	NetGameServer network_server;
	SlotContainer cc;

	NetGameEventDispatcher<ServerUser*> login_events;
	NetGameEventDispatcher<ServerUser*> game_events;

	int next_user_id;
	int  nb_users_connected;
  CBiotop* m_pBiotop;
  float m_biotopSpeed; // set 1.0 for real time speed. Biotp update every 1sec
	event_manager m_EventManager;
	bool m_bManualMode;

public:
  // Functions for CMD line
  static bool CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2);
  static bool CmdStopBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdStartBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdSetBiotopSpeed(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
};
