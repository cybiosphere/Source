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

// LogServerView.cpp : implementation of the CLogServerView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "LogServerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLogServerView

IMPLEMENT_DYNCREATE(CLogServerView, CFormView)

BEGIN_MESSAGE_MAP(CLogServerView, CFormView)
	//{{AFX_MSG_MAP(CLogServerView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogServerView construction/destruction

CLogServerView::CLogServerView()
	: CFormView(CLogServerView::IDD)
{
	//{{AFX_DATA_INIT(CLogServerView)
  m_strLog = _T("");
	//}}AFX_DATA_INIT

}

CLogServerView::~CLogServerView()
{

}

void CLogServerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogServerView)
  DDX_Control(pDX, IDC_EDIT_LOG, m_ServerLogEditBox);
  DDX_Text(pDX, IDC_EDIT_LOG, m_strLog);
	//}}AFX_DATA_MAP
}

BOOL CLogServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CLogServerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
  UpdateData(false);
	ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CLogServerView printing

BOOL CLogServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLogServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLogServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLogServerView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CLogServerView diagnostics

#ifdef _DEBUG
void CLogServerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogServerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLogServerDoc* CLogServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogServerDoc)));
	return (CLogServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogServerView message handlers

/////////////////////////////////////////////////////////////////////////////
// CLogServerView functionnal


void CLogServerView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
  if (m_ServerLogEditBox.m_hWnd != NULL)
  {
    m_ServerLogEditBox.MoveWindow(1, 1, cx - 2, cy - 2, true);
    m_ServerLogEditBox.LineScroll(m_ServerLogEditBox.GetLineCount());
    UpdateWindow();
  }
}

void CLogServerView::AddLog(std::string newLog)
{
  if (newLog != "")
  {
    m_strLog += newLog.c_str();
    UpdateData(false);
    m_ServerLogEditBox.LineScroll(m_ServerLogEditBox.GetLineCount());
  }
}


