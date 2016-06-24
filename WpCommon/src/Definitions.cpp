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
// FILE: definitions.cpp
//   
// GENERAL DESCRIPTION:
//         Contains all shared definitions
//     
// (C) COPYRIGHT 2005.  All Rights Reserved.
//
//  26/12/2005 : Creation
//
//===========================================================================

#include "Definitions.h"
#include <stdio.h> 
#include <stdarg.h>
#include <fstream>
#include <memory.h>
#include <string.h>
#include <stdio.h>

#ifdef CYBIOCORE_LOG_IN_FILE
ofstream gLogFile;
#endif  /* CYBIOCORE_LOG_IN_FILE */

int DLL_CYBIOCORE_API round(double val)
{
  if (val==0.0)
    return 0;

  if (val>0)
  {
    if (val-floor(val)>0.5)
      return (int)ceil(val);
    else
      return (int)floor(val);
  }
  else
  {
    if (ceil(val)-val>0.5)
      return (int)floor(val);
    else
      return (int)ceil(val);
  }
}

int DLL_CYBIOCORE_API getRandInt (int max)
{
  return ((int)((max+1.0)*rand()/(RAND_MAX+1.0)));
}

bool DLL_CYBIOCORE_API testChance(double luckRate)
{
  int randNb =   (int) ( 100.0 * rand() / RAND_MAX);          
  if (randNb < luckRate)
    return (true);
  else
    return (false);
}

bool DLL_CYBIOCORE_API testChance (double luckRate1,double luckRate2)
{
  double randNb =   100.0 * 100.0 * rand() / RAND_MAX;          
  if (randNb < luckRate1*luckRate2) 
    return (true);
  else
    return (false);
}

  int seconds;
  int hours;
  int days;
  int years;

timeCountType DLL_CYBIOCORE_API convertBioTimeToCount(BiotopTime_t bioTime)
{
  return ((timeCountType)bioTime.seconds + (timeCountType)bioTime.hours*(timeCountType)3600 
        + (timeCountType)bioTime.days*(timeCountType)86400 + (timeCountType)bioTime.years*(timeCountType)86400*(timeCountType)365);
}

BiotopTime_t DLL_CYBIOCORE_API convertCountToBioTime(timeCountType count)
{
  BiotopTime_t bioTime;
  bioTime.years = count/86400/365;
  count -= bioTime.years*86400*365; 
  bioTime.days = count/86400;
  count -= bioTime.days*86400; 
  bioTime.hours = count/3600;
  count -= bioTime.hours*3600; 
  bioTime.seconds = count;

  return(bioTime);
}

int DLL_CYBIOCORE_API getStringSectionFromFile(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        char*  lpReturnedString,
                        int nSize,
                        string fileNameWithPath
                        )
{
  char* pbuf;
  unsigned long int begin, end;
  long int fLength; 
  ifstream f1;
  bool resu = false;

  f1.open( fileNameWithPath.c_str());
  begin = f1.tellg();
  f1.seekg(0, ios::end);
  end = f1.tellg();
  fLength = (end - begin);
  f1.seekg(0, ios::beg);

  if ( (fLength==0) || (fLength>1000000) )
  {
    lpReturnedString[0] = '\0';
    f1.close();
    return (0);
  }

  pbuf = new char[fLength+1];
  memset(pbuf,0,fLength+1);
  f1.read(pbuf,fLength);
  string fileString = pbuf;
  f1.close();
  delete[] pbuf;

  return ( getStringSection(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,fileString) );
}

int DLL_CYBIOCORE_API getStringSection(
                        string lpAppName,
                        string lpKeyName,
                        string lpDefault,
                        char*  lpReturnedString,
                        int nSize,
                        string lpSourceString
                        )
{
  string foundStr;
  int indexSection,indexStart, indexEnd, lenght;
  indexSection = lpSourceString.find(lpAppName,0);
  indexStart = lpSourceString.find(lpKeyName,indexSection);
  indexEnd = lpSourceString.find("\n",indexStart);

  if ((indexStart<0) || (indexEnd<0))
  {
    lenght = lpDefault.length();
    memcpy(lpReturnedString, lpDefault.c_str(), lenght+1);
    return (lenght);
  }

  foundStr = lpSourceString.substr(indexStart,indexEnd-indexStart);
  indexStart = foundStr.find("=",0);
  lenght = foundStr.length() - indexStart - 1;
  
  if ( (lenght>0) && (lenght<nSize) )
  {
    memcpy(lpReturnedString, (foundStr.substr(foundStr.length()-lenght, lenght)).c_str(), lenght+1);
  }
  else
  {
    lenght = lpDefault.length();
    memcpy(lpReturnedString, lpDefault.c_str(), lenght+1);
  }
  return (lenght);
}


bool DLL_CYBIOCORE_API writeStringSectionToFile(
                        string lpAppName,
                        string lpKeyName,
                        string lpString,
                        string fileNameWithPath
                        )
{
  string fileString = "";
  char* pbuf;
  unsigned long int begin, end;
  long int fLength; 
  ifstream f1;
  bool resu = false;

  f1.open( fileNameWithPath.c_str());
  begin = f1.tellg();
  f1.seekg(0, ios::end);
  end = f1.tellg();
  fLength = (end - begin);
  f1.seekg(0, ios::beg);

  if ( (fLength>0) && (fLength<1000000) )
  {
    pbuf = new char[fLength+1];
    memset(pbuf,0,fLength+1);
    f1.read(pbuf,fLength);
    fileString = pbuf;
    f1.close();
    delete[] pbuf;
  }

  writeStringSection(lpAppName, lpKeyName, lpString, fileString);

  ofstream f2;
  f2.open(fileNameWithPath.c_str());
  //f2.seekp(0, ios::end);
  //f2.write(fileString.c_str(), fileString.length());
  f2 << fileString/*.c_str()*/;
  f2.close();

  return true;
}


bool DLL_CYBIOCORE_API writeStringSection(
                        string lpAppName,
                        string lpKeyName,
                        string lpString,
                        string &lpFileString
                        )
{
  int indexSection,indexStart,indexEnd;
  indexSection = lpFileString.find(lpAppName,0);

  if (indexSection<0)
  {
    // new section
    lpFileString += "[" + lpAppName + "]\n" + lpKeyName + "=" + lpString + "\n";
  }
  else
  {
    indexStart = lpFileString.find(lpKeyName,indexSection);
    if (indexStart<0)
    {
      //new field
      indexStart = lpFileString.find('[',indexSection) - 1; //Go just before next section
      if (indexStart>0)
      {
        lpFileString.insert(indexStart,lpKeyName + "=" + lpString + "\n");
      }
      else
      {
        lpFileString += lpKeyName + "=" + lpString + "\n";
      }
    }
    else
    {
      //overwrite field
      indexEnd = lpFileString.find('\n',indexStart) + 1;
      lpFileString.erase(indexStart,indexEnd-indexStart);
      lpFileString.insert(indexStart,lpKeyName + "=" + lpString + "\n");
    }

  }
  
  return true;
}

string DLL_CYBIOCORE_API FormatString( const char *szFormat, ...) 
{ 
  if(szFormat==NULL) 
    return "";

  std::string strRet;
  int nSize=10000;
  char *szRet= new char[nSize];

  va_list argList;
  va_start(argList, szFormat);
  vsprintf(szRet, szFormat, argList);
  va_end(argList);

  strRet.assign(szRet);
  delete []szRet;

  return strRet; 
} 


bool DLL_CYBIOCORE_API copy_file(char *src_filename, char  *new_filename)
{
  FILE  *ptr_old, *ptr_new;
  int  a;
  
  ptr_old = fopen(src_filename, "rb");
  if(ptr_old == NULL)
    return  false;

  ptr_new = fopen(new_filename, "wb");  
  if(ptr_new == NULL)
  {
    fclose(ptr_old);
    return  false;
  }
  
  while(1)
  {
    a  =  fgetc(ptr_old);
    
    if(!feof(ptr_old))
      fputc(a, ptr_new);
    else
      break;
  }
  
  fclose(ptr_new);
  fclose(ptr_old);
  return  true;
}