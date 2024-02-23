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
#include "CGeoMapPurpose.h"

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
  m_bIsSensorMap = FALSE;
  m_bIsPopulationMap = FALSE;
  m_bIsFertilityMap = FALSE;

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
  m_populationTxt1 = _T("");
  m_populationTxt2 = _T("");
  m_populationTxt3 = _T("");
  m_populationTxt4 = _T("");
  m_populationTxt5 = _T("");
  m_populationTxt6 = _T("");
  m_populationDateTxt = _T("");
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
  DDX_Check(pDX, IDC_CHECK5, m_bIsPopulationMap);
  DDX_Check(pDX, IDC_CHECK6, m_bIsFertilityMap);
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
  DDX_Text(pDX, IDC_POPULATION_TXT1, m_populationTxt1);
  DDX_Text(pDX, IDC_POPULATION_TXT2, m_populationTxt2);
  DDX_Text(pDX, IDC_POPULATION_TXT3, m_populationTxt3);
  DDX_Text(pDX, IDC_POPULATION_TXT4, m_populationTxt4);
  DDX_Text(pDX, IDC_POPULATION_TXT5, m_populationTxt5);
  DDX_Text(pDX, IDC_POPULATION_TXT6, m_populationTxt6);
  DDX_Text(pDX, IDC_POPULATION_DATE, m_populationDateTxt);
  DDX_Control(pDX, IDC_SLIDER_M1, m_SliderM1);
  DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
  DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
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
  ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
  ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
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
  ON_BN_CLICKED(IDC_RADIO_POPULATION1, OnRadioPopulation1)
  ON_BN_CLICKED(IDC_RADIO_POPULATION2, OnRadioPopulation2)
  ON_BN_CLICKED(IDC_RADIO_POPULATION3, OnRadioPopulation3)
  ON_BN_CLICKED(IDC_RADIO_POPULATION4, OnRadioPopulation4)
  ON_BN_CLICKED(IDC_RADIO_POPULATION5, OnRadioPopulation5)
  ON_BN_CLICKED(IDC_RADIO_POPULATION6, OnRadioPopulation6)
  ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_M1, OnReleasedcaptureSliderM1)
  ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoadSpecieMap)
  ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSaveSpecieMap)
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

  HICON saveIco = theApp.LoadIcon(IDI_ICON_SAVE);
  HICON loadIco = theApp.LoadIcon(IDI_ICON_OPEN);
  m_ButtonSave.SetIcon(saveIco);
  m_ButtonLoad.SetIcon(loadIco);

  return (true);
}

void CMapConfigView::RefreshDisplay()
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
    case MAP_TYPE_POPULATION:
      if (forceRefresh)
      {
        resu = BuildPopulationMap(m_CurMapSubType);
        theApp.GetBiotopViewPtr()->ForceRefreshDisplay();
      }
      break;
    case MAP_TYPE_FERTILITY:
      resu = BuildFertilityMap();
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
  int i,j;
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
      m_pBiotop->getOdorLevels(coord, 6, odorLevel);
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

bool CMapConfigView::BuildFertilityMap()
{
  int i, j;
  double fertility;
  DWORD r, g, b;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  Point_t curCoord;
  Point_t startCoord = theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetGridCoordFromScreenPos(CPoint(1, 1));

  int rangeDisplay = 2 * (theApp.GetBiotopViewPtr()->GetpBiotopDisplay()->GetCurrentGridCenterPos().x - startCoord.x);

  // Trace temperature map
  for (i = 0; i < rangeDisplay; i++)
  {
    for (j = 0; j < rangeDisplay; j++)
    {
      curCoord.x = startCoord.x + i;
      curCoord.y = startCoord.y - j;
      if (m_pBiotop->isCoordValid(curCoord, 2))
      {
        fertility = m_pBiotop->getFertility(curCoord);
        if (fertility < 15)
        {
          r = 250; g = 80 + fertility * 10; b = 10;
        }
        else if (fertility < 30)
        {
          r = 300 - 5 * fertility; g = 200 + fertility; b = 10;
        }
        else
        {
          r = 10; g = 300 - 2 * fertility; b = 10 + 2 * fertility;
        }

        tBioSquare[curCoord.x][curCoord.y].customColor = (b << 0x10) + (g << 0x08) + r;
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

  if ((pEntity==NULL) || (pEntity->getBrain()==NULL))
  { 
    if (m_bIsPurposeMap)
      ClearMap();
    m_pSelectedEntity = NULL;
    UpdateData(FALSE);
    return true;
  }

  CGeoMapPurpose* pGeoMap = pEntity->getBrain()->GetGeographicMap();

  // Update button label if necessary
  if ((m_pSelectedEntity != pEntity) && (pGeoMap != NULL))
  {
    CPurpose* pPurpose;

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(0));
    if (pPurpose != NULL)
      m_purposeTxt1 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(1));
    if (pPurpose != NULL)
      m_purposeTxt2 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(2));
    if (pPurpose != NULL)
      m_purposeTxt3 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(3));
    if (pPurpose != NULL)
      m_purposeTxt4 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(4));
    if (pPurpose != NULL)
      m_purposeTxt5 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(5));
    if (pPurpose != NULL)
      m_purposeTxt6 = pPurpose->GetLabel().c_str();

    pPurpose = pEntity->getBrain()->GetPurposeByUniqueId(pGeoMap->GettPurposeUniqueId(6));
    if (pPurpose != NULL)
      m_purposeTxt7 = pPurpose->GetLabel().c_str();
  }
  else if (m_CurMapType == MAP_TYPE_PURPOSE)
  {
    ClearMap();
  }

  m_sensorTxt1 = "View";
  m_sensorTxt2 = "Ear";
  m_sensorTxt3 = "Smell";
  m_sensorTxt4 = "Pheromons";

  m_pSelectedEntity = pEntity;
  UpdateData(FALSE);

  if((m_bIsPurposeMap) || (m_bIsSensorMap))
    BuildMap(true);

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

  CGeoMapPurpose* pGeoMap = m_pSelectedEntity->getBrain()->GetGeographicMap();

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
  if ((m_pSelectedEntity == NULL) || (m_pSelectedEntity->isToBeRemoved()) || (m_pSelectedEntity->getBrain() == NULL))
  {
    return true;
  }
  m_pBiotop->SetColorizeSearchMode(true);
  CSensor* pSensor = NULL;
  for (int ind = 0; ind < m_pSelectedEntity->getBrain()->GetNumberSensor(); ind++)
  {
    pSensor = m_pSelectedEntity->getBrain()->GetSensorByIndex(ind);
    if ((index == 0) && (pSensor->GetLabel().find("View") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable();
    else if ((index == 1) && (pSensor->GetLabel().find("Ear") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable();
    else if ((index == 2) && (pSensor->GetLabel().find("Smell") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable();
    else if ((index == 3) && (pSensor->GetLabel().find("Pheromone") != std::string::npos))
      pSensor->UpdateAndGetStimulationTable();
  }
  m_pBiotop->SetColorizeSearchMode(false);
  return true;
}

bool CMapConfigView::BuildPopulationMap(int index)
{
  int i, j;
  BiotopSquare_t** tBioSquare;
  tBioSquare = m_pBiotop->getpBioSquare();

  ClearMap();
  if ((m_pBiotop->getNbOfGeoMapSpecie() == 0) || (m_CurMapSubType > m_pBiotop->getNbOfGeoMapSpecie()))
  {
    return true;
  }

  CGeoMapPopulation* pGeoMap = m_pBiotop->getGeoMapSpecieByIndex(index);
  if (pGeoMap == NULL)
    return false;

  Point_t curCoord;
  Point_t dim = theApp.GetBiotop()->getDimension();
  Point_t geoMapPos;
  int populationSize = 0;
  DWORD r, g, b;
  size_t day = m_SliderM1.GetPos();

  // Trace map
  for (i = 0; i < dim.x; i++)
  {
    for (j = 0; j < dim.y; j++)
    {
      curCoord.x = i;
      curCoord.y = j;
      pGeoMap->GridCoordToGeoMapCoord(curCoord, geoMapPos);
      populationSize = pGeoMap->GetPopulationInSquareMap(day, geoMapPos);

      if (populationSize > 0)
      {
        if (populationSize < 10)
        {
          r = 200 - populationSize * 20;
          g = 240;
          b = 200 - populationSize * 20;
        }
        else
        {
          r = 0;
          g = 240;
          b = 0;
        }
        tBioSquare[curCoord.x][curCoord.y].customColor = (b << 0x10) + (g << 0x08) + r;
      }
    }
  }
  return (true);
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
  m_bIsPopulationMap = FALSE;
  m_bIsFertilityMap = FALSE;
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
  m_bIsPopulationMap = FALSE;
  m_bIsFertilityMap = FALSE;
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
  m_bIsPopulationMap = FALSE;
  m_bIsFertilityMap = FALSE;
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
  m_bIsPopulationMap = FALSE;
  m_bIsFertilityMap = FALSE;
  UpdateData(FALSE);
}

void CMapConfigView::OnCheck5()
{
  UpdateData(TRUE);
  OnRadioPopulation1();

  m_SliderM1.SetRange(0, 364);
  m_SliderM1.SetPos(0);
  m_SliderM1.SetPageSize(1);

  size_t nbSpecieMap = m_pBiotop->getNbOfGeoMapSpecie();
  if (nbSpecieMap > 0)
    m_populationTxt1 = m_pBiotop->getGeoMapSpecieByIndex(0)->GetSpecieName().c_str();
  if (nbSpecieMap > 1)
    m_populationTxt2 = m_pBiotop->getGeoMapSpecieByIndex(1)->GetSpecieName().c_str();
  if (nbSpecieMap > 2)
    m_populationTxt3 = m_pBiotop->getGeoMapSpecieByIndex(2)->GetSpecieName().c_str();
  if (nbSpecieMap > 3)
    m_populationTxt4 = m_pBiotop->getGeoMapSpecieByIndex(3)->GetSpecieName().c_str();
  if (nbSpecieMap > 4)
    m_populationTxt5 = m_pBiotop->getGeoMapSpecieByIndex(4)->GetSpecieName().c_str();
  if (nbSpecieMap > 5)
    m_populationTxt6 = m_pBiotop->getGeoMapSpecieByIndex(5)->GetSpecieName().c_str();

  if (m_bIsPopulationMap)
  {
    m_CurMapType = MAP_TYPE_POPULATION;
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
  m_bIsSensorMap = FALSE;
  m_bIsFertilityMap = FALSE;
  UpdateData(FALSE);
}

void CMapConfigView::OnCheck6()
{
  UpdateData(TRUE);
  if (m_bIsFertilityMap)
  {
    m_CurMapType = MAP_TYPE_FERTILITY;
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
  m_bIsPopulationMap = FALSE;
  m_bIsTemperatureMap = FALSE;
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


void CMapConfigView::OnRadioPopulation1()
{
  m_CurMapSubType = 0;
  BuildMap();
}

void CMapConfigView::OnRadioPopulation2()
{
  m_CurMapSubType = 1;
  BuildMap();
}

void CMapConfigView::OnRadioPopulation3()
{
  m_CurMapSubType = 2;
  BuildMap();
}

void CMapConfigView::OnRadioPopulation4()
{
  m_CurMapSubType = 3;
  BuildMap();
}

void CMapConfigView::OnRadioPopulation5()
{
  m_CurMapSubType = 4;
  BuildMap();
}

void CMapConfigView::OnRadioPopulation6()
{
  m_CurMapSubType = 5;
  BuildMap();
}

void CMapConfigView::OnReleasedcaptureSliderM1(NMHDR* pNMHDR, LRESULT* pResult)
{
  m_populationDateTxt.Format("Day %d", m_SliderM1.GetPos());
  UpdateData(FALSE);
  BuildMap();
  *pResult = 0;
}

void CMapConfigView::OnButtonLoadSpecieMap()
{
  // Open module
  CString fileName;
  CFileDialog fileDlg(true, LPCTSTR("xml"), LPCTSTR(""), 0, LPCTSTR("Records Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select records");
  long nResp = fileDlg.DoModal();
  if (nResp == IDOK)
  {
    fileName = fileDlg.GetPathName();
    size_t nbSpecieMap = CGeoMapPopulation::getNumberSpeciesStoredInFile(fileName.GetBuffer(0));
    for (int indexRecord = 0; indexRecord < nbSpecieMap; indexRecord++)
    {
      m_pBiotop->addGeoMapSpeciePopulation("Unset");
      CGeoMapPopulation* pGeoMapPopu = m_pBiotop->getGeoMapSpecieByIndex(m_pBiotop->getNbOfGeoMapSpecie() - 1);
      if (pGeoMapPopu != NULL)
      {
        pGeoMapPopu->loadFromXmlFile(fileName.GetBuffer(0), indexRecord);
      }
    }
    OnCheck5();
  }
}

void CMapConfigView::OnButtonSaveSpecieMap()
{
  // Open module
  CString fileName;
  CFileDialog fileDlg(false, LPCTSTR("xml"), LPCTSTR(""), 0, LPCTSTR("Records Files (*.xml)|*.xml; *.xml|All Files (*.*)|*.*||"));
  fileDlg.m_ofn.lpstrTitle = LPCTSTR("Select records");
  long nResp = fileDlg.DoModal();
  if (nResp == IDOK)
  {
    fileName = fileDlg.GetPathName();
    m_pBiotop->saveAllGeoMapsInFile(fileName.GetBuffer(0));
  }
}