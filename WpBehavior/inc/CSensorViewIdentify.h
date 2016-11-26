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
// FILE: CSensorViewIdentify.h
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

#if !defined(SENSORVIEWIDENTITY_H)
#define SENSORVIEWIDENTITY_H

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

#define VIEW_IDENTIFY_SIZE_PER_FOCUS ( IDENTIFICATION_STATIC_NUMBER_TYPE + 5*IDENTIFICATION_MOBILE_NUMBER_TYPE )

typedef struct 
{
  CBasicEntity*        pEntity;
  int                  index;
  entitySignatureType  signature;
  double               computedWeight;
  double               weightTab[VIEW_IDENTIFY_SIZE_PER_FOCUS];
} EntityViewIdentifyWeight_t;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorViewIdentify : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
  int             m_nRange;
  int             m_Layer;
  ViewAngleType_e m_Angle;
  bool            m_bDistanceEval;
  // Sect6 Sect4 Sect2
  // Sect8   0>  Sect1
  // Sect7 Sect5 Sect3
  int             m_nFocusObjectsSect1; 
  int             m_nFocusObjectsSect2; 
  int             m_nFocusObjectsSect3; 
  int             m_nFocusObjectsSect4; 
  int             m_nFocusObjectsSect5; 
  int             m_nFocusObjectsSect6;
  int             m_nFocusObjectsSect7;
  int             m_nFocusObjectsSect8;

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  CBrainAnimal* m_pBrain;
  EntityViewIdentifyWeight_t m_followedEntityWeight;
  CBasicEntity*              m_pFollowedEntity;


//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CSensorViewIdentify(CBrainAnimal* pBrain, 
              double* pWeightRate,
              int range, 
              int layer,
              ViewAngleType_e angle,
              int totalNbFocusObjects,
              bool distanceEvaluation = true);
	virtual ~CSensorViewIdentify();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  int UpdateAndGetStimulationTable(sensorValType*& pStimulationVal);
  string GetSubCaptorLabel(int index);
  bool IsSexSpecific(int captorIndex);

//---------------------------------------------------------------------------
// other functions
//---------------------------------------------------------------------------
public:

  int GetRange();
  int GetLayer();
  int GetSubCaptorIndexForDirection(int relativeDirection, int entityIndex);
  double GetViewedEntityWeight(CBasicEntity* pEntity);

  static int GetSubCaptorSubIndexForProximity(IdentificationType_e identity);
  static int GetSubCaptorSubIndexForEscapeSpeed(IdentificationType_e identity);
  static int GetSubCaptorSubIndexForApproachSpeed(IdentificationType_e identity);
  static int GetSubCaptorSubIndexForDirLeft(IdentificationType_e identity);
  static int GetSubCaptorSubIndexForDirRight(IdentificationType_e identity);


private:
  bool Scan45degSector(int stimulationTabOffset, int maxNumFocusObject, int direction);

};

#endif // !defined(SENSORVIEWIDENTITY_H)
