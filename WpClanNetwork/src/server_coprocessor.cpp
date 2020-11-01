#include "API/core.h"
#include "API/network.h"

using namespace clan;
#include "server_coprocessor.h"
#include "clan_server.h"
#include "CAnimMammal.h"

void ServerCoprocessor::update_all_entities_control()
{
  CBasicEntity* pCurEntity = nullptr;
  for (int i = 0; i < m_pBiotop->getNbOfEntities(); i++)
  {
    pCurEntity = m_pBiotop->getEntityByIndex(i);
    update_entity_control(pCurEntity, true);
  }
}

void ServerCoprocessor::update_entity_control(CBasicEntity* pEntity, bool isNewEntity)
{
  // Do not change control for pregnant animals
  if ((pEntity->getClass() == CLASS_MAMMAL) && (((CAnimMammal*)pEntity)->getGestationBabyNumber() > 0))
  {
    //log_event("Server", "Skip Transfer control for pregnant animal");
    return;
  }

  int spatialOffset = 0;
  if (!isNewEntity) spatialOffset = spatialHisteresisSize;

  if (pEntity->isRemoteControlled())
  {
    if (pEntity->getGridCoord().x < (m_startCoordX - spatialOffset))
    {
      // Transfer control from coprocessor to server
      if (!isNewEntity)
      {
        log_event("Server", "Transfer control from coprocessor to server");
        //m_pServer->send_event_request_entity_refresh(pEntity, m_pUser);
      }
      m_pServer->send_event_change_remote_control(pEntity, true, m_pUser);  
      pEntity->setRemoteControlled(false);
    }
  }
  else
  {
    if (pEntity->getGridCoord().x > (m_startCoordX + spatialOffset))
    {
      // Transfer control from server to coprocessor
      if (!isNewEntity)
      {
        log_event("Server", "Transfer control from server to coprocessor");
        //m_pServer->send_event_update_entity_data(pEntity, m_pUser);
      }
      m_pServer->send_event_change_remote_control(pEntity, false, m_pUser);
      pEntity->setRemoteControlled(true);
    }
  }
}

bool ServerCoprocessor::checkNextSecondComplete()
{
  return m_pUser->isNextSecondCompleted;
}

void ServerCoprocessor::forceNextSecondComplete(bool newValue)
{
  m_pUser->isNextSecondCompleted = newValue;
}
