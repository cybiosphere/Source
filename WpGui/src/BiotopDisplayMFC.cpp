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

// BiotopDisplay.cpp : implementation of the CBiotopDisplayMFC class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "BiotopView.h"
#include "BiotopDisplayMFC.h"

#include "CBrain.h"
#include "CBiotop.h"
#include "CBasicEntity.h"

#include "CLifeStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC construction/destruction

CBiotopDisplayMFC::CBiotopDisplayMFC(bool isSatelliteVew)
{
  m_bAppIsActive = false;

  m_nBitmapNumberX = 0;
  m_nBitmapNumberY = 0;
  m_nBitmapPixSizeX = 16;
  m_nBitmapPixSizeY = 16;
  m_ZoomFactor = 1.0;
  m_SatelliteView = isSatelliteVew;

  m_curViewSizeX = 0;
  m_curViewSizeY = 0;

  m_pView = NULL;
}

CBiotopDisplayMFC::~CBiotopDisplayMFC()
{
  KillWindow();	// Shutdown
}


/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC init

bool CBiotopDisplayMFC::Initialize(CScrollView* pView) 
{

    m_pView = pView;

    if (  m_pView == NULL )	{							// Did We Get A Device Context?																	
        KillWindow ();													// Reset The Display
        AfxMessageBox (LPCTSTR("Can't Create A Device Context."), MB_OK | MB_ICONEXCLAMATION );
        return FALSE;												
    }

    m_bmpVeget.LoadBitmap(IDB_VEGET);
    m_bmpMineral.LoadBitmap(IDB_MINERAL);
    m_bmpAnim[0].LoadBitmap(IDB_ANIM0);
    m_bmpAnim[1].LoadBitmap(IDB_ANIM1);
    m_bmpAnim[2].LoadBitmap(IDB_ANIM2);
    m_bmpAnim[3].LoadBitmap(IDB_ANIM3);
    m_bmpAnim[4].LoadBitmap(IDB_ANIM4);
    m_bmpAnim[5].LoadBitmap(IDB_ANIM5);
    m_bmpAnim[6].LoadBitmap(IDB_ANIM6);

    m_bAppIsActive = TRUE;

    m_nBitmapNumberX = m_pBiotop->getDimension().x;
    m_nBitmapNumberY = m_pBiotop->getDimension().y;

    DisplayView();
    m_pView->ResizeParentToFit(TRUE);
    return true;
}

void CBiotopDisplayMFC::DisplayView()
{
  if (m_pView != NULL)
  {
    // Calculate the total size of this view
    CSize sizeTotal, sizePage, sizeLine;
    sizeTotal.cx = (m_nBitmapNumberX + 1) * m_nBitmapPixSizeX;
    sizeTotal.cy = (m_nBitmapNumberY + 1) * m_nBitmapPixSizeY;
    sizePage.cx = m_nBitmapPixSizeX * 100;
    sizePage.cy = m_nBitmapPixSizeY * 100;
    sizeLine.cx = m_nBitmapPixSizeX;
    sizeLine.cy = m_nBitmapPixSizeY;
    m_pView->SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
  }
}

//////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC KillWindow

void CBiotopDisplayMFC::KillWindow(void)										// Properly Kill The Window
{																																																				

}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC sizing

void CBiotopDisplayMFC::ResizeScene(int cx, int cy) 
{
	m_curViewSizeX = cx;
	m_curViewSizeY = cy;
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC button

Point_t CBiotopDisplayMFC::GetGridCoordFromScreenPos(CPoint screenPos)
{
  Point_t coordEntity;
  coordEntity.x = (screenPos.x + m_pView->GetScrollPos(SB_HORZ)) / m_nBitmapPixSizeX;
  coordEntity.y = (m_pView->GetTotalSize().cy - screenPos.y - m_pView->GetScrollPos(SB_VERT)) / m_nBitmapPixSizeY - 1;

  CRect refreshRect;
  refreshRect.left   = 0;
  refreshRect.top    = 0;
  refreshRect.right  = m_pView->GetTotalSize().cx;
  refreshRect.bottom = m_pView->GetTotalSize().cy;
  m_pView->InvalidateRect(refreshRect,true);

  return (coordEntity);
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC 

void CBiotopDisplayMFC::RedrawScene()
{
  if (!m_bAppIsActive)
    return;

  if (m_SatelliteView)
  {
    RedrawSceneSatelliteView();
  }
  else
  {
    RedrawSceneAerialView();
  }
}


 void CBiotopDisplayMFC::RedrawSceneAerialView()
 {
   if ((m_nBitmapNumberX != m_pBiotop->getDimension().x) || (m_nBitmapNumberY != m_pBiotop->getDimension().y))
   {
     // Update m_nBitmapNumberX if biotop has changed
     m_nBitmapNumberX = m_pBiotop->getDimension().x;
     m_nBitmapNumberY = m_pBiotop->getDimension().y;
     DisplayView();
   }

  int visibleCoordX = m_pView->GetScrollPos(SB_HORZ);
  int visibleCoordY = m_pView->GetScrollPos(SB_VERT);

  CPaintDC dc(m_pView); 
  CPaintDC* pDc = &dc;
  CDC MemDCEnt;

  // Create a memory device compatible with the above CPaintDC variable
  MemDCEnt.CreateCompatibleDC(pDc);
	
  // remind the previous bitmap
  CBitmap *bmpPrevious = MemDCEnt.SelectObject(&m_bmpVeget);

  size_t i,j,pos;
  int coordX, coordY;
  Point_t bioCoord;
  LayerType_e curLayer;
  COLORREF custColor;

  Point_t startLoopCoord = GetVisibleGridCoordStart();
  Point_t endLoopCoord = GetVisibleGridCoordEnd();
  for (i = startLoopCoord.x; i < endLoopCoord.x; i++)
  {
    for (j = startLoopCoord.y; j < endLoopCoord.y; j++)
    { 
      bioCoord.x = i;
      bioCoord.y = j;
      coordX = m_nBitmapPixSizeX * i - visibleCoordX;
      coordY = m_pView->GetTotalSize().cy - m_nBitmapPixSizeY * (j + 2) - visibleCoordY;

      // Draw water
      curLayer = m_pBiotop->getLayerType(bioCoord,1); 
      if (curLayer == LAYER_OVER_WATER)
      {
        pDc->FillSolidRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00DDCCBB);
      }
      else if (curLayer == LAYER_OVER_WET_GROUND)
      {
        pDc->FillSolidRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00FFEEDD);
      }
      else if (curLayer == LAYER_GLOBAL_GRASS)
      {
        pDc->FillSolidRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00EEFFEE);
      }
      else if (curLayer == LAYER_GLOBAL_ROCK)
      {
        pDc->FillSolidRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00FBF8F8);
      }
      // draw user color
      custColor = m_pBiotop->getCustomColor(bioCoord);
      if (custColor != 0x00FFFFFF)
      {
        pDc->FillSolidRect(coordX+1, coordY+1, m_nBitmapPixSizeX-2, m_nBitmapPixSizeY-2, custColor);
      }

      // Draw entity
      CBasicEntity* pEntity = m_pBiotop->findTopLevelEntity(bioCoord);
      if ( (pEntity!=NULL) && (pEntity->getId()>=ENTITY_ID_FIRST_USER_ENTITY) )
      {
        pos = (int)((m_nBitmapPixSizeX - 4) * pEntity->getDirection());

        // Display bitmap when zoom is max or squares with smaller zoom
        if (m_nBitmapPixSizeX == 16)
        {
          if ((pEntity->getClass() >= CLASS_VEGETAL_FIRST) && (pEntity->getClass() <= CLASS_VEGETAL_LAST))
            MemDCEnt.SelectObject(&m_bmpVeget);
          else if ((pEntity->getClass() >= CLASS_ANIMAL_FIRST) && (pEntity->getClass() <= CLASS_ANIMAL_LAST))
            MemDCEnt.SelectObject(&m_bmpAnim[pEntity->getCurrentLifeStage()->getStageType()]);
          else
            MemDCEnt.SelectObject(&m_bmpMineral);

          // Copy the bits from the memory DC into the current dc
          pDc->BitBlt(coordX + 2, coordY + 2, m_nBitmapPixSizeX - 4, m_nBitmapPixSizeY - 4, &MemDCEnt, pos, 0, SRCCOPY);
          // Draw a little color square with real color
          pDc->FillSolidRect(coordX + 6, coordY + 6, 4, 4, pEntity->getColorRgb());
        }
        else
        {
          pDc->FillSolidRect(coordX + 1, coordY + 1, m_nBitmapPixSizeX-2, m_nBitmapPixSizeY-2, pEntity->getColorRgb());
          // Draw a square around animals
          if (pEntity->getClass() >= CLASS_ANIMAL_FIRST)
          {
            pDc->Draw3dRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x000F0F0F, 0x000E0E0E);
          }
        }

        // Draw frame around marked entity
        if (pEntity->isMarked())
        {
          pDc->Draw3dRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00BB66FF, 0x00AA55FF);
        }

        // Draw frame around selected entity
        if (pEntity->getId() == m_nSelectedEntityId)
        {
          pDc->Draw3dRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY,0x001E1EEF,0x000E0ECF);
        }

        // Draw frame around focused entity
        if (pEntity->getId() == m_nFocusedEntityId)
        {
          pDc->Draw3dRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY,0x001EEEEF,0x000ECECF);
        }
      }
    } 
  } 

  // Draw Map Editor helpers
  if (m_bMapEditorModeLine)
  {
    coordX = m_nBitmapPixSizeX * m_clickStartGridCoord.x - visibleCoordX;
    coordY = m_pView->GetTotalSize().cy - m_nBitmapPixSizeY * (m_clickStartGridCoord.y + 2) - visibleCoordY;
    pDc->Draw3dRect(coordX, coordY, m_nBitmapPixSizeX, m_nBitmapPixSizeY, 0x00DBD8F8, 0x000ECECF);
    coordX += m_nBitmapPixSizeX / 2;
    coordY += m_nBitmapPixSizeY / 2;
    pDc->MoveTo(coordX, coordY);
    pDc->LineTo(m_MouseCoord.x, m_MouseCoord.y);
  }

  // Restore the old bitmap
  pDc->SelectObject(bmpPrevious);
  
}

void CBiotopDisplayMFC::RedrawSceneSatelliteView()
{
  if ((m_nBitmapNumberX != m_pBiotop->getDimension().x / NB_GRID_PER_GEOMAP_SQUARE) 
    || (m_nBitmapNumberY != m_pBiotop->getDimension().y / NB_GRID_PER_GEOMAP_SQUARE))
  {
    // Update m_nBitmapNumberX if biotop has changed
    m_nBitmapNumberX = m_pBiotop->getDimension().x / NB_GRID_PER_GEOMAP_SQUARE;
    m_nBitmapNumberY = m_pBiotop->getDimension().y / NB_GRID_PER_GEOMAP_SQUARE;
    DisplayView();
  }

  int visibleCoordX = m_pView->GetScrollPos(SB_HORZ);
  int visibleCoordY = m_pView->GetScrollPos(SB_VERT);

  CPaintDC dc(m_pView);
  CPaintDC* pDc = &dc;

  size_t i, j;
  int coordX, coordY;
  Point_t bioCoord;
  LayerType_e curLayer;
  COLORREF custColor;

  // Display square around biotop
  pDc->Draw3dRect(-visibleCoordX, -visibleCoordY, 
                  m_nBitmapNumberX * m_nBitmapPixSizeX,
                  m_nBitmapNumberY * m_nBitmapPixSizeY, 0x001E1EEF, 0x000E0ECF);

  for (i = 0; i < m_nBitmapNumberX; i++)
  {
    for (j = 0; j < m_nBitmapNumberY; j++)
    {
      bioCoord.x = i * NB_GRID_PER_GEOMAP_SQUARE + 1;
      bioCoord.y = j * NB_GRID_PER_GEOMAP_SQUARE + 1;
      coordX = m_nBitmapPixSizeX * i - visibleCoordX;
      coordY = m_pView->GetTotalSize().cy - m_nBitmapPixSizeY * (j + 2) - visibleCoordY;

      custColor = m_pBiotop->getCustomColor(bioCoord);
      if (custColor != 0x00FFFFFF)
      {
        pDc->FillSolidRect(coordX + 1, coordY + 1, m_nBitmapPixSizeX - 2, m_nBitmapPixSizeY - 2, custColor);
      }
    }
  }
}

void CBiotopDisplayMFC::RedrawSceneIdleNoCPU()
{
  CPaintDC dc(m_pView); 
  CPaintDC* pDc = &dc;//m_pPaintDC;
  pDc->FillSolidRect(0, 0, m_pView->GetTotalSize().cx, m_pView->GetTotalSize().cy,0x00FFFFFF);
  CRect rect(10, 10, 600,30);
  pDc->DrawText(LPCTSTR("DISPLAY STOPPED TO SAVE CPU"),rect,DT_CENTER);
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayMFC
/////////////////////////////////////////////////////////////////////////////

void CBiotopDisplayMFC::RefreshNextSecond()
{
  if (!m_bAppIsActive)
    return;

  BiotopEventPair eventPair = m_pBiotop->getNextUnreadGuiBiotopEvent();
  while (eventPair.first != ENTITY_ID_INVALID)
  {
    BiotopEvent_t& bioEvent{ eventPair.second };
    entityIdType entityId = eventPair.first;
    if ((bioEvent.pEntity) && ((bioEvent.eventList.count() > 1) || (!bioEvent.eventList.test(BIOTOP_EVENT_ENTITY_BEHAVIOR_CHANGE))))
    {
      CBasicEntity* pEntity = bioEvent.pEntity;
      InvalidateRectForOneEntity(pEntity);
    }
    eventPair = m_pBiotop->getNextUnreadGuiBiotopEvent();
  }
}

void CBiotopDisplayMFC::InvalidateRectForOneEntity(CBasicEntity* pEntity)
{
  if (pEntity == NULL)
    return;

  int visibleCoordX = m_pView->GetScrollPos(SB_HORZ);
  int visibleCoordY = m_pView->GetScrollPos(SB_VERT);
  int coordX, coordY;
  CRect refreshRect;
  Point_t prevCoord = pEntity->getAndUpdateGuiGridCoord();

  if ((prevCoord.x != pEntity->getGridCoord().x) || (prevCoord.y != pEntity->getGridCoord().y))
  {
    coordX = m_nBitmapPixSizeX * prevCoord.x - visibleCoordX;
    coordY = m_pView->GetTotalSize().cy - m_nBitmapPixSizeY * (prevCoord.y + 2) - visibleCoordY;
    refreshRect.left = coordX;
    refreshRect.top = coordY;
    refreshRect.right = coordX + m_nBitmapPixSizeX;
    refreshRect.bottom = coordY + m_nBitmapPixSizeY;
    m_pView->InvalidateRect(refreshRect, true);
  }
  coordX = m_nBitmapPixSizeX * pEntity->getGridCoord().x - visibleCoordX;
  coordY = m_pView->GetTotalSize().cy - m_nBitmapPixSizeY * (pEntity->getGridCoord().y + 2) - visibleCoordY;
  refreshRect.left = coordX;
  refreshRect.top = coordY;
  refreshRect.right = coordX + m_nBitmapPixSizeX;
  refreshRect.bottom = coordY + m_nBitmapPixSizeY;
  m_pView->InvalidateRect(refreshRect, true);
}

/*void CBiotopDisplayMFC::RefreshScene()
{

}*/

void CBiotopDisplayMFC::RefreshSceneIdleNoCPU()
{
  m_pView->Invalidate();
}

void CBiotopDisplayMFC::OnMouseMove(CPoint point) 
{
  if (m_bLButtonIsDown)
  {
    m_pView->Invalidate();
  }
  m_MouseCoord = { (size_t)point.x, (size_t)point.y };
}

void CBiotopDisplayMFC::ScrollToGridCoord(Point_t centerPos)
{
  CPoint coord;
  coord.x = centerPos.x * m_nBitmapPixSizeX - m_curViewSizeX/2;
  coord.y = m_pView->GetTotalSize().cy - centerPos.y * m_nBitmapPixSizeY - m_curViewSizeY/2;
  m_pView->ScrollToPosition( coord );
}

Point_t CBiotopDisplayMFC::GetCurrentGridCenterPos()
{
  Point_t gridCoord;
  CPoint pixCoord(m_curViewSizeX/2, m_curViewSizeY/2);
  gridCoord = GetGridCoordFromScreenPos(pixCoord);

  return gridCoord;
}

Point_t CBiotopDisplayMFC::GetVisibleGridCoordStart() 
{ 
  Point_t gridCoord{ invalidCoord, invalidCoord };
  int startPosGridX = m_pView->GetScrollPos(SB_HORZ) / m_nBitmapPixSizeX;
  (startPosGridX > 10) ? startPosGridX -= 10 : startPosGridX = 0;
  int startPosGridY = (m_pView->GetTotalSize().cy - m_pView->GetScrollPos(SB_VERT) - m_curViewSizeY) / m_nBitmapPixSizeY;
  (startPosGridY > 10) ? startPosGridY -= 10 : startPosGridY = 0;
  gridCoord.x = (size_t)startPosGridX;
  gridCoord.y = (size_t)startPosGridY;
  return(gridCoord); 
};

Point_t CBiotopDisplayMFC::GetVisibleGridCoordEnd()
{
  Point_t gridCoord{ invalidCoord, invalidCoord };
  int endPosGridX = cybio_min(m_pBiotop->getDimension().x, (m_pView->GetScrollPos(SB_HORZ) + m_curViewSizeX) / m_nBitmapPixSizeX + 10);
  int endPosGridY = cybio_min(m_pBiotop->getDimension().y, (m_pView->GetTotalSize().cy - m_pView->GetScrollPos(SB_VERT)) / m_nBitmapPixSizeY + 10);
  gridCoord.x = (size_t)endPosGridX;
  gridCoord.y = (size_t)endPosGridY;
  return(gridCoord);
};

void CBiotopDisplayMFC::SetZoomFactor(double zoomFactor)
{
  m_ZoomFactor = zoomFactor;

  if (m_ZoomFactor > 0.8)
  {
    m_nBitmapPixSizeX = 16;
    m_nBitmapPixSizeY = 16;
  }
  else if (m_ZoomFactor > 0.7)
  {
    m_nBitmapPixSizeX = 9;
    m_nBitmapPixSizeY = 9;
  }
  else if (m_ZoomFactor > 0.6)
  {
    m_nBitmapPixSizeX = 8;
    m_nBitmapPixSizeY = 8;
  }
  else if (m_ZoomFactor > 0.5)
  {
    m_nBitmapPixSizeX = 7;
    m_nBitmapPixSizeY = 7;
  }
  else if (m_ZoomFactor > 0.4)
  {
    m_nBitmapPixSizeX = 6;
    m_nBitmapPixSizeY = 6;
  }
  else if (m_ZoomFactor > 0.3)
  {
    m_nBitmapPixSizeX = 5;
    m_nBitmapPixSizeY = 5;
  }
  else if (m_ZoomFactor > 0.1)
  {
    m_nBitmapPixSizeX = 4;
    m_nBitmapPixSizeY = 4;
  }
  else
  {
    m_nBitmapPixSizeX = 3;
    m_nBitmapPixSizeY = 3;
  }

  DisplayView();
  RefreshSceneIdleNoCPU();
}

void CBiotopDisplayMFC::SetFocusedEntityId(entityIdType id)
{
  InvalidateRectForOneEntity(m_pBiotop->getEntityById(m_nFocusedEntityId));
  CBiotopDisplay::SetFocusedEntityId(id);
  InvalidateRectForOneEntity(m_pBiotop->getEntityById(m_nFocusedEntityId));
}

