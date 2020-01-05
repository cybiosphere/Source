#pragma once

#include <string>

namespace clan
{
  // From Client to Server only
  const std::string labelEventLogin            { "Login" };
  const std::string labelEventRequestStart     { "Game-RequestStart" };
  const std::string labelEventChangeBiotopSpeed{ "Biotop-Change speed" };
  const std::string labelEventForceEntityAction{ "Biotop-Force entity action" };

  // From Server to Client only
  const std::string labelEventLoginOk          { "Login-Success" };
  const std::string labelEventLoginKo          { "Login - Fail" };
  const std::string labelEventLoadMap          { "Game-LoadMap" };
  const std::string labelEventStart            { "Game-Start" };
  const std::string labelEventNextSecStart     { "Biotop-Next second start" };
  const std::string labelEventNextSecEnd       { "Biotop-Next second end" };

  // Both directions
  const std::string labelEventAddEntity        { "Biotop-Add entity" };
  const std::string labelEventAddCloneEntity   { "Biotop-Add clone" };
  const std::string labelEventUpdateEntityData { "Biotop-Update entity data" };
  const std::string labelEventUpdateEntityPos  { "Biotop-Update entity position" };
  const std::string labelEventRemoveEntity     { "Biotop-Remove entity" };
  
}

