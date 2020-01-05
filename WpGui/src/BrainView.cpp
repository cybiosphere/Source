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

// BrainView.cpp : implementation of the CBrainView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "BrainDoc.h"
#include "BrainView.h"
#include "CAnimal.h"
#include "BrainEditorDlg.h"
#include "BrainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBrainView

IMPLEMENT_DYNCREATE(CBrainView, CFormView)

BEGIN_MESSAGE_MAP(CBrainView, CFormView)
	//{{AFX_MSG_MAP(CBrainView)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, OnButtonExecAuto)
	ON_EN_CHANGE(IDC_LEARNING, OnChangeLearning)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_IDENTIFY, OnButtonEditIdentify)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_NONE, OnRadioNone)
	ON_BN_CLICKED(IDC_RADIO_STICK, OnRadioStick)
	ON_BN_CLICKED(IDC_RADIO_CARROT, OnRadioCarrot)
	ON_BN_CLICKED(IDC_BUTTON_PRED, OnButtonPrediction)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
  END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrainView construction/destruction

CBrainView::CBrainView()
	: CFormView(CBrainView::IDD)
{
	//{{AFX_DATA_INIT(CBrainView)
	m_Feedback = _T("");
	m_LabelAction = _T("");
	m_LearningRate = 100.0;
	m_FeelingCurrent = _T("");
	m_FeelingPrevious = _T("");
	m_FeelingDif = _T("");
    m_pBrain = NULL;
	m_radioMode = -1;
	//}}AFX_DATA_INIT

  for (int i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
  {
    m_pActionButtonTable[i] = NULL;
  }
}

CBrainView::~CBrainView()
{
  for (int i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
  {
    if (m_pActionButtonTable[i] != NULL)
    {
      delete m_pActionButtonTable[i];
      m_pActionButtonTable[i] = NULL;
    }
  }
}

void CBrainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrainView)
	DDX_Control(pDX, IDC_BUTTON_EXEC, m_ButtonAutoChoice);
	DDX_Text(pDX, IDC_FEEDBACK, m_Feedback);
	DDX_Text(pDX, IDC_CUR_ACTION, m_LabelAction);
    DDX_GridControl(pDX, IDC_BRAIN_GRID, m_BrainGrid);
	DDX_Text(pDX, IDC_LEARNING, m_LearningRate);
	DDX_Text(pDX, IDC_FEEL_CUR, m_FeelingCurrent);
	DDX_Text(pDX, IDC_FEEL_PREV, m_FeelingPrevious);
	DDX_Text(pDX, IDC_FEEL_DIF, m_FeelingDif);
	DDX_Radio(pDX, IDC_RADIO_AUTO, m_radioMode);
	//}}AFX_DATA_MAP
}

BOOL CBrainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CBrainView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();
  ResizeParentToFit();
  m_ButtonAutoChoice.EnableWindow(false);
  m_radioMode = 0;
  OnRadioAuto();
  UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CBrainView printing

BOOL CBrainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBrainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBrainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBrainView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CBrainView diagnostics

#ifdef _DEBUG
void CBrainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBrainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBrainDoc* CBrainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrainDoc)));
	return (CBrainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBrainView message handlers

/////////////////////////////////////////////////////////////////////////////
// CBrainView functionnal

bool CBrainView::SetBrain(CBrain* pBrain)
{
  int i;
  m_pBrain = pBrain;
  m_LabelAction = "";

  // Delete previous ButtonTable
  for (i=0;i<MAX_NUMBER_BUTTONS_ACTION;i++)
  {
    if (m_pActionButtonTable[i] != NULL)
    {
      delete m_pActionButtonTable[i];
      m_pActionButtonTable[i] = NULL;
    }
  }

  if (m_pBrain!=NULL)
  {
    int iDpi = GetDpiForWindow(this->m_hWnd);
    int offsetX = MulDiv(12, iDpi, 96);
    int startX = offsetX;
    int startY = MulDiv(56, iDpi, 96);
    int buttonWidth;
    if (iDpi == 120)
      buttonWidth = MulDiv(158, iDpi, 96);
    else
      buttonWidth = MulDiv(148, iDpi, 96);
    int buttonHeight = MulDiv(14, iDpi, 96);

    for (i=0;(i<MAX_NUMBER_BUTTONS_ACTION)&&(i<m_pBrain->GetNumberReaction());i++)
    {
      m_pActionButtonTable[i] = new CActionButton(m_pBrain->GetReactionByIndex(i),i,this);
      startX = offsetX + (i&0x01) * (buttonWidth + 3);
      CRect rect(startX, startY, startX + buttonWidth, startY + buttonHeight);
      m_pActionButtonTable[i]->CreateNewButton(rect,this,2000+i);
      startY += (i&0x01) * (buttonHeight + 3);
    }

    m_BrainGrid.SetBrain(m_pBrain);
    m_ButtonAutoChoice.EnableWindow(true);
    m_BrainGrid.RefreshBrainData();
  }
  else
  {
    m_BrainGrid.SetBrain(NULL);
    m_ButtonAutoChoice.EnableWindow(false);
  }

  return (true);
}

void CBrainView::OnButtonExecAuto() 
{
  ExecuteActionAndGetAutoFeedback(-1);
}

void CBrainView::OnChangeLearning() 
{
  // Update windows
	UpdateData(true);
}

void CBrainView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (m_BrainGrid.m_hWnd != NULL)
	{
    int iDpi = GetDpiForWindow(this->m_hWnd);
		m_BrainGrid.MoveWindow(MulDiv(338, iDpi, 96), MulDiv(8, iDpi, 96), cx - MulDiv(344, iDpi, 96), cy - MulDiv(16, iDpi, 96), true);
		UpdateWindow ();
    m_BrainGrid.Refresh();
	}
}

void CBrainView::OnButtonEdit() 
{
  if (m_pBrain!=NULL)
  {
    CBrainEditorDlg dlg(m_pBrain);
    dlg.DoModal();
    m_BrainGrid.RefreshBrainData();
  }
}

void CBrainView::OnButtonEditIdentify() 
{
  if (m_pBrain!=NULL)
  {
    CBrainEditorDlg dlg(m_pBrain, true);
    dlg.DoModal();
    m_BrainGrid.RefreshBrainData();
  }
}

void CBrainView::ExecuteActionAndGetAutoFeedback(int actionIndex)
{
  if ( (m_pBrain == NULL) || !(m_pBrain->GetEntity()->isAlive()) )
  {
    return;
  }

  feedbackValType curFeedback = theApp.forceEntityAction(m_pBrain->GetEntity()->getId(), actionIndex);

  double prevFeel = m_pBrain->GetPreviousFeelingWelfare();
  double curFeel  = m_pBrain->GetCurrentFeelingWelfare();

  m_FeelingPrevious.Format(LPCTSTR("%.1f"),prevFeel);
  m_FeelingCurrent.Format(LPCTSTR("%.1f"),curFeel);
  m_FeelingDif.Format(LPCTSTR("%.1f"),curFeel-prevFeel);

  int curActionInd = m_pBrain->GetCurrentReactionIndex();
  m_LabelAction = m_pBrain->GetReactionByIndex(curActionInd)->GetLabel().c_str();

  bool resuMemorize = false;

  switch(m_LearningMode)
  {
  case BRAIN_LEARNING_AUTO:
    resuMemorize = m_pBrain->MemorizeExperience(curFeedback,m_LearningRate);
    break;
  case BRAIN_LEARNING_NONE:
    resuMemorize = false;
    break;
  case BRAIN_LEARNING_STICK:
    curFeedback  = -MAX_FEEDBACK_VAL;
    resuMemorize = m_pBrain->MemorizeExperience(curFeedback,m_LearningRate);
    break;
  case BRAIN_LEARNING_CARROT:
    curFeedback  = MAX_FEEDBACK_VAL;
    resuMemorize = m_pBrain->MemorizeExperience(curFeedback,m_LearningRate); 
    break;
  default:
    resuMemorize = false;
    break;
  }
  
  if (resuMemorize)
    m_Feedback.Format(LPCTSTR("%.2f"),curFeedback);
  else
    m_Feedback.Format(LPCTSTR("%.2f"),0);

  m_BrainGrid.RefreshBrainData();

  // Update windows
  UpdateData(false);

  // Update Entity view
  theApp.NextSecondRefreshAllViews();
  theApp.GetBiotopViewPtr()->ForceRefreshDisplay(false);
}

void CBrainView::OnRadioAuto() 
{
	// TODO: Add your control notification handler code here
	m_LearningMode = BRAIN_LEARNING_AUTO;
  UpdateData(true);
}

void CBrainView::OnRadioNone() 
{
	// TODO: Add your control notification handler code here
	m_LearningMode = BRAIN_LEARNING_NONE;
  UpdateData(true);
}

void CBrainView::OnRadioStick() 
{
	// TODO: Add your control notification handler code here
	m_LearningMode = BRAIN_LEARNING_STICK;
  UpdateData(true);
}

void CBrainView::OnRadioCarrot() 
{
	// TODO: Add your control notification handler code here
	m_LearningMode = BRAIN_LEARNING_CARROT;
  UpdateData(true);
}

void CBrainView::OnButtonPrediction() 
{
  if ( (m_pBrain == NULL) || !(m_pBrain->GetEntity()->isAlive()) )
  {
    return;
  }

  int curActionInd = theApp.GetBiotop()->predictEntityAction(m_pBrain->GetEntity()->getId());

  m_FeelingPrevious.Format(LPCTSTR("NA"));
  m_FeelingCurrent.Format(LPCTSTR("NA"));
  m_FeelingDif.Format(LPCTSTR("NA"));

  m_LabelAction = m_pBrain->GetReactionByIndex(curActionInd)->GetLabel().c_str();
  
  m_BrainGrid.RefreshBrainData();

  m_Feedback.Format(LPCTSTR("NA"));

  // Update windows
  UpdateData(false);	
}




