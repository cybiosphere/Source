#pragma once

#include "API/network.h"
#include "CBiotop.h"

#define SENT_BUFFER_MAX_SIZE        30000
#define SENT_BUFFER_MAX_NB_BLOCKS   12

namespace clan
{
  typedef struct
  {
	  int        transactionId;
	  int        nb_blocks;
	  int        nb_blocks_received;
	  DataBuffer buffer[SENT_BUFFER_MAX_NB_BLOCKS];
  } LongBufferEvent_t;

  class event_manager
  {
  public:
	  static bool buildEventsAddEntity(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
	  void handleEventAddEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static NetGameEvent buildEventAddCloneEntity(CBasicEntity* pEntity, entityIdType modelEntityId);
	  bool handleEventAddCloneEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static bool buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
	  void handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static NetGameEvent buildEventUpdateEntityPos(CBasicEntity* pEntity);
	  static CBasicEntity* handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static NetGameEvent buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId);
    static bool handleEventRemoveEntity(const NetGameEvent& e, CBiotop* pBiotop);

    static NetGameEvent buildEventChangeBiotopSpeed(const float newBiotopSpeed);
    static bool handleEventChangeBiotopSpeed(const NetGameEvent& e, float& newBiotopSpeed);

  private:
	  std::vector<LongBufferEvent_t> m_tEntityBufferEvent;

	  static bool build_events_long_string(const std::string event_label, const DataBuffer& data, const int transactionId,
                                         const int custom1, const int custom2, const int custom3, const int custom4,
                                         std::vector<NetGameEvent>& eventVector);
    bool addEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, const entityIdType entityId,
                                const int stepCoordX, const int stepCoordY, const int layer, const int stepDirection,
                                CBiotop* pBiotop, bool setAsRemoteControl);
    bool updateEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop, bool setAsRemoteControl);
  };
}