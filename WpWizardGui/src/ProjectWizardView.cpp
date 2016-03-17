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

// ProjectWizardView.cpp : implementation of the CProjectWizardView class
//

#include "stdafx.h"
#include "ProjectWizard.h"

#include "ProjectWizardDoc.h"
#include "ProjectWizardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardView

IMPLEMENT_DYNCREATE(CProjectWizardView, CFormView)

BEGIN_MESSAGE_MAP(CProjectWizardView, CFormView)
	//{{AFX_MSG_MAP(CProjectWizardView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardView construction/destruction

CProjectWizardView::CProjectWizardView()
	: CFormView(CProjectWizardView::IDD)
{
	//{{AFX_DATA_INIT(CProjectWizardView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
    m_pEntity = NULL;
    m_ClassType = CLASS_UNSET;
    m_bSexualDimorphism = false;
}

CProjectWizardView::~CProjectWizardView()
{
  if ( m_pEntity != NULL)
    delete m_pEntity;
}

void CProjectWizardView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectWizardView)
	DDX_Control(pDX, IDC_TAB1, m_WizardTab);
	//}}AFX_DATA_MAP
}

BOOL CProjectWizardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CProjectWizardView::OnInitialUpdate()
{	
    CFormView::OnInitialUpdate();
    ResizeParentToFit(FALSE);

    if (m_WizardTab.m_hWnd != NULL)
    {
      m_WizardTab.Init();
    }
}

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardView diagnostics

#ifdef _DEBUG
void CProjectWizardView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectWizardView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CProjectWizardDoc* CProjectWizardView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectWizardDoc)));
	return (CProjectWizardDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardView message handlers

void CProjectWizardView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (m_WizardTab.m_hWnd != NULL)
	{
		m_WizardTab.Resize(cx, cy);
		UpdateWindow ();
	}

}

CBasicEntity* CProjectWizardView::GetpEntity()
{
  return(m_pEntity);
}

ClassType_e CProjectWizardView::GetClassType()
{
  return(m_ClassType);
}

bool CProjectWizardView::IsSexualDimorphism()
{
  return(m_bSexualDimorphism);
}

void CProjectWizardView::SetpEntity(CBasicEntity* pEntity, bool bSexualDimorphism)
{
  // delete previous entity
  if (m_pEntity != NULL)
    delete m_pEntity;

  m_pEntity = pEntity;
  m_ClassType = pEntity->getClass();
  m_bSexualDimorphism = bSexualDimorphism;

  // Check if behavior tabs have to be set
  if ((m_pEntity!=NULL) && (m_pEntity->getBrain() != NULL))
  {
    m_WizardTab.AddTabMask(TAB_MASK_SENSORS);
    m_WizardTab.AddTabMask(TAB_MASK_BEHAVIOR);
  }
  else
  {
    m_WizardTab.RemoveTabMask(TAB_MASK_SENSORS);
    m_WizardTab.RemoveTabMask(TAB_MASK_BEHAVIOR);
  }

  if (bSexualDimorphism)
  {
    m_WizardTab.AddTabMask(TAB_MASK_CAR_MALE);
  }
  else
  {
    m_WizardTab.RemoveTabMask(TAB_MASK_CAR_MALE);
  }

  m_WizardTab.UpdateTabsWithNewEntity();
  m_WizardTab.SetTabDisplay();
}


CDialogTab1* CProjectWizardView::GetTabCaractFemale()
{
  return ((CDialogTab1*)m_WizardTab.m_tabPages[0]);
}

CDialogTab1* CProjectWizardView::GetTabCaractMale()
{
  return ((CDialogTab1*)m_WizardTab.m_tabPages[1]);
}

CDialogTab2* CProjectWizardView::GetTabSensors()
{
  return ((CDialogTab2*)m_WizardTab.m_tabPages[2]);
}

CDialogTab3* CProjectWizardView::GetTabBehavior()
{
  return ((CDialogTab3*)m_WizardTab.m_tabPages[3]);
}

CDialogTab4* CProjectWizardView::GetTabGenerate()
{
  return ((CDialogTab4*)m_WizardTab.m_tabPages[4]);
}