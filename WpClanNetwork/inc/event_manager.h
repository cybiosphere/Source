#pragma once

#include "API/network.h"
#include "CBiotop.h"

#define SENT_BUFFER_MAX_SIZE        30000
#define SENT_BUFFER_MAX_NB_BLOCKS   30

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
    static bool buildEventsCreateBiotop(CBiotop* pBiotop, std::vector<NetGameEvent>& eventVector);
    void handleEventCreateBiotop(const NetGameEvent& e, CBiotop* pBiotop);

	  static bool buildEventsAddEntity(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
    CBasicEntity* handleEventAddEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

    static bool buildEventsAddCloneEntities(entityIdType modelEntityId, std::vector<BiotopEntityPosition_t> vectPositions, std::vector<NetGameEvent>& eventVector);
    static bool handleEventAddCloneEntity(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

	  static bool buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
    void handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop);

	  static NetGameEvent buildEventUpdateEntityPos(CBasicEntity* pEntity);
	  static CBasicEntity* handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool forceEntityUpdate);

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

    static NetGameEvent buildEventChangeEntityRemoteControl(entityIdType entityId, bool setRemoteControl);
    static bool handleEventChangeEntityRemoteControl(const NetGameEvent& e, CBiotop* pBiotop);

  private:
    std::vector<LongBufferEvent_t> m_tBiotopBufferEvent;
	  std::vector<LongBufferEvent_t> m_tEntityBufferEvent;
    std::vector<LongBufferEvent_t> m_tMeasureBufferEvent;
    std::vector<LongBufferEvent_t> m_tGeoMapBufferEvent;

	  static bool buildEventsLongString(const std::string event_label, const DataBuffer& data, const int transactionId,
                                      const int custom1, const int custom2, const int custom3, const int custom4,
                                      std::vector<NetGameEvent>& eventVector);
    static bool handleEventsLongString(const NetGameEvent& e,
                                       std::vector<LongBufferEvent_t>& tBufferEvent, DataBuffer& bufferOutput,
                                       int& transactionId, int& custom1, int& custom2, int& custom3, int& custom4);

    CBasicEntity* addEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, const entityIdType entityId,
                                const int stepCoordX, const int stepCoordY, const int layer, const int stepDirection,
                                CBiotop* pBiotop, bool setAsRemoteControl);
    bool updateEntityWithZipBuffer(const DataBuffer& xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop);
    bool createMeasureWithZipBuffer(const DataBuffer& dataZipBuffer, CBiotop* pBiotop, const int measureId,
                                    const int period, const int typeSubType, const int paramId, const int entityId);
    bool createSpawnerWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop, const int spawnerId,
      const int intensity, const int period, const bool isProportional);
    bool createGeoMapSpecieWithZipBuffer(const DataBuffer& xmlZipBuffer, CBiotop* pBiotop);
  };
}
