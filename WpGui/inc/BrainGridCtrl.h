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

#if !defined(AFX_BRAINGRIDCTRL_H__9745BB23_F1C0_4B8F_AA43_BC502421997A__INCLUDED_)
#define AFX_BRAINGRIDCTRL_H__9745BB23_F1C0_4B8F_AA43_BC502421997A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrainGridCtrl.h : header file
//

#include "GridCtrl.h"
#include "CBrain.h" 


/////////////////////////////////////////////////////////////////////////////
// CNeuronMatrixCtrl window

class CNeuronMatrixCtrl : public CGridCtrl
{
// Construction
public:
	int CBrainGridCtrl();

// Attributes
public:
  CNeuronMatrix* m_pNeuronMatrix;
  CNeuronMatrix* m_pReferenceNeuronMatrixToCompare;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainGridCtrl)
	//}}AFX_VIRTUAL

public:
  bool SetNeuronMatrix(CNeuronMatrix* pNeuronMatrix);
  void SetReferenceNeuronMatrixToCompare(CNeuronMatrix* pNeuronMatrix);
  void RefreshNeuronMatrixData();
  void RefreshInputOutputVectAndThresholds(CMatrix* pInputVect, CMatrix* pOutputVect, double maxVal, double midThreshold, double lowThreshold);
  void ColorizeDeltaFromReference();
  void ClearColorizedCells();

// Implementation
public:
	virtual ~CNeuronMatrixCtrl();

};

/////////////////////////////////////////////////////////////////////////////
// CBrainGridCtrl window

class CBrainGridCtrl : public CNeuronMatrixCtrl
{
// Construction
public:
	CBrainGridCtrl();

// Attributes
public:
  CBrain* m_pBrain;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainGridCtrl)
	//}}AFX_VIRTUAL

public:
  // Function for Decision Brain
  bool SetBrain(CBrain* pBrain);
  void RefreshBrainData();

  // Function for Identification Brain
  bool SetIdentifyBrain(CBrain* pBrain);
  void RefreshIdentifyBrainData();

  //Search methodes
  void SelectRowFromString(CString findStr);

// Implementation
public:
	virtual ~CBrainGridCtrl();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINGRIDCTRL_H__9745BB23_F1C0_4B8F_AA43_BC502421997A__INCLUDED_)
