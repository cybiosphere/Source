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

// BiotopDisplay.cpp : implementation of the CBiotopDisplayGL class
//

#include "stdafxGL.h"
#include "Cybiosphere.h"

#include "BiotopView.h"
#include "BiotopDisplayGL.h"

#include "CBrain.h"
#include "CBiotop.h"
#include "CBasicEntity.h"
#include "CAnimal.h"

#include "CLifeStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL construction/destruction

CBiotopDisplayGL::CBiotopDisplayGL()
{
    m_BiotopSizeX = 0; // Doit etre impaire !!!
    m_BiotopSizeY = 0;
    m_pView = NULL;

    m_hDC = NULL;
    m_hRC = NULL;
    m_cxClient = 0;
    m_cyClient = 0;
    m_hPal = NULL;
    m_bAppIsActive = FALSE;

    // calculateFramesPerSec initialization code
    m_fStartFrameTime = 0.0;
    m_fEndFrameTime = 0.0;
    m_fTime = 0.0;
    m_iFrames = 0;
    m_iFramesPerSec = 0;

    m_nbRefreshPerStep = 1;
    m_RefreshCounter = 0;
    m_ZoomFactor = 1.0;

}

CBiotopDisplayGL::~CBiotopDisplayGL()
{
  for (int i=m_tAnimatedSprite.GetSize()-1; i>=0; i--) 
  {
    if (m_tAnimatedSprite.GetAt(i) != NULL)
    {  
      delete (m_tAnimatedSprite.GetAt(i)); 
    }

    m_tAnimatedSprite.RemoveAt(i);
  }

	KillWindow();	// Shutdown
}


/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL init

bool CBiotopDisplayGL::Initialize(CScrollView* pView) 
{
	if (true)
	{
    m_pView = pView;
    m_tAnimatedSprite.SetSize(0);
    m_hDC  = GetDC ( pView->m_hWnd );

		GLuint	PixelFormat;												// Holds The Results After Searching For A Match
		static	PIXELFORMATDESCRIPTOR pfd=									// pfd Tells Windows How We Want Things To Be
		{																		
			sizeof(PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,																// Version Number
			PFD_DRAW_TO_WINDOW |											// Format Must Support Window
			PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,												// Must Support Double Buffering
			PFD_TYPE_RGBA,													// Request An RGBA Format
			16,														// Select Our Color Depth
			0, 0, 0, 0, 0, 0,												// Color Bits Ignored
			0,																// No Alpha Buffer
			0,																// Shift Bit Ignored
			0,																// No Accumulation Buffer
			0, 0, 0, 0,														// Accumulation Bits Ignored
			16,																// 16Bit Z-Buffer (Depth Buffer)  
			0,																// No Stencil Buffer
			0,																// No Auxiliary Buffer
			PFD_MAIN_PLANE,													// Main Drawing Layer
			0,																// Reserved
			0, 0, 0															// Layer Masks Ignored
		};																		
	
		if ( m_hDC == NULL )	{							// Did We Get A Device Context?																	
			KillWindow ();													// Reset The Display
			AfxMessageBox ( "Can't Create A GL Device Context.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !( PixelFormat = ChoosePixelFormat ( m_hDC, &pfd ) ) )	{		// Did Windows Find A Matching Pixel Format?																		
			KillWindow ();												// Reset The Display
			AfxMessageBox ( "Can't Find A Suitable PixelFormat.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !SetPixelFormat ( m_hDC, PixelFormat, &pfd ) ){				// Are We Able To Set The Pixel Format?																		
			KillWindow ();												// Reset The Display
			AfxMessageBox ( "Can't Set The PixelFormat.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;												
		}
																			
		if ( !( m_hRC = wglCreateContext ( m_hDC ) ) ) {					// Are We Able To Get A Rendering Context?																	
			KillWindow ();												// Reset The Display
			AfxMessageBox( " Can't Create A GL Rendering Context.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}
																			
		if ( !wglMakeCurrent ( m_hDC, m_hRC ) )	{							// Try To Activate The Rendering Context																		
			KillWindow ();												// Reset The Display
			AfxMessageBox ( "Can't Activate The GL Rendering Context.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}

		if ( !LoadData () ) {													// Initialize Our Newly Created GL Window																		
			KillWindow ();												// Reset The Display
			AfxMessageBox ( "Initialization Failed.", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;													
		}

		m_bAppIsActive = TRUE;
	}

	return true;
}

void CBiotopDisplayGL::DisplayView()
{
  if (m_pView != NULL)
  {
	// Calculate the total size of this view
	CSize sizeTotal;
	sizeTotal.cx = 20;
  sizeTotal.cy = 10;
	m_pView->SetScrollSizes(MM_TEXT, sizeTotal);
  }
}

//////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL LoadData

int CBiotopDisplayGL::LoadData(void)												// All Setup For OpenGL Goes Here
{
    // Build Map according to biotop
    BuildMapFromBiotop();

	// EXAMPLE TEXTURE CODE START ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);
  
	m_Map.SetViewResolution(m_cxClient, m_cyClient);
	m_Map.StartMap(0,0);

	//m_appBindTexArray.SetSize( m_Map.MAXTILES + 12);

	/*char buff[30];
	for( int i = 0; i < m_Map.MAXTILES; i++)
	{
		//itoa(i,buff,10);
		//strcat(buff,".bmp");
    sprintf(buff,"./Maps/%d.bmp",i);
    m_appBindTexArray[i] = m_appTextures.LoadGLTexImage2DMipMap(buff);
	}*/
    int nbTile = m_appTextures.LoadGLTexImage2DGroup("./Maps/tiles.bmp",m_Map.MAXTILES,m_appBindTexArray);
	    
    CURSOR_LEFT_ID = m_Map.MAXTILES;
    m_appBindTexArray[CURSOR_LEFT_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/cursorleft.bmp");
    m_appBindTexArray[CURSOR_LEFT_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/maskcursorleft.bmp");

    CURSOR_RIGHT_ID = m_Map.MAXTILES+2;
    m_appBindTexArray[CURSOR_RIGHT_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/cursorright.bmp");
    m_appBindTexArray[CURSOR_RIGHT_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/maskcursorright.bmp");

    CURSOR_TOP_ID = m_Map.MAXTILES+4;
    m_appBindTexArray[CURSOR_TOP_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/cursortop.bmp");
    m_appBindTexArray[CURSOR_TOP_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/maskcursortop.bmp");

    CURSOR_DOWN_ID = m_Map.MAXTILES+6;
    m_appBindTexArray[CURSOR_DOWN_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/cursordown.bmp");
    m_appBindTexArray[CURSOR_DOWN_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Cursor/maskcursordown.bmp");

    SELECT_ID = m_Map.MAXTILES+8;
    m_appBindTexArray[SELECT_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Select/Select.bmp");
    m_appBindTexArray[SELECT_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Select/MaskSelect.bmp");

    SQUARE_ID = m_Map.MAXTILES+10;
    m_appBindTexArray[SQUARE_ID] = m_appTextures.LoadGLTexImage2DMipMap("./Select/Square.bmp");
    m_appBindTexArray[SQUARE_ID+1] = m_appTextures.LoadGLTexImage2DMipMap("./Select/MaskSquare.bmp");

    m_SprArrow.SetSpriteSize(SIZE_ARROW_X,SIZE_ARROW_Y);
    m_SprSquare.SetSpriteSize(96,64);

	return TRUE;															// Initialization Went OK
}

//////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL KillWindow

void CBiotopDisplayGL::KillWindow(void)										// Properly Kill The Window
{																																																				
	if ( m_hRC ) {															// Do We Have A Rendering Context?																		
		if ( !wglMakeCurrent ( NULL, NULL ) )	{							// Are We Able To Release The DC And RC Contexts?
			AfxMessageBox ( "Release Of DC And RC Failed.", MB_OK | MB_ICONINFORMATION );
		}

		if ( !wglDeleteContext ( m_hRC ) ) {								// Are We Able To Delete The RC?
			AfxMessageBox ( "Release Rendering Context Failed.",  MB_OK | MB_ICONINFORMATION );
		}
		m_hRC = NULL;														// Set RC To NULL
	}

	// EXAMPLE TEXTURE CODE START ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//m_appBindTexArray.RemoveAll();
	// EXAMPLE TEXTURE CODE END ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL sizing

void CBiotopDisplayGL::ResizeScene(int cx, int cy) 
{
  m_cxClient = cx;
  m_cyClient = cy;

  m_Map.SetViewResolution(m_cxClient, m_cyClient);

  if (cy==0)										// Prevent A Divide By Zero By
  {
	  cy=1;										// Making Height Equal One
  }
  glViewport(0,0,cx,cy);						// Reset The Current Viewport
  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix	
  // Calculate The Aspect Ratio Of The Window
  glOrtho(0.0f,cx,0.0f,cy,0.001,1000.0);	
  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  glLoadIdentity();									// Reset The Modelview Matrix
}


/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL button

Point_t CBiotopDisplayGL::GetGridCoordFromScreenPos(CPoint screenPos)
{
  // Return Biotop
  int posMapX,posMapY,posBioX,posBioY;
  m_Map.GetBlockPosFromWndCoord(screenPos.x,screenPos.y,posMapX,posMapY);
  ConvertCoordMapToBiotopGrid(posMapX,posMapY,posBioX,posBioY);

  Point_t bioCoord;
  bioCoord.x = posBioX;
  bioCoord.y = posBioY;

  return (bioCoord);
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL RenderGLScene

void CBiotopDisplayGL::RefreshScene()
{
  RenderScene();
}

void CBiotopDisplayGL::RefreshNextSecond()
{
  ResetAllSprites();
}

void CBiotopDisplayGL::RedrawSceneIdleNoCPU()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
	glLoadIdentity();

  glPushMatrix();															// Push Matrix Onto Stack (Copy The Current Matrix)
  gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);  
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);			// Keep the font color from coloring the textures
		
  m_SprArrow.MoveSpriteAtPosition(m_cxClient-40,m_cyClient/2-40);
  m_SprArrow.DrawSprite(m_appBindTexArray[CURSOR_RIGHT_ID]);
    
  SwapBuffers(m_hDC);
}

void CBiotopDisplayGL::RenderScene(bool reset)
{
	if (!m_bAppIsActive)
		return;

	//calculateFramesPerSec();									// Calculate frames per second function

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
	glLoadIdentity();

  glPushMatrix();															// Push Matrix Onto Stack (Copy The Current Matrix)
  gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);  
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			// Keep the font color from coloring the textures

  // Display Maps
  m_Map.DrawMap();
  // Display all entities
  DrawAllVisibleEntities(reset);
  // Set luminosity
  SetLuminosity(m_pBiotop->getSunlight());

  // Display scroll arrows
  bool resu = false;
	if (pt_MousePosition.x > 0 && pt_MousePosition.x <= 40) 
	{
    if(m_bLButtonIsDown)
		  m_Map.ScrollLeft(6);
		m_SprArrow.MoveSpriteAtPosition(20,m_cyClient/2-20);
		m_SprArrow.DrawSprite(m_appBindTexArray[CURSOR_LEFT_ID]);
	}
	else if (pt_MousePosition.x >= m_cxClient-40 && pt_MousePosition.x <= m_cxClient) 
	{
    if(m_bLButtonIsDown)
		  m_Map.ScrollRight(6);
		m_SprArrow.MoveSpriteAtPosition(m_cxClient-20,m_cyClient/2-20);
		m_SprArrow.DrawSprite(m_appBindTexArray[CURSOR_RIGHT_ID]);
	}

	if (pt_MousePosition.y > 0 && pt_MousePosition.y <= 40) 
	{
    if(m_bLButtonIsDown)
		  m_Map.ScrollUp(4);
		m_SprArrow.MoveSpriteAtPosition(m_cxClient/2,m_cyClient-36);
		m_SprArrow.DrawSprite(m_appBindTexArray[CURSOR_TOP_ID]);
	}
	else if (pt_MousePosition.y >= m_cyClient-40 && pt_MousePosition.y <= m_cyClient) 
	{
    if(m_bLButtonIsDown)
		  m_Map.ScrollDown(4);
		m_SprArrow.MoveSpriteAtPosition(m_cxClient/2-20,4);
		m_SprArrow.DrawSprite(m_appBindTexArray[CURSOR_DOWN_ID]);
	}

  // Pop Matrix Off The Stack
	glPopMatrix();														
	// Swap our scene to the front
	SwapBuffers(m_hDC);

	//Invalidate(FALSE);
}

void CBiotopDisplayGL::ResetAllSprites()
{
  CAnimatedSprite* pAnimatedSprite = NULL;
  int inst = 0;
  for (int i=0; i<m_tSpriteDisplayPrio.GetSize(); i++) 
  {
    DisplayPriority_t element = m_tSpriteDisplayPrio.GetAt(i);
    CBasicEntity* pEntity = element.pEntity; 
    pAnimatedSprite = GetAnimatedSprite(pEntity);
    if (pAnimatedSprite == NULL)
      break;
    inst = pAnimatedSprite->GetInstance(pEntity->getId(), pEntity->getColorRgb(), pEntity->getWeight());
    pAnimatedSprite->ResetSteps(inst);
  }
}

void CBiotopDisplayGL::OnMouseMove(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt_MousePosition = point;
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopDisplayGL calculateFramesPerSec

/*void CBiotopDisplayGL::calculateFramesPerSec() 
{
	// Get current time in seconds! Sytem time is in milliseconds since win98 started
  m_fTime = static_cast<float>(timeGetTime())*0.001f;
	m_fEndFrameTime = m_fTime;
	
	//Calculate frames/sec	
	if ( ( m_fEndFrameTime - m_fStartFrameTime ) >= 1.0f ) {
		m_iFramesPerSec  = m_iFrames;
		m_iFrames = 0;
		m_fStartFrameTime = m_fTime;
	} else {
		m_iFrames++;
	}
}*/

void CBiotopDisplayGL::ConvertCoordBiotopGridToMap (int cbx, int cby, int &cmx, int &cmy)
{
  cmx = (int)floor ( (cbx+cby)/2.0 );
  cmy = cbx - cby + m_BiotopSizeY - 1;
}

void CBiotopDisplayGL::ConvertCoordMapToBiotopGrid (int cmx, int cmy, int &cbx, int &cby)
{
  cby = cmx - (int)floor( (cmy-m_BiotopSizeY+1)/2.0 );
  cbx = cmx + (int)floor( (cmy-m_BiotopSizeY)/2.0 ) + 1;
}

void CBiotopDisplayGL::ConvertCoordBiotopStepToMapStep (int cbx, int cby, int &cmx, int &cmy)
{
  cmx = (int)floor ( (cbx+cby)/2.0) ;
  cmy = (int)floor ( (cbx-cby) + (m_BiotopSizeY-1)*NB_STEPS_PER_GRID_SQUARE );
}

void CBiotopDisplayGL::ConvertCoordMapToBiotopStep (int cmx, int cmy, int &cbx, int &cby)
{
  cby = NB_STEPS_PER_GRID_SQUARE * (cmx - (int)floor( (cmy-m_BiotopSizeY+1)/2.0 ));
  cbx = NB_STEPS_PER_GRID_SQUARE * (cmx + (int)floor( (cmy-m_BiotopSizeY)/2.0 ) + 1);
}

bool CBiotopDisplayGL::BuildMapFromBiotop()
{
  if ( m_pBiotop == NULL )
    return (false);  

  m_BiotopSizeX = m_pBiotop->getDimension().x;
  m_BiotopSizeY = m_pBiotop->getDimension().y;

  if ( 2*(m_BiotopSizeY/2) == m_BiotopSizeY )
    m_BiotopSizeY++; //DOIT ETRE IMPAIR !

  m_Map.SetMapSize( (m_BiotopSizeX + m_BiotopSizeY) / 2 , (m_BiotopSizeX + m_BiotopSizeY) );

  int i,j,cmx,cmy;
  LayerType_e curLayer;
  WORD curMapId;
  Point_t bioCoord;

  for (i=0;i<m_BiotopSizeX;i++)
  {
    for (j=0; j<m_BiotopSizeY; j++)
    { 
      bioCoord.x = i;
      bioCoord.y = j;
      curLayer = m_pBiotop->getLayerType(bioCoord,1);
          
      ConvertCoordBiotopGridToMap(i,j,cmx,cmy);

      if (curLayer == LAYER_OVER_WATER)
        m_Map.SetBlock(cmx,cmy,3);
      else if (curLayer == LAYER_OVER_WET_GROUND)
        m_Map.SetBlock(cmx,cmy,4);
      else if (curLayer == LAYER_GLOBAL_GRASS)
        m_Map.SetBlock(cmx,cmy,5);
      else if (curLayer == LAYER_GLOBAL_ROCK)
        m_Map.SetBlock(cmx, cmy, 7);
      else
        m_Map.SetBlock(cmx,cmy,6);
    } 
  } 

  return (true);
}

void CBiotopDisplayGL::GetVisibleBiotopCoord(int &cbx_min, int &cby_min, int &cbx_max, int &cby_max)
{
  int cmx_min = m_Map.xTileStart;
  int cmy_min = m_Map.yTileStart;
  int cmx_max = m_Map.xTileStart + m_Map.ScreenMAPWidth;
  int cmy_max = m_Map.yTileStart + m_Map.ScreenMAPHeight;
  
  int tmpx, tmpy;
  ConvertCoordMapToBiotopStep(cmx_min, cmy_max, tmpx, cby_min);
  ConvertCoordMapToBiotopStep(cmx_min, cmy_min, cbx_min, tmpy);    
  ConvertCoordMapToBiotopStep(cmx_max, cmy_max, cbx_max, tmpy);
  ConvertCoordMapToBiotopStep(cmx_max, cmy_min, tmpx, cby_max);

  return;
}

void CBiotopDisplayGL::DrawAllVisibleEntities(bool noStep)
{
  int cbx_min, cby_min, cbx_max, cby_max;
  GetVisibleBiotopCoord(cbx_min, cby_min, cbx_max, cby_max);
  int i;
  CAnimatedSprite* pAnimatedSprite = NULL;
  bool isSelected = false;
  int inst = 0;
  int curAction = 0;

  m_tSpriteDisplayPrio.RemoveAll();

  for (i=0; i<m_pBiotop->getNbOfEntities(); i++) 
  {
    int posMapX,posMapY,coordx,coordy,offsetx,offsety;
    CBasicEntity* pEntity = m_pBiotop->getEntityByIndex(i); 

    if ( (pEntity == NULL) || (pEntity->isToBeRemoved()) )
      continue;
 
    Point_t coord = pEntity->getStepCoord();

    if ( ((int)coord.x>=cbx_min)&&((int)coord.x<=cbx_max)
       &&((int)coord.y>=cby_min)&&((int)coord.y<=cby_max) )
    {
      // Process coordonate
      ConvertCoordBiotopStepToMapStep(coord.x,coord.y,posMapX,posMapY);
      m_Map.GetAbsCoordFromBlockStepPos(posMapX,posMapY,coordx,coordy,offsetx,offsety);
 
      DisplayPriority_t element;
      element.pEntity         = pEntity;
      element.displayPriority = ((pEntity->getLayer())<<16) - coordy;
      element.newMapCoordX    = coordx;
      element.newMapCoordY    = coordy;      
      element.spriteOffsetX   = offsetx;
      element.spriteOffsetY   = offsety;

      // Insert the item in proper position according to displayPriority
      INT_PTR start = 0, end = m_tSpriteDisplayPrio.GetSize(); 
      while(start < end) 
      { 
          INT_PTR pos = (start + end) / 2; 
          if(element.displayPriority > m_tSpriteDisplayPrio.GetAt(pos).displayPriority) 
              start = pos + 1; 
          else 
              end = pos; 
      } 
      m_tSpriteDisplayPrio.InsertAt(start, element); 
    }
    else
    {
      // Force step attribute for next move
      pAnimatedSprite = GetAnimatedSprite(pEntity);
      if (pAnimatedSprite != NULL)
      {
        inst = pAnimatedSprite->GetInstance(pEntity->getId(), pEntity->getColorRgb(), pEntity->getWeight());
        // Force NO step attribute for next move
        pAnimatedSprite->SetStepMode(inst,true);
      }
    }
  }

  m_RefreshCounter = (m_RefreshCounter+1)%m_nbRefreshPerStep;

  for (i=0; i<m_tSpriteDisplayPrio.GetSize(); i++) 
  {
    int coordx,coordy,offsetx,offsety;

    DisplayPriority_t element = m_tSpriteDisplayPrio.GetAt(i);
    CBasicEntity* pEntity = element.pEntity; 
    coordx  = element.newMapCoordX;
    coordy  = element.newMapCoordY;
    offsetx = element.spriteOffsetX;
    offsety = element.spriteOffsetY;

    // Check if selected entity
    isSelected = (pEntity->getId() == m_nSelectedEntityId);

    pAnimatedSprite = GetAnimatedSprite(pEntity);

    if (pAnimatedSprite == NULL)
      break;

    inst = pAnimatedSprite->GetInstance(pEntity->getId(), pEntity->getColorRgb(), pEntity->getWeight());

    if (noStep)
      pAnimatedSprite->MoveSprite(inst,coordx,coordy);
    else if ( m_RefreshCounter == 0)
      pAnimatedSprite->MoveSpriteByStep(inst,coordx,coordy);

    if (pEntity->getBrain() != NULL)
    {
      curAction = pEntity->getBrain()->GetCurrentReactionIndex();
      // According to speed, use walk or run animation instead of real action
      /*if ( pEntity->getCurrentSpeed() > 20)
        curAction = pAnimatedSprite->m_idActionRun;   
      else if ( pEntity->getCurrentSpeed() > 0)
        curAction = pAnimatedSprite->m_idActionWalk;*/
    }
    else
      curAction = 0;

    if (isSelected)
    {
      int posMapX,posMapY,coordx,coordy,offsetx,offsety;
      Point_t coord = pEntity->getGridCoord();
      ConvertCoordBiotopGridToMap(coord.x,coord.y,posMapX,posMapY);
      m_Map.GetAbsCoordFromBlockStepPos((posMapX)*NB_STEPS_PER_GRID_SQUARE+(1+(posMapY&1))*NB_STEPS_PER_GRID_SQUARE/2,(posMapY+1)*NB_STEPS_PER_GRID_SQUARE,coordx,coordy,offsetx,offsety);
      m_SprSquare.MoveSpriteAtPosition(coordx+offsetx,coordy+offsety);
      m_SprSquare.DrawSprite(m_appBindTexArray[SQUARE_ID]);
    }

    if (pEntity->getClass()>=CLASS_ANIMAL_FIRST)
      pAnimatedSprite->DrawSprite(inst,offsetx,offsety,pEntity->getDirection(),0,curAction,isSelected);
    else
      pAnimatedSprite->DrawSprite(inst,offsetx,offsety,0,0,curAction,isSelected);

    // Force step attribute for next move
    if ( m_RefreshCounter == 0)
      pAnimatedSprite->SetStepMode(inst,false);
  }

}

void CBiotopDisplayGL::SetLuminosity (double rate)
{
  double luminosity = rate / 100.0 + 0.3;
  if ((luminosity>1)||(luminosity<0))
  luminosity = 1;

  glEnable(GL_BLEND);							// Enable Blending	
  glDisable(GL_DEPTH_TEST);						
  glBlendFunc(GL_DST_COLOR,GL_ZERO);				
  // tile now holds the ID# of the tile at location map[y][x].
  glBindTexture(GL_TEXTURE_2D, 2);
  glColor3f(luminosity, luminosity, luminosity);
  // Here we bind the texture that we need, either texture[0] or texture[1], based on the value of tile, which holds the current tile's ID#.	
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(float(0), float(0), 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(float(m_cxClient), float(0), 0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(float(m_cxClient), float(m_cyClient), 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(float(0), float(m_cyClient), 0.0f);    
  glEnd();
  glColor3f(1, 1, 1);
  glEnable(GL_DEPTH_TEST);						// Enable Depth Testing
  glDisable(GL_BLEND);
}

void CBiotopDisplayGL::SetZoomFactor (double zoomFactor)
{
  m_ZoomFactor = zoomFactor;

  for (int i=0; i<m_tAnimatedSprite.GetSize(); i++)
  { 
    m_tAnimatedSprite.GetAt(i)->SetZoomFactor(zoomFactor);
  }
  
  m_SprSquare.SetSpriteSize ( (int)floor(zoomFactor*96.0), (int)floor(zoomFactor*64.0) );

  m_Map.SetZoomFactor(zoomFactor);
  RenderScene(true);
}

// return pointer on AnimatedSprite object. Create and load data it if not existing yet
// TBD Cleanup is required for died entities...
CAnimatedSprite* CBiotopDisplayGL::GetAnimatedSprite(CBasicEntity* pEntity)
{
  int nDirection, nPosition, nLifeStages, nActions;
  switch (pEntity->getClass())
  {
  case CLASS_MAMMAL:
  case CLASS_REPTILE:
  case CLASS_ARTHROPOD:
  case CLASS_AVE:
    {
      nDirection  = 8;
      nPosition   = 5;
      nLifeStages = 1;
      nActions    = pEntity->getBrain()->GetNumberReaction();
      break;
    }
  case CLASS_SPERMATOPHYTA:
    {
      nDirection  = 1;
      nPosition   = 1;
      nLifeStages = 1;
      nActions    = 1;
      break;
    }
  default:
    {
      nDirection  = 1;
      nPosition   = 1;
      nLifeStages = 1;
      nActions    = 1;
      break;
    }
  }

  // Check if data already loaded
  for (int i=0; i<m_tAnimatedSprite.GetSize(); i++)
  { 
    if ( ( pEntity->getGenome()->getSpecieName().c_str() == m_tAnimatedSprite.GetAt(i)->GetSpriteName() ) 
       &&( nActions == m_tAnimatedSprite.GetAt(i)->GetNbActions() ) )
    {
      // Sprite already exist
      return ( m_tAnimatedSprite.GetAt(i) );
    }
  }

  // Create new sprite
  CAnimatedSprite* pNewSprite = new CAnimatedSprite(pEntity->getGenome()->getSpecieName().c_str(),
                                                    nDirection,nPosition,nLifeStages,nActions,
                                                    m_appBindTexArray[SELECT_ID]);
  
  CString dataDirName;
  CString ageString = _T("\\Age0\\");

  // TBD : loop also on life stages
  for (int idAct=0; idAct<nActions; idAct++)
  {
    if (pEntity->getBrain() != NULL)
    {
      CReaction* pReac = pEntity->getBrain()->GetReactionByIndex(idAct);
      CString strReac;
      {
        strReac = pReac->GetLabel().c_str();
      }

      dataDirName = pEntity->getFormStrName(pEntity->getForm()).c_str() + ageString + strReac;
    }
    else
    {
      //dataDirName = pEntity->getGenome()->getSpecieName().c_str() + ageString + "Data";
      dataDirName = pEntity->getFormStrName(pEntity->getForm()).c_str() + ageString + "Data";
    }

    pNewSprite->LoadData(dataDirName,0,idAct);
    pNewSprite->SetZoomFactor(m_ZoomFactor);
  }
  
  if (pNewSprite != NULL)
    m_tAnimatedSprite.Add(pNewSprite);

  return (pNewSprite);

}

void CBiotopDisplayGL::ScrollToGridCoord(Point_t centerPos)
{
  int mapCoordX, mapCoordY;
  ConvertCoordBiotopGridToMap(centerPos.x, centerPos.y, mapCoordX, mapCoordY);

  m_Map.ScrollToMapCoord(mapCoordX, mapCoordY);
}

Point_t CBiotopDisplayGL::GetCurrentGridCenterPos()
{
  Point_t gridCoord;
  CPoint pixCoord(m_cxClient/2, m_cyClient/2);
  gridCoord = GetGridCoordFromScreenPos(pixCoord);

  return gridCoord;
}


void CBiotopDisplayGL::RefreshBiotop()
{
  BuildMapFromBiotop();
  DrawAllVisibleEntities(true);
}