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

#if !defined(AFX_BIOTOPDISPLAY_INCLUDED_)
#define AFX_BIOTOPDISPLAY_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CBiotop.h"

class CBiotopDisplay
{

// Operations
public:
	CBiotopDisplay()  {m_pBiotop = NULL; m_nSelectedEntityId = -1; m_nFocusedEntityId = -1; m_bLButtonIsDown = false;}; 
  ~CBiotopDisplay() {0;}; 

public:
  virtual bool Initialize(CScrollView* pView) {return(false);}; 
  virtual void ResizeScene(int cx, int cy)  {0;}; 
  virtual Point_t GetGridCoordFromScreenPos(CPoint screenPos) {Point_t bioCoord={-1,-1};return(bioCoord);}; 
  virtual void RefreshScene()   {0;};
  virtual void RefreshBiotop()  {0;};
  virtual void RefreshSceneIdleNoCPU()   {0;};
  virtual void RedrawScene()    {0;}; 
  virtual void RedrawSceneIdleNoCPU()    {0;}; 
  virtual void KillWindow(void) {0;}; 
  virtual void OnMouseMove(CPoint point)  {0;};
  virtual void SetZoomFactor (double zoomFactor) {0;}; 
  virtual void RefreshNextSecond() {0;}; 
  virtual void DisplayView()    {0;}; 
  virtual void ScrollToGridCoord(Point_t centerPos)  {0;}; 
  virtual Point_t GetCurrentGridCenterPos()  { Point_t bioCoord={0,0};return(bioCoord);}

  void SetBiotop(CBiotop* pBiotop) {m_pBiotop = pBiotop;}; 
  void SetSelectedEntityId(entityIdType id) {m_nSelectedEntityId = id;};
  void SetFocusedEntityId(entityIdType id) {m_nFocusedEntityId = id;};
  void SetLButtonIsDown(bool isDown) {m_bLButtonIsDown = isDown;}; 
  void SetNbRefreshPerStep (int nbRefresh) {m_nbRefreshPerStep = nbRefresh;};

//Attributes
protected:
  CBiotop*      m_pBiotop;
  bool          m_bAppIsActive;
  CScrollView*  m_pView;								// Window Context
  entityIdType  m_nSelectedEntityId;
  entityIdType  m_nFocusedEntityId;
  bool          m_bLButtonIsDown;
  int           m_nbRefreshPerStep;

};

#endif // !defined(AFX_BIOTOPDISPLAY_INCLUDED_)
