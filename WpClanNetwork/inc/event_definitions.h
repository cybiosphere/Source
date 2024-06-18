#pragma once

#include <string>

namespace clan
{
  // Label events from Client to Server only
  const std::string labelEventLogin            { "Login" };
  const std::string labelEventRequestStart     { "Game-RequestStart" };
  const std::string labelEventForceEntityAction{ "ForceAction" }; // "Biotop-Force entity action"

  // Label events from Server to Client only
  const std::string labelEventLoginOk            { "Login-Success" };
  const std::string labelEventLoginKo            { "Login - Fail" };
  const std::string labelEventLoadMap            { "Game-LoadMap" };
  const std::string labelEventStart              { "Game-Start" };
  const std::string labelEventNextSecStart       { "SecStart" }; // "Biotop-Next second start"
  const std::string labelEventChangeRemoteControl{ "ChangeCtrl" }; // "Biotop-Change entity remote control"

  // Label events foth directions
  const std::string labelEventAddEntity        { "AddEntity" }; // "Biotop-Add entity"
  const std::string labelEventAddCloneEntity   { "AddClone" }; // "Biotop-Add clone"
  const std::string labelEventUpdateEntityData { "UpdData" }; // "Biotop-Update entity data"
  const std::string labelEventUpdateEntityPos  { "UpdPos" }; // "Biotop-Update entity position"
  const std::string labelEventUpdateEntityPhy  { "UpdPhys" }; // "Biotop-Update entity phys"
  const std::string labelEventRemoveEntity     { "RemoveEntity" }; // "Biotop-Remove entity"
  const std::string labelEventCreateMeasure    { "CreateMeasure" }; // "Biotop-Create measure"
  const std::string labelEventAddEntitySpawner { "AddSpawner" }; // "Biotop-Add entity spawner"
  const std::string labelEventCreateSpecieMap  { "CreateMap" }; // "Biotop-Create specie map"
  const std::string labelEventMarkEntities     { "MarkEntities" }; // "Biotop-Mark entities"
  const std::string labelEventChangeBiotopSpeed{ "ChangeSpeed" }; // "Biotop-Change speed"
  const std::string labelEventNextSecEnd       { "EndSec" }; // "Biotop-Next second end"
  const std::string labelEventReqEntityRefresh { "ReqRefresh" }; // "Biotop-Req entity refresh"
  const std::string labelEventUpdateClimate    { "Updateclimate" }; // "Biotop-Update climate"

  // Label logs
  const std::string labelAttach{ "Attach" };
  const std::string labelLogin { "Login " };
  const std::string labelInput { "Input " };
  const std::string labelError { "ERROR " };
  const std::string labelClient{ "Client" };
  const std::string labelServer{ "Server" };
  const std::string labelInfo  { "INFO  " };
  const std::string labelEvent { "Event " };

}

