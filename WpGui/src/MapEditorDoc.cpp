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

// MapEditorDoc.cpp : implementation of the CMapEditorDoc class
//

#include "stdafx.h"
#include "Cybiosphere.h"

#include "MapEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc

IMPLEMENT_DYNCREATE(CMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditorDoc, CDocument)
  //{{AFX_MSG_MAP(CMapEditorDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc construction/destruction

CMapEditorDoc::CMapEditorDoc()
{
  // TODO: add one-time construction code here

}

CMapEditorDoc::~CMapEditorDoc()
{
}

BOOL CMapEditorDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc serialization

void CMapEditorDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: add storing code here
  }
  else
  {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc diagnostics

#ifdef _DEBUG
void CMapEditorDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CMapEditorDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc commands
