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

// BioCtrlView.cpp : implementation of the CBioCtrlView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "BioCtrlDoc.h"
#include "BioCtrlView.h"
#include "BiotopEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView

IMPLEMENT_DYNCREATE(CBioCtrlView, CFormView)

BEGIN_MESSAGE_MAP(CBioCtrlView, CFormView)
	//{{AFX_MSG_MAP(CBioCtrlView)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckModeManual)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, OnReleasedcaptureSliderSpeed)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ZOOM, OnReleasedcaptureSliderZoom)
	ON_BN_CLICKED(IDC_EDIT_BIOTOP, OnEdit)
  ON_BN_CLICKED(IDC_EDIT_BIOTOP2, OnDefaultSpeed)
  ON_BN_CLICKED(IDC_EDIT_BIOTOP3, OnMaxSpeed)
	ON_BN_CLICKED(IDC_CHECK2, OnCheckStopOnEv)
	ON_CBN_SELCHANGE(IDC_COMBO_WIND, OnSelchangeComboWind)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView construction/destruction

CBioCtrlView::CBioCtrlView()
	: CFormView(CBioCtrlView::IDD)
{
	//{{AFX_DATA_INIT(CBioCtrlView)
	m_bManualMode = FALSE;
	m_StrTime = _T("");
	m_bStopOnEvent = FALSE;
	m_StrScenarioScore = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

  for (int i=0;i<MAX_NUMBER_BIOCTRL_DISPLAY;i++)
  {
    m_pParamCtrl[i] = NULL;
  }
}

CBioCtrlView::~CBioCtrlView()
{
  for (int i=0;i<MAX_NUMBER_BIOCTRL_DISPLAY;i++)
  {
    if (m_pParamCtrl[i] != NULL)
    {
      delete m_pParamCtrl[i];
      m_pParamCtrl[i] = NULL;
    }
  }
}

void CBioCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBioCtrlView)
	DDX_Control(pDX, IDC_COMBO_WIND, m_ComboWind);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_SliderSpeed);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_SliderZoom);
	DDX_Check(pDX, IDC_CHECK1, m_bManualMode);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_StrTime);
	DDX_Check(pDX, IDC_CHECK2, m_bStopOnEvent);
	DDX_Text(pDX, IDC_EDIT_SCENARIO, m_StrScenarioScore);
	//}}AFX_DATA_MAP
}

BOOL CBioCtrlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CBioCtrlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

  m_SliderZoom.SetRange(20,100);
  m_SliderZoom.SetPos(100);
  m_SliderZoom.SetPageSize(5);
  theApp.GetBiotopViewPtr()->SetZoomRate(100);

  m_SliderSpeed.SetRange(0,100);
  m_SliderSpeed.SetPos(1);
  m_SliderSpeed.SetPageSize(5);
  theApp.modifyBiotopSpeed(1.2);
  m_UserSpeed = 1;

  if (theApp.GetBiotop()->getWindStrenght()==0)
    m_ComboWind.SetCurSel(0);
  else
   m_ComboWind.SetCurSel(theApp.GetBiotop()->getWindDirection()+1);

	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView printing

BOOL CBioCtrlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBioCtrlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBioCtrlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBioCtrlView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView diagnostics

#ifdef _DEBUG
void CBioCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBioCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBioCtrlDoc* CBioCtrlView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBioCtrlDoc)));
	return (CBioCtrlDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView message handlers

/////////////////////////////////////////////////////////////////////////////
// CBioCtrlView functionnal

bool CBioCtrlView::SetBiotop(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;

  int iDpi = GetDpiForWindow(this->m_hWnd);
  CRect rect(MulDiv(16, iDpi, 96), MulDiv(118, iDpi, 96), MulDiv(190, iDpi, 96), MulDiv(125, iDpi, 96));
  int dpiScaledOffset = MulDiv(32, iDpi, 96);

  m_pParamCtrl[0] = new CParameterCrtl();
  m_pParamCtrl[0]->CreateNewParam(m_pBiotop->getParamFertility(),rect,this,1004);

  m_pParamCtrl[1] = new CParameterCrtl();
  rect.top += dpiScaledOffset;
  rect.bottom += dpiScaledOffset;
  m_pParamCtrl[1]->CreateNewParam(m_pBiotop->getParamSunlight(),rect,this,1004);

  m_pParamCtrl[2] = new CParameterCrtl();
  rect.top += dpiScaledOffset;
  rect.bottom += dpiScaledOffset;
  m_pParamCtrl[2]->CreateNewParam(m_pBiotop->getParamRadioactivity(),rect,this,1004);

  m_pParamCtrl[3] = new CParameterCrtl();
  rect.top += dpiScaledOffset;
  rect.bottom += dpiScaledOffset;
  m_pParamCtrl[3]->CreateNewParam(m_pBiotop->getParamTemperature(),rect,this,1004);

  return (true);
}

void CBioCtrlView::OnCheckModeManual() 
{
	// TODO: Add your control notification handler code here
  UpdateData();
	theApp.SetModeManual(m_bManualMode);
}

void CBioCtrlView::ForceModeManual(BOOL newMode)
{
  if (m_bManualMode!=newMode)
  {
    m_bManualMode = newMode;
    UpdateData(false);
  }
}

void CBioCtrlView::OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
  theApp.SetModeMaxSpeed(false);
	theApp.modifyBiotopSpeed(m_SliderSpeed.GetPos());
  m_UserSpeed = m_SliderSpeed.GetPos();
	*pResult = 0;
}

void CBioCtrlView::OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	theApp.GetBiotopViewPtr()->SetZoomRate(m_SliderZoom.GetPos());
	*pResult = 0;
}

bool CBioCtrlView::ForceChangeSpeed(int rate)
{
  int newPos = m_SliderSpeed.GetPos()+rate;
  bool resu = true;

  // Check bounds
  if (newPos<0)
  {
    newPos = 0;
    resu = false;
  }
  if (newPos>100)
  {
    newPos = 100;
    resu = false;
  }
  m_SliderSpeed.SetPos(newPos);
  RedrawWindow();
  theApp.SetModeMaxSpeed(false);
  theApp.modifyBiotopSpeed(m_SliderSpeed.GetPos());
  return (resu);
}

bool CBioCtrlView::ForceSetSpeed(float rate)
{
  int newPos = cybio_round(rate);
  bool resu = true;

  // Check bounds
  if (newPos < 0)
  {
    newPos = 0;
    resu = false;
  }
  if (newPos > 100)
  {
    newPos = 100;
    resu = false;
  }
  m_SliderSpeed.SetPos(newPos);
  m_UserSpeed = m_SliderSpeed.GetPos();
  RedrawWindow();
  theApp.modifyBiotopSpeed(rate);
  return (resu);
}

bool CBioCtrlView::RestoreUserSpeed()
{
  if (m_SliderSpeed.GetPos()<m_UserSpeed)
    ForceChangeSpeed(1);

  return true;
}

void CBioCtrlView::UpdateTimerDisplay(BiotopTime_t* pNewTime)
{
  m_StrTime.Format(LPCTSTR("Y%d D%d %02d:%02d:%02d"), pNewTime->years, pNewTime->days,
                   pNewTime->hours, pNewTime->seconds/60, pNewTime->seconds%60); 
  UpdateData(false);
  UpdateParametersDisplay(false);
}

void CBioCtrlView::UpdateParametersDisplay(bool resetMinMax)
{
  // Update also parameter bars
  for (int i = 0; (i < MAX_NUMBER_BIOCTRL_DISPLAY); i++)
  {
    if (m_pParamCtrl[i] != NULL)
      m_pParamCtrl[i]->RefreshDisplayParam(resetMinMax);
  }
  UpdateData(false);
}

void CBioCtrlView::OnEdit() 
{
	// TODO: Add your control notification handler code here
	CBiotopEditorDlg dlg(m_pBiotop);
  dlg.DoModal();
  m_pParamCtrl[3]->RefreshDisplayParam(true);

  // Redraw biotop View
  UpdateParametersDisplay(true);
  theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
}

void CBioCtrlView::OnDefaultSpeed()
{
  theApp.SetModeMaxSpeed(false);
  theApp.modifyBiotopSpeed(1.2);
  m_SliderSpeed.SetPos(1);
  m_UserSpeed = m_SliderSpeed.GetPos();
  RedrawWindow();
}

void CBioCtrlView::OnMaxSpeed()
{
  theApp.modifyBiotopSpeed(100);
  theApp.SetModeMaxSpeed(true);
  m_SliderSpeed.SetPos(100);
  m_UserSpeed = m_SliderSpeed.GetPos();
  RedrawWindow();
}

void CBioCtrlView::OnCheckStopOnEv() 
{
	// TODO: Add your control notification handler code here
  UpdateData();
	theApp.SetModeStopOnEvent(m_bStopOnEvent);
}


void CBioCtrlView::SetStrScenarioScore(CString text)
{
  m_StrScenarioScore = text;
  UpdateData(false);
}


void CBioCtrlView::OnSelchangeComboWind() 
{
  if (m_ComboWind.GetCurSel() == 0)
  {
    theApp.GetBiotop()->setWindStrenght(0);
  }
  else
  {
    theApp.GetBiotop()->setWindStrenght(1);
    theApp.GetBiotop()->setWindDirection(m_ComboWind.GetCurSel()-1);
  }
}
