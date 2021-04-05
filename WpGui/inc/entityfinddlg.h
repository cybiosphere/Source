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

#if !defined(AFX_ENTITYFINDDLG_H__4EFBD19E_CFBD_46B5_B029_5CA4BF8E1A44__INCLUDED_)
#define AFX_ENTITYFINDDLG_H__4EFBD19E_CFBD_46B5_B029_5CA4BF8E1A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityFindDlg.h : header file
//

#include "CBiotop.h"

/////////////////////////////////////////////////////////////////////////////
// CEntityFindDlg dialog

class CEntityFindDlg : public CDialog
{
// Construction
public:
	CEntityFindDlg(CBiotop* pBiotop, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEntityFindDlg)
	enum { IDD = IDD_DLG_ENTITY_FIND };
	CListCtrl	m_EntityList;
	BOOL	m_bSelect;
	CString	m_FilterString;
	//}}AFX_DATA

	void fileEntityList(CString filterStr);
  int GetSelectionId(void);
  bool IsForceSelection(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  CBiotop* m_pBiotop;
	entityIdType m_CurSelId;

	// Generated message map functions
	//{{AFX_MSG(CEntityFindDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilterString();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYFINDDLG_H__4EFBD19E_CFBD_46B5_B029_5CA4BF8E1A44__INCLUDED_)
