#ifndef __ClanClient_h_
#define __ClanClient_h_

#include <API/core.h>
#include <API/network.h>
#include "CBiotop.h"
#include "clan_server.h"
#include "event_manager.h"

using namespace clan;

class CybiOgre3DApp;

class Client
{
public:
	Client(std::string serverAddr, std::string portId, CybiOgre3DApp* pCybiOgre3DApp = NULL);
	~Client();

	void exec();
  void connect_to_server();
  void disconnect_from_server();
  void process_new_events();
  bool check_if_event_next_second_start_and_clean();
  bool check_if_event_next_second_end_and_clean();
  CBiotop* get_pBiotop();
  bool is_biotop_config_complete();
  bool is_logged_in();
  float get_biotop_speed();

  static bool CmdHelp(CBiotop* pBiotop, string path, string commandParam, int* unused1, int* unused2);
  static bool CmdDisplayBiotop(CBiotop* pBiotop, string path, string commandParam, int* pBiotopSpeed, int* unused);

  void send_event_add_entity(CBasicEntity* pEntity);
  void send_events_add_clone_entities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions);
  void send_event_update_entity_data(CBasicEntity* pEntity);
  void send_event_update_entity_position(CBasicEntity* pEntity);
  void send_event_remove_entity(CBasicEntity* pEntity, entityIdType entityId);
  void send_event_change_biotop_speed(const float newBiotopSpeed, const bool isManualMode);
  void send_event_force_entity_action(const entityIdType entityId, const int actionIndex);
  void send_event_create_measure(CMeasure* pMeasure);
  void send_event_request_entity_refresh(CBasicEntity* pEntity, const entityIdType entityId);
  void send_event_add_entity_spawner(int index, BiotopRandomEntitiyGeneration_t& generator);
  void send_event_create_specie_map(CGeoMapPopulation* pGeoMapSpecie);

private:
	void on_connected();
	void on_disconnected();

	void on_event_received(const NetGameEvent &e);

	void on_event_login_success(const NetGameEvent &e);
	void on_event_login_fail(const NetGameEvent &e);
	void on_event_game_loadmap(const NetGameEvent &e);
	void on_event_game_startgame(const NetGameEvent &e);
	void on_event_biotop_nextsecond_start(const NetGameEvent &e);
  void on_event_biotop_nextsecond_end(const NetGameEvent &e);
  void on_event_biotop_addentity(const NetGameEvent& e);
  void on_event_biotop_addcloneentity(const NetGameEvent& e);
	void on_event_biotop_updatefullentity(const NetGameEvent &e);
	void on_event_biotop_updateentityposition(const NetGameEvent &e);
  void on_event_biotop_removeentity(const NetGameEvent &e);
  void on_event_biotop_createmeasure(const NetGameEvent& e);
  void on_event_biotop_addEntitySpawner(const NetGameEvent& e);
  void on_event_biotop_createspeciemap(const NetGameEvent& e);

  void displayBiotopEntities();
  void displayBiotopEntityDetail(entityIdType entityId);

private:
	NetGameClient network_client;
	SlotContainer cc;

	NetGameEventDispatcher<> login_events;
	NetGameEventDispatcher<> game_events;

	bool quit;
	bool logged_in;

  std::string m_ServerAddr; //"localhost"; //"192.168.1.67"
  std::string m_PortId;

  CBiotop* m_pBiotop;
  bool     m_bBiotopConfigComplete;
  event_manager m_EventManager;
  bool m_bEventNextSecondStart;
  bool m_bEventNextSecondEnd;
  int m_lastEventTimeStamp;
  float m_biotopSpeed; // Controled by server. 1.0 is real time speed. Biotp update every 1sec

  CybiOgre3DApp* m_pCybiOgre3DApp;
};

#endif // #ifndef __ClanClient_h_