#if !defined(AFX_TRAININGDLG_H__970AF72F_D6AC_4DAA_88B9_CA65ADFF2A5A__INCLUDED_)
#define AFX_TRAININGDLG_H__970AF72F_D6AC_4DAA_88B9_CA65ADFF2A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrainingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainingDlg dialog

class CTrainingDlg : public CDialog
{
// Construction
public:
	CTrainingDlg(CString strTrainFilesDirectory, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrainingDlg)
	enum { IDD = IDD_DIALOG_TRAIN };
	CProgressCtrl	m_ProgressCtrl;
	UINT	m_iteration;
	UINT	m_estimatedTime;
	//}}AFX_DATA

	CString	m_strTrainFilesDirectory;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrainingDlg)
	afx_msg void OnChangeIteration();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAININGDLG_H__970AF72F_D6AC_4DAA_88B9_CA65ADFF2A5A__INCLUDED_)
