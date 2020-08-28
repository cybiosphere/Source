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

#if !defined(AFX_DIALOGTAB4_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_)
#define AFX_DIALOGTAB4_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTab4.h : header file
//

#include "CBasicEntity.h"
#include "CAnimal.h"
#include "GeneButton.h"
#include "FileSelectButton.h"
#include "GenomeTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTab4 dialog

class CDialogTab4 : public CDialog
{
// Construction
public:
	CDialogTab4(CWnd* pParent = NULL);   // standard constructor
    ~CDialogTab4();

// Dialog Data
	//{{AFX_DATA(CDialogTab4)
	enum { IDD = IDD_DIALOG_TAB4 };
	CEdit	m_EditSexChromIdx;
	CButton	m_ButtonMale;
	CButton	m_ButtonFemale;
	CGenomeTreeCtrl	m_GenomeTree;
	UINT	m_nbChromosomes;
	UINT	m_SexualChrom_Idx;
	UINT	m_Age;
	UINT	m_Seed;
	CString	m_strData1;
	CString	m_strData2;
	CString	m_strData3;
	CString	m_strData4;
	CString	m_strData5;
	CString	m_strLabel1;
	CString	m_strLabel2;
	CString	m_strLabel3;
	CString	m_strLabel4;
	int		m_SliderVar;
	UINT	m_Variation;
	BOOL	m_IsAutoLearning;
	CString	m_ScoreDisplay;
	UINT	m_NbIterations;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTab4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    CGenome*      m_pGenome;
    CBasicEntity* m_pEntity;
    UINT	        m_CurChrom_Idx;
    UINT          m_GeneSpreadFactor;
    double        m_EntitySpeedMax;
    
    bool CreateGenome(SexType_e sex);
    void DisplayGenome(CGenome* pGenome);
    void DisplayPair(CPairOfChromosome* pPair);
    void DisplayChromosome(CChromosome* pChromosome);
    void DisplayGene(CGene* pGene);

    void AddGenesForCaracters(SexType_e sex);
    void AddGenesCarFromCombo(CComboBox* pComboFemale, CComboBox* pComboMale, SexType_e sex, GeneSubTypeCaracter_e subtype);
    void AddGenesForParameters(SexType_e sex);
    void AddGenesForLifeStages(SexType_e sex);
    void AddGenesForPhysic();
    void AddGenesForSensors();
    void AddGenesForFeelings();
    void AddGenesForReactions();
    void AddGenesForPurposes();
    void AddGenesForBrain();

    void PrepareBrainInstinct(CAnimal* pAnimal);

    bool PrepareLearningFilesMammal(CString newFolder);

	// Generated message map functions
	//{{AFX_MSG(CDialogTab4)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCreateFemale();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonCreateMale();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSaveEntity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTAB4_H__A9AB6BA7_E39A_471D_9180_13CCF044AF60__INCLUDED_)
