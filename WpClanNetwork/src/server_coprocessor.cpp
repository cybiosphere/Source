#include "API/core.h"
#include "API/network.h"

using namespace clan;
#include "server_coprocessor.h"
#include "clan_server.h"
#include "CAnimMammal.h"

void ServerCoprocessor::reset_all_entities_control(CBiotop* pBiotop)
{
  for (int i = 0; i < pBiotop->getNbOfEntities(); i++)
  {
    pBiotop->getEntityByIndex(i)->setRemoteControlled(false);
  }
}

void ServerCoprocessor::assign_all_entities_control()
{
  CBasicEntity* pCurEntity = nullptr;
  for (int i = 0; i < m_pBiotop->getNbOfEntities(); i++)
  {
    pCurEntity = m_pBiotop->getEntityByIndex(i);
    assign_entity_control(pCurEntity);
  }
}

bool ServerCoprocessor::isEntityInAssignZone(CBasicEntity* pEntity)
{
  if ((pEntity->getGridCoord().x >= m_startCoordX) && (pEntity->getGridCoord().x < m_endCoordX))
  {
    return true;
  }
  return false ;
}

bool ServerCoprocessor::isEntityInExtendedZone(CBasicEntity* pEntity)
{
  if (((int)pEntity->getGridCoord().x >= ((int)m_startCoordX - spatialHisteresisSize)) && (pEntity->getGridCoord().x < (m_endCoordX + spatialHisteresisSize)))
  {
    return true;
  }
  return false;
}

bool ServerCoprocessor::isEntityInExclusiveZone(CBasicEntity* pEntity)
{
  if ((pEntity->getGridCoord().x >= (m_startCoordX + spatialHisteresisSize)) && ((int)pEntity->getGridCoord().x < ((int)m_endCoordX - spatialHisteresisSize)))
  {
    return true;
  }
  return false;
}

bool ServerCoprocessor::isEntityInMonitoringZone(CBasicEntity* pEntity)
{
  constexpr int monitoringOffset = spatialHisteresisSize + 10;
  if (((int)pEntity->getGridCoord().x >= ((int)m_startCoordX - monitoringOffset)) && (pEntity->getGridCoord().x < (m_endCoordX + monitoringOffset)))
  {
    return true;
  }
  return false;
}


void ServerCoprocessor::assign_entity_control(CBasicEntity* pEntity)
{
  if (!pEntity->isRemoteControlled())
  {
    if (isEntityInAssignZone(pEntity))
    {
      // Transfer control from server to coprocessor
      m_pServer->send_event_change_remote_control(pEntity, false, m_pUser);
      pEntity->setRemoteControlled(true);
    }
  }
}

void ServerCoprocessor::update_entity_control(CBasicEntity* pEntity)
{
  // Do not change control for pregnant animals
  if ((pEntity->getClass() == CLASS_MAMMAL) && (((CAnimMammal*)pEntity)->getGestationBabyNumber() > 0))
  {
    //log_event("Server", "Skip Transfer control for pregnant animal");
    return;
  }

  if (!isEntityInMonitoringZone(pEntity))
  {
    return;
  }

  if (pEntity->isRemoteControlled())
  {
    if (!isEntityInExtendedZone(pEntity))
    {
      // Transfer control from coprocessor to server
      log_event("Server", "Transfer control from coprocessor to server entiy %1 ID %2", pEntity->getLabel(), (int)pEntity->getId());
      m_pServer->send_event_request_entity_refresh(pEntity, m_pUser);
      m_pServer->send_event_change_remote_control(pEntity, true, m_pUser);  
      pEntity->setRemoteControlled(false);
    }
  }
  else
  {
    if (isEntityInExclusiveZone(pEntity))
    {
      // Transfer control from server to coprocessor
      log_event("Server", "Transfer control from server to coprocessor entiy %1 ID %2", pEntity->getLabel(), (int)pEntity->getId());
      m_pServer->send_event_update_entity_data(pEntity, m_pUser);
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

bool ServerCoprocessor::checkIfEntityOwner(CBasicEntity* pEntity)
{
  if (pEntity && pEntity->isRemoteControlled() && isEntityInExtendedZone(pEntity))
  {
    return true;
  }
  else
  {
    return false;
  }
}

ServerUser* ServerCoprocessor::getUser()
{
  return m_pUser;
}
