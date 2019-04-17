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

// stdafxGL.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFXGL_H_INCLUDED_)
#define AFX_STDAFXGL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

// Include OpenGL header files
#include <gl\gl.h>			// opengl32.lib 
//#include <gl\glaux.h>		// glaux.lib 
#include <gl\glu.h>			// glu32.lib 
#include <mmsystem.h>		// winmm.lib (used for timeGetTime function)

// Generate auto linkage for OpenGL libraries
#pragma comment(lib, "OPENGL32.LIB")
//#pragma comment(lib, "GLAUX.LIB")
#pragma comment(lib, "GLU32.LIB")

// Generate auto linkage for Direct Sound libraries
//#pragma comment(lib, "DSOUND.LIB")
//#pragma comment(lib, "WINMM.LIB")

#endif // !defined(AFX_STDAFXGL_H_INCLUDED_)
