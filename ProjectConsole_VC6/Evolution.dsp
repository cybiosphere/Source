# Microsoft Developer Studio Project File - Name="Evolution" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Evolution - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Evolution.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Evolution.mak" CFG="Evolution - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Evolution - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Evolution - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Evolution - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../WpGenetic/inc" /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpMeasure/inc" /I "../WpTinyXml/inc" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /D "DLL_CYBIOCORE_CLIENT" /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\ExportDll_VC6\lib\CybioCore.lib /nologo /subsystem:console /machine:I386 /out:"..\Exec/Evolution.exe"

!ELSEIF  "$(CFG)" == "Evolution - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Exec"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../WpGenetic/inc" /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpMeasure/inc" /I "../WpTinyXml/inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /D "DLL_CYBIOCORE_CLIENT" /FR /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\ExportDll_VC6\lib\CybioCoreD.lib /nologo /subsystem:console /incremental:no /map /debug /machine:I386 /out:"..\Exec/EvolutionD.exe" /pdbtype:sept /fixed:no
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Evolution - Win32 Release"
# Name "Evolution - Win32 Debug"
# Begin Group "WpBehavior"

# PROP Default_Filter ""
# Begin Group "IncBehavior"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrain.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrainAnimal.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CFeelingWelfare.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CPurpose.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReaction.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionAttack.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionCopulate.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionDrink.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionEat.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionNothing.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionStepBackward.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionStepForward.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnLeft.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnRight.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensor.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorHunger.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorPleasure.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSmell.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSuffering.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTactile.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorThirst.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorView.h
# End Source File
# End Group
# End Group
# Begin Group "WpEntity"

# PROP Default_Filter ""
# Begin Group "IncEntity"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\WpEntity\inc\CAnimal.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CAnimMammal.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CBasicEntity.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CLifeStage.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CMineral.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CPhysicalWelfare.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CVegetable.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CVegetSpermatophyta.h
# End Source File
# Begin Source File

SOURCE=..\WpEntity\inc\CWater.h
# End Source File
# End Group
# End Group
# Begin Group "WpEnvironment"

# PROP Default_Filter ""
# Begin Group "IncEnvironment"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\WpEnvironment\inc\CBiotop.h
# End Source File
# End Group
# End Group
# Begin Group "WpCommon"

# PROP Default_Filter ""
# Begin Group "IncTools"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\WpCommon\inc\CGenericParam.h
# End Source File
# Begin Source File

SOURCE=..\WpCommon\inc\Definitions.h
# End Source File
# Begin Source File

SOURCE=..\WpCommon\inc\Matrix.h
# End Source File
# End Group
# End Group
# Begin Group "WpGenetic"

# PROP Default_Filter ""
# Begin Group "IncGenetic"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpGenetic\inc\CChromosome.h
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\inc\CGene.h
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\inc\CGenome.h
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\inc\CPairOfChromosome.h
# End Source File
# End Group
# End Group
# Begin Group "ProjectConsole"

# PROP Default_Filter ""
# Begin Group "SrcConsole"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Evolution.cpp
# End Source File
# End Group
# End Group
# Begin Group "Library Files"

# PROP Default_Filter "lib"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
