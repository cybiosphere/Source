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
		ServerCoprocessor(Server* pServer, ServerUser* pUser, CBiotop* pBiotop, unsigned int startCoordX, unsigned int startCoordY) :
			m_pServer(pServer),
			m_pUser(pUser),
			m_pBiotop(pBiotop),
			m_startCoordX(startCoordX),
			m_startCoordY(startCoordY)
		{};

		static void reset_all_entities_control(CBiotop* pBiotop);
		void update_all_entities_control();
		void update_entity_control(CBasicEntity* pEntity, bool isNewEntity);
		bool checkNextSecondComplete();
		void forceNextSecondComplete(bool newValue);
		bool checkIfEntityOwner(CBasicEntity* pEntity);
		ServerUser* getUser();

	private:
		Server* m_pServer;
		ServerUser* m_pUser;
		CBiotop* m_pBiotop;
		unsigned int m_startCoordX;
		unsigned int m_startCoordY;
	};
