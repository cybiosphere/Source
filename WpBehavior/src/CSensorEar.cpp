/*
https://github.com/cybiosphere
copyright (c) 2005-2014 Frederic RIBERT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

//===========================================================================
// FILE: CSensorEar.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for Ear
//         It is a set of Y = 8 sub-sensors dedicated, for each sector
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  10/10/2005 : Creation
//
//===========================================================================

#include "CSensorEar.h"
#include "CBrainAnimal.h"
#include "CAnimal.h"
#include "CBiotop.h"

//===========================================================================
// Construction/Destruction
//===========================================================================

//---------------------------------------------------------------------------
// METHOD:       CSensorEar::CSensorEar
//  
// DESCRIPTION:  Constructor 
// 
// ARGUMENTS:    CBrainAnimal* pBrain: reference on Brain
//               double pWeightRate[] : table of weight for each sector
//               int range: Distance scanned to find entities
//   
// RETURN VALUE: None
//  
// REMARKS:      None
//---------------------------------------------------------------------------
CSensorEar::	CSensorEar(CBrainAnimal* pBrain, std::vector<double>& tWeightRate, int range)
:CSensor(8, tWeightRate, UID_BASE_SENS_EAR /*+ 10*range*/)
{
  m_pBrain = pBrain;
  m_nRange = range;
  m_Label = "Ear";
}

CSensorEar::~CSensorEar()
{

}

//===========================================================================
// Virtual overload
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorEar::UpdateAndGetStimulationTable
//  
// DESCRIPTION:  Stimulate a table of sensor according to Eared entities
//               The Weight of captors is taken into account here 
// 
// ARGUMENTS:    sensorValType*& pStimulationVal : Pointer on result table
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
const std::vector<sensorValType>& CSensorEar::UpdateAndGetStimulationTable()
{
  // Reinit m_tStimulationValues
  std::fill(m_tStimulationValues.begin(), m_tStimulationValues.end(), 0);

  CAnimal* pAnimal = m_pBrain->getAnimal();
  
  // Fill Sector 1
  int direction = pAnimal->getDirection();
  int offset = 0;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+1) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+7) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+2) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+6) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+3) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+5) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  direction = (pAnimal->getDirection()+4) % 8;
  offset++;
  Scan45degSector(&(m_tStimulationValues[offset]), direction);

  applySubCaptorWeightRate();
  
  return m_tStimulationValues;
}

//===========================================================================
// other functions
//===========================================================================
//---------------------------------------------------------------------------
// METHOD:       CSensorEar::Scan45degSector
//  
// DESCRIPTION:  Stimulate a table of ensor according to Eared in 45o direction 
// 
// ARGUMENTS:    sensorValType* pStimulationVal : Pointer on result table
//               int maxNumFocusObject : number of entities to be set
//               int direction : {0..7} , int layer : {0..max layer}
//   
// RETURN VALUE: int: Number of returned values in pStimulationVal
//  
// REMARKS:      Do not delete *pStimulationVal
//---------------------------------------------------------------------------
bool CSensorEar::Scan45degSector(sensorValType* pStimulationVal,
                                 int direction)
{
  CBasicEntity* pCurEntity = NULL;
  CAnimal* pAnimal = m_pBrain->getAnimal();
  CBiotop* pBiotop = pAnimal->getBiotop();
  UCHAR visionSectorBmp = 0x01 << direction;
  double noiseRate = 0;

  // Scan all over ground layers
  for (size_t i=1; i<pBiotop->getNbLayer(); i++)
  {
    // Find entities according to angle, distance and layer:
    const std::vector<FoundEntity_t>& tFoundIds = pBiotop->findEntities(pAnimal->getGridCoord(), visionSectorBmp, m_nRange, i, true);
  
    for (size_t j = 0; j < tFoundIds.size(); j++)
    {
      pCurEntity = tFoundIds[j].pEntity;
      if (pCurEntity!=NULL)
      {
        noiseRate += pCurEntity->getNoise()/(tFoundIds[j].distance);
      } 
    }
  }

  pStimulationVal[0] = noiseRate * MAX_SENSOR_VAL / 100.0;

  return (true);
}

//---------------------------------------------------------------------------
// METHOD:       CSensorEar::GetSubCaptorLabel
//  
// DESCRIPTION:  Return a description string for each sub-captor
//
// ARGUMENTS:    int index
//   
// RETURN VALUE: CString: Description string
//  
// REMARKS:      
//---------------------------------------------------------------------------
string CSensorEar::GetSubCaptorLabel(size_t index)
{
  return (CBasicEntity::getRelativePosStrName(index));
}


bool CSensorEar::IsSexSpecific(size_t captorIndex)
{
  return false;
}