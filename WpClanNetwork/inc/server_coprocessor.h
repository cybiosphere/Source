#pragma once

using namespace clan;
#include "clan_server_user.h"
#include "CBiotop.h"

using namespace clan;

class Server;

constexpr int spatialHisteresisSize = 10;

class ServerCoprocessor
{
	public:
		ServerCoprocessor(Server* pServer, ServerUser* pUser, CBiotop* pBiotop, unsigned int startCoordX, unsigned int startCoordY) :
			m_pServer(pServer),
			m_pUser(pUser),
			m_pBiotop(pBiotop),
			m_startCoordX(startCoordX),
			m_startCoordY(startCoordY)
		{};

		void update_all_entities_control();
		void update_entity_control(CBasicEntity* pEntity, bool isNewEntity);
		bool checkNextSecondComplete();
		void resetNextSecondComplete();

	private:
		Server* m_pServer;
		ServerUser* m_pUser;
		CBiotop* m_pBiotop;
		unsigned int m_startCoordX;
		unsigned int m_startCoordY;
	};
