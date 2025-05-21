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

//===========================================================================
// FILE: CNeuronMatrix.cpp
//   
// GENERAL DESCRIPTION:
//         This CLASS represents an animal brain
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  21/08/2005 : Creation
//
//===========================================================================

#include "CNeuronMatrix.h"
#include <fstream>

// FRI TBVerified
#define MEMORIZATION_FEEDBACK_THRESHOLD 0.05

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeuronMatrix::CNeuronMatrix()
{
  // Neuronal matrix table
  m_mNeuronTable.SetSize(0,0);
  m_xmlFileLabel = "UNDEF";
}

CNeuronMatrix::~CNeuronMatrix()
{
  //m_mNeuronTable.SetSize(0,0);
}


//////////////////////////////////////////////////////////////////////
// Tools
//////////////////////////////////////////////////////////////////////
bool CNeuronMatrix::NormalizeNeuronMatrix()
{
  bool resu = false;
  double norm = 0;

  for (size_t i=0; i<m_mNeuronTable.RowNo(); i++)
  {
    //Process vector norm
    norm = 0;
    for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
    {
      norm += pow(m_mNeuronTable(i,j),2.0);
    }
    
    if (norm == 0)
    {
      resu = false;
    }
    else
    {
      norm = sqrt(norm);
      // divide vector by its norm
      for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
      {
        m_mNeuronTable(i,j)=m_mNeuronTable(i,j)/norm;
      }
      resu = true;
    }
  }

  return resu;
}

//////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////

bool CNeuronMatrix::InitializeNeuronMatrixNeutral(string xmlFileLabel, size_t sizeInput, size_t sizeOutput)
{
  m_xmlFileLabel = xmlFileLabel;
  m_mNeuronTable.SetSize(sizeInput,sizeOutput);
  return ResetNeuronMatrixNeutral();
}

bool CNeuronMatrix::ResetNeuronMatrixNeutral()
{
  for (size_t i=0; i<m_mNeuronTable.RowNo(); i++)
  {
    for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
    {
      m_mNeuronTable(i,j) = 1;
    }
  }
  return NormalizeNeuronMatrix();
}

//////////////////////////////////////////////////////////////////////
// Process
//////////////////////////////////////////////////////////////////////

void CNeuronMatrix::ComputeVectorChoice (CMatrix& pVectorInput, CMatrix& pVectorChoice)
{
  // Optim CPU
  //*pVectorChoice = ~(~(*pVectorInput)*m_mNeuronTable);

  // Reset pVectorChoice
  size_t i, j;
  for (i = 0; i < pVectorChoice.RowNo(); i++)
  {
    pVectorChoice(i, 0) = 0;
  }
  // Compute pVectorChoice
  for (i = 0; i < pVectorInput.RowNo(); i++)
  {
    if (pVectorInput(i, 0) != 0)
    {
      for (j = 0; j < pVectorChoice.RowNo(); j++)
      {
        pVectorChoice(j, 0) += m_mNeuronTable(i, j) * pVectorInput(i, 0);
      }
    }
  }
}

bool CNeuronMatrix::MemorizeExperience (neuroneValType coefFeedback, CMatrix* pMatrixInputHistory, CMatrix* pVectorDecisionHistory)
{
  // coef is the coefficient of effect of the previous action
  // The effect of an action cannot exceed 1% when learningRate is at 100%
  CMatrix expMatrix = (*pMatrixInputHistory) * ~(*pVectorDecisionHistory);
  for (size_t i=0; i<m_mNeuronTable.RowNo(); i++)
  {
    for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
    {
      if (expMatrix(i, j) != 0.0) // CPU optim. expMatrix is often 0
      {
        m_mNeuronTable(i, j) += coefFeedback * expMatrix(i, j);
      }
    }
  }

  NormalizeNeuronMatrix();
  return true;
}

string CNeuronMatrix::buildStringDataFromNeuronTable()
{
  string tmpStr;
  WORD tempVal;
  string rawDataRsp = "";
  for (size_t i=0; i<m_mNeuronTable.RowNo(); i++)
  {
    for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
    {
      tempVal = (WORD)( (1.0 + m_mNeuronTable(i,j)) * 32767.0);
      tmpStr = FormatString("%4X",tempVal);
      rawDataRsp += tmpStr;
    }
  }
  return rawDataRsp;
}

bool CNeuronMatrix::buildNeuronTableFromStringData(string rawData)
{
  string tmpStr = "0000";
  WORD tempVal;
  string rawDataRsp = "";
  size_t strOffset = 0;
  size_t dataLen = rawData.length();
  char* pEnd;
  for (size_t i=0; i<m_mNeuronTable.RowNo(); i++)
  {
    for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
    {
      if ((strOffset + 4) > dataLen)
      {
        NormalizeNeuronMatrix();
        return false;
      }
      tmpStr = rawData.substr(strOffset, 4);
      tempVal = (WORD)strtol(tmpStr.c_str(), &pEnd, 16);
      m_mNeuronTable(i,j) = (double)tempVal / 32767.0 - 1.0;
      strOffset += 4;
    }
  }
  NormalizeNeuronMatrix();
  return true;
}

bool CNeuronMatrix::buildNeuronLineFromRawData (size_t lineId, size_t lenData, WORD* pRawData)
{
  if ( (lenData != m_mNeuronTable.ColNo()) || (lineId >= m_mNeuronTable.RowNo()) )
  {
    ASSERT("Bad raw data lenght for brain");
    return false;
  }

  for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
  {
    m_mNeuronTable(lineId,j) = (double)pRawData[j] / 32767.0 - 1.0;
  }

  return true;
}

int CNeuronMatrix::buildRawDataFromNeuronLine (size_t lineId, WORD*& pRawData)
{
  if (lineId >= m_mNeuronTable.RowNo())
  {
    ASSERT("Bad raw data lenght for brain");
    return false;
  }

  pRawData = new WORD[(int)m_mNeuronTable.ColNo()];

  for (size_t j=0; j<m_mNeuronTable.ColNo();j++)
  {
    pRawData[j] = cybio_round( (m_mNeuronTable(lineId,j) + 1.0) * 32767.0 );
  }

  return (int)m_mNeuronTable.ColNo();
}

size_t CNeuronMatrix::GetNeuronTableRowCount()
{
  return m_mNeuronTable.RowNo();
}

size_t CNeuronMatrix::GetNeuronTableColumnCount()
{
  return m_mNeuronTable.ColNo();
}

neuroneValType CNeuronMatrix::GetNeuronTableData(size_t row, size_t col)
{
  return m_mNeuronTable(row,col);
}

bool CNeuronMatrix::SetNeuronTableData(size_t row, size_t col, neuroneValType newVal)
{
  if ( (row >= m_mNeuronTable.RowNo()) || (col >= m_mNeuronTable.ColNo()) )
     return false;

  m_mNeuronTable(row,col) = newVal;
  return true;
}

bool CNeuronMatrix::ChangeNeuronTableVal(size_t row, size_t col, double variation, bool normalize)
{
  if ( (row >= m_mNeuronTable.RowNo()) || (col >= m_mNeuronTable.ColNo()) )
    return false;

  bool resu = true;
  m_mNeuronTable(row, col) += variation;

  if (normalize)
    resu = NormalizeNeuronMatrix();

  return resu;

}

bool CNeuronMatrix::saveInXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  xmlDoc.LoadFile();
  saveInXmlFile(&xmlDoc);
  resu = xmlDoc.SaveFile();
  return resu;
}

bool CNeuronMatrix::saveInXmlFile(TiXmlDocument *pXmlDoc)
{
  TiXmlNode* pNodeEntity = NULL;
  TiXmlNode* pNodeBrain = NULL;

  if (pXmlDoc==NULL) 
    return false;

  pNodeEntity = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if (pNodeEntity==NULL)
  {
    TiXmlElement newNode(XML_NODE_ENTITY);
    pNodeEntity = pXmlDoc->InsertEndChild(newNode);
  }

  pNodeBrain = pNodeEntity->FirstChild(m_xmlFileLabel);
  if (pNodeBrain==NULL)
  {
    // Create Brain node
    TiXmlElement newNode(m_xmlFileLabel);
    pNodeBrain = pNodeEntity->InsertEndChild(newNode);
  }
  else
  {
    // Clear previous Brain
    while (pNodeBrain->FirstChild() != NULL)
      pNodeBrain->RemoveChild(pNodeBrain->FirstChild());
  }

  // Set Brain text
  if (pNodeBrain != NULL)
  {
    TiXmlText newBrainTxt(buildStringDataFromNeuronTable());
    pNodeBrain->InsertEndChild(newBrainTxt);
  }

  return true;
}

bool CNeuronMatrix::loadFromXmlFile(string fileNameWithPath)
{
  bool resu = false;
  TiXmlDocument xmlDoc(fileNameWithPath);
  resu = xmlDoc.LoadFile();
  loadFromXmlFile(&xmlDoc);
  return resu;
}

bool CNeuronMatrix::loadFromXmlFile(TiXmlDocument *pXmlDoc)
{
  bool resu = false;

  if (pXmlDoc==NULL)
    return false;

  TiXmlNode* pNode = pXmlDoc->FirstChild(XML_NODE_ENTITY);
  if (pNode!=NULL)
  {
    pNode = pNode->FirstChild(m_xmlFileLabel);
    if (pNode!=NULL)
    {
      pNode = pNode->FirstChild();
      if ((pNode!=NULL) && (pNode->Type() == TiXmlNode::TINYXML_TEXT))
        resu = buildNeuronTableFromStringData(pNode->Value());
    }
  }

  return resu;
}

bool CNeuronMatrix::saveInFile(string fileNameWithPath)
{
  string savedBrain;
  savedBrain = buildStringDataFromNeuronTable();

  ofstream f1;
  f1.open(fileNameWithPath.c_str());
  f1.write(savedBrain.c_str(), savedBrain.length());
  f1.close();  

  return true;
}

bool CNeuronMatrix::loadFromFile(string fileNameWithPath)
{
  string loadedBrain;
  char* pbuf;
  std::streamoff begin, end;
  long fLength; 
  ifstream f1;
  bool resu = false;

  f1.open( fileNameWithPath.c_str());
  begin = f1.tellg();
  f1.seekg(0, ios::end);
  end = f1.tellg();
  fLength = ((long)end - (long)begin);
  f1.seekg(0, ios::beg);

  if ( (fLength==0) || (fLength>1000000) )
  {
    return false;
  }

  pbuf = new char[fLength+1];
  memset(pbuf,0,fLength+1);
  f1.read(pbuf,fLength);
  loadedBrain = pbuf;
  f1.close();

  delete[] pbuf;

  if (loadedBrain.length()>0)
  {
    resu = buildNeuronTableFromStringData(loadedBrain);
  }

  return resu;
}
