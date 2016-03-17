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

#if !defined(AFX_GENOME_EDITOR_DLG_H)
#define AFX_GENOME_EDITOR_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenomeEditorDlg.h : header file
//
#include "CGenome.h"
#include "GenomeTreeCtrl.h"
#include "SensWeightListCtrl.h"

typedef enum
{
  EDIT_MODE_INIT     = 0,
  EDIT_MODE_GENOME   = 1,
  EDIT_MODE_PAIRE    = 2,
  EDIT_MODE_CHROMO   = 3,
  EDIT_MODE_GENE     = 4,
} GenomeEditorMode_e;


/////////////////////////////////////////////////////////////////////////////
// CGenomeEditorDlg dialog

class CGenomeEditorDlg : public CDialog
{
// Construction
public:
	CGenomeEditorDlg(CBasicEntity* pEntity, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenomeEditorDlg)
	enum { IDD = IDD_DLG_GENOME_EDITOR };
	CEdit	m_EditLabel;
	CSensWeightListCtrl	m_WeightListCtrl_P;
	CSensWeightListCtrl	m_WeightListCtrl_M;
	CEdit	m_MuteRateBox;
	CComboBox	m_ComboMuteType;
	CComboBox	m_ComboRight;
	CComboBox	m_ComboLeft;
	CButton	m_ButtonSave;
	CButton	m_ButtonLoad;
	CSliderCtrl	m_SliderP4;
	CSliderCtrl	m_SliderP3;
	CSliderCtrl	m_SliderP2;
	CSliderCtrl	m_SliderP1;
	CSliderCtrl	m_SliderM4;
	CSliderCtrl	m_SliderM3;
	CSliderCtrl	m_SliderM2;
	CSliderCtrl	m_SliderM1;
	CComboBox	m_Combo2;
	CComboBox	m_Combo1;
	CButton	m_ButtonAdd;
	CButton	m_ButtonOK;
	CGenomeTreeCtrl	m_GenomeTreeEdit;
	CString	m_SliderM1Title;
	CString	m_SliderM2Title;
	CString	m_SliderM3Title;
	CString	m_SliderM4Title;
	CString	m_SliderP1Title;
	CString	m_SliderP2Title;
	CString	m_SliderP3Title;
	CString	m_SliderP4Title;
	CString	m_strDescriptionM;
	CString	m_strDescriptionP;
	int		m_MuteRate;
	UINT	m_InitAge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenomeEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenomeEditorDlg)
	afx_msg void OnRadioMineral();
	afx_msg void OnRadioVegetal();
	afx_msg void OnRadioAnimal();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnChangeEntityName();
	virtual void OnOK();
	afx_msg void OnSelchangedTreeGenome(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnReleasedcaptureSliderM1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderM2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderM3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderM4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderP1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderP2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderP3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderP4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnSetBrainInGenes();
	afx_msg void OnSelchangeComboLeft();
	afx_msg void OnSelchangeComboRight();
	afx_msg void OnChangeEditMuteRate();
	afx_msg void OnSelchangeComboMuteType();
	afx_msg void OnEndlabeleditListWeightM(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditListWeightP(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditLabel();
	afx_msg void OnItemchangingListWeightM(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingListWeightP(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void DisplayGenome(CGenome* pGenome);
  void DisplayPair(CPairOfChromosome* pPair);
  void DisplayChromosome(CChromosome* pChromosome);
  void DisplayGene(CGene* pGene);

  void ClearAndHideAllSliders();

  bool EditGenePair();

  void UpdateCombo1(bool rebuildGene);
  void UpdateCombo2(bool rebuildGene);
  bool RefreshCurrentGeneM(bool rebuildGene, bool resetWeightTable = true);
  bool RefreshCurrentGeneP(bool rebuildGene, bool resetWeightTable = true);

  CGenome*      m_pNewGenome;  // This is the copy of entity genome used for changes
  CBasicEntity* m_pOldEntity;

  GenomeEditorMode_e m_EditMode;

  // Ref on current selected object.
  // 1 only must be != NULL at a time
  CPairOfChromosome* m_pCurrentPair;
  CChromosome*       m_pCurrentChromosome;
  CGene*             m_pCurrentGene;

  CGene*             m_pCurrentEditGeneM;
  CGene*             m_pCurrentEditGeneP;

  bool               m_bIsWeightChanging; // Use to avoid weight change after sel change
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENOME_EDITOR_DLG_H)
