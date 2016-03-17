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

// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxGL.h"
#include "Texture.h"
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTexture construction/destruction

CTexture::CTexture()
{
	m_nbTexture = 0;
  m_SizeX = 0;
  m_SizeY = 0;
}

CTexture::~CTexture()
{
	

}


/////////////////////////////////////////////////////////////////////////////
// CTexture LoadBMP

AUX_RGBImageRec* CTexture::LoadBMP(TCHAR* Filename)
{
	if (!Filename)									// Make Sure A Filename Was Given
    {
		return NULL;								// If Not Return NULL
    }

	CFile file;
	CFileException fileError;

	if(file.Open(_T(Filename), CFile::modeRead, &fileError))
	{
		file.Close ();
		m_nbTexture++;
		return auxDIBImageLoad(Filename);			// Load The Bitmap And Return A Pointer
	}
	else
	{
		//fileError.ReportError();
		return NULL;								// If Load Failed Return NULL
	}

	return NULL;									// If Load Failed Return NULL

}


/////////////////////////////////////////////////////////////////////////////
// CTexture LoadGLTexImage2DNearest

GLuint CTexture::LoadGLTexImage2DNearest(TCHAR* Filename)
{
	tex currTex;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create Nearest Filtered Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, currTex.image->sizeX, currTex.image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);

    m_SizeX = currTex.image->sizeX;
    m_SizeY = currTex.image->sizeY;

    if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	return currTex.bindTexID;

}


/////////////////////////////////////////////////////////////////////////////
// CTexture LoadGLTexImage2DLinear

GLuint CTexture::LoadGLTexImage2DLinear(TCHAR* Filename)
{
	tex currTex;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create Linear Filtered Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, currTex.image->sizeX, currTex.image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);

    m_SizeX = currTex.image->sizeX;
    m_SizeY = currTex.image->sizeY;

    if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	return currTex.bindTexID;
}

/////////////////////////////////////////////////////////////////////////////
// CTexture LoadGLTexImage2DMipMap

GLuint CTexture::LoadGLTexImage2DMipMap(TCHAR* Filename)
{
	tex currTex;
  currTex.bindTexID = 0;
	if (currTex.image = LoadBMP(Filename))	
    {	
		glGenTextures(1, &currTex.bindTexID);				// Create One Texture
		glBindTexture(GL_TEXTURE_2D, currTex.bindTexID);

		// Create MipMapped Texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, currTex.image->sizeX, currTex.image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data);

    m_SizeX = currTex.image->sizeX;
    m_SizeY = currTex.image->sizeY;

    if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
	}
	return currTex.bindTexID;
}

/////////////////////////////////////////////////////////////////////////////
// CTexture LoadGLTexImage2DGroup

int CTexture::LoadGLTexImage2DGroup(TCHAR* Filename, int maxTexNb, GLuint* pTexIdTable)
{
	tex currTex;
  int nbTex = 0;
  int offset = 0;

	if (currTex.image = LoadBMP(Filename))	
  {	
    nbTex = currTex.image->sizeY / currTex.image->sizeX; // Text squares stored verticaly

    if (nbTex>maxTexNb)
      nbTex = maxTexNb;

		glGenTextures(nbTex, pTexIdTable);				// Create nbTex Textures

    for (int i=0; i<nbTex; i++)
    {
		  glBindTexture(GL_TEXTURE_2D, pTexIdTable[i]);
      currTex.bindTexID = pTexIdTable[i];
		  // Create MipMapped Texture
		  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, currTex.image->sizeX, currTex.image->sizeX, GL_RGB, GL_UNSIGNED_BYTE, currTex.image->data + offset);

      m_SizeX = currTex.image->sizeX;
      m_SizeY = currTex.image->sizeX;

      offset += m_SizeX * m_SizeX * 3; // 3bytes per pix
	  }

    if (currTex.image)							// If Texture Exists
		{
			if (currTex.image->data)					// If Texture Image Exists
			{
				free(currTex.image->data);			// Free The Texture Image Memory
			}
			free(currTex.image);						// Free The Image Structure
		}
  }

	return nbTex;
}

/////////////////////////////////////////////////////////////////////////////
// CTexture Use

void CTexture::Use(GLuint BIND_ID)
{
	glBindTexture(GL_TEXTURE_2D,BIND_ID);
}
