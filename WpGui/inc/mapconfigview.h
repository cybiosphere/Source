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

#if !defined(AFX_MAPCONFIGVIEW_H__92559A8E_8C2C_4C14_891F_7C0424A592E5__INCLUDED_)
#define AFX_MAPCONFIGVIEW_H__92559A8E_8C2C_4C14_891F_7C0424A592E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapConfigView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapConfigView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

typedef enum
{
  MAP_TYPE_NONE = 0,
  MAP_TYPE_ODOR,
  MAP_TYPE_TEMPERATURE,
  MAP_TYPE_PURPOSE,
  MAP_TYPE_SENSORS,
  MAP_NUMBER_TYPE 
} MapConfigType_e;


class CMapConfigView : public CFormView
{
protected:
	CMapConfigView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMapConfigView)

// Form Data
public:
	//{{AFX_DATA(CMapConfigView)
	enum { IDD = IDD_MAP_FORMVIEW };
	BOOL	m_bIsOdorMap;
	BOOL	m_bIsTemperatureMap;
	BOOL	m_bIsPurposeMap;
  BOOL	m_bIsSensorMap;
	CString	m_purposeTxt1;
	CString	m_purposeTxt2;
	CString	m_purposeTxt3;
	CString	m_purposeTxt4;
	CString	m_purposeTxt5;
	CString	m_purposeTxt6;
	CString	m_purposeTxt7;
  CString	m_sensorTxt1;
  CString	m_sensorTxt2;
  CString	m_sensorTxt3;
  CString	m_sensorTxt4;
	//}}AFX_DATA

// Attributes
protected:
   CBiotop* m_pBiotop;
   MapConfigType_e m_CurMapType;
   int             m_CurMapSubType;
   CBasicEntity*   m_pSelectedEntity;

// Operations
public:

    void NextSecond();
    bool SetBiotop(CBiotop* pBiotop);
    bool BuildMap (bool forceRefresh = true);
    bool BuildOdorMap(OdorType_e odorType);
    bool BuildTemperatureMap();
    bool BuildPurposeMap(int index);
    bool BuildSensorsMap(int index);
    bool ClearMap();
    bool UpdateSelectedEntity(CBasicEntity* pEntity);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapConfigView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMapConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMapConfigView)
	afx_msg void OnRadioOdor1();
	afx_msg void OnRadioOdor2();
	afx_msg void OnRadioOdor3();
	afx_msg void OnRadioOdor4();
	afx_msg void OnRadioOdor5();
	afx_msg void OnRadioOdor6();
	afx_msg void OnRadioOdor7();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
  afx_msg void OnCheck4();
	afx_msg void OnRadioPurpose1();
	afx_msg void OnRadioPurpose2();
	afx_msg void OnRadioPurpose3();
	afx_msg void OnRadioPurpose4();
	afx_msg void OnRadioPurpose5();
	afx_msg void OnRadioPurpose6();
	afx_msg void OnRadioPurpose7();
  afx_msg void OnRadioSensor1();
  afx_msg void OnRadioSensor2();
  afx_msg void OnRadioSensor3();
  afx_msg void OnRadioSensor4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPCONFIGVIEW_H__92559A8E_8C2C_4C14_891F_7C0424A592E5__INCLUDED_)
