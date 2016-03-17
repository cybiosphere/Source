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

// SensWeightListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "SensWeightListCtrl.h"
#include "definitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSensWeightListCtrl

CSensWeightListCtrl::CSensWeightListCtrl()
{
  m_pSensor = NULL;
  m_pTmpEdtitBox = NULL;
  m_pCurWeightBuf = NULL;
  m_CurWeightBufSize = 0;

}

CSensWeightListCtrl::~CSensWeightListCtrl()
{
  if (m_pCurWeightBuf!=NULL)
    delete [] m_pCurWeightBuf;
}


BEGIN_MESSAGE_MAP(CSensWeightListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSensWeightListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSensWeightListCtrl message handlers

bool CSensWeightListCtrl::SetSensor(CSensor* pSens, WORD* geneDataBuf, bool bUseNegative)
{

  DeleteAllItems();
  if (GetColumnWidth(0)!=32)
  {
    InsertColumn(0, "rate", LVCFMT_CENTER, 32);
    InsertColumn(1, "name", LVCFMT_LEFT, 200);  
  }

  m_pSensor = pSens;  
  CString tmpStr;

  if (m_pSensor != NULL)
  {
    if (m_pCurWeightBuf!=NULL)
      delete m_pCurWeightBuf;
    m_CurWeightBufSize = m_pSensor->GetSubCaptorNumber();
    m_pCurWeightBuf = new short[m_CurWeightBufSize];
    
    for (int i=0; i<m_CurWeightBufSize; i++)
    {
      if (geneDataBuf == NULL)
      {
        m_pCurWeightBuf[i] = 0; // Set 0% as def values
      }
      else
      {
        if (bUseNegative)
          m_pCurWeightBuf[i] = (short)round(((double)geneDataBuf[i] * 2000.0) / 65535.0 - 1000.0);
        else
          m_pCurWeightBuf[i] = (short)round(((double)geneDataBuf[i] * 2000.0) / 65535.0);
      }
      tmpStr.Format("%5.01f", (double)m_pCurWeightBuf[i]/10.0);
      InsertItem(i,tmpStr); // Set 10% as def values
      SetItemText(i, 1, m_pSensor->GetSubCaptorLabel(i).c_str());
    }
  }

  return (true);
}

void CSensWeightListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
  POSITION pos = GetFirstSelectedItemPosition();
  int index = GetNextSelectedItem(pos);
  if (index>-1)
  { 
	  m_pTmpEdtitBox = EditLabel(index);
    m_pTmpEdtitBox->SetLimitText(7);
  }
	*pResult = 0;
}


short* CSensWeightListCtrl::GetWeightBuf()
{
  return (m_pCurWeightBuf);
}

int CSensWeightListCtrl::GetWeightBufSize()
{
  return (m_CurWeightBufSize);
}

void CSensWeightListCtrl::UdpateSelectedData(void) 
{
  POSITION pos = GetFirstSelectedItemPosition();
  int index = GetNextSelectedItem(pos);
  if ( (index>-1) && (m_pTmpEdtitBox!=NULL) )
  { 
    CString newTxt;
    m_pTmpEdtitBox->GetWindowText(newTxt);
    double newRate = atof(newTxt);
    newTxt.Format("%5.01f",newRate);
    SetItemText(index, 0, newTxt);
    m_pCurWeightBuf[index] =  (WORD)(newRate * 10.0);
  }

  m_pTmpEdtitBox = NULL;
}
