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

// CNeuronMatrix.h: interface for the CBrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURON_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_)
#define AFX_NEURON_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Includes 
//===========================================================================
#include "Definitions.h"
#include "Matrix.h"
#include "CybioXmlDef.h"

//===========================================================================
// Definitions            
//===========================================================================
typedef double neuroneValType;
typedef matrix<neuroneValType> CMatrix;

//===========================================================================
//                                    CLASS            
//===========================================================================
class DLL_CYBIOCORE_API CNeuronMatrix  
{
//===========================================================================
// Attributes 
//===========================================================================
private:
  // Label used to open/save neurone table from/to XML file
  string m_xmlFileLabel;

  // MATRIX
  // Neuronal matrix table
  CMatrix m_mNeuronTable;

//===========================================================================
// Methods 
//===========================================================================

//---------------------------------------------------------------------------
// Constructors / Destructors
//---------------------------------------------------------------------------
public:
	CNeuronMatrix();
	virtual ~CNeuronMatrix();


//---------------------------------------------------------------------------
// public methods
//---------------------------------------------------------------------------
public:

  bool NormalizeNeuronMatrix();
  bool InitializeNeuronMatrixNeutral(string xmlFileLabel, size_t sizeInput, size_t sizeOutput);
  bool ResetNeuronMatrixNeutral();
  void ComputeVectorChoice(CMatrix& pVectorInput, CMatrix& pVectorChoice);
  bool MemorizeExperience(neuroneValType coefFeedback, CMatrix* pMatrixInputHistory, CMatrix* pVectorDecisionHistory);

  string buildStringDataFromNeuronTable();
  bool buildNeuronTableFromStringData(string rawData);
  bool buildNeuronLineFromRawData(size_t lineId, size_t lenData, WORD* pRawData);
  int  buildRawDataFromNeuronLine(size_t lineId, WORD*& pRawData);

  size_t GetNeuronTableRowCount();
  size_t GetNeuronTableColumnCount();
  neuroneValType GetNeuronTableData(size_t row, size_t col);
  bool SetNeuronTableData(size_t row, size_t col, neuroneValType newVal);
  bool ChangeNeuronTableVal(size_t row, size_t col, double variation, bool normalize=true);

  bool saveInXmlFile(string fileNameWithPath);
  bool saveInXmlFile(TiXmlDocument *pXmlDoc);
  bool loadFromXmlFile(string fileNameWithPath);
  bool loadFromXmlFile(TiXmlDocument *pXmlDoc);

  bool saveInFile(string fileNameWithPath);
  bool loadFromFile(string fileNameWithPath);

};

#endif // !defined(AFX_NEURON_H__AA2E81DE_1CD7_4BEF_B88B_0B04AD788087__INCLUDED_)
