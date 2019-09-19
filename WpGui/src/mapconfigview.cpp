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

// MapConfigView.cpp : implementation file
//

#include "stdafx.h"
#include "cybiosphere.h"
#include "MapConfigView.h"
#include "CAnimal.h"
#include "CGeoMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCybiosphereApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMapConfigView

IMPLEMENT_DYNCREATE(CMapConfigView, CFormView)

CMapConfigView::CMapConfigView()
	: CFormView(CMapConfigView::IDD)
{
    m_pBiotop = NULL;
    m_CurMapType = MAP_TYPE_NONE;
    m_CurMapSubType = 0;
    m_pSelectedEntity = NULL;

	//{{AFX_DATA_INIT(CMapConfigView)
	m_bIsOdorMap = FALSE;
  m_bIsTemperatureMap = FALSE;
  m_bIsPurposeMap = FALSE;
	m_purposeTxt1 = _T("");
	m_purposeTxt2 = _T("");
	m_purposeTxt3 = _T("");
	m_purposeTxt4 = _T("");
	m_purposeTxt5 = _T("");
	m_purposeTxt6 = _T("");
	m_purposeTxt7 = _T("");
  m_sensorTxt1 = _T("");
  m_sensorTxt2 = _T("");
  m_sensorTxt3 = _T("");
  m_sensorTxt4 = _T("");
	//}}AFX_DATA_INIT
}

CMapConfigView::~CMapConfigView()
{
}

void CMapConfigView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapConfigView)
	DDX_Check(pDX, IDC_CHECK1, m_bIsOdorMap);
	DDX_Check(pDX, IDC_CHECK2, m_bIsTemperatureMap);
  DDX_Check(pDX, IDC_CHECK3, m_bIsPurposeMap);
  DDX_Check(pDX, IDC_CHECK4, m_bIsSensorMap);
	DDX_Text(pDX, IDC_PURPOSE_TXT1, m_purposeTxt1);
	DDX_Text(pDX, IDC_PURPOSE_TXT2, m_purposeTxt2);
	DDX_Text(pDX, IDC_PURPOSE_TXT3, m_purposeTxt3);
	DDX_Text(pDX, IDC_PURPOSE_TXT4, m_purposeTxt4);
	DDX_Text(pDX, IDC_PURPOSE_TXT5, m_purposeTxt5);
	DDX_Text(pDX, IDC_PURPOSE_TXT6, m_purposeTxt6);
	DDX_Text(pDX, IDC_PURPOSE_TXT7, m_purposeTxt7);
  DDX_Text(pDX, IDC_SENSOR_TXT1, m_sensorTxt1);
  DDX_Text(pDX, IDC_SENSOR_TXT2, m_sensorTxt2);
  DDX_Text(pDX, IDC_SENSOR_TXT3, m_sensorTxt3);
  DDX_Text(pDX, IDC_SENSOR_TXT4, m_sensorTxt4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapConfigView, CFormView)
	//{{AFX_MSG_MAP(CMapConfigView)
	ON_BN_CLICKED(IDC_RADIO_ODOR1, OnRadioOdor1)
	ON_BN_CLICKED(IDC_RADIO_ODOR2, OnRadioOdor2)
	ON_BN_CLICKED(IDC_RADIO_ODOR3, OnRadioOdor3)
	ON_BN_CLICKED(IDC_RADIO_ODOR4, OnRadioOdor4)
	ON_BN_CLICKED(IDC_RADIO_ODOR5, OnRadioOdor5)
	ON_BN_CLICKED(IDC_RADIO_ODOR6, OnRadioOdor6)
	ON_BN_CLICKED(IDC_RADIO_ODOR7, OnRadioOdor7)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
  ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE1, OnRadioPurpose1)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE2, OnRadioPurpose2)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE3, OnRadioPurpose3)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE4, OnRadioPurpose4)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE5, OnRadioPurpose5)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE6, OnRadioPurpose6)
	ON_BN_CLICKED(IDC_RADIO_PURPOSE7, OnRadioPurpose7)
  ON_BN_CLICKED(IDC_RADIO_SENSOR1, OnRadioSensor1)
  ON_BN_CLICKED(IDC_RADIO_SENSOR2, OnRadioSensor2)
  ON_BN_CLICKED(IDC_RADIO_SENSOR3, OnRadioSensor3)
  ON_BN_CLICKED(IDC_RADIO_SENSOR4, OnRadioSensor4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapConfigView diagnostics

#ifdef _DEBUG
void CMapConfigView::AssertValid() const
{
  CFormView::AssertValid();
}

void CMapConfigView::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}
#endif //_DEBUG

bool CMapConfigView::SetBiotop(CBiotop* pBiotop)
{
  m_pBiotop = pBiotop;
  return (true);
}

void CMapConfigView::NextSecond()
{
  BuildMap(false);
}

/////////////////////////////////////////////////////////////////////////////
// CMapConfigView message handlers

bool CMapConfigView::BuildMap(bool forceRefresh)
{
  if (m_pBiotop==NULL)
    return (false);

  bool resu = false;

  switch(m_CurMapType)
  {
    case MAP_TYPE_ODOR:
      if (m_CurMapSubType>0)
        resu = BuildOdorMap((OdorType_e)m_CurMapSubType);
      else
        ClearMap();
      if (forceRefresh)
        theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
      break;
    case MAP_TYPE_TEMPERATURE:
      resu = BuildTemperatureMap();
      if (forceRefresh)
        theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
      break;
    case MAP_TYPE_PURPOSE:
      if (forceRefresh)
      {
        resu = BuildPurposeMap(m_CurMapSubType);
        theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
      }
      break;
    case MAP_TYPE_SENSORS:
      resu = BuildSensorsMap(m_CurMapSubType);
      if (forceRefresh)
        theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
      break;
    default:
      resu = false;
      break;
  }

  return resu;
}

bool CMapConfigView::BuildOdorMap(OdorType_e odorType)
{
  int i,j,k;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  double odorLevel[NUMBER_ODORS];
  Point_t coord;
  // Trace odor map
  for (i=0;i<m_pBiotop->getDimension().x;i++)
  {
    for (j=0; j<m_pBiotop->getDimension().y; j++)
    { 
      coord.x = i;
      coord.y = j;
      m_pBiotop->getOdorLevels(coord,5, odorLevel);
      tBioSquare[i][j].customColor = 0x00FFFFFF - (DWORD)(floor(25.0*odorLevel[odorType-ODOR_FIRST_TYPE])*257);  
    }
  }

  // Refresh mandatory
  theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
  return (true);
}

bool CMapConfigView::BuildTemperatureMap()
{
  int i,j;
  double temperature;
  DWORD r,g,b;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  Point_t curCoord;
  Point_t startCoord = theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetGridCoordFromScreenPos(CPoint(1,1));

  int rangeDisplay = 2*(theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetCurrentGridCenterPos().x - startCoord.x);

  // Trace temperature map
  for (i=0; i<rangeDisplay;i++)
  {
    for (j=0; j<rangeDisplay; j++)
    { 
      curCoord.x = startCoord.x + i;
      curCoord.y = startCoord.y - j;
      if (m_pBiotop->isCoordValid(curCoord,2))
      {
        temperature = m_pBiotop->getTemperature(curCoord,2);
        if (temperature>25)
        {
          r=250;g=500-temperature*10;b=10;
        }
        else if (temperature>10)
        {
          r=(temperature-10)*10;g=250;b=10;
        }
        else
        {
          r=10;g=150;b=abs(temperature-10)*5;
        }

        tBioSquare[curCoord.x][curCoord.y].customColor = (b<<0x10) + (g<<0x08) + r;  
      }
    }
  }

  return (true);
}


bool CMapConfigView::UpdateSelectedEntity(CBasicEntity* pEntity)
{
  m_purposeTxt1 = "";
  m_purposeTxt2 = "";
  m_purposeTxt3 = "";
  m_purposeTxt4 = "";
  m_purposeTxt5 = "";
  m_purposeTxt6 = "";
  m_purposeTxt7 = "";
  m_sensorTxt1 = "";
  m_sensorTxt2 = "";
  m_sensorTxt3 = "";
  m_sensorTxt4 = "";

  if ((pEntity==NULL)||(pEntity->getBrain()==NULL)|| (pEntity->getBrain()->GetGeographicMap()==NULL))
  { 
    if (m_CurMapType == MAP_TYPE_PURPOSE)
      ClearMap();
    m_pSelectedEntity = NULL;
    UpdateData(FALSE);
    return true;
  }

  CGeoMap* pGeoMap = pEntity->getBrain()->GetGeographicMap();

  // Update button label if necessary
  if (m_pSelectedEntity != pEntity)
  {
    CPurpose* pPurpose;

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(0));
    if (pPurpose!=NULL)
      m_purposeTxt1 = pPurpose->GetLabel().c_str();
      
    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(1));
    if (pPurpose!=NULL)
      m_purposeTxt2 = pPurpose->GetLabel().c_str(); 

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(2));
    if (pPurpose!=NULL)
      m_purposeTxt3 = pPurpose->GetLabel().c_str(); 

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(3));
    if (pPurpose!=NULL)
      m_purposeTxt4 = pPurpose->GetLabel().c_str(); 

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(4));
    if (pPurpose!=NULL)
      m_purposeTxt5 = pPurpose->GetLabel().c_str(); 

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(5));
    if (pPurpose!=NULL)
      m_purposeTxt6 = pPurpose->GetLabel().c_str(); 

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(6));
    if (pPurpose!=NULL)
      m_purposeTxt7 = pPurpose->GetLabel().c_str(); 

    m_sensorTxt1 = "View";
    m_sensorTxt2 = "Ear";
    m_sensorTxt3 = "Smell";
    m_sensorTxt4 = "Pheromons";

    m_pSelectedEntity = pEntity;
    UpdateData(FALSE);

    if((m_bIsPurposeMap) || (m_bIsSensorMap))
      BuildMap(true);
  }

  return true;
}


bool CMapConfigView::BuildPurposeMap(int index)
{
  int i,j;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  ClearMap();
  if ((m_pSelectedEntity==NULL)||(m_pSelectedEntity->getBrain()==NULL)|| (m_pSelectedEntity->getBrain()->GetGeographicMap()==NULL))
  { 
    return true;
  }

  CGeoMap* pGeoMap = m_pSelectedEntity->getBrain()->GetGeographicMap();

  Point_t curCoord;
  Point_t dim = theApp.GetBiotop()->getDimension();
  //Point_t startCoord = theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetGridCoordFromScreenPos(CPoint(1,1));
  //int rangeDisplay = 2*(theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetCurrentGridCenterPos().x - startCoord.x);
  Point_t geoMapPos;
  int successWeight = 0;
  DWORD r,g,b;

  // Trace map
  for (i=0; i<dim.x;i++)
  {
    for (j=0; j<dim.y; j++)
    { 
      curCoord.x = i;
      curCoord.y = j;
      pGeoMap->GridCoordToGeoMapCoord(curCoord, geoMapPos);
      successWeight = pGeoMap->GetSuccessWeight(index, geoMapPos);
      if (successWeight>0)
      {
        r=200 - successWeight * 2 / 10; 
        g=240;
        b=200 - successWeight * 2 / 10;
        tBioSquare[curCoord.x][curCoord.y].customColor = (b<<0x10) + (g<<0x08) + r; 
      }
      else if (successWeight<0)
      {
        r=240;
        g=200 + successWeight * 2;
        b=200 + successWeight * 2;
        tBioSquare[curCoord.x][curCoord.y].customColor = (b<<0x10) + (g<<0x08) + r; 
      }
    }
  }

  return (true);
}

bool  CMapConfigView::BuildSensorsMap(int index)
{
  ClearMap();
  if ((m_pSelectedEntity == NULL) || (m_pSelectedEntity->getBrain() == NULL))
  {
    return true;
  }
  m_pBiotop->SetColorizeSearchMode(true);
  sensorValType* pSensorVal = NULL;
  CSensor* pSensor = NULL;
  for (int ind = 0; ind < m_pSelectedEntity->getBrain()->GetNumberSensor(); ind++)
  {
    pSensor = m_pSelectedEntity->getBrain()->GetSensorByIndex(ind);
    if ((index == 0) && (pSensor->GetLabel().find("View") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable(pSensorVal);
    else if ((index == 1) && (pSensor->GetLabel().find("Ear") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable(pSensorVal);
    else if ((index == 2) && (pSensor->GetLabel().find("Smell") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable(pSensorVal);
    else if ((index == 3) && (pSensor->GetLabel().find("Pheromone") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable(pSensorVal);
  }
  m_pBiotop->SetColorizeSearchMode(false);
}

bool CMapConfigView::ClearMap()
{
  int i,j;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  for (i=0;i<m_pBiotop->getDimension().x;i++)
  {
    for (j=0; j<m_pBiotop->getDimension().y; j++)
    {   
      tBioSquare[i][j].customColor = 0x00FFFFFF;  
    }
  }

  theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
  return (true);
}

void CMapConfigView::OnRadioOdor1() 
{
  m_CurMapSubType = ODOR_AROMATIC;
  BuildMap();
}

void CMapConfigView::OnRadioOdor2() 
{
  m_CurMapSubType = ODOR_FRAGRANT;
  BuildMap();	
}

void CMapConfigView::OnRadioOdor3() 
{
  m_CurMapSubType = ODOR_ALLIACEOUS;
  BuildMap();	
}

void CMapConfigView::OnRadioOdor4() 
{
  m_CurMapSubType = ODOR_AMBROSIAL;
  BuildMap();
}

void CMapConfigView::OnRadioOdor5() 
{
  m_CurMapSubType = ODOR_HIRCINE;
  BuildMap();	
}

void CMapConfigView::OnRadioOdor6() 
{
  m_CurMapSubType = ODOR_REPULSIVE;
  BuildMap();	
}

void CMapConfigView::OnRadioOdor7() 
{
  m_CurMapSubType = ODOR_NAUSEOUS;
  BuildMap();
}

void CMapConfigView::OnCheck1() 
{
  UpdateData(TRUE);
  OnRadioOdor1();
  if (m_bIsOdorMap)
  {
    m_CurMapType = MAP_TYPE_ODOR;
    BuildMap();
  }
  else
  {
    m_CurMapType = MAP_TYPE_NONE; 
    ClearMap();
  }
  m_bIsTemperatureMap = FALSE;
  m_bIsPurposeMap = FALSE;
  m_bIsSensorMap = FALSE;
  UpdateData(FALSE);
}

void CMapConfigView::OnCheck2() 
{
  UpdateData(TRUE);
  if (m_bIsTemperatureMap)
  {
    m_CurMapType = MAP_TYPE_TEMPERATURE;
    BuildMap();
  }
  else
  {
    m_CurMapType = MAP_TYPE_NONE; 
    ClearMap();
  }
  m_bIsOdorMap = FALSE;
  m_bIsPurposeMap = FALSE;
  m_bIsSensorMap = FALSE;
  UpdateData(FALSE);
}

void CMapConfigView::OnCheck3() 
{
  UpdateData(TRUE);
  OnRadioPurpose1();
  if (m_bIsPurposeMap)
  {
    m_CurMapType = MAP_TYPE_PURPOSE;
    BuildMap();
  }
  else
  {
    m_CurMapType = MAP_TYPE_NONE; 
    ClearMap();
  }
  m_bIsOdorMap = FALSE;
  m_bIsTemperatureMap = FALSE;
  m_bIsSensorMap = FALSE;
  UpdateData(FALSE);	
}

void CMapConfigView::OnCheck4()
{
  UpdateData(TRUE);
  OnRadioSensor1();
  if (m_bIsSensorMap)
  {
    m_CurMapType = MAP_TYPE_SENSORS;
    BuildMap();
  }
  else
  {
    m_CurMapType = MAP_TYPE_NONE;
    ClearMap();
  }
  m_bIsOdorMap = FALSE;
  m_bIsTemperatureMap = FALSE;
  m_bIsPurposeMap = FALSE;
  UpdateData(FALSE);
}

void CMapConfigView::OnRadioPurpose1() 
{
  m_CurMapSubType = 0;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose2() 
{
  m_CurMapSubType = 1;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose3() 
{
  m_CurMapSubType = 2;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose4() 
{
  m_CurMapSubType = 3;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose5() 
{
  m_CurMapSubType = 4;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose6() 
{
  m_CurMapSubType = 5;
  BuildMap();
}

void CMapConfigView::OnRadioPurpose7() 
{
  m_CurMapSubType = 6;
  BuildMap();
}

void CMapConfigView::OnRadioSensor1()
{
  m_CurMapSubType = 0;
  BuildMap();
}

void CMapConfigView::OnRadioSensor2()
{
  m_CurMapSubType = 1;
  BuildMap();
}

void CMapConfigView::OnRadioSensor3()
{
  m_CurMapSubType = 2;
  BuildMap();
}

void CMapConfigView::OnRadioSensor4()
{
  m_CurMapSubType = 3;
  BuildMap();
}