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

// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bmp.h"

class CTexture  
{
public:
	CTexture();
	virtual ~CTexture();
	AUX_RGBImageRec* LoadBMP(TCHAR* Filename);
	GLuint LoadGLTexImage2DNearest(TCHAR* Filename);
	GLuint LoadGLTexImage2DLinear(TCHAR* Filename);
	GLuint LoadGLTexImage2DMipMap(TCHAR* Filename);
  int CTexture::LoadGLTexImage2DGroup(TCHAR* Filename, int maxTexNb, GLuint* pTexIdTable);
	void Use(GLuint BIND_ID);
  int m_SizeX;
  int m_SizeY;

private:
  
  GLuint m_nbTexture; // count number of texture

	struct tex 
	{
		AUX_RGBImageRec* image;
		GLuint bindTexID;
	};
};


#endif // !defined(AFX_TEXTURE_H__FB03BE80_B34C_11D4_A3B0_0050DA731472__INCLUDED_)
