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

    static bool buildEventsAddCloneEntities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, std::vector<NetGameEvent>& eventVector);
    static bool handleEventAddCloneEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static bool buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
	  void handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static NetGameEvent buildEventUpdateEntityPos(CBasicEntity* pEntity);
	  static CBasicEntity* handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static NetGameEvent buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId);
    static bool handleEventRemoveEntity(const NetGameEvent& e, CBiotop* pBiotop);

    static NetGameEvent buildEventChangeBiotopSpeed(const float newBiotopSpeed, const bool isManualMode);
    static bool handleEventChangeBiotopSpeed(const NetGameEvent& e, float& newBiotopSpeed, bool& isManualMode);

    static NetGameEvent buildEventForceEntityAction(const entityIdType entityId, const int actionIndex);
    static bool handleEventForceEntityAction(const NetGameEvent& e, CBiotop* pBiotop);

    static bool buildEventsCreateMeasure(CMeasure* pMeasure, std::vector<NetGameEvent>& eventVector);
    void handleEventCreateMeasure(const NetGameEvent& e, CBiotop* pBiotop);

    static NetGameEvent buildEventReqEntityRefresh(CBasicEntity* pEntity, entityIdType entityId);
    static CBasicEntity* handleEventReqEntityRefresh(const NetGameEvent& e, CBiotop* pBiotop);

    static bool buildEventsAddEntitySpawner(int index, BiotopRandomEntitiyGeneration_t& generator, std::vector<NetGameEvent>& eventVector);
    void handleEventAddEntitySpawner(const NetGameEvent& e, CBiotop* pBiotop);

    static bool buildEventsCreateGeoMapSpecie(CGeoMapPopulation* pGeoMap, std::vector<NetGameEvent>& eventVector);
    void handleEventCreateGeoMapSpecie(const NetGameEvent& e, CBiotop* pBiotop);

  private:
	  std::vector<LongBufferEvent_t> m_tEntityBufferEvent;
    std::vector<LongBufferEvent_t> m_tMeasureBufferEvent;
    std::vector<LongBufferEvent_t> m_tGeoMapBufferEvent;

	  static bool build_events_long_string(const std::string event_label, const DataBuffer& data, const int transactionId,
                                         const int custom1, const int custom2, const int custom3, const int custom4,
                                         std::vector<NetGameEvent>& eventVector);
    bool addEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, const entityIdType entityId,
                                const int stepCoordX, const int stepCoordY, const int layer, const int stepDirection,
                                CBiotop* pBiotop, bool setAsRemoteControl);
    bool updateEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop, bool setAsRemoteControl);
    bool createMeasureWithZipBuffer(const DataBuffer& dataZipBuffer, CBiotop* pBiotop, const int measureId,
                                    const int period, const int typeSubType, const int paramId, const int entityId);
    bool createSpawnerWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop, const int spawnerId,
      const int intensity, const int period, const bool isProportional);
    bool createGeoMapSpecieWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop);
  };
}
