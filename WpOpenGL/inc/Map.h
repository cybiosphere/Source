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

// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__5B0BA1CF_4851_11D5_A8FE_0050043B0F51__INCLUDED_)
#define AFX_MAP_H__5B0BA1CF_4851_11D5_A8FE_0050043B0F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tile.h"

#define SIZE_TILE_X 64
#define SIZE_TILE_Y 64

class CMapTile  
{
public:
	CMapTile();
	CMapTile(TCHAR* FileTextName);	
	virtual ~CMapTile();
	
   void DrawMap(void);
   bool ScrollLeft(int npixel=1);
   bool ScrollRight(int npixel=1);
   bool ScrollDown(int npixel=1);
   bool ScrollUp(int npixel=1);
   bool ScrollToMapCoord(int coordX, int coordY);
   void LoadBaseFile(const char* FileTextName);

public:
   int  GetBlock(int Xpos, int Ypos);   
   void SetBlock(int Xpos, int Ypos, int blockId);
   void StartMap(int XBlockArrayPos, int YBlockArrayPos);
   void SetViewResolution(int dimX, int dimY);
   void InitFail(LPCTSTR szError );
   void save(void);
   void SetMapSize(int width, int height);
   void GetBlockPosFromWndCoord(int cx, int cy, int &xPos, int &yPos);
   void GetAbsCoordFromBlockStepPos(int xPos, int yPos, int &cx, int &cy, int &offsetx, int &offsety);
   void SetZoomFactor(double zoomFactor);
public:	
   int MAXCOLS; // is the total number of columns of blocks in map.
   int MAXROWS; // is the total number of rows of blocks in map.
   int MAXTILES; // is the total tile used in map.
   int *LevelMap; // Array to construct a level out of the blocks.   
   int XOffset; // coordonate upper left of the Visible screen in the Map
   int YOffset;
   CTile m_Tile; // Tile objet to manage the blocks.
   int ScreenWidth; // Lenght of the current resolution. 
   int ScreenHeight; // Height of the current resolution. 
   int TotWidthInPixel; // Total width in pixel of Map
   int TotHeightInPixel; // Total height in pixel of Map
   int xTileStart; // keep the current column block position in the array LevelMap.
   int yTileStart; // keep the current row block position in the array LevelMap.
   int ScreenMAPWidth;
   int ScreenMAPHeight;
   double m_ZoomFactor;
};

#endif // !defined(AFX_MAP_H__5B0BA1CF_4851_11D5_A8FE_0050043B0F51__INCLUDED_)
