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

// Map.cpp: implementation of the CMapTile class.
//
//////////////////////////////////////////////////////////////////////
/* --------------------------------------------------------------------------------
   CMapTile Class.
   The goal of this class is to reduce several screens of artworks 
   to a single screen of unique blocks. We want to reconstruct the
   original pictures from the Tile. To do that, we used an array 
   to hold the block data as it relates to the original pictures.
----------------------------------------------------------------------------------- */

#include "stdafxGL.h"
#include "Map.h"
#include "BiotopDisplay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapTile::CMapTile()
{
  LevelMap = NULL;
  m_ZoomFactor = 1;
  MAXTILES = 13;
  m_Tile.SetTileSize ((int)(m_ZoomFactor*1.5*SIZE_TILE_X),(int)(m_ZoomFactor*2*SIZE_TILE_Y));

}

//--------------------------------------------------------------------------------
// Name: CMapTile
// Desc: Constructor     
//--------------------------------------------------------------------------------
CMapTile::CMapTile(TCHAR* FileTextName)
{
   LoadBaseFile(FileTextName);
}

//--------------------------------------------------------------------------------
// Name: CMapTile
// Desc: Desstructor     
//--------------------------------------------------------------------------------
CMapTile::~CMapTile(void)
{
   // free memory
   delete [] LevelMap;
}

//--------------------------------------------------------------------------------
// Name: LoadBaseFile
// Desc: Constructor     
//--------------------------------------------------------------------------------
void CMapTile::LoadBaseFile(const char* FileTextName)
{
   // The constructor read the file who contain informations
   // about Tile. Number of columns and of rows of blocks.
   // The structure of the file is easy.
   // line 1: total of tiles
   // line 2: total of columns
   // line 3: total of rows
   // line 4 to EOF: contain all number of block of fist row 
   //                of array at the end of the array. 
   //                Each data is separated by a comma.

   MAXTILES = MAXCOLS = MAXROWS = xTileStart = yTileStart = ScreenHeight = ScreenWidth = 0;
   XOffset = YOffset = 0;   
   TotHeightInPixel = TotWidthInPixel = 0;

   if(FileTextName == NULL) InitFail("The name of the level file is null.");

   FILE *fp;

	if((fp = fopen(FileTextName, "r"))==NULL ) InitFail("Cannot open the level file.");
   else
   {
      char string[10];
      
      fgets(string,sizeof(string),fp);
      MAXTILES = atoi(string);

      fgets(string,sizeof(string),fp);
      MAXCOLS = atoi(string);

      fgets(string,sizeof(string),fp);
      MAXROWS = atoi(string);

      LevelMap = new int[MAXCOLS*MAXROWS];

      int c = 0,i=0;            
      while(i<MAXCOLS*MAXROWS)
      {      
         string[c] = fgetc(fp);
         while( string[c] != '\n' )            
         {                     
            if (string[c] == ',')
            {
               string[c] = NULL;
               LevelMap[i] =  atoi(string);
               i++;
               c=0;
            }      
            else
            {
               c++;
            }            
            string[c] = fgetc(fp);			
         }
      }      
      fclose(fp);	
      // save(); method create a file with name res.txt and see if the data is loaded correctely.
   }
}

//-----------------------------------------------------------------------------
// Name: SetViewResolution(int _ResolX, int _ResolY)
// Desc: This function set the handle of the current window.
//-----------------------------------------------------------------------------
void CMapTile::SetViewResolution(int dimX, int dimY)
{     
  ScreenWidth = dimX; // Lenght of the current resolution. 
  ScreenHeight = dimY; // Height of the current resolution. 

  // count the maximum tile to draw on screen   
  ScreenMAPWidth  = (int)(dimX / m_Tile.GetWidth()) + 2; // for the width
  ScreenMAPHeight = (int)(4.0 * dimY / m_Tile.GetHeight()) + 2; // for the height

  // If resize on left on bottom limit, move TileStart
  if ((xTileStart+ScreenMAPWidth)>MAXCOLS)
  {
    xTileStart = MAXCOLS - ScreenMAPWidth + 1;
  }
  if ((yTileStart+ScreenMAPHeight)>MAXROWS)
  {
    yTileStart = MAXROWS - ScreenMAPHeight;
  }

}

//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
void CMapTile::InitFail(LPCTSTR szError )
{
    MessageBox(NULL, szError, "Error detected on CMapTile Class", MB_OK);
}

//--------------------------------------------------------------------------------
// Name: GetBlock()
// Desc: This fonction give the current Tile.     
//--------------------------------------------------------------------------------
int CMapTile::GetBlock(int Xpos, int Ypos)
{
   if( (Xpos >= MAXCOLS || Ypos >= MAXROWS) || (Xpos < 0 || Ypos < 0) ) return 0;
   return LevelMap[Ypos*(MAXCOLS)+Xpos];
}

//--------------------------------------------------------------------------------
// Name: SetBlock()
// Desc: This fonction set the current Tile.     
//--------------------------------------------------------------------------------
void CMapTile::SetBlock(int Xpos, int Ypos, int blockId)
{
   if( (Xpos >= MAXCOLS || Ypos >= MAXROWS) || (Xpos < 0 || Ypos < 0) ) 
     return;
   else
     LevelMap[Ypos*(MAXCOLS)+Xpos] = blockId;
   return;
}

//--------------------------------------------------------------------------------
// Name: save()
// Desc: This fonction save on text fil with name res 
//       the datas of the array LevelMap.
//--------------------------------------------------------------------------------
void CMapTile::save(void)
{  
   FILE *fp;

	if((fp = fopen("res.txt", "w"))==NULL ) InitFail("Cannot create the level file.");
   else
   {
      char str[10];
      int BASE = 10 ;

      itoa(MAXTILES,str,BASE);
      fputs(str,fp);  fputc('\n',fp);

      itoa(MAXCOLS,str,BASE);
      fputs(str,fp);  fputc('\n',fp);

      itoa(MAXROWS,str,BASE);
      fputs(str,fp);  fputc('\n',fp);

      for (int y=0; y < MAXROWS ;y++)
      {
         for (int x=0; x < MAXCOLS ;x++)
         {   
            itoa(LevelMap[y*MAXCOLS+x],str,BASE);
            fputs(str,fp);
            fputc(',',fp); 
         }
         fputc('\n',fp);          
      }
   }
   fclose(fp);
}



void CMapTile::SetMapSize(int width, int height)
{
  MAXCOLS = width;
  MAXROWS = height;
  // Delete previous map
  if (LevelMap != NULL)
    delete LevelMap;
  // Create new map
  LevelMap = new int[MAXCOLS*MAXROWS];
  // Init to 0
  for(int i=0; i<MAXCOLS*MAXROWS; i++)
    LevelMap[i] = 0;

  xTileStart = yTileStart = 0;
}

//--------------------------------------------------------------------------------
// Name: StartMap()
// Desc: This function draw the map.
// Note: Mandatory and must be used after GetResolution() method and 
//       after defined the width and height of blocks with
//       SetBlock() method.
//--------------------------------------------------------------------------------
void CMapTile::StartMap( int XBlockArrayPos, int YBlockArrayPos)
{
   // the total lenght & width in pixel of the map.
   TotWidthInPixel = (MAXCOLS + 1) * m_Tile.GetWidth() + m_Tile.GetWidth()/2;
   TotHeightInPixel = (MAXROWS/2 + 1) * m_Tile.GetHeight(); // -- If MAXROWS multiple of 2

   // current position in the integer array map block
   xTileStart = XBlockArrayPos; 
   yTileStart = YBlockArrayPos;
   
   XOffset = 0;
   YOffset = 0;
}

//--------------------------------------------------------------------------------
// Name: DrawMap()
// Desc: This fonction Draw the map.
//--------------------------------------------------------------------------------
void CMapTile::DrawMap(void)
{
   // scrolling horizontal
   if( XOffset >= m_Tile.GetWidth()  )
   {
      XOffset = XOffset - m_Tile.GetWidth();
      xTileStart-- ;
   }   
   else if( XOffset < 0 )
   {
	  XOffset = m_Tile.GetWidth() + XOffset;
      xTileStart++ ;
   }

   // scrolling vertical
   if( YOffset >= (m_Tile.GetHeight() /4))
   {
      YOffset = YOffset - (m_Tile.GetHeight() /4);
      yTileStart-- ;
   }  
   else if( YOffset < 0 )
   {
      YOffset = (m_Tile.GetHeight()/4 ) + YOffset;
      yTileStart++ ;
   }

	// Drawing the Tiles
	// Ok, now we're ready to finally draw the tiles. I'll go through the code of this tile drawing function step by step.		
	
	// tile is our place holder variable that will be used in determining if the tile at map[y][x], for example, has an ID# of 0 or 1 when we loop through the map array.
  int xTileEnd = ScreenMAPWidth + xTileStart;
  int yTileEnd = ScreenMAPHeight + yTileStart;

  int x=0,y=0,tile=0;
  for (int yPos = yTileStart-1; yPos < yTileEnd;  yPos++)      
  {      	  	  	   
    for (int xPos = xTileStart-1; xPos < xTileEnd; xPos++)
    {
      x=0,y=0,tile=0;
      //Here we have initialized a doubly-nested loop which will run through our map array, capture each tile's ID#, and then store that ID# in tile, as shown in the next step.
      tile = GetBlock(xPos,yPos);
      x = ( xPos - xTileStart - 1) * ( m_Tile.GetWidth() ) + (yPos & 1) * (m_Tile.GetWidth()/2) + XOffset; 
      y = ( yPos - yTileStart )    * ( m_Tile.GetHeight() / 4 ) + YOffset;
      m_Tile.DrawIsometricTile( x, ScreenHeight-y, tile);
    }
  }
}

//--------------------------------------------------------------------------------
// Name: ScrollRight()
// Desc: This method draw the scroll on right direction.
//--------------------------------------------------------------------------------
bool CMapTile::ScrollRight(int npixel)
{
  bool resu = false;

  if( xTileStart <= MAXCOLS - ScreenMAPWidth )
  {
    XOffset-=npixel;
    resu = true;
  }
  return (resu);
}

//--------------------------------------------------------------------------------
// Name: ScrollLeft()
// Desc: This method draw the scroll on left direction.
//--------------------------------------------------------------------------------
bool CMapTile::ScrollLeft(int npixel)
{
  bool resu = false;

  if( (xTileStart > 0) || ((xTileStart == 0) && (XOffset < m_Tile.GetWidth()/2)))
  {
    XOffset+=npixel;
    resu = true;
  }
  return (resu);
}

//--------------------------------------------------------------------------------
// Name: ScrollDown()
// Desc: This method draw the scroll on down direction.
//--------------------------------------------------------------------------------
bool CMapTile::ScrollUp(int npixel)
{
  bool resu = false;

  if( yTileStart > 0 )
  {
    YOffset+=npixel;
    resu = true;
  }
  return (resu);
}

//--------------------------------------------------------------------------------
// Name: ScrollUp()
// Desc: This method draw the scroll on Up direction.
//--------------------------------------------------------------------------------
bool CMapTile::ScrollDown(int npixel)
{
  bool resu = false;
  
  if( yTileStart < (MAXROWS-ScreenMAPHeight)  )
  {
    YOffset-=npixel;
    resu = true;
  }
  return (resu);
}

bool CMapTile::ScrollToMapCoord(int coordX, int coordY)
{
  xTileStart = coordX - ScreenMAPWidth/2;
  yTileStart = coordY - ScreenMAPHeight/2; 

  return (true);
}


void CMapTile::GetBlockPosFromWndCoord(int cx, int cy, int &xPos, int &yPos)
{
 yPos = ( cy + m_Tile.GetHeight() / 8 - YOffset ) * 4 / m_Tile.GetHeight() + yTileStart;
 xPos = ( cx - XOffset - (yPos&1) * m_Tile.GetWidth() / 2 ) / m_Tile.GetWidth() + xTileStart + 1;
}

void CMapTile::GetAbsCoordFromBlockStepPos(int xPos, int yPos, int &cx, int &cy, int &offsetx, int &offsety)
{
  cx = ( xPos * m_Tile.GetWidth() + (yPos&1) * m_Tile.GetWidth() / 2 ) / NB_STEPS_PER_GRID_SQUARE - 1;
  cy = yPos * m_Tile.GetHeight() / 4 / NB_STEPS_PER_GRID_SQUARE;
  cy = ScreenHeight - cy;

  offsetx = XOffset - ( xTileStart*m_Tile.GetWidth()) - m_Tile.GetWidth();
  offsety = (yTileStart * m_Tile.GetHeight() / 4) - YOffset;
}

void CMapTile::SetZoomFactor(double zoomFactor)
{ 
  m_ZoomFactor = zoomFactor;
  m_Tile.SetTileSize ((int)(m_ZoomFactor*1.5*SIZE_TILE_X),(int)(m_ZoomFactor*2*SIZE_TILE_Y));
  // count the maximum tile to draw on screen   
  ScreenMAPWidth  = (int)(ScreenWidth / m_Tile.GetWidth()) + 2; // for the width
  ScreenMAPHeight = (int)(4.0 * ScreenHeight / m_Tile.GetHeight()) + 2; // for the height
  // the total lenght & width in pixel of the map.
  TotWidthInPixel = (MAXCOLS + 1) * m_Tile.GetWidth() + m_Tile.GetWidth()/2;
  TotHeightInPixel = (MAXROWS/2 + 1) * m_Tile.GetHeight(); // -- If MAXROWS multiple of 2
}