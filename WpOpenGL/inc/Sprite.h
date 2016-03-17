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

// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__7A6ABA30_4904_11D5_A8FF_0050043B0F51__INCLUDED_)
#define AFX_SPRITE_H__7A6ABA30_4904_11D5_A8FF_0050043B0F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSprite  
{
public:
	CSprite();
	virtual ~CSprite();
	void MoveSpriteAtPosition(int x, int y);
	void DrawSprite(int position, GLbyte red=100, GLbyte green=100, GLbyte blue=100);
	void MoveRight(int npixel=1);
	void MoveLeft(int npixel=1);
	void MoveDown(int npixel=1);
	void MoveUp(int npixel=1);
  void SetSpriteSize(int width, int height) { m_SpriteWidth = width; m_SpriteHeight = height; };
  int  GetSpriteWidth(void)  { return (m_SpriteWidth); };
  int  GetSpriteHeight(void)  { return (m_SpriteHeight); };

private:
// Attributes
  int m_SpriteWidth;
	int	m_SpriteHeight;	
	int m_PosX;
	int m_PosY;

private:

};

#endif // !defined(AFX_SPRITE_H__7A6ABA30_4904_11D5_A8FF_0050043B0F51__INCLUDED_)
