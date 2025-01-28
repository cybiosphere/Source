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

// MapEditorView.cpp : implementation of the CMapEditorView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "MapEditorDoc.h"
#include "MapEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CFormView)

BEGIN_MESSAGE_MAP(CMapEditorView, CFormView)
  //{{AFX_MSG_MAP(CMapEditorView)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
  ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
  ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
  ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
  ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
  ON_BN_CLICKED(IDC_EDIT, OnSpawnEntities)
  ON_CBN_SELCHANGE(IDC_COMBO1, OnSelChangeComboGround)
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView construction/destruction

CMapEditorView::CMapEditorView()
  : CFormView(CMapEditorView::IDD)
{
  //{{AFX_DATA_INIT(CMapEditorView)
  m_radioEditMode = -1;
  //}}AFX_DATA_INIT

  m_pBiotop = NULL;
  m_pFont = new CFont; 
  m_pFont->CreatePointFont(72, LPCTSTR("Arial"));
  m_editMode = MAP_EDITOR_MODE_NONE;
}

CMapEditorView::~CMapEditorView()
{
  if (m_pFont != NULL)
  {
    m_pFont->DeleteObject();
    delete m_pFont;
  }
}

void CMapEditorView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMapEditorView)
  DDX_Radio(pDX, IDC_RADIO1, m_radioEditMode);
  DDX_Control(pDX, IDC_COMBO1, m_ComboGroundType);
  //}}AFX_DATA_MAP
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CFormView::PreCreateWindow(cs);
}

void CMapEditorView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();
  m_radioEditMode = 0;
  m_ComboGroundType.SetCurSel(0);
  OnRadio1();
  UpdateData(false);
  ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView printing

BOOL CMapEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CMapEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CMapEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

void CMapEditorView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
  // TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView diagnostics

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
  CFormView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}

CStatisticDoc* CMapEditorView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStatisticDoc)));
  return (CStatisticDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView message handlers

/////////////////////////////////////////////////////////////////////////////
// CMapEditorView functionnal

bool CMapEditorView::SetBiotop(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;
  return (true);
}

void CMapEditorView::OnSize(UINT nType, int cx, int cy) 
{
}

void CMapEditorView::OnRadio1() 
{
  m_editMode = MAP_EDITOR_MODE_NONE;
  UpdateData(true);
}

void CMapEditorView::OnRadio2()
{
  m_editMode = (MapEditorModeType_e)(MAP_EDITOR_MODE_GROUND_NONE + m_ComboGroundType.GetCurSel());
  UpdateData(true);
}

void CMapEditorView::OnRadio3() 
{
  m_editMode = MAP_EDITOR_MODE_FENCE;
  UpdateData(true);
}

void CMapEditorView::OnRadio4() 
{
  m_editMode = MAP_EDITOR_MODE_ROCK;
  UpdateData(true);
}

void CMapEditorView::OnRadio5() 
{
  m_editMode = MAP_EDITOR_MODE_NONE;
  UpdateData(true);
}

void CMapEditorView::OnSpawnEntities()
{
}

void CMapEditorView::OnSelChangeComboGround()
{
  OnRadio2();
}