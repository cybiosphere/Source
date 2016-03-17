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

// CybiosphereSet.cpp : implementation of the CCybiosphereSet class
//

#include "stdafx.h"
#include "Cybiosphere.h"
#include "CybiosphereSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereSet implementation

IMPLEMENT_DYNAMIC(CCybiosphereSet, CRecordset)

CCybiosphereSet::CCybiosphereSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCybiosphereSet)
	m_N_ = 0;
	m_Label = _T("");
	m_DimensionX = 0;
	m_DimensionY = 0;
	m_DimensionZ = 0;
	m_tEntity = 0;
	m_IdEntity = 0;
	m_CoordX = 0;
	m_CoordY = 0;
	m_Layer = 0;
	m_Status = 0;
	m_RessourcePath = _T("");
	m_Label2 = _T("");
	m_Generation = 0;
	m_DayCounter = 0;
	m_tParameter = 0;
	m_IdParameter = 0;
	m_Label3 = _T("");
	m_ValMin = 0;
	m_ValCurrent = 0;
	m_ValNominal = 0;
	m_ValMax = 0;
	m_nFields = 22;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CCybiosphereSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access Database");
}

CString CCybiosphereSet::GetDefaultSQL()
{
	return _T("[Biotop],[Entity],[Parameter]");
}

void CCybiosphereSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCybiosphereSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[N°]"), m_N_);
	RFX_Text(pFX, _T("[Biotop].[Label]"), m_Label);
	RFX_Long(pFX, _T("[DimensionX]"), m_DimensionX);
	RFX_Long(pFX, _T("[DimensionY]"), m_DimensionY);
	RFX_Long(pFX, _T("[DimensionZ]"), m_DimensionZ);
	RFX_Long(pFX, _T("[tEntity]"), m_tEntity);
	RFX_Long(pFX, _T("[IdEntity]"), m_IdEntity);
	RFX_Long(pFX, _T("[CoordX]"), m_CoordX);
	RFX_Long(pFX, _T("[CoordY]"), m_CoordY);
	RFX_Long(pFX, _T("[Layer]"), m_Layer);
	RFX_Long(pFX, _T("[Status]"), m_Status);
	RFX_Text(pFX, _T("[RessourcePath]"), m_RessourcePath);
	RFX_Text(pFX, _T("[Entity].[Label]"), m_Label2);
	RFX_Long(pFX, _T("[Generation]"), m_Generation);
	RFX_Long(pFX, _T("[DayCounter]"), m_DayCounter);
	RFX_Long(pFX, _T("[tParameter]"), m_tParameter);
	RFX_Long(pFX, _T("[IdParameter]"), m_IdParameter);
	RFX_Text(pFX, _T("[Parameter].[Label]"), m_Label3);
	RFX_Long(pFX, _T("[ValMin]"), m_ValMin);
	RFX_Long(pFX, _T("[ValCurrent]"), m_ValCurrent);
	RFX_Long(pFX, _T("[ValNominal]"), m_ValNominal);
	RFX_Long(pFX, _T("[ValMax]"), m_ValMax);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCybiosphereSet diagnostics

#ifdef _DEBUG
void CCybiosphereSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCybiosphereSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
