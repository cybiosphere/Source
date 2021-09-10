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
// FILE: CSensorViewIdentifyFar.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a sensor for view
//         It is a set of Y = totalNbFocusObjects * (FORM_NUMBER_TYPE + 6) 
//         sub-sensors dedicated, for each FocusObjects to:
//         Size,Proximity,Red,Green,Blue,Reflection,FORM_0,..,FORM_N      
//         A Y sized weight table must be used to set visible aspect sentibility.
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  10/10/2005 : Creation
//
//===========================================================================

#if !defined(SENSORVIEWIDENTIFYFAR_H)
#define SENSORVIEWIDENTIFYFAR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "CSensor.h"
#include "CBrainAnimal.h"

//===========================================================================
// Definitions            
//===========================================================================
class CBrainAnimal;
#define VIEW_IDENTIFY_SIZE_PER_DYNAMIC 5
#define VIEW_IDENTIFY_SIZE_PER_FOCUS ( IDENTIFICATION_STATIC_NUMBER_TYPE + VIEW_IDENTIFY_SIZE_PER_DYNAMIC*IDENTIFICATION_MOBILE_NUMBER_TYPE )

typedef struct 
{
  CBasicEntity*        pEntity;
  size_t               index;
  entitySignatureType  signature;
  double               computedWeight;
  double               weightTab[VIEW_IDENTIFY_SIZE_PER_FOCUS];
} EntityViewIdentifyFarWeight_t;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorViewIdentifyFar : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
  int             m_nRangeMin;
  int             m_nRangeMax;
  ViewAngleType_e m_Angle;
  bool            m_bDistanceEval;
  // Sect6 Sect4 Sect2
  // Sect8   0>  Sect1
  // Sect7 Sect5 Sect3
  size_t          m_nFocusObjectsSect1; 
  size_t          m_nFocusObjectsSect2;
  size_t          m_nFocusObjectsSect3;
  size_t          m_nFocusObjectsSect4;
  size_t          m_nFocusObjectsSect5;
  size_t          m_nFocusObjectsSect6;
  size_t          m_nFocusObjectsSect7;
  size_t          m_nFocusObjectsSect8;
  EntityViewIdentifyFarWeight_t m_pEntityViewIdentifyFarTab[MAX_FOUND_ENTITIES];

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  CBrainAnimal*                 m_pBrain;
  EntityViewIdentifyFarWeight_t m_followedEntityWeight;
  CBasicEntity*                 m_pFollowedEntity;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CSensorViewIdentifyFar(CBrainAnimal* pBrain, 
              std::vector<double>& tWeightRate,
              int rangeMin, 
              int rangeMax,
              ViewAngleType_e angle,
              int totalNbFocusObjects,
              bool distanceEvaluation = true);
	virtual ~CSensorViewIdentifyFar();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  const std::vector<sensorValType>& UpdateAndGetStimulationTable();
  string GetSubCaptorLabel(size_t index);
  bool IsSexSpecific(size_t captorIndex);

//---------------------------------------------------------------------------
// other functions
//---------------------------------------------------------------------------
public:
  size_t GetSubCaptorIndexForDirection(int relativeDirection, size_t entityIndex);
  double GetViewedEntityWeight(CBasicEntity* pEntity);

  static size_t GetSubCaptorSubIndexForProximity(IdentificationType_e identity);
  static size_t GetSubCaptorSubIndexForEscapeSpeed(IdentificationType_e identity);
  static size_t GetSubCaptorSubIndexForApproachSpeed(IdentificationType_e identity);
  static size_t GetSubCaptorSubIndexForDirLeft(IdentificationType_e identity);
  static size_t GetSubCaptorSubIndexForDirRight(IdentificationType_e identity);

private:
  bool Scan45degSector(size_t stimulationTabOffset, size_t maxNumFocusObject, int direction);

};

#endif // !defined(SENSORVIEWIDENTIFYFAR_H)
