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

#if !defined(AFX_PARAMETERSLIDER_H_INCLUDED_)
#define AFX_PARAMETERSLIDER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterSlider.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParameterSlider window

class CParameterSlider : public CSliderCtrl
{
// Construction
public:
	CParameterSlider(bool allowRangeEditMin = false, bool allowRangeEditMax = false);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameterSlider)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	bool CreateNewParam(CGenericParam* pParam, const RECT& rect, CWnd* pParentWnd, UINT nID);
	bool RefreshDisplayParam();
	virtual ~CParameterSlider();
    CGenericParam* GetpParam();

	// Generated message map functions
protected:
	//{{AFX_MSG(CParameterSlider)
	afx_msg void OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
  CEdit*   m_pMinStatic;
  CEdit*   m_pValStatic;
  CEdit*   m_pMaxStatic;
  CStatic* m_pTextStatic;
  CGenericParam* m_pParam;
  CFont *m_pFont;
  bool m_bIsChanging;
  bool m_bAllowRangeEditMin;
  bool m_bAllowRangeEditMax;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_PARAMETERSLIDER_H_INCLUDED_}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERSLIDER_H_INCLUDED_)
