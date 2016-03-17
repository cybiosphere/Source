// CybiosphereSet.h : interface of the CCybiosphereSet class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYBIOSPHERESET_H__1F51EC1B_BB3F_44AC_92BA_DFB92745BA14__INCLUDED_)
#define AFX_CYBIOSPHERESET_H__1F51EC1B_BB3F_44AC_92BA_DFB92745BA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCybiosphereSet : public CRecordset
{
public:
	CCybiosphereSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCybiosphereSet)

// Field/Param Data
	//{{AFX_FIELD(CCybiosphereSet, CRecordset)
	long	m_N_;
	CString	m_Label;
	long	m_DimensionX;
	long	m_DimensionY;
	long	m_DimensionZ;
	long	m_tEntity;
	long	m_IdEntity;
	long	m_CoordX;
	long	m_CoordY;
	long	m_Layer;
	long	m_Status;
	CString	m_RessourcePath;
	CString	m_Label2;
	long	m_Generation;
	long	m_DayCounter;
	long	m_tParameter;
	long	m_IdParameter;
	CString	m_Label3;
	long	m_ValMin;
	long	m_ValCurrent;
	long	m_ValNominal;
	long	m_ValMax;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCybiosphereSet)
	public:
	virtual CString GetDefaultConnect();	// Default connection string
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYBIOSPHERESET_H__1F51EC1B_BB3F_44AC_92BA_DFB92745BA14__INCLUDED_)

