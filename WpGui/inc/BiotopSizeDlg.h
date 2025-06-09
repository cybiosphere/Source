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

#if !defined(AFX_BIOTOPSIZEDLG_INCLUDED_)
#define AFX_BIOTOPSIZEDLG_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityFindDlg.h : header file
//

#include "CBiotop.h"

/////////////////////////////////////////////////////////////////////////////
// CBiotopSizeDlg dialog

class CBiotopSizeDlg : public CDialog
{
// Construction
public:
	CBiotopSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBiotopSizeDlg)
	enum { IDD = IDD_DIALOG_BIOTOPSIZE };
	int	m_BiotopSizeX;
	int	m_BiotopSizeY;
	//}}AFX_DATA

	size_t GetBiotopSizeX(void);
	size_t GetBiotopSizeY(void);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CBiotopSizeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// 
	// Generated message map functions
	//{{AFX_MSG(CBiotopSizeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_BIOTOPSIZEDLG_INCLUDED_)
