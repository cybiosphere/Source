// TrainingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "TrainingDlg.h"
#include "time.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrainingDlg dialog


CTrainingDlg::CTrainingDlg(CString strTrainFilesDirectory, CWnd* pParent /*=NULL*/)
	: CDialog(CTrainingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainingDlg)
	m_iteration = 1;
	m_estimatedTime = 1;
	//}}AFX_DATA_INIT
  m_strTrainFilesDirectory = strTrainFilesDirectory;
}


void CTrainingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainingDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_iteration);
	DDV_MinMaxUInt(pDX, m_iteration, 1, 100);
	DDX_Text(pDX, IDC_EDIT2, m_estimatedTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainingDlg, CDialog)
	//{{AFX_MSG_MAP(CTrainingDlg)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeIteration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainingDlg message handlers

void CTrainingDlg::OnChangeIteration() 
{
  UpdateData(true);
	m_estimatedTime = cybio_round((double)m_iteration * 70.0 / 60.0);
  UpdateData(false);
}

void CTrainingDlg::OnOK() 
{
  CBiotop tmpBiotop(1,1,1);
  CString tmpStr;
  CString scoreDisplay;
  CScenarioPlayer player(&tmpBiotop);
  int scoreTotal = 0;
  int scoreSuccess = 0;

  m_ProgressCtrl.SetRange(0,100);
  m_ProgressCtrl.SetStep(1);
  m_ProgressCtrl.StepIt();
  m_ProgressCtrl.SetStep(100 / (3*m_iteration));

  //EnableWindow(FALSE);

  for (int i=0; i<m_iteration; i++)
  { 
    theApp.PumpMessage();
    player.ReadScenarioFile("babyLearning.scp", (char*)m_strTrainFilesDirectory.GetBuffer(0));
    theApp.PumpMessage();
    player.PlayScenario();
    if (player.GetTotalScore()>0)
    {
      tmpStr.Format(LPCTSTR("%d"), player.GetSuccessScore() * 100 / player.GetTotalScore());
      scoreDisplay += LPCTSTR("\tScen1:") + tmpStr + LPCTSTR("\t ");
      scoreTotal   += player.GetTotalScore();
      scoreSuccess += player.GetSuccessScore();
    }
    //EnableWindow(TRUE);
    theApp.PumpMessage();
    m_ProgressCtrl.StepIt();
    m_ProgressCtrl.UpdateWindow();
    UpdateData(false);
    theApp.PumpMessage();
    //EnableWindow(FALSE);

    player.ReadScenarioFile("childLearning.scp", (char*)m_strTrainFilesDirectory.GetBuffer(0));
    theApp.PumpMessage();
    player.PlayScenario();
    if (player.GetTotalScore()>0)
    {
      tmpStr.Format(LPCTSTR("%d"), player.GetSuccessScore() * 100 / player.GetTotalScore());
      scoreDisplay += LPCTSTR("\tScen2: ") + tmpStr + LPCTSTR("\t ");
      scoreTotal   += player.GetTotalScore();
      scoreSuccess += player.GetSuccessScore();
    }
    //EnableWindow(TRUE);
    theApp.PumpMessage();
    m_ProgressCtrl.StepIt();
    m_ProgressCtrl.UpdateWindow();
    UpdateData(false);
    theApp.PumpMessage();
    //EnableWindow(FALSE);

    player.ReadScenarioFile("adultLearning.scp", (char*)m_strTrainFilesDirectory.GetBuffer(0));
    theApp.PumpMessage();
    player.PlayScenario();
    if (player.GetTotalScore()>0)
    {
      tmpStr.Format(LPCTSTR("%d"), player.GetSuccessScore() * 100 / player.GetTotalScore());
      scoreDisplay += LPCTSTR("\tScen3: ") + tmpStr + LPCTSTR("\n ");
      scoreTotal   += player.GetTotalScore();
      scoreSuccess += player.GetSuccessScore();
    }
    //EnableWindow(TRUE);
    theApp.PumpMessage();
    m_ProgressCtrl.StepIt();
    m_ProgressCtrl.UpdateWindow();
    UpdateData(false);
    //EnableWindow(FALSE);
  }

  tmpStr.Format(LPCTSTR("%d"), scoreSuccess * 100 / scoreTotal);
  scoreDisplay += LPCTSTR("TOTAL: ") + tmpStr;

  //EnableWindow(TRUE);

  AfxMessageBox(scoreDisplay);
	
	CDialog::OnOK();
}

void CTrainingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
