/*******************************************************************************
 * 
 *------------------------------------------------------------------------------
 * Revision History
 *------------------------------------------------------------------------------
 * 01/01/2011 | F.Ribert| Creation
 ******************************************************************************/

#if !defined(CYBIO_XML_DEF_INCLUDED_)
#define CYBIO_XML_DEF_INCLUDED_

#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif

#include "tinyxml.h"

#define XML_NODE_BIOTOP       "BIOTOP"
#define XML_NODE_GRID         "GRID"
#define XML_NODE_LAYER        "LAYER"
#define XML_NODE_ENTITIES     "ENTITIES"

#define XML_NODE_ENTITY       "ENTITY"
#define XML_NODE_PARAMETER    "PARAMETER"
#define XML_NODE_CARACTER     "CARACTER"
#define XML_NODE_GESTATION    "GESTATION"
#define XML_NODE_GENOME       "GENOME"
#define XML_NODE_PAIR         "PAIR"
#define XML_NODE_GENE         "GENE"
#define XML_NODE_BRAIN        "BRAIN"
#define XML_NODE_IDENTIFY     "IDENTIFY"
#define XML_NODE_LIFE_STAGES  "LIFE_STAGES"
#define XML_NODE_LIFE_STAGE   "LIFE_STAGE"
#define XML_NODE_ACTION_LEVEL "ACTION_LEVELS"
#define XML_NODE_ACTION       "ACTION"
#define XML_NODE_MEMO_ENTITY  "MEMORIZED_ENTITIES"
#define XML_NODE_MEMO         "MEMO"

#define XML_ATTR_LABEL        "Label"
#define XML_ATTR_GENERATION   "Generation"
#define XML_ATTR_STATUS       "Status"
#define XML_ATTR_LAYER        "Layer"
#define XML_ATTR_DIRECTION    "Direction"
#define XML_ATTR_HOUR_COUNT   "HourCounter" 
#define XML_ATTR_IMMORTAL     "Immortal"
#define XML_ATTR_CURRENT_IND  "CurrentIndex"
#define XML_ATTR_INDEX        "Index"
#define XML_ATTR_DAY_COUNT    "DayCount"
#define XML_ATTR_NAME         "Name"
#define XML_ATTR_VALUE        "Value"
#define XML_ATTR_TOTAL_CHILDS "TotalChildNumber"
#define XML_ATTR_FILE_NAME    "FileName"
#define XML_ATTR_RAW_DATA     "Data"
#define XML_ATTR_COORD_X      "CoordX"
#define XML_ATTR_COORD_Y      "CoordY"

#define XML_ATTR_CLASS_NAME   "ClassName"
#define XML_ATTR_CLASS_ID     "ClassId"
#define XML_ATTR_SPECIE       "Specie"
#define XML_ATTR_DIMORPHISM   "Dimorphism"
#define XML_ATTR_TYPE         "Type"
#define XML_ATTR_GENE_DATA_M  "DataM"
#define XML_ATTR_GENE_DATA_P  "DataP"

#define XML_ATTR_SUCCESS      "Success"
#define XML_ATTR_FRUSTRATION  "Frustration"

#define XML_ATTR_SIGNATURE    "Signature"
#define XML_ATTR_IDENTITY     "Identity"

#define XML_ATTR_SIZE_X       "SizeX"
#define XML_ATTR_SIZE_Y       "SizeY"
#define XML_ATTR_SIZE_LAYER   "SizeLayer"
#define XML_ATTR_BIO_TIME     "BioTime"

#endif // !defined(CYBIO_XML_DEF_INCLUDED_)