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

#if !defined(AFX_DIALOGTAB3_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_)
#define AFX_DIALOGTAB3_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTab3.h : header file
//

#include "CBasicEntity.h"
#include "GeneButton.h"
#include "FileSelectButton.h"

#define MAX_NUMBER_BUTTONS_ACTION  20
#define MAX_NUMBER_BUTTONS_PREY     5
#define MAX_NUMBER_BUTTONS_PREDATOR 5

/////////////////////////////////////////////////////////////////////////////
// CDialogTab3 dialog

class CDialogTab3 : public CDialog
{
// Construction
public:
	CDialogTab3(CWnd* pParent = NULL);   // standard constructor
    ~CDialogTab3();
    
    bool LoadSensorsAndPredatorsEntities();

    int GetPreyEntityList(CBasicEntity*** pEntityTab, bool includeWater);
    int GetPredatorEntityList(CBasicEntity*** pEntityTab);

// Dialog Data
	//{{AFX_DATA(CDialogTab3)
	enum { IDD = IDD_DIALOG_TAB3 };
	CComboBox	m_ComboBehavior;
	CComboBox	m_ComboHierarchy;
	CComboBox	m_ComboSocial;
	CComboBox	m_ComboLifeMode;
  BOOL	m_bBabiesStayHome;
	//}}AFX_DATA

   CGeneButton* m_pActionButtonTable[MAX_NUMBER_BUTTONS_ACTION];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTab3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    

    CFileSelectButton* m_pPreyButtonTable[MAX_NUMBER_BUTTONS_PREY + 1]; // Add water as default
    CFileSelectButton* m_pPredatorButtonTable[MAX_NUMBER_BUTTONS_PREDATOR];

    CBasicEntity* m_pPreyEntities[MAX_NUMBER_BUTTONS_PREY + 1]; // Add water as default
    CBasicEntity* m_pPredatorEntities[MAX_NUMBER_BUTTONS_PREDATOR];

	// Generated message map functions
	//{{AFX_MSG(CDialogTab3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTAB3_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_)
