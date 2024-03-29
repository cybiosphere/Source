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
// FILE: CSensorView.h
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

#if !defined(SENSORVIEW_H)
#define SENSORVIEW_H

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

#define VIEW_NUMBER_COLORS       ( COLOR_CARACTER_NUMBER_TYPE - COLOR_CARACTER_FIRST_TYPE )
#define VIEW_NUMBER_FORMS        ( FORM_NUMBER_TYPE - FORM_FIRST_TYPE )
#define VIEW_NUMBER_TEXTURES     ( TEXTURE_NUMBER_TYPE - TEXTURE_FIRST_TYPE )
#define VIEW_NUMBER_PHY_ATTRIBUT ( PHY_ATTRIBUTE_NUMBER_TYPE - PHY_ATTRIBUTE_FIRST_TYPE )

#define VIEW_SIZE_PER_FOCUS ( 4 + VIEW_NUMBER_COLORS + VIEW_NUMBER_FORMS + VIEW_NUMBER_TEXTURES + VIEW_NUMBER_PHY_ATTRIBUT )

#define VIEW_MAX_NUMBER_FOCUSSED_ENTITIES 100

typedef struct 
{
  CBasicEntity*        pEntity;
  size_t               index;
  entitySignatureType  signature;
  double               computedWeight;
  double               weightTab[VIEW_SIZE_PER_FOCUS];
} EntityViewWeight_t;


//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CSensorView : public CSensor  
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal data
//---------------------------------------------------------------------------
private:
  int             m_nRange;
  size_t          m_Layer;
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
  EntityViewWeight_t m_pEntityViewTab[MAX_FOUND_ENTITIES];

//---------------------------------------------------------------------------
// associations
//---------------------------------------------------------------------------
private:
  CBrainAnimal* m_pBrain;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CSensorView(CBrainAnimal* pBrain, 
              std::vector<double>& tWeightRate,
              int range, 
              size_t layer,
              ViewAngleType_e angle,
              int totalNbFocusObjects,
              bool distanceEvaluation = true);
	virtual ~CSensorView();

//---------------------------------------------------------------------------
// Virtual overload
//---------------------------------------------------------------------------
public:
  const std::vector<sensorValType>& UpdateAndGetStimulationTable();
  string GetSubCaptorLabel(size_t index);

//---------------------------------------------------------------------------
// other functions
//---------------------------------------------------------------------------
public:

  int GetRange();
  size_t GetLayer();
  size_t GetSubCaptorIndexForDirection(int relativeDirection, size_t entityIndex);

  static size_t GetSubCaptorSubIndexForSizeBig();
  static size_t GetSubCaptorSubIndexForSizeSmall();
  static size_t GetSubCaptorSubIndexForMoving();
  static size_t GetSubCaptorSubIndexForProximity();
  static size_t GetSubCaptorSubIndexForForm(FormType_e form);
  static size_t GetSubCaptorSubIndexForColor(ColorCaracterType_e color);
  static size_t GetSubCaptorSubIndexForTexture(TextureType_e texture);
  static size_t GetSubCaptorSubIndexForPhyAttribute(PhyAttributeType_e attribute);


private:
  bool Scan45degSector(size_t stimulationTabOffset, size_t maxNumFocusObject, int direction);

};

#endif // !defined(SENSORVIEW_H)
