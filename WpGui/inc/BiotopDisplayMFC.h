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

// BiotopDisplayMFC.h : interface of the CBiotopDisplay class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIOTOPDISPLAYMFC_INCLUDED_)
#define AFX_BIOTOPDISPLAYMFC_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h"
#include "BiotopDisplay.h"

class CBiotopDisplayMFC : public CBiotopDisplay
{

//Attributes
protected:

	int m_nBitmapNumberY;
	int m_nBitmapNumberX;
	int m_nBitmapPixSizeY;
	int m_nBitmapPixSizeX;

	int m_curViewSizeX;
	int m_curViewSizeY;

  double m_ZoomFactor;
  bool m_SatelliteView;

// Operations
public:
	CBiotopDisplayMFC(bool isSatelliteVew);
  ~CBiotopDisplayMFC();

public:
// Virtual overload
  bool Initialize(CScrollView* pView);
  void DisplayView();
  void ResizeScene(int cx, int cy);
  Point_t GetGridCoordFromScreenPos(CPoint screenPos);
  void RefreshNextSecond();
  //void RefreshScene();
  void RefreshSceneIdleNoCPU();
  void RedrawScene();
  void RedrawSceneIdleNoCPU();
  void KillWindow(void);
  void OnMouseMove(CPoint point);
  void ScrollToGridCoord(Point_t centerPos); 
  Point_t GetCurrentGridCenterPos();
  Point_t GetVisibleGridCoordStart();
  Point_t GetVisibleGridCoordEnd();
  void SetZoomFactor(double zoomFactor);

  CBitmap m_bmpVeget;
  CBitmap m_bmpAnim[7];
  CBitmap m_bmpMineral;

protected:
  void RedrawSceneAerialView();
  void RedrawSceneSatelliteView();
  void InvalidateRectForOneEntity(CBasicEntity* pEntity);
};

#endif // !defined(AFX_BIOTOPDISPLAYMFC_INCLUDED_)
