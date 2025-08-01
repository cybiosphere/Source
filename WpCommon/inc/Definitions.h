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
// FILE: definitions.h
//   
// GENERAL DESCRIPTION:
//         Contains all shared definitions
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2005 : Creation
//
//===========================================================================

#if !defined( DEFINITIONS__INCLUDED_)
#define DEFINITIONS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning( disable: 4251 )
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <cassert>
#include <fstream>
#include <limits>
#include <stdint.h>
#include <iostream>
#ifdef WIN32
#include <direct.h> // _getcwd
#else
#include <unistd.h> // _getcwd
#endif

using namespace std ;

#ifdef WIN32
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
#else
typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value
#endif

#ifndef ASSERT
#define ASSERT assert
#endif

typedef unsigned int entitySignatureType;
typedef long int entityIdType;

#define CYBIO_PI 3.1415926535

// WIN32 Dll interface definitions
#ifdef DLL_CYBIOCORE_DLL
  #define DLL_CYBIOCORE_API  __declspec(dllexport)   // export DLL information
#else // DLL_CYBIOCORE_DLL
  #ifdef DLL_CYBIOCORE_CLIENT
     #define DLL_CYBIOCORE_API  __declspec(dllimport)   // import DLL information
  #else // DLL_CYBIOCORE_CLIENT
     #define DLL_CYBIOCORE_API
  #endif // DLL_CYBIOCORE_CLIENT
#endif // DLL_CYBIOCORE_DLL

#ifdef DLL_DEFLATOR_DLL
  #define DLL_DEFLATOR_API  __declspec(dllexport)   // export DLL information
#else // DLL_DEFLATOR_DLL
  #ifdef DLL_DEFLATOR_CLIENT
     #define DLL_DEFLATOR_API  __declspec(dllimport)   // import DLL information
  #else // DLL_DEFLATOR_CLIENT
     #define DLL_DEFLATOR_API
  #endif // DLL_DEFLATOR_CLIENT
#endif // DLL_DEFLATOR_DLL


#define NUMBER_SECONDS_PER_MINUTE 60
#define NUMBER_SECONDS_PER_HOUR 3600
#define NUMBER_HOURS_PER_DAY 24
#define NUMBER_DAYS_PER_YEAR 365
#define NUMBER_SECONDS_PER_DAY (NUMBER_SECONDS_PER_HOUR * NUMBER_HOURS_PER_DAY)
#define DEFAULT_SEASON_PERIOD_IN_DAYS 36

typedef struct 
{
  int seconds;
  int hours;
  int days;
  int years;
} BiotopTime_t;


typedef struct 
{
  size_t x;
  size_t y;
} Point_t;

constexpr size_t invalidCoord{ std::string::npos };
constexpr size_t invalidIndex{ std::string::npos };
constexpr char invalidCharIndex{ -1 };

typedef struct
{
  int x;
  int y;
} RelativePos_t;

typedef DWORD   COLORREF;
typedef unsigned char UCHAR;
typedef unsigned long  timeCountType;
#define MAX_TIMECOUNT_VALUE 0xFFFFFFFF

template<typename A, typename B> inline A cybio_min(A a, B b) { return a < b ? a : b; }
template<typename A, typename B> inline A cybio_max(A a, B b) { return a > b ? a : b; }
int  DLL_CYBIOCORE_API cybio_round(double val);
template <class type> type getRandInt(type max)
{
  return ((int)((max + 1.0) * rand() / (RAND_MAX + 1.0)));
};
bool DLL_CYBIOCORE_API testChance (double luckRate);
bool DLL_CYBIOCORE_API testChance (double luckRate1,double luckRate2);

timeCountType DLL_CYBIOCORE_API convertBioTimeToCount(BiotopTime_t bioTime);
BiotopTime_t DLL_CYBIOCORE_API convertCountToBioTime(timeCountType count);

void DLL_CYBIOCORE_API clearWindowsEolIfNeeded(std::string& textToClean);

size_t DLL_CYBIOCORE_API getStringSectionFromFile(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        char*  lpReturnedString,
                        int nSize,
                        string fileNameWithPath
                        );

size_t DLL_CYBIOCORE_API getStringSection(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        char*  lpReturnedString,
                        int nSize,
                        string lpSourceString
                        );

bool DLL_CYBIOCORE_API writeStringSectionToFile(
                        string lpAppName,
                        string lpKeyName,
                        string lpString,
                        string fileNameWithPath
                        );

bool DLL_CYBIOCORE_API writeStringSection(
                        string lpAppName,
                        string lpKeyName,
                        string lpString,
                        string &lpFileString
                        );

string DLL_CYBIOCORE_API FormatString( const char *szFormat, ...);

std::string get_working_path();

#ifndef CYBIOCORE_LOG
#ifdef CYBIOCORE_LOG_IN_FILE
  extern ofstream gLogFile;
  #define CYBIOCORE_LOG_INIT(fileName)   gLogFile.open(fileName); 
  #define CYBIOCORE_LOG_CLOSE  gLogFile.close();  
  #define CYBIOCORE_LOG(...) gLogFile << FormatString(__VA_ARGS__); gLogFile.flush()
  #define CYBIOCORE_LOG_TIME_NOT_AVAILABLE gLogFile << "                 ";
  #define CYBIOCORE_LOG_TIME(bioTime) \
  gLogFile << FormatString("Y%d-D%03d %02d:%02d:%02d ", bioTime.years, bioTime.days, bioTime.hours, \
                           bioTime.seconds / NUMBER_SECONDS_PER_MINUTE, bioTime.seconds % NUMBER_SECONDS_PER_MINUTE);

#else
#ifdef _DEBUG
  #define CYBIOCORE_LOG_INIT(ignore)
  #define CYBIOCORE_LOG_CLOSE
  #define CYBIOCORE_LOG printf
  #define CYBIOCORE_LOG_TIME_NOT_AVAILABLE
  #define CYBIOCORE_LOG_TIME(bioTime) \
  printf ("Y%d-D%03d %02d:%02d:%02d ", bioTime.years, bioTime.days, bioTime.hours, \
          bioTime.seconds % NUMBER_SECONDS_PER_MINUTE, bioTime.seconds / NUMBER_SECONDS_PER_MINUTE);
#else
  #define CYBIOCORE_LOG_INIT(ignore)
  #define CYBIOCORE_LOG_CLOSE
  #define CYBIOCORE_LOG
  #define CYBIOCORE_LOG_TIME_NOT_AVAILABLE
  #define CYBIOCORE_LOG_TIME(ignore)
#endif   /* DEBUG */
#endif  /* CYBIOCORE_LOG_IN_FILE */
#endif /* CYBIOCORE_LOG */

// C/C++ Macro RGB to HSV
// 0 <= Hue  <= 360  
// 0 <= Sat  <= 100  
// 0 <= Val  <= 255  
#define PIX_RGB_TO_HSVL_COMMON(R,G,B,H,S,V,L,NORM)                     \
if((B > G) && (B > R))                                                 \
{                                                                      \
  V = B;                                                               \
  if(V != 0)                                                           \
  {                                                                    \
    double min;                                                        \
    if(R > G) min = G;                                                 \
    else      min = R;                                                 \
    const double delta = V - min;                                      \
    if(delta != 0)                                                     \
      { S = (delta/V); H = 4 + (R - G) / delta; }                      \
    else                                                               \
      { S = 0;         H = 4 + (R - G); }                              \
    H *=   60; if(H < 0) H += 360;                                     \
    if(!NORM) V =  (V/255);                                            \
    else      S *= (100);                                              \
    L = (V + min) / 2;                                                 \
  }                                                                    \
  else                                                                 \
    { S = 0; H = 0; L = V;}                                            \
}                                                                      \
else if(G > R)                                                         \
{                                                                      \
  V = G;                                                               \
  if(V != 0)                                                           \
  {                                                                    \
    double min;                                                        \
    if(R > B) min = B;                                                 \
    else      min = R;                                                 \
    const double delta = V - min;                                      \
    if(delta != 0)                                                     \
      { S = (delta/V); H = 2 + (B - R) / delta; }                      \
    else                                                               \
      { S = 0;         H = 2 + (B - R); }                              \
    H *=   60; if(H < 0) H += 360;                                     \
    if(!NORM) V =  (V/255);                                            \
    else      S *= (100);                                              \
    L = (V + min) / 2;                                                 \
  }                                                                    \
  else                                                                 \
    { S = 0; H = 0; L = V;}                                            \
}                                                                      \
else                                                                   \
{                                                                      \
  V = R;                                                               \
  if(V != 0)                                                           \
  {                                                                    \
    double min;                                                        \
    if(G > B) min = B;                                                 \
    else      min = G;                                                 \
    const double delta = V - min;                                      \
    if(delta != 0)                                                     \
      { S = (delta/V); H = (G - B) / delta; }                          \
    else                                                               \
      { S = 0;         H = (G - B); }                                  \
    H *=   60; if(H < 0) H += 360;                                     \
    if(!NORM) V =  (V/255);                                            \
    else      S *= (100);                                              \
    L = (V + min) / 2;                                                 \
  }                                                                    \
  else                                                                 \
    { S = 0; H = 0; L = V;}                                            \
}

typedef enum
{
  VIEW_ANGLE_45  =     0,
  VIEW_ANGLE_135,     
  VIEW_ANGLE_225, 
  VIEW_ANGLE_315,
  VIEW_ANGLE_360,
} ViewAngleType_e;


#endif // !defined(DEFINITIONS__INCLUDED_)



