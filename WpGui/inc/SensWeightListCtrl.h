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

#if !defined(AFX_SENSWEIGHTLISTCTRL_H__300E9EB3_D55D_485F_8970_03B25A27923D__INCLUDED_)
#define AFX_SENSWEIGHTLISTCTRL_H__300E9EB3_D55D_485F_8970_03B25A27923D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SensWeightListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSensWeightListCtrl window

class CSensWeightListCtrl : public CListCtrl
{
// Construction
public:
	CSensWeightListCtrl();

// Attributes
protected:
   CSensor* m_pSensor;
   CEdit*   m_pTmpEdtitBox;

   short* m_pCurWeightBuf;         // Data with scale: [0..0x1000] displayed with format: [0..100.0]
   int    m_CurWeightBufSize;

// Operations
public:

  bool   SetSensor(CSensor* pSens, WORD* geneDataBuf = NULL, bool bUseNegative = true);
  void   UdpateSelectedData(void);
  short* GetWeightBuf();
  int    GetWeightBufSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSensWeightListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSensWeightListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSensWeightListCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENSWEIGHTLISTCTRL_H__300E9EB3_D55D_485F_8970_03B25A27923D__INCLUDED_)
