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

// Sprite.h: interface for the CAnimatedSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CANIMATEDSPRITE_H)
#define CANIMATEDSPRITE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h"
#include "BiotopDisplay.h"

#include "stdafx.h"
#include <afxtempl.h>
#include "Texture.h"
#include "Sprite.h"

#define SIZE_SPRITE_X 64
#define SIZE_SPRITE_Y 64

typedef struct 
{
  entityIdType entityId ;
  CSprite*     pSprite;
  double       absCoordX;
  double       absCoordY;
  int          targetCoordX;
  int          targetCoordY;
  double       stepSizeX;
  double       stepSizeY;
  int          curStep;
  int          curPos;
  bool         noStep;
  GLbyte       red;
  GLbyte       green;
  GLbyte       blue;
  double       weightScale;
} AnimatedSpriteInstance_t;


class CAnimatedSprite  
{
public:
  CAnimatedSprite(CString spriteName,int nbDirections, int nbPositions, int nbLifeStages, int nbActions, int pSelectSpriteId);
  virtual ~CAnimatedSprite();

  bool LoadData(CString directoryName,int idLifeStage, int idAction, bool compressed = false);
  int  GetInstance(entityIdType entityId, COLORREF color, double weight);
  void MoveSprite(int instanceId, int absx, int absy);
  void MoveSpriteByStep(int instanceId, int absx, int absy);
  void DrawSprite(int instanceId, int offsetx, int offsety, int direction, int lifeStage, int action, bool isSelect = true);
  void SetStepMode(int instanceId, bool noStep);
  void ResetSteps(int instanceId);

  void SetZoomFactor(double zoomFactor);
  void SetMaxStep (int maxStep) { m_nMaxStep = maxStep; };

  CString GetSpriteName(void);

  int GetNbActions();

  //int m_idActionWalk; // id to be used instead of action nb when walking
  //int m_idActionRun;  // id to be used instead of action nb when running

private:
  CString m_SpriteName;
  GLuint* m_bitmapIdsArray;
  CTexture m_spriteTextures;	
  double m_ZoomFactor;
  int m_nDirections;
  int m_nPositions;
  int m_nLifeStages;
  int m_nActions;
  int m_nMaxStep;

  // Instance table
  CArray<AnimatedSpriteInstance_t*,AnimatedSpriteInstance_t*> m_tSpriteInstance;
  // Select sprite
  int m_pSelectSpriteId;
};

#endif // !defined(CANIMATEDSPRITE_H)
