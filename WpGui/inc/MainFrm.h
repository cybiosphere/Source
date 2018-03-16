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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__25B61CB4_3C2D_4D22_88F4_811679EBD1DC__INCLUDED_)
#define AFX_MAINFRM_H__25B61CB4_3C2D_4D22_88F4_811679EBD1DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum
{
  DISPLAY_MODE_FULL_SIZE =     0,
  DISPLAY_MODE_LAB_GENETIC,
  DISPLAY_MODE_LAB_BRAIN,
  DISPLAY_MODE_STATISTIC,
  DISPLAY_MODE_MAP,
  DISPLAY_MODE_LOGSERVER
} AppliDisplayMode_e;

typedef enum
{
  VIEW_MODE_2D =     0,
  VIEW_MODE_3D
} AppliViewMode_e;

typedef enum
{
  SELECT_MODE_VIEW =     0,
  SELECT_MODE_CREATE,
  SELECT_MODE_STICK,
  SELECT_MODE_CARROT
} AppliSelectionMode_e;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

  AppliDisplayMode_e   m_DisplayMode;
  AppliViewMode_e      m_ViewMode;
  AppliSelectionMode_e m_SelectionMode;
 
  void resizeAllChildFrames(int cx, int cy);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAppNewBiotop();
	afx_msg void OnAppOpenBiotop();
	afx_msg void OnAppSaveBiotop();
	afx_msg void OnAppSaveAsBiotop();
	afx_msg void OnAppRunLearningScenario();
  afx_msg void OnAppDisplayLogServer();
	afx_msg void OnAppDisplayMap();
	afx_msg void OnAppDisplayBrain();
	afx_msg void OnAppDisplayGenetic();
	afx_msg void OnAppDisplayStat();
	afx_msg void OnAppSelectView();
	afx_msg void OnAppSelectCreate();
	afx_msg void OnAppSelectStick();
	afx_msg void OnAppSelectCarrot();
	afx_msg void OnAppDisplay2d();
	afx_msg void OnAppDisplay3d();
  afx_msg void OnAppFindEntity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__25B61CB4_3C2D_4D22_88F4_811679EBD1DC__INCLUDED_)
