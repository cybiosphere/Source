/*
https://github.com/cybiosphere
copyright (c) 2022 Frederic RIBERT

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

// FindStrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "BiotopSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindStrDlg dialog

CBiotopSizeDlg::CBiotopSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBiotopSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindStrDlg)
	m_BiotopSizeX = 100;
	m_BiotopSizeY = 100;
	//}}AFX_DATA_INIT
}

void CBiotopSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBiotopSizeDlg)
  DDX_Text(pDX, IDC_BIOTOP_SIZEX, m_BiotopSizeX);
	DDV_MinMaxLong(pDX, m_BiotopSizeX, 1, 100000);
	DDX_Text(pDX, IDC_BIOTOP_SIZEY, m_BiotopSizeY);
	DDV_MinMaxLong(pDX, m_BiotopSizeY, 1, 100000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBiotopSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CBiotopSizeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBiotopSizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the input focus to your control
	GotoDlgCtrl(GetDlgItem(IDC_BIOTOP_SIZEX));
	return FALSE;  
}

/////////////////////////////////////////////////////////////////////////////
// CBiotopSizeDlg message handlers

size_t CBiotopSizeDlg::GetBiotopSizeX(void)
{
  return m_BiotopSizeX;
}

size_t CBiotopSizeDlg::GetBiotopSizeY(void)
{
	return m_BiotopSizeY;
}