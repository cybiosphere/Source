#pragma once

#include "API/core.h"
#include "API/network.h"
#include "CBiotop.h"
#include "CScenarioPlayer.h"
#include "event_manager.h"

using namespace clan;

class ServerUser;
class ServerCoprocessor;

class Server
{
public:
	Server(std::string portStr, CBiotop* pBiotop, CScenarioPlayer* pScenarioPlayer, double defaultSpeed = 1.0);
	~Server();

	// start only
	void startServer();
	void processBiotopEvents();

	// Start and loop on ProcessEvents
	void exec();
	float get_biotop_speed();
	bool get_manual_mode();
	void set_manual_mode(bool newManualMode);
	bool get_maxSpeed_mode();
	void set_maxSpeed_mode(bool newMaxSpeedMode);
	void process_new_events();
	bool checkAllCoprocessorCompleteSecond();
private:
	ServerUser* getCoprocessorOwnerUser(CBasicEntity* pEntity);
	ServerCoprocessor& getCoprocessorFromUser(ServerUser* pUser);

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
	void on_event_biotop_updateentityphysic(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_removeentity(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_changespeed(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_forceentityaction(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_createmeasure(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_requestentityrefresh(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_addEntitySpawner(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_createspeciemap(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_markEntitiesWithGene(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_updateClimate(const NetGameEvent& e, ServerUser* user);
	void on_event_biotop_nextsecond_end(const NetGameEvent& e, ServerUser* user);

public:
	void send_event_add_entity(CBasicEntity* pEntity, ServerUser* user = NULL);
	void send_events_add_clone_entities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, ServerUser* user = NULL);
  void send_event_update_entity_data(CBasicEntity* pEntity, ServerUser *user = NULL);
  void send_event_update_entity_position(CBasicEntity* pEntity, ServerUser *user = NULL);
	void send_event_update_entity_physic(CBasicEntity* pEntity, ServerUser* user = NULL);
  void send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId, ServerUser *user = NULL);
	void send_event_create_measure(CMeasure* pMeasure, ServerUser* user = NULL);
	void send_event_add_entity_spawner(int index, BiotopRandomEntitiyGeneration_t& generator, ServerUser* user = NULL);
	void send_event_create_specie_map(CGeoMapPopulation* pGeoMapSpecie, ServerUser* user = NULL);
	void send_event_change_remote_control(CBasicEntity* pEntity, bool setRemoteControl, ServerUser* user = NULL);
	void send_event_change_biotop_speed(const float newBiotopSpeed, const bool isManualMode, const bool isMaxSpeedMode, ServerUser* user = NULL);
	void send_event_new_second_start(ServerUser* user = NULL);
	void send_event_new_second_end(ServerUser* user = NULL);
	void send_event_request_entity_refresh(CBasicEntity* pEntity, ServerUser* user = NULL);
	void send_event_mark_entities_with_gene(CGene& modelGene, bool markDominantAlleleOnly, ServerUser* user = NULL);
	void send_event_update_biotop_climate(CCyclicParam& fertility, CCyclicParam& temperature, ServerUser* user = NULL);

private:
  bool process_cmd_line(const std::string input_cmd_string);

private:
	NetGameServer network_server;
	SlotContainer cc;

	NetGameEventDispatcher<ServerUser*> login_events;
	NetGameEventDispatcher<ServerUser*> game_events;

	std::string serverPortStr;
	CBiotop* m_pBiotop;
	CScenarioPlayer* m_pScenarioPlayer;
	int next_user_id;
	int  nb_users_connected;
  float m_biotopSpeed; // set 1.0 for real time speed. Biotp update every 1sec
	bool m_bManualMode;
	bool m_bMaxSpeedMode;
	size_t m_MaxSpeedStepfactor;
	static const size_t MaxNumberCoprocessor = 2;
	std::vector<ServerCoprocessor> m_tCoprocessors;
	event_manager m_EventManager;

public:
  // Functions for CMD line
  static bool CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2);
  static bool CmdStopBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdStartBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
  static bool CmdSetBiotopSpeed(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);
};
