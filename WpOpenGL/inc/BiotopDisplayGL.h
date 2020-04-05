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

// BiotopDisplay.h : interface of the CBiotopDisplay class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIOTOPDISPLAYGL_INCLUDED_)
#define AFX_BIOTOPDISPLAYGL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h"

#include "Texture.h"
#include "Map.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

#define SIZE_ARROW_X 32
#define SIZE_ARROW_Y 32

typedef struct 
{
  CBasicEntity* pEntity;
  long          displayPriority;
  int           newMapCoordX;
  int           newMapCoordY;
  int           spriteOffsetX;
  int           spriteOffsetY;
} DisplayPriority_t;


class CBiotopDisplayGL : public CBiotopDisplay
{

//Attributes
protected:

  // Dimensions
  int m_BiotopSizeX;  // Biotop width
  int m_BiotopSizeY;  // Biotop height

  double m_ZoomFactor;

  // Windows
  HDC m_hDC; 
  HGLRC m_hRC;							// Rendering Context
  HPALETTE m_hPal;
  CPoint pt_MousePosition;
  UINT m_cxClient;  // Screen width
  UINT m_cyClient;  // Screen height

	//frame calcs and time since last frame
  float m_fStartFrameTime;
  float m_fEndFrameTime;
  float m_fTime;
  int m_iFrames;
  int m_iFramesPerSec;

  // Map drawing
  CMapTile  m_Map;
  CSprite   m_SprArrow;
  CSprite   m_SprSquare;
  CTexture  m_appTextures;	
  GLuint m_appBindTexArray[100]; // FRED Use a define!

  // Sprite Drawing
  CArray<DisplayPriority_t,DisplayPriority_t&> m_tSpriteDisplayPrio;
  int CURSOR_LEFT_ID; // id value of texture for mouse border sprite
  int CURSOR_RIGHT_ID;
  int CURSOR_DOWN_ID;
  int CURSOR_TOP_ID;
  int SELECT_ID;
  int SQUARE_ID;
  int m_RefreshCounter;

// Operations
public:
	CBiotopDisplayGL();
  ~CBiotopDisplayGL();

public:
// Virtual overload
  bool Initialize(CScrollView* pView);
  void DisplayView();
  void ResizeScene(int cx, int cy);
  Point_t GetGridCoordFromScreenPos(CPoint screenPos);
  void RefreshScene();
  void RefreshBiotop();
  void RefreshNextSecond();
  void RedrawSceneIdleNoCPU();
  void KillWindow(void);
  void OnMouseMove(CPoint point);
  void ScrollToGridCoord(Point_t centerPos); 
  Point_t GetCurrentGridCenterPos();
  void SetLuminosity (double rate);
  void SetZoomFactor (double zoomFactor);
  CAnimatedSprite* GetAnimatedSprite(CBasicEntity* pEntity);

protected:

  void RenderScene(bool reset = false);
  void ResetAllSprites();

  int LoadData(void);
  //void calculateFramesPerSec();

  void ConvertCoordBiotopGridToMap (int cbx,int cby, int &cmx, int &cmy);
  void ConvertCoordMapToBiotopGrid (int cmx,int cmy, int &cbx, int &cby);
  void ConvertCoordBiotopStepToMapStep (int cbx,int cby, int &cmx, int &cmy);
  void ConvertCoordMapToBiotopStep (int cmx,int cmy, int &cbx, int &cby);

  bool BuildMapFromBiotop();
  
  void GetVisibleBiotopCoord(int &cbx_min, int &cby_min, int &cbx_max, int &cby_max);
  void DrawAllVisibleEntities(bool noStep = false);

private:
  CArray<CAnimatedSprite*,CAnimatedSprite*> m_tAnimatedSprite;

};

#endif // !defined(AFX_BIOTOPDISPLAYGL_INCLUDED_)
