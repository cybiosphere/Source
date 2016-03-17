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

#if !defined(AFX_DIALOGTAB1_H__12EBCEA7_F364_437C_854F_494503207433__INCLUDED_)
#define AFX_DIALOGTAB1_H__12EBCEA7_F364_437C_854F_494503207433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTab1.h : header file
//

#include "CBasicEntity.h"
#include "ParameterSlider.h"

#define MAX_NUMBER_PARAMETER_DISPLAY  24

/////////////////////////////////////////////////////////////////////////////
// CDialogTab1 dialog

class CDialogTab1 : public CDialog
{
// Construction
public:
	CDialogTab1(CWnd* pParent = NULL, bool bHideHeader = false);   // standard constructor
    ~CDialogTab1();
    void DisplayParamSliders();

    COLORREF m_color;

    int    m_weightParamId;
    int    m_growthSpeedParamId;
    double m_growthSpeed;

// Dialog Data
	//{{AFX_DATA(CDialogTab1)
	enum { IDD = IDD_DIALOG_TAB1 };
	CEdit	m_EditWeightMin;
	CEdit	m_EditWeight;
	CButton	m_Chk_SexualDimCtrl;
	CEdit	m_EditLayerCtrl;
	CEdit	m_EditBoxSpecieCtrl;
	CButton	m_RadioTypeCtrl1;
	CButton	m_RadioTypeCtrl2;
	CButton	m_RadioTypeCtrl3;
	CSpinButtonCtrl	m_Spin6;
	CSpinButtonCtrl	m_Spin5;
	CSpinButtonCtrl	m_Spin4;
	CSpinButtonCtrl	m_Spin3;
	CSpinButtonCtrl	m_Spin2;
	CComboBox	m_ComboTexture;
	CComboBox	m_ComboTaste;
	CComboBox	m_ComboReproduct;
	CComboBox	m_ComboMvt;
	CComboBox	m_ComboHabitat;
	CComboBox	m_ComboConsum;
	CComboBox	m_ComboAttribut;
	CSpinButtonCtrl	m_Spin1;
	CComboBox	m_ComboOdor;
	CComboBox	m_ComboForm;
	CButton	m_ColorButton;
	CComboBox	m_ComboClass;
	CString	m_EditBoxSpecieName;
	UINT	m_LifeExpectency;
	UINT	m_DurationStage1;
	CString	m_ColorName;
	UINT	m_DecompositionTime;
	UINT	m_DurationStage2;
	UINT	m_DurationStage3;
	UINT	m_DurationStage4;
	UINT	m_DurationStage5;
	UINT	m_DurationStage6;
	double	m_Weight;
	UINT	m_Layer;
	BOOL	m_bSexualDimorphism;
	double	m_WeightMin;
	//}}AFX_DATA

    void InitView(void);
    CBasicEntity* GetpEntity();

    CParameterSlider* m_pParamSlider[MAX_NUMBER_PARAMETER_DISPLAY];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTab1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
  
    bool m_bInitDone;
    bool m_bHideHeader;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTab1)
	afx_msg void OnRadioMineral();
	afx_msg void OnRadioVegetal();
	afx_msg void OnRadioAnimal();
	afx_msg void OnButtonColor();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnSelchangeComboClass();
	afx_msg void OnCheck1();
	afx_msg void OnChangeEditName();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChangeEditWeight();
	afx_msg void OnChangeEditWeightMin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTAB1_H__12EBCEA7_F364_437C_854F_494503207433__INCLUDED_)
