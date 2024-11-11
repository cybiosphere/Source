#pragma once

using namespace clan;
#include "clan_server_user.h"
#include "CBiotop.h"

using namespace clan;

class Server;

constexpr int spatialHisteresisSize = 20;

class ServerCoprocessor
{
	public:
		ServerCoprocessor(Server* pServer, ServerUser* pUser, CBiotop* pBiotop, size_t startCoordX, size_t endCoordX) :
			m_pServer(pServer),
			m_pUser(pUser),
			m_pBiotop(pBiotop),
			m_startCoordX(startCoordX),
			m_endCoordX(endCoordX)
		{};

		static void reset_all_entities_control(CBiotop* pBiotop);
		void assign_all_entities_control();
		void assign_entity_control(CBasicEntity* pEntity);
		void update_entity_control(CBasicEntity* pEntity);
		bool checkNextSecondComplete();
		void forceNextSecondComplete(bool newValue);
		bool checkIfEntityOwner(CBasicEntity* pEntity);
		ServerUser* getUser();
		size_t getStartCoordX() { return m_startCoordX; };
		size_t getEndCoordX() { return m_endCoordX; };

	private:
		bool isEntityInAssignZone(CBasicEntity* pEntity);
		bool isEntityInExclusiveZone(CBasicEntity* pEntity);
		bool isEntityInExtendedZone(CBasicEntity* pEntity);
		bool isEntityInMonitoringZone(CBasicEntity* pEntity);

		Server* m_pServer;
		ServerUser* m_pUser;
		CBiotop* m_pBiotop;
		size_t m_startCoordX;
		size_t m_endCoordX;
	};
