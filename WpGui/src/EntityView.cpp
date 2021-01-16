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

// EntityView.cpp : implementation of the CEntityView class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "EntityDoc.h"
#include "EntityView.h"
#include "EntityEditorDlg.h"
#include "CPhysicalWelfare.h"
#include "CAnimMammal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEntityView

IMPLEMENT_DYNCREATE(CEntityView, CFormView)

BEGIN_MESSAGE_MAP(CEntityView, CFormView)
	//{{AFX_MSG_MAP(CEntityView)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_IMMORTAL, OnCheckImmortal)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityView construction/destruction

CEntityView::CEntityView()
	: CFormView(CEntityView::IDD)
{
	//{{AFX_DATA_INIT(CEntityView)
	m_nEntityId = ENTITY_ID_INVALID;
	m_Label = _T("");
	m_StageString = _T("");
	m_checkImmortal = FALSE;
	m_PurposeString = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

  m_pEntity = NULL;
  for (int i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    m_pParamCtrl[i] = NULL;
  }

  m_pIconSex        = NULL;
  m_pIconForm       = NULL;
  m_pIconOdor       = NULL;
  m_pIconPheromon   = NULL;
  m_pIconTaste      = NULL;
  m_pIconTexture    = NULL;
  m_pIconAttribute  = NULL;
  m_pIconHealth     = NULL;
}

CEntityView::~CEntityView()
{
  if (m_pIconSex!=NULL)
    delete (m_pIconSex);
  if (m_pIconForm!=NULL)
    delete (m_pIconForm);
  if (m_pIconOdor!=NULL)
    delete (m_pIconOdor);
  if (m_pIconPheromon!=NULL)
    delete (m_pIconPheromon);
  if (m_pIconTaste!=NULL)
    delete (m_pIconTaste);
  if (m_pIconTexture!=NULL)
    delete (m_pIconTexture);
  if (m_pIconAttribute!=NULL)
    delete (m_pIconAttribute);
  if (m_pIconHealth!=NULL)
    delete (m_pIconHealth);
  
  for (int i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
  {
    if (m_pParamCtrl[i] != NULL)
    {
      delete m_pParamCtrl[i];
      m_pParamCtrl[i] = NULL;
    }
  }
}

void CEntityView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityView)
	DDX_Text(pDX, IDC_EDIT1, m_nEntityId);
	DDX_Text(pDX, IDC_EDIT2, m_Label);
	DDX_Text(pDX, IDC_EDIT3, m_StageString);
	DDX_Check(pDX, IDC_CHECK_IMMORTAL, m_checkImmortal);
	DDX_Text(pDX, IDC_EDIT4, m_PurposeString);
	//}}AFX_DATA_MAP
}

BOOL CEntityView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CEntityView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

  m_pIconSex = new CImageList();
  ASSERT(m_pIconSex != NULL);  
  m_pIconSex->Create(IDB_SEX_ICONS,16,16,0x0FFF);
  m_pIconForm = new CImageList();
  ASSERT(m_pIconForm != NULL);  
  m_pIconForm->Create(IDB_FORM_ICONS,16,16,0x0FFF);
  m_pIconOdor = new CImageList();
  ASSERT(m_pIconOdor != NULL);  
  m_pIconOdor->Create(IDB_ODOR_ICONS,16,16,0x0FFF);
  m_pIconPheromon = new CImageList();
  ASSERT(m_pIconPheromon != NULL);  
  m_pIconPheromon->Create(IDB_PHEROMON_ICONS,16,16,0x0FFF);
  m_pIconTaste = new CImageList();
  ASSERT(m_pIconTaste != NULL);  
  m_pIconTaste->Create(IDB_TASTE_ICONS,16,16,0x0FFF);
  m_pIconTexture = new CImageList();
  ASSERT(m_pIconTexture != NULL);  
  m_pIconTexture->Create(IDB_TEXTURE_ICONS,16,16,0x0FFF);
  m_pIconAttribute = new CImageList();
  ASSERT(m_pIconAttribute != NULL);  
  m_pIconAttribute->Create(IDB_ATTRIBUTE_ICONS,16,16,0x0FFF);
  m_pIconHealth = new CImageList();
  ASSERT(m_pIconHealth != NULL);  
  m_pIconHealth->Create(IDB_HEALTH_ICONS,16,16,0x0FFF);

}

/////////////////////////////////////////////////////////////////////////////
// CEntityView printing

BOOL CEntityView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEntityView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEntityView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEntityView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CEntityView diagnostics

#ifdef _DEBUG
void CEntityView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEntityView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEntityDoc* CEntityView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEntityDoc)));
	return (CEntityDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEntityView message handlers

/////////////////////////////////////////////////////////////////////////////
// CEntityView functionnal

void CEntityView::SelectAndDisplayEntity (CBasicEntity* pEntity)
{
  int i;

  if (pEntity==NULL)
  {
    m_pEntity = NULL;
    // Update data 
    m_nEntityId = ENTITY_ID_INVALID;
    m_Label = "No selection";
    m_StageString = "";

    // Clean previous paramCtrl
    for (i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
    {
      if (m_pParamCtrl[i] != NULL)
      {
        delete m_pParamCtrl[i];
        m_pParamCtrl[i] = NULL;
      }
    }

    // Clear Brain view
	  theApp.GetBrainViewPtr()->SetBrain(NULL);
    // Clear Genome view
    theApp.GetGeneticViewPtr()->SetEntity(NULL);
  }
  else
  {
    if (m_pEntity != pEntity) // New entity
    {
      m_pEntity = pEntity;
      // Update data
      m_nEntityId = m_pEntity->getId();
      m_Label = m_pEntity->getLabel().c_str();
      if (m_pEntity->isRemoteControlled())
      {
        // identify remote control with *
        m_Label = "*" + m_Label;
      }

      if (m_pEntity->getCurrentLifeStage()!=NULL)
      {
        m_StageString = m_pEntity->getCurrentLifeStage()->getLabel().c_str();
      }
      else
      {
        m_StageString = "";
      }

      if ( (m_pEntity->getBrain()) && (m_pEntity->getBrain()->GetCurrentPurpose()) )
      {
        m_PurposeString = m_pEntity->getBrain()->GetCurrentPurpose()->GetLabel().c_str();
      }
      else
      {
        m_PurposeString = "";
      }

      // Clean previous paramCtrl
      for (i=0;i<MAX_NUMBER_PARAMETER_DISPLAY;i++)
      {
        if (m_pParamCtrl[i] != NULL)
        {
          delete m_pParamCtrl[i];
          m_pParamCtrl[i] = NULL;
        }
      }
      // Set new params
      int iDpi = GetDpiForWindow(this->m_hWnd);

      int startY = MulDiv(100, iDpi, 96) - GetScrollPos(SB_VERT);
      int endY = startY + MulDiv(7, iDpi, 96);
      for (i=0;(i<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<m_pEntity->getNumParameter());i++)
      {
        m_pParamCtrl[i] = new CParameterCrtl();
        CRect rect(MulDiv(16, iDpi, 96), startY, MulDiv(190, iDpi, 96), endY);
        m_pParamCtrl[i]->CreateNewParam(m_pEntity->getParameter(i),rect,this,1003);
        startY += MulDiv(34, iDpi, 96);
        endY += MulDiv(34, iDpi, 96);
      }

      // Update Brain view
	    theApp.GetBrainViewPtr()->SetBrain(m_pEntity->getBrain());
      // Update Genome view
      theApp.GetGeneticViewPtr()->SetEntity(m_pEntity);
    }
    else // Just refresh params values
    {
      for (int i=0;(i<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<m_pEntity->getNumParameter());i++)
      {
        m_pParamCtrl[i]->RefreshDisplayParam();
      }
    }

    m_checkImmortal = m_pEntity->isImmortal();
  }

  // Update windows
  UpdateData(false);
  RedrawWindow();

}

void CEntityView::RefreshParameters()
{
  if (m_pEntity!=NULL)
  {
    for (int i=0;(i<MAX_NUMBER_PARAMETER_DISPLAY)&&(i<m_pEntity->getNumParameter());i++)
    {
      m_pParamCtrl[i]->RefreshDisplayParam();
    }

    if (m_pEntity->getCurrentLifeStage()!=NULL)
    {
      m_StageString = m_pEntity->getCurrentLifeStage()->getLabel().c_str();
    }
    else
    {
      m_StageString = "";
    }
    if ( (m_pEntity->getBrain()) && (m_pEntity->getBrain()->GetCurrentPurpose()) )
    {
      m_PurposeString = m_pEntity->getBrain()->GetCurrentPurpose()->GetLabel().c_str();
    }
    else
    {
      m_PurposeString = "";
    }  
    UpdateData(false);
  }
}

void CEntityView::OnPaint() 
{
  // Prepare icon drawing
  CPaintDC dc(this);      // device context for painting
  CDC* pDc = &dc;

  CPoint coord;
  int iDpi = GetDpiForWindow(this->m_hWnd);

  coord.x = MulDiv(90, iDpi, 96);
  coord.y = MulDiv(40, iDpi, 96) - GetScrollPos(SB_VERT);
  // Color
  m_pIconSex->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame
  if (m_pEntity != NULL)
    pDc->FillSolidRect( coord.x + 1, coord.y + 1, 14, 14, m_pEntity->getColorRgb() );

  // Form
  coord.x += 18;
  if (m_pEntity != NULL)
    m_pIconForm->Draw(pDc,m_pEntity->getForm(),coord,ILD_NORMAL);
  else
    m_pIconSex->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Texture
  coord.x += 18;
  if (m_pEntity != NULL)
    m_pIconTexture->Draw(pDc,m_pEntity->getTexture(),coord,ILD_NORMAL);
  else
    m_pIconTexture->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Taste
  coord.x += 18;
  if (m_pEntity != NULL)
    m_pIconTaste->Draw(pDc,m_pEntity->getTaste(),coord,ILD_NORMAL);
  else
    m_pIconTaste->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Attribute
  if (m_pEntity != NULL)
  {
    for (PhyAttributeType_e attrib=PHY_ATTRIBUTE_FIRST_TYPE; attrib<PHY_ATTRIBUTE_NUMBER_TYPE; attrib=(PhyAttributeType_e)((int)attrib+1))
    {
      if (m_pEntity->isPhyAttributePresent(attrib))
      {
        coord.x += 18;
        m_pIconAttribute->Draw(pDc,attrib,coord,ILD_NORMAL);
      }
    }
  }

  // Sex
  coord.x = MulDiv(90, iDpi, 96);
  coord.y += 20;
  if (m_pEntity != NULL)
    m_pIconSex->Draw(pDc,m_pEntity->getSex(),coord,ILD_NORMAL);
  else
    m_pIconSex->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Repro type
  coord.x += 18;
  m_pIconSex->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Odor
  coord.x += 18;
  if (m_pEntity != NULL)
    m_pIconOdor->Draw(pDc,m_pEntity->getOdor(),coord,ILD_NORMAL);
  else
    m_pIconOdor->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Pherom
  coord.x += 18;
  if (m_pEntity != NULL)
    m_pIconPheromon->Draw(pDc,m_pEntity->getPheromone(),coord,ILD_NORMAL);
  else
    m_pIconPheromon->Draw(pDc,0,coord,ILD_NORMAL); // Draw empty frame

  // Health
  if (m_pEntity != NULL)
  {
    CPhysicalWelfare* pPhysWelfare = m_pEntity->getpPhysicalWelfare();
    if (pPhysWelfare != NULL)
    {
      coord.x += 18;
      if (pPhysWelfare->GetInjuryMalus()>0.2)
        m_pIconHealth->Draw(pDc,1,coord,ILD_NORMAL); // Draw icon
      else if (pPhysWelfare->GetInjuryMalus()>0)
        m_pIconHealth->Draw(pDc,0,coord,ILD_NORMAL); // Draw icon
      else 
        coord.x -= 18;

      coord.x += 18;
      if (pPhysWelfare->GetDiseaseMalus()>0.2)
        m_pIconHealth->Draw(pDc,3,coord,ILD_NORMAL); // Draw icon
      else if (pPhysWelfare->GetDiseaseMalus()>0)
        m_pIconHealth->Draw(pDc,2,coord,ILD_NORMAL); // Draw icon
    }
  }
}

void CEntityView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
  RedrawWindow();
  CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEntityView::OnCheckImmortal() 
{
  UpdateData(true);
  if (m_pEntity != NULL)
  {
    m_pEntity->setImmortal(m_checkImmortal);
    theApp.refreshSelectedEntity(m_pEntity);
  }
}

void CEntityView::OnEdit() 
{
  if (m_pEntity != NULL)
  {
    CEntityEditorDlg dlg(m_pEntity);
    dlg.DoModal();
    // Refresh label
    m_Label = m_pEntity->getLabel().c_str();
    if (m_pEntity->getClass() == CLASS_MAMMAL)
    {
      ((CAnimMammal*)m_pEntity)->checkCoherenceAndAdjustBabyNumberParam();
    }
    UpdateData(false);
  }
  theApp.refreshSelectedEntity(m_pEntity);
}
