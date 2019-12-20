#pragma once

#include "API/network.h"
#include "CBiotop.h"

namespace clan
{

#define SENT_BUFFER_MAX_SIZE        31000
#define SENT_BUFFER_MAX_NB_BLOCKS   10

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
		static bool buildEventsUpdateEntityData(CBasicEntity* pEntity, std::vector<NetGameEvent>& eventVector);
		void handleEvenUpdateEntityData(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

		static NetGameEvent buildEventUpdateEntityPos(CBasicEntity* pEntity);
		static CBasicEntity* handleEventUpdateEntityPosition(const NetGameEvent& e, CBiotop* pBiotop, bool setAsRemoteControl);

		static NetGameEvent buildEventRemoveEntity(CBasicEntity* pEntity, entityIdType entityId);
		static bool handleEventRemoveEntity(const NetGameEvent& e, CBiotop* pBiotop);

	private:
		std::vector<LongBufferEvent_t> m_tEntityBufferEvent;

		static bool build_events_long_string(const std::string event_label, DataBuffer data, int transactionId, std::vector<NetGameEvent>& eventVector);
		void updateBiotopWithEntityZipBuffer(DataBuffer xmlZipBuffer, entityIdType entityId, CBiotop* pBiotop, bool setAsRemoteControl);

	};
}
