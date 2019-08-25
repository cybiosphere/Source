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

// BrainGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "BrainGridCtrl.h"
#include "CAnimal.h"
#include "CBrainAnimal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNeuronMatrixCtrl

/*CNeuronMatrixCtrl::CNeuronMatrixCtrl()
{
}*/

CNeuronMatrixCtrl::~CNeuronMatrixCtrl()
{
}

bool CNeuronMatrixCtrl::SetNeuronMatrix(CNeuronMatrix* pNeuronMatrix)
{
  m_pNeuronMatrix = pNeuronMatrix;
  int i;

  if (m_pNeuronMatrix!=NULL)
  {
    SetRowCount(m_pNeuronMatrix->GetNeuronTableRowCount() + 2);
    SetColumnCount(m_pNeuronMatrix->GetNeuronTableColumnCount() + 2);
    SetEditable(true);
    SetFixedRowCount(2);
    SetFixedColumnCount(2);
    int iDpi = GetDpiForWindow(this->m_hWnd);
    SetColumnWidth(0, MulDiv(144, iDpi, 96));
    for (i=1; i<GetColumnCount(); i++)
      SetColumnWidth(i, MulDiv(50, iDpi, 96));
  }
  else
  {
    SetRowCount(0);
    SetColumnCount(0);
  }

  RefreshNeuronMatrixData();

  // Cell cannot be edited manually
  SetEditable(FALSE);

  return (true);
}

void CNeuronMatrixCtrl::RefreshNeuronMatrixData()
{
	if (m_pNeuronMatrix == NULL)
    return;

  CString curTxt;
  double curVal=0;

  for (size_t i=0; i<m_pNeuronMatrix->GetNeuronTableRowCount(); i++)
  {
    for (size_t j=0; j<m_pNeuronMatrix->GetNeuronTableColumnCount();j++)
    {
      curTxt.Format(LPCTSTR("%02.2f"), m_pNeuronMatrix->GetNeuronTableData(i,j));
      SetItemText(i+2,j+2,curTxt); 
    }
  }

  // Update windows
  Invalidate();
}

void CNeuronMatrixCtrl::RefreshInputOutputVectAndThresholds(CMatrix* pInputVect, CMatrix* pOutputVect, double highThreshold, double midThreshold, double lowThreshold)
{
  if (m_pNeuronMatrix == NULL)
    return;

  CString curTxt;
  double curVal=0;
  size_t i;

  for (i=0; i<pInputVect->RowNo(); i++)
  {
    curTxt.Format(LPCTSTR("%02.2f"), (*pInputVect)(i,0));
    SetItemText(i+2,1,curTxt);
  }

  for (i=0; i<pOutputVect->RowNo(); i++)
  {
    curVal = (*pOutputVect)(i,0);
    curTxt.Format(LPCTSTR("%02.2f"), curVal);
    SetItemText(1,i+2,curTxt);

    if (curVal >= highThreshold)
      SetItemBkColour(1,i+2,0x009CF52F);
    else if (curVal > midThreshold)
      SetItemBkColour(1,i+2,0x00C9F27C);
    else if (curVal > lowThreshold)
      SetItemBkColour(1,i+2,0x00EAF1BD);
    else
      SetItemBkColour(1,i+2,0x00D0D0D0);
  }

  // Update windows
  Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// CBrainGridCtrl

CBrainGridCtrl::CBrainGridCtrl()
{
}

CBrainGridCtrl::~CBrainGridCtrl()
{
}

bool CBrainGridCtrl::SetBrain(CBrain* pBrain)
{
  m_pBrain = pBrain;
  int i;

  if (m_pBrain!=NULL)
  {
    // resset prev grid
    SetNeuronMatrix(NULL);
    // set new grid
    SetNeuronMatrix(m_pBrain->GetDecisionNeuronTable());

    for (i=0; i<m_pBrain->GetDecisionNeuronTable()->GetNeuronTableColumnCount(); i++)
      SetItemText(0,i+2, LPCTSTR(m_pBrain->GetColumnLabel(i).c_str()));
    for (i=0; i<m_pBrain->GetDecisionNeuronTable()->GetNeuronTableRowCount(); i++)
      SetItemText(i+2,0, LPCTSTR(m_pBrain->GetRowLabel(i).c_str()));

    // Set a few color lines as reference marks
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_HUNGER, 1, 0, 0)      , 0, 0x00D0E0D0); // green
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_THIRST, 1, 0, 0)      , 0, 0x00F0D0D0); // blue
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_SUFFER, 1, 0, 0)      , 0, 0x00A0A0E0); // red 
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_STOMACH_FULL, 1, 0, 0), 0, 0x00C0C0E0); // red
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_OBSCURITY, 1, 0, 0)   , 0, 0x00A0A0A0); // grey dark
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_TIREDNESS, 1, 0, 0)   , 0, 0x00C0C0C0); // grey
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_LIBIDO, 1, 0, 0)      , 0, 0x00D0D0F0); // pink
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_PLEASURE, 1, 0, 0)    , 0, 0x00C8C8F0); // pink
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_FEAR, 1, 0, 0)        , 0, 0x00B0B0B0); // grey mid

    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_HUNGER, 1, 0, 1)      , 0, 0x00D0E0D0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_THIRST, 1, 0, 1)      , 0, 0x00F0D0D0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_SUFFER, 1, 0, 1)      , 0, 0x00A0A0E0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_STOMACH_FULL, 1, 0, 1), 0, 0x00C0C0E0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_OBSCURITY, 1, 0, 1)   , 0, 0x00A0A0A0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_TIREDNESS, 1, 0, 1)   , 0, 0x00C0C0C0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_LIBIDO, 1, 0, 1)      , 0, 0x00D0D0F0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_PLEASURE, 1, 0, 1)    , 0, 0x00C8C8F0);
    SetItemBkColour( 2 + m_pBrain->GetBrainMatrixRowIndex(UID_BASE_SENS_FEAR, 1, 0, 1)        , 0, 0x00B0B0B0);

    // Update windows
    Invalidate();

    RefreshBrainData();
  }
  else
  {
    SetNeuronMatrix(NULL);
  }

  return (true);
}

void CBrainGridCtrl::RefreshBrainData()
{
  if (m_pBrain!=NULL)
  {
    double maxVal,midThreshold,lowThreshold;
    double curiosity = ((CBrainAnimal*)m_pBrain)->getAnimal()->getCuriosityRate();
    m_pBrain->GetVectorChoiceThresholds(curiosity, maxVal, midThreshold, lowThreshold);
    RefreshInputOutputVectAndThresholds(m_pBrain->GetInputVect(), m_pBrain->GetChoiceVect(), maxVal,midThreshold,lowThreshold);

    RefreshNeuronMatrixData();
  }
}


bool CBrainGridCtrl::SetIdentifyBrain(CBrain* pBrain)
{
  m_pBrain = pBrain;
  int i;

  if (m_pBrain!=NULL)
  {
    SetNeuronMatrix(m_pBrain->GetIdentifyNeuronTable());

    for (i=0; i<m_pBrain->GetIdentifyNeuronTable()->GetNeuronTableColumnCount(); i++)
      SetItemText(0,i+2, LPCTSTR(m_pBrain->getIdentificationLabel(i).c_str()));

    for (i=0; i<m_pBrain->GetIdentifyNeuronTable()->GetNeuronTableRowCount(); i++)
      SetItemText(i+2,0, LPCTSTR(m_pBrain->getIdentifyInputLabel(i).c_str()));

    RefreshIdentifyBrainData();
  }
  else
  {
    SetNeuronMatrix(NULL);
  }

  return (true);
}

void CBrainGridCtrl::RefreshIdentifyBrainData()
{
  if (m_pBrain!=NULL)
  {
    double highThreshold=0,midThreshold=0,lowThreshold=0;
    double curiosity = ((CBrainAnimal*)m_pBrain)->getAnimal()->getCuriosityRate();
    m_pBrain->GetVectorIdentifyThresholds(highThreshold, midThreshold, lowThreshold);
    RefreshInputOutputVectAndThresholds(m_pBrain->GetIdentifyInputVect(), m_pBrain->GetIdentifyOutputVect(), highThreshold,midThreshold,lowThreshold);
    
    RefreshNeuronMatrixData();
  }
}

/////////////////////////////////////////////////////////////////////////////
// CBrainGridCtrl message handlers
