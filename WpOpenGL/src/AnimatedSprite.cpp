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

// Sprite.cpp: implementation of the CAnimatedSprite class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafxGL.h"
#include "AnimatedSprite.h"
//#include "Deflator.h"
#include "Cybiosphere.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCybiosphereApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimatedSprite::CAnimatedSprite(CString spriteName,int nbDirections, int nbPositions, int nbLifeStages, int nbActions, int pSelectSpriteId)
{
  m_SpriteName = spriteName;

  m_nDirections = nbDirections;
  m_nPositions  = nbPositions;
  m_nLifeStages = nbLifeStages;
  m_nActions    = nbActions;
  m_pSelectSpriteId = pSelectSpriteId;
  
  m_bitmapIdsArray = new GLuint[2 * m_nDirections * m_nPositions * m_nLifeStages * m_nActions];

  m_ZoomFactor = 1;
  m_nMaxStep = 20;//9;
  m_tSpriteInstance.SetSize(0);
}

CAnimatedSprite::~CAnimatedSprite()
{
  for (int i=m_tSpriteInstance.GetSize()-1; i>=0; i--) 
  {
    if (m_tSpriteInstance.GetAt(i) != NULL)
    {  
      if (m_tSpriteInstance.GetAt(i)->pSprite != NULL)
        delete (m_tSpriteInstance.GetAt(i)->pSprite); 

      delete (m_tSpriteInstance.GetAt(i)); 
    }

    m_tSpriteInstance.RemoveAt(i);
  }
  delete[] m_bitmapIdsArray;
}

bool CAnimatedSprite::LoadData(CString directoryName,int idLifeStage, int idAction, bool compressed)
{
  CString buff;

  int index = 2 * (idLifeStage*m_nActions + idAction) * m_nDirections * m_nPositions;
  int nPos;

  // TBD: loop also on nbLifeStages
  for( int i = 0; i < m_nPositions; i++)
  {
      buff.Format("%s\\Pos%d.bmp",directoryName,i);
      buff = theApp.GetAppliPath() + buff;
      nPos = m_spriteTextures.LoadGLTexImage2DGroup(buff.GetBuffer(0),16,&m_bitmapIdsArray[index + 16*i]);
  }

	/*for( int j = 0; j < m_nDirections; j++)
	{
	  for( int i = 0; i < m_nPositions; i++)
	  {
        buff.Format("%s\\Dir%d_Pos%d_Mask.bmp",directoryName,j,i);
        buff = theApp.GetAppliPath() + buff;
      m_bitmapIdsArray[index] = m_spriteTextures.LoadGLTexImage2DMipMap(buff.GetBuffer(0));
      if ( (m_bitmapIdsArray[index] == 0) && (j == 0) ) // Skip following position
      {
        m_nPositions = i;
        break;
      }
      index++;
		  buff.Format("%s\\Dir%d_Pos%d.bmp",directoryName,j,i);
      buff = theApp.GetAppliPath() + buff;
      m_bitmapIdsArray[index] = m_spriteTextures.LoadGLTexImage2DMipMap(buff.GetBuffer(0));
	    index++;

      // to use zip:
		  /*buff.Format("%s/Walk_Age0_Dir%d_Pos%d_Mask.bmp",directoryName,j,i);
      bool resu = extractAndSaveFile("mouton.zip",buff.GetBuffer(0),"toto.bmp");

      m_bitmapIdsArray[index] = m_spriteTextures.LoadGLTexImage2DMipMap("toto.bmp");
      index++;

		  buff.Format("%s/Walk_Age0_Dir%d_Pos%d.bmp",directoryName,j,i);
      resu = extractAndSaveFile("mouton.zip",buff.GetBuffer(0),"titi.bmp");

      m_bitmapIdsArray[index] = m_spriteTextures.LoadGLTexImage2DMipMap("titi.bmp");
	    index++;*/
//    }
//  }

  return (true);
}

int CAnimatedSprite::GetInstance(entityIdType entityId, COLORREF color, double weight)
{
  int instanceId = -1;
  for (int i=0; i<m_tSpriteInstance.GetSize(); i++)
  {
    if (m_tSpriteInstance.GetAt(i)->entityId == entityId)
    {
      instanceId = i;
      m_tSpriteInstance.GetAt(i)->weightScale  = min(weight/200 + 0.5, 1);
    }
  }

  if (instanceId == -1)
  {
    AnimatedSpriteInstance_t* pInstance = new AnimatedSpriteInstance_t;
    CSprite* pSpr = new CSprite();
    pSpr->SetSpriteSize ( (int)floor(m_ZoomFactor*(double)m_spriteTextures.m_SizeX), (int)floor(m_ZoomFactor*(double)m_spriteTextures.m_SizeY));

    pInstance->entityId     = entityId;
    pInstance->pSprite      = pSpr;
    pInstance->absCoordX    = -1;
    pInstance->absCoordY    = -1;
    pInstance->targetCoordX = -1;
    pInstance->targetCoordY = -1;
    pInstance->stepSizeX    = 0;
    pInstance->stepSizeY    = 0;
    pInstance->curPos       = 0;
    pInstance->curStep      = 0;    
    pInstance->noStep       = true;
    pInstance->red          = (color&0xFF) / 3;
    pInstance->green        = ((color&0xFF00)>>8) / 3;
    pInstance->blue         = ((color&0xFF0000)>>16) / 3;
    pInstance->weightScale  = min(weight/200 + 0.5, 1);
    instanceId = m_tSpriteInstance.Add(pInstance);
  }
  
  return (instanceId);
}

void CAnimatedSprite::MoveSprite(int instanceId, int absx, int absy)
{
  AnimatedSpriteInstance_t* pInstance = m_tSpriteInstance.GetAt(instanceId);

  if (pInstance != NULL)
  {
    pInstance->absCoordX    = absx;
    pInstance->absCoordY    = absy;
    pInstance->targetCoordX = absx;
    pInstance->targetCoordY = absy;
    pInstance->stepSizeX    = 0;
    pInstance->stepSizeY    = 0;
    pInstance->curPos       = 0;
    pInstance->curStep      = 0;  
  }
}

void CAnimatedSprite::SetStepMode(int instanceId, bool noStep)
{
  //m_nMaxStep = nbSteps;
  AnimatedSpriteInstance_t* pInstance = m_tSpriteInstance.GetAt(instanceId);
  if (pInstance != NULL)
  {
    pInstance->noStep = noStep;
    if (noStep)
    {
      pInstance->absCoordX    = -1;
      pInstance->absCoordY    = -1;
      pInstance->targetCoordX = -1;
      pInstance->targetCoordY = -1;
      pInstance->stepSizeX = 0;
      pInstance->stepSizeY = 0;
      pInstance->curPos    = 0;
      pInstance->curStep   = 0; 
    }
  }
}

void CAnimatedSprite::ResetSteps(int instanceId)
{
  AnimatedSpriteInstance_t* pInstance = m_tSpriteInstance.GetAt(instanceId);
  if (pInstance != NULL)
  {
    pInstance->curPos    = 0;
    pInstance->curStep   = 0; 
  }
}

void CAnimatedSprite::MoveSpriteByStep(int instanceId, int absx, int absy)
{
  AnimatedSpriteInstance_t* pInstance = m_tSpriteInstance.GetAt(instanceId);

  if (pInstance != NULL)
  {
    pInstance->targetCoordX = absx;
    pInstance->targetCoordY = absy;

    if (pInstance->noStep == true) // No step => re-init
    {
      pInstance->absCoordX    = absx;
      pInstance->absCoordY    = absy;
    }
    else
    {
      // if new step, process step
      if (pInstance->curStep == 0)
      {
        if ( pInstance->absCoordX != pInstance->targetCoordX )
        {
          pInstance->stepSizeX = (pInstance->targetCoordX - pInstance->absCoordX) / m_nMaxStep;
        }
        else
        {
          pInstance->stepSizeX = 0;
        }
        if ( pInstance->absCoordY != pInstance->targetCoordY ) 
        {
          pInstance->stepSizeY = (pInstance->targetCoordY - pInstance->absCoordY) / m_nMaxStep;
        }
        else
        {
          pInstance->stepSizeY = 0;
        }
      }
      // Move step and display
      pInstance->absCoordX += pInstance->stepSizeX;
      pInstance->absCoordY += pInstance->stepSizeY;

      // Go next step
      pInstance->curStep = (pInstance->curStep + 1)%m_nMaxStep;
      // Go next Position
      pInstance->curPos = (int)( (double)pInstance->curStep * (double)m_nPositions / (double)m_nMaxStep );
    }
    // Reset no step mode (must be force at each move
    pInstance->noStep = true; 
  }
}

void CAnimatedSprite::DrawSprite(int instanceId, int offsetx, int offsety, int direction, int idLifeStage, int idAction, bool isSelect)
{
  AnimatedSpriteInstance_t* pInstance = m_tSpriteInstance.GetAt(instanceId);

  if (pInstance != NULL)
  {
    int absx = pInstance->absCoordX;
    int absy = pInstance->absCoordY;
    int defWidth  = pInstance->pSprite->GetSpriteWidth();
    int defHeight = pInstance->pSprite->GetSpriteHeight();
    int width  = defWidth * pInstance->weightScale;
    int height = defHeight * pInstance->weightScale;

    // use WeightScale
    pInstance->pSprite->SetSpriteSize(width, height);
    pInstance->pSprite->MoveSpriteAtPosition(absx /*- width/2*/ + offsetx, absy - height/2 + offsety);
    if (isSelect)
      pInstance->pSprite->DrawSprite(m_pSelectSpriteId);

    int index = 2 * ( direction + m_nDirections * ( pInstance->curPos + m_nPositions * (idAction + m_nActions*idLifeStage) ) );

    pInstance->pSprite->DrawSprite(m_bitmapIdsArray[index], pInstance->red, pInstance->green, pInstance->blue);

    // Reset Sprite def scale
    pInstance->pSprite->SetSpriteSize(defWidth, defHeight);
  }
}

void CAnimatedSprite::SetZoomFactor(double zoomFactor) 
{ 
  m_ZoomFactor = zoomFactor; 

  for (int i=0; i<m_tSpriteInstance.GetSize(); i++)
  {
    m_tSpriteInstance.GetAt(i)->pSprite->SetSpriteSize ( (int)floor(m_ZoomFactor*(double)m_spriteTextures.m_SizeX), (int)floor(m_ZoomFactor*(double)m_spriteTextures.m_SizeY));
  }
}

CString CAnimatedSprite::GetSpriteName(void)
{
  return (m_SpriteName);
}

int  CAnimatedSprite::GetNbActions()
{
  return (m_nActions);
}