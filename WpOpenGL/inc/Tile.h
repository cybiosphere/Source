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

// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__5B0BA1CE_4851_11D5_A8FE_0050043B0F51__INCLUDED_)
#define AFX_TILE_H__5B0BA1CE_4851_11D5_A8FE_0050043B0F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTile  
{
public:
	CTile();
	virtual ~CTile();

  void SetTileSize (int width, int height);
  int  GetWidth();
  int  GetHeight();
  void DrawIsometricTile(int xPos, int yPos, int TileID);

private:  
	int m_TileWidth;
	int	m_TileHeight;	


};

#endif // !defined(AFX_TILE_H__5B0BA1CE_4851_11D5_A8FE_0050043B0F51__INCLUDED_)
