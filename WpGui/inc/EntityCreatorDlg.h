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

#if !defined(AFX_ENTITYCREATORDLG_H_INCLUDED_)
#define AFX_ENTITYCREATORDLG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityEditorDlg.h : header file
//
#include "CBasicEntity.h"


/////////////////////////////////////////////////////////////////////////////
// CEntityCreatorDlg dialog

class CEntityCreatorDlg : public CDialog
{
// Construction
public:
	CEntityCreatorDlg(CWnd* pParent = NULL, int coordX = 1, int coordY = 1, int layer = 1);   // standard constructor

  ~CEntityCreatorDlg();

// Dialog Data
	//{{AFX_DATA(CEntityCreatorDlg)
	enum { IDD = IDD_DLG_ENTITY_CREATOR };
	CButton	m_ButtonLoad;
	CButton	m_ButtonOK;
	CComboBox	m_ComboClass;
	CString	m_EditBoxName;
	CString	m_EditBoxSpecieName;
	UINT	m_StartCoordX;
	UINT	m_StartCoordY;
	int 	m_StartLayer;
	//}}AFX_DATA
  entityIdType GetNewEntityId(void);
  bool IsGenomeEditRequired();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityCreatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityCreatorDlg)
	afx_msg void OnRadioMineral();
	afx_msg void OnRadioVegetal();
	afx_msg void OnRadioAnimal();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSpecieName();
	afx_msg void OnDropdownComboClass();
	virtual void OnCancel();
	afx_msg void OnChangeEntityName();
	virtual void OnOK();
	afx_msg void OnButtonLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  bool CheckSpecieDataRssource();

  entityIdType m_NewEntityId; // This entity will be used for gene setting but not inserted in 
  bool         m_GenomeEditRequired;
  CString      m_OpenedFileName;
  CString      m_OpenedDirectoryName;
  CGenome*     m_pTempGenome;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYCREATORDLG_H_INCLUDED_)
