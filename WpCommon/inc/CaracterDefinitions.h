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

#if !defined(CARACTERDEF_INCLUDED_)
#define CARACTERDEF_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"

#define MAX_VAL_WEIGHT  1000

enum ReproType_e
{
  REPRODUCT_UNSET = -1,
  REPRODUCT_NONE,
  REPRODUCT_CLONAGE,
  REPRODUCT_SEXUAL, 
  REPRODUCT_NUMBER_TYPE,  
};

enum ColorCaracterType_e
{
  COLOR_CARACTER_UNSET = -1,
  COLOR_CARACTER_FIRST_TYPE,
  COLOR_CARACTER_WHITE = COLOR_CARACTER_FIRST_TYPE,
  COLOR_CARACTER_BLACK,
  COLOR_CARACTER_GREY,
  COLOR_CARACTER_RED,
  COLOR_CARACTER_BLUE,
  COLOR_CARACTER_GREEN,
  COLOR_CARACTER_YELLOW,
  COLOR_CARACTER_ORANGE,
  COLOR_CARACTER_PINK,
  COLOR_CARACTER_BEIGE,
  COLOR_CARACTER_BROWN,
  COLOR_CARACTER_NUMBER_TYPE
};

enum TextureType_e
{
  TEXTURE_UNSET = -1,
  TEXTURE_NONE,
  TEXTURE_FIRST_TYPE,
  TEXTURE_STRIPPED = TEXTURE_FIRST_TYPE,
  TEXTURE_SPOTTED,
  TEXTURE_REFLECT, 
  TEXTURE_NUMBER_TYPE
};

enum PhyAttributeType_e
{
  PHY_ATTRIBUTE_UNSET = -1,
  PHY_ATTRIBUTE_NONE,
  PHY_ATTRIBUTE_FIRST_TYPE,
  PHY_ATTRIBUTE_UDDER = PHY_ATTRIBUTE_FIRST_TYPE,
  PHY_ATTRIBUTE_HORNS,
  PHY_ATTRIBUTE_TUSKS,
  PHY_ATTRIBUTE_MANE,
  PHY_ATTRIBUTE_NUMBER_TYPE
};

enum OdorType_e
{
  ODOR_UNSET = -1,
  ODOR_NONE,
  ODOR_FIRST_TYPE,
  ODOR_AROMATIC = ODOR_FIRST_TYPE,
  ODOR_FRAGRANT,
  ODOR_ALLIACEOUS,
  ODOR_AMBROSIAL,
  ODOR_HIRCINE,
  ODOR_REPULSIVE,
  ODOR_NAUSEOUS,
  ODOR_NUMBER_TYPE
};

enum PheromoneType_e
{
  PHEROMONE_UNSET = -1,
  PHEROMONE_NONE,
  PHEROMONE_FIRST_TYPE,
  PHEROMONE_SEXUAL_MALE = PHEROMONE_FIRST_TYPE,
  PHEROMONE_SEXUAL_FEMALE,
  PHEROMONE_MATERNAL,
  PHEROMONE_BABY,
  PHEROMONE_DOMINANT,
  PHEROMONE_NUMBER_TYPE
};

enum FormType_e
{
  FORM_UNSET = -1,
  FORM_FIRST_TYPE,
  FORM_UNKNOWN = FORM_FIRST_TYPE,
  FORM_PLAN_SMOOTH,
  FORM_PLAN_ROUGH,
  FORM_ROUNDISH,
  FORM_GEOMETRIC,
  FORM_VEGET_LONG_LEEF,
  FORM_VEGET_BUSH,
  FORM_VEGET_TREE,
  FORM_QUADRIPED_THIN,
  FORM_QUADRIPED_LONG_NECK,
  FORM_QUADRIPED_FAT,
  FORM_BIPED,
  FORM_CARRION,
// TBD: complete form type
  FORM_NUMBER_TYPE  
};

enum TasteType_e
{
  TASTE_UNSET = -1,
  TASTE_NONE,
  TASTE_SWEET,
  TASTE_ACID,
  TASTE_SALTY,
  TASTE_BITTER,
  TASTE_UMANI,
  TASTE_NUMBER_TYPE,  
};

enum HabitatType_e
{
  HABITAT_UNSET = -1,
  HABITAT_FIRST_TYPE,
  HABITAT_EARTH = HABITAT_FIRST_TYPE,
  HABITAT_AMPHIBIAN, 
  HABITAT_WATER, 
  HABITAT_AIR, 
  HABITAT_NUMBER_TYPE,  
};

enum ConsumeType_e
{
  CONSUM_UNSET = -1,
  CONSUM_NONE,
  CONSUM_VEGETAL, 
  CONSUM_OMNI,
  CONSUM_MEAT,
  CONSUM_NUMBER_TYPE,  
};

enum MoveType_e
{
  MOVE_UNSET = -1,
  MOVE_NONE,
  MOVE_WALK, 
// TBD: complete move type
  MOVE_NUMBER_TYPE  
};

// CPU optim: use Macro
#define indexToColorType(index)  (ColorCaracterType_e)(index + COLOR_CARACTER_FIRST_TYPE)
#define indexToTextureType(index) (TextureType_e)(index + TEXTURE_FIRST_TYPE)
#define indexToFormType(index) (FormType_e)(index + FORM_FIRST_TYPE)
#define indexToAttributeType(index) (PhyAttributeType_e)(index + PHY_ATTRIBUTE_FIRST_TYPE)
#define indexToOdorType(index) (OdorType_e)(index + ODOR_FIRST_TYPE)

#define PheromoneTypeToIndex(type) (type - PHEROMONE_FIRST_TYPE)
#define TextureTypeToIndex(type) (type - TEXTURE_FIRST_TYPE)
#define OdorTypeToIndex(type) (type - ODOR_FIRST_TYPE)
#define ColorTypeToIndex(type) (type - COLOR_CARACTER_FIRST_TYPE)
#define FormTypeToIndex(type) (type - FORM_FIRST_TYPE)
#define AttributeTypeToIndex(type) (type - PHY_ATTRIBUTE_FIRST_TYPE)

#endif // !defined(CARACTERDEF_INCLUDED_)


