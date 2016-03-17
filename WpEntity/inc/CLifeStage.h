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
// FILE: CLifeStage.h
//   
// GENERAL DESCRIPTION:
//         This CLASS represents a life stage for an entity.
//         Behavior and parameters of any entity are updated when stage change
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  20/08/2005 : Creation
//
//===========================================================================

#if !defined( CLIFESTAGE_INCLUDED_)
#define CLIFESTAGE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
//#include <afxtempl.h>

#include "Definitions.h"

//===========================================================================
// Definitions            
//===========================================================================

// Note: Stage are sort according to the value in this enum.
//       Keep care that the order remains chronological
typedef enum
{
  STAGE_0 = 0,// fetus,egg,seed,...
  STAGE_1,    // baby,larva,...
  STAGE_2,    // child,young
  STAGE_3,    // adult
  STAGE_4,    // old
  STAGE_5,    // dead
  STAGE_6,    // roten
} LifeStage_e;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CLifeStage
{
//===========================================================================
// Attributes 
//===========================================================================

//---------------------------------------------------------------------------
// internal proprieties:
//---------------------------------------------------------------------------
protected:
  string        m_Label;        // User defined name
  LifeStage_e   m_StageType;
  int           m_TotalDayDuration;
  int           m_CurrentDayCounter;


//===========================================================================
// methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------  
public:
  CLifeStage(string label, LifeStage_e type, int dayDuration);

//---------------------------------------------------------------------------
// Time management
//---------------------------------------------------------------------------
public:
  bool increaseDayCountAndCheckEnd(void);

//---------------------------------------------------------------------------
// Get / Set for attributes
//---------------------------------------------------------------------------
public:
  LifeStage_e getStageType(void);
  string getLabel(void);
  int getTotalDayDuration(void);
  void setTotalDayDuration(int newDuration);
  int getCurDayCount(void);
  void setCurDayCount(int newDayCount);

};// end CLifeStage

#endif // !defined(CLIFESTAGE_INCLUDED_)


