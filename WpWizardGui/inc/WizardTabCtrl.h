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

#if !defined(AFX_WIZARDTABCTRL_H__C580229C_C92E_4749_BBA1_FEA76E05E7F7__INCLUDED_)
#define AFX_WIZARDTABCTRL_H__C580229C_C92E_4749_BBA1_FEA76E05E7F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardTabCtrl.h : header file
//

#define NUM_TABS 5

#include "CBasicEntity.h"

#define TAB_MASK_CAR_FEMALE 0x01
#define TAB_MASK_CAR_MALE   0x02
#define TAB_MASK_SENSORS    0x04
#define TAB_MASK_BEHAVIOR   0x08
#define TAB_MASK_GENERATE   0x10

/////////////////////////////////////////////////////////////////////////////
// CWizardTabCtrl window

class CWizardTabCtrl : public CTabCtrl
{
// Construction
public:
	CWizardTabCtrl();

// Attributes
private:
  int      m_tabIndex[NUM_TABS];
  CImageList* m_pIconTabs;
  int m_tabCurrent;
  unsigned char m_tab_CurMask;
  bool m_bInitComplete;

// Operations
public:
  void Init(void);
  void Resize(int cx, int cy);
  void AddTabMask(unsigned char tabsMask);
  void RemoveTabMask(unsigned char tabsMask);
  void SetTabDisplay();
  void UpdateTabsWithNewEntity();
  CDialog* m_tabPages[NUM_TABS];

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardTabCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWizardTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWizardTabCtrl)
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDTABCTRL_H__C580229C_C92E_4749_BBA1_FEA76E05E7F7__INCLUDED_)
