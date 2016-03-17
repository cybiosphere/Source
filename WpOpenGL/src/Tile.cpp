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

// Tile.cpp: implementation of the CTile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafxGL.h"
#include "Tile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTile::CTile()
{
	m_TileWidth  = 1;
	m_TileHeight = 1;	
}

CTile::~CTile()
{

}

void CTile::SetTileSize (int width, int height)
{
	m_TileWidth  = width;
	m_TileHeight = height;	
}

int  CTile::GetWidth()
{
	return (m_TileWidth);	
}

int  CTile::GetHeight()
{
	return (m_TileHeight);	
}

void CTile::DrawIsometricTile(int xPos, int yPos, int TileID)
{
	// tile now holds the ID# of the tile at location map[y][x].
	glBindTexture(GL_TEXTURE_2D, TileID+1);    
	/////////////////////////////////////////////////////////////////////////////
	// Formula 
	// int x = ( xPos - xTileDeb - 1) * ( TileWidth ) + ( yPos & 1) * (TileWidth/2); 
	// int y = ( yPos - yTileDeb )    * ( TileWidth / 4 );
	/////////////////////////////////////////////////////////////////////////////
	int x = xPos;
	int y = yPos;
    glBegin(GL_TRIANGLE_STRIP );
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x), float(y), 0.0f);
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x + (m_TileWidth >> 1)), float(y - ( m_TileHeight >> 2 )), 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x + (m_TileWidth >> 1)), float(y + ( m_TileHeight >> 2 )), 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x + m_TileWidth), float(y), 0.0f);
	glEnd();	
}
