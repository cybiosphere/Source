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

// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxGL.h"
#include "Sprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite()
{
}

CSprite::~CSprite()
{

}

void CSprite::MoveSpriteAtPosition(int x, int y)
{
	m_PosX = x; 
	m_PosY = y;
}

void CSprite::DrawSprite(int SpriteID, GLbyte red, GLbyte green, GLbyte blue)
{
	glEnable(GL_BLEND);							// Enable Blending	
	glDisable(GL_DEPTH_TEST);						
	
    int halfWidth = m_SpriteWidth/2;

    glBlendFunc(GL_DST_COLOR,GL_ZERO);				

	// tile now holds the ID# of the tile at location map[y][x].
	glBindTexture(GL_TEXTURE_2D, SpriteID+1);
 
	// Here we bind the texture that we need, either texture[0] or texture[1], based on the value of tile, which holds the current tile's ID#.	
    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(m_PosX-halfWidth), float(m_PosY), 0.0f);
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(m_PosX+halfWidth), float(m_PosY), 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(float(m_PosX+halfWidth), float(m_PosY + m_SpriteHeight), 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(float(m_PosX-halfWidth), float(m_PosY + m_SpriteHeight), 0.0f);
	glEnd();

    glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, SpriteID);
    glColor3b(red, green, blue);

    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(m_PosX-halfWidth), float(m_PosY), 0.0f);
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(m_PosX+halfWidth), float(m_PosY), 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(float(m_PosX+halfWidth), float(m_PosY + m_SpriteHeight), 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(float(m_PosX-halfWidth), float(m_PosY + m_SpriteHeight), 0.0f);
	glEnd();

    glColor3f(1, 1, 1); // FR

	glEnable(GL_DEPTH_TEST);						// Enable Depth Testing
	glDisable(GL_BLEND);							
}

void CSprite::MoveRight(int npixel)
{
	m_PosX+=npixel;
}

void CSprite::MoveLeft(int npixel)
{
	m_PosX-=npixel;
}

void CSprite::MoveDown(int npixel)
{
	m_PosY-=npixel;
}

void CSprite::MoveUp(int npixel)
{
	m_PosY+=npixel;
}
