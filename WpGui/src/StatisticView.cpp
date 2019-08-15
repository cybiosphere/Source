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

// StatisticView.cpp : implementation of the CStatisticView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "StatisticDoc.h"
#include "StatisticView.h"
#include "MeasureEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CStatisticView

IMPLEMENT_DYNCREATE(CStatisticView, CFormView)

BEGIN_MESSAGE_MAP(CStatisticView, CFormView)
	//{{AFX_MSG_MAP(CStatisticView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_EDIT, OnEditMeasure)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticView construction/destruction

CStatisticView::CStatisticView()
	: CFormView(CStatisticView::IDD)
{
	//{{AFX_DATA_INIT(CStatisticView)
	m_radioDuration = -1;
	//}}AFX_DATA_INIT

  m_pBiotop = NULL;
  m_TimeScaleSecPerPix = 1;
  m_RefreshCounter = 0;
  m_TimeMin = 0;
  m_TimeMax = 0;
  m_IsScrollMode = false;

  for (int i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
  {
    m_pMeasDisplayList[i].pMeasure = NULL;
    m_pMeasDisplayList[i].pChkBox  = NULL;
    m_pMeasDisplayList[i].color    = 0;
  }

  m_pTmpPointTable = new POINT[MAX_MEASUREMENT_DATA_SIZE];

  m_pFont = new CFont; 
  m_pFont->CreatePointFont(72, LPCTSTR("Arial"));

}

CStatisticView::~CStatisticView()
{
  for (int i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
  {
    if (m_pMeasDisplayList[i].pChkBox!= NULL)
    {
      delete (m_pMeasDisplayList[i].pChkBox);
      m_pMeasDisplayList[i].pChkBox = NULL;
      m_pMeasDisplayList[i].pMeasure = NULL;
      m_pMeasDisplayList[i].color    = 0;
    }
  }

  if (m_pTmpPointTable)
    delete [] m_pTmpPointTable;

  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }
}

void CStatisticView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticView)
	DDX_Radio(pDX, IDC_RADIO1, m_radioDuration);
	//}}AFX_DATA_MAP
}

BOOL CStatisticView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CStatisticView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
  m_radioDuration = 0;
  OnRadio1();
  UpdateData(false);
	ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CStatisticView printing

BOOL CStatisticView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStatisticView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStatisticView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CStatisticView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CStatisticView diagnostics

#ifdef _DEBUG
void CStatisticView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStatisticView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CStatisticDoc* CStatisticView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStatisticDoc)));
	return (CStatisticDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStatisticView message handlers

/////////////////////////////////////////////////////////////////////////////
// CStatisticView functionnal

bool CStatisticView::SetBiotop(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;
  RebuildMeasChkBox();

  return (true);
}

void CStatisticView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  CBrush brush(0x00FFFFFF);
  CRect frameRect = m_GraphRect;
  COLORREF black = 0x00000000;

  frameRect.top -= 18;
  frameRect.bottom += 18;

  dc.FillRect(frameRect,&brush);
  dc.SelectObject(m_pFont);

  for (int i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
  { 
    if( (m_pMeasDisplayList[i].pChkBox!=NULL) && (m_pMeasDisplayList[i].pChkBox->GetState()&0x0003) )
    {
      dc.Draw3dRect(m_pMeasDisplayList[i].coord, m_pMeasDisplayList[i].color, m_pMeasDisplayList[i].color);
      DrawMeasure( &dc, m_pMeasDisplayList[i].pMeasure, m_pMeasDisplayList[i].color,i);
    }
  }

  dc.Draw3dRect(frameRect, black, 0x00BBBBBB);
  CPen penBlack(PS_SOLID,1,black);
  dc.SelectObject(&penBlack);
  dc.MoveTo(m_GraphRect.left+10, m_GraphRect.top - 12);
  dc.LineTo(m_GraphRect.left+10, m_GraphRect.bottom + 12);
  dc.MoveTo(m_GraphRect.left+6, m_GraphRect.top);
  dc.LineTo(m_GraphRect.left+14, m_GraphRect.top);
  dc.MoveTo(m_GraphRect.left+6, m_GraphRect.bottom);
  dc.LineTo(m_GraphRect.left+14, m_GraphRect.bottom);

	// Do not call CFormView::OnPaint() for painting messages
}

void CStatisticView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
  if (cx>420)
  {
	  m_GraphRect.SetRect(420, 30, cx-12, cy-30);
    ResetMinMaxTime();
  }
}

void CStatisticView::RebuildMeasChkBox()
{
  int i;
  // Delete previous Button in Table
  for (i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
  {
    if (m_pMeasDisplayList[i].pChkBox != NULL)
    {
      delete (m_pMeasDisplayList[i].pChkBox);
      m_pMeasDisplayList[i].pChkBox  = NULL;
      m_pMeasDisplayList[i].pMeasure = NULL;
      m_pMeasDisplayList[i].color    = 0;
    }
  }

  if (m_pBiotop!=NULL)
  {
    int startX = 12;
    int startY = 24;
    for (i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
    {
      CMeasure* pMeas = m_pBiotop->getMeasureById(i);
      if (pMeas!=NULL)
      {
        m_pMeasDisplayList[i].pChkBox = new CColorButton();
        CRect rect(startX, startY, startX+325, startY+18);
        m_pMeasDisplayList[i].pChkBox->Create(LPCTSTR(pMeas->GetLabel().c_str()),BS_AUTOCHECKBOX|WS_VISIBLE|WS_TABSTOP,rect,this,2100+i );
        m_pMeasDisplayList[i].pMeasure = pMeas;
        m_pMeasDisplayList[i].color    = RGB(startY*4,(325-startY*3),startY*2);
        m_pMeasDisplayList[i].coord.SetRect(startX-1,startY-1,startX+ 325,startY+20);
      }
      startY += 22;
    }
  }
}

void CStatisticView::OnRadio1() 
{
  m_TimeScaleSecPerPix = 1;
  ResetMinMaxMeasures();
  ResetMinMaxTime();
  UpdateData(true);
}

void CStatisticView::OnRadio2() 
{
  m_TimeScaleSecPerPix = 60;
  ResetMinMaxMeasures();
	ResetMinMaxTime();
  UpdateData(true);
}

void CStatisticView::OnRadio3() 
{
  m_TimeScaleSecPerPix = 3600;
  ResetMinMaxMeasures();
	ResetMinMaxTime();
  UpdateData(true);
}

void CStatisticView::OnRadio4() 
{
  m_TimeScaleSecPerPix = 86400;
  ResetMinMaxMeasures();
  ResetMinMaxTime();
  UpdateData(true);
}

void CStatisticView::OnRadio5() 
{
  m_TimeScaleSecPerPix = 86400*365;
  ResetMinMaxMeasures();
  ResetMinMaxTime();
  UpdateData(true);
}

void CStatisticView::NextSecond()
{
  m_RefreshCounter += 1;
  if (m_RefreshCounter==m_TimeScaleSecPerPix)
  {
    if (m_IsScrollMode)
    {
      m_TimeMin+=m_TimeScaleSecPerPix;
      m_TimeMax+=m_TimeScaleSecPerPix;
    }
    m_RefreshCounter = 0;

    CRect frameRect = m_GraphRect;
    frameRect.top -= 18;
    frameRect.bottom += 18;
    InvalidateRect(frameRect,false);
  }
}

void CStatisticView::ResetMinMaxTime()
{
  if (m_pBiotop == NULL)
    return;

  m_RefreshCounter = 0;
  m_IsScrollMode = false;
  m_TimeMax = convertBioTimeToCount(m_pBiotop->getBiotopTime());

  if (m_TimeMax > (m_TimeScaleSecPerPix*(timeCountType)m_GraphRect.Width()))
  {
    m_TimeMin = m_TimeMax - m_TimeScaleSecPerPix * (timeCountType)m_GraphRect.Width();
  }
  else
  {
    m_TimeMin = 1;
    m_TimeMax = m_TimeScaleSecPerPix * (timeCountType)m_GraphRect.Width();
  }
  RedrawWindow();
}

void CStatisticView::ResetMinMaxMeasures()
{
  for (int i=0;i<MAX_NB_DISPLAY_MEASURES;i++)
  {
    if (m_pMeasDisplayList[i].pMeasure != NULL)
      m_pMeasDisplayList[i].pMeasure->AutoUpdateRange();
  }
}

void CStatisticView::DrawMeasure(CPaintDC *pDC, CMeasure* pMeasure, COLORREF color, int index)
{
  MeasureData_t* pData = pMeasure->GetPMeasureData();
  int indexData = pMeasure->GetIndexCurData();
  int indexPoint = 0;
  //int x,y;
  double curData;
  double maxData = pMeasure->GetRangeMax();
  double minData = pMeasure->GetRangeMin();
  double tempValue;

  timeCountType curTimeCount;
  double timeWidth = m_TimeMax - m_TimeMin;
  if (timeWidth==0)
    return;

  for (int i=0;i<MAX_MEASUREMENT_DATA_SIZE;i++)
  {
    // Point on value following current
    indexData = (indexData+1)%MAX_MEASUREMENT_DATA_SIZE;

    curTimeCount = pData[indexData].timeCount;
    curData = pData[indexData].value;
    if ( (curTimeCount >= m_TimeMin) && (curTimeCount <= m_TimeMax) )
    {
      tempValue = (double)(curTimeCount - m_TimeMin) * (double)m_GraphRect.Width() / timeWidth + (double)m_GraphRect.left;
      m_pTmpPointTable[indexPoint].x = (timeCountType)tempValue;
      m_pTmpPointTable[indexPoint].y = (maxData-curData)*m_GraphRect.Height()/(maxData-minData) + m_GraphRect.top;
      indexPoint++;
    }
  }

  if (indexPoint>1)
  {
    CPen penColor(PS_SOLID,1,color);
    pDC->SelectObject(&penColor);
    pDC->Polyline(m_pTmpPointTable,indexPoint);
    
    pDC->SetBkColor(0x00FFFFFF);
    pDC->SetTextColor(color);    
    
    CString valStr;
    curData = pMeasure->GetCurrentValue();
    valStr.Format(LPCTSTR("max %5.1f"),maxData);
    pDC->TextOut(m_GraphRect.left + 10 + 60 * index, m_GraphRect.top - 12, valStr); 
    valStr.Format(LPCTSTR("cur %5.1f"), curData);
    pDC->TextOut(m_GraphRect.left + 10 + 60 * index, m_GraphRect.top + 4, valStr);
    valStr.Format(LPCTSTR("min %5.1f"),minData);
    pDC->TextOut(m_GraphRect.left + 10 + 60 * index, m_GraphRect.bottom + 1, valStr); 

    // Check if end is reached
    if ( m_pTmpPointTable[indexPoint-1].x > (m_GraphRect.right-20) )
    {
      m_IsScrollMode = true;
    }
  }
}

void CStatisticView::OnEditMeasure() 
{
	CMeasureEditorDlg measureDlg(m_pBiotop);
  measureDlg.DoModal();
	RebuildMeasChkBox();
}
