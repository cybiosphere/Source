# Microsoft Developer Studio Project File - Name="CybioCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CybioCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CybioCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CybioCore.mak" CFG="CybioCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CybioCore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CybioCore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CybioCore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Exec"
# PROP Intermediate_Dir "../Temp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../ProjectDll/inc" /I "." /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpGenetic/inc" /I "../WpGui/inc" /I "../WpMeasure/inc" /I "../WpTinyXml/inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "DLL_CYBIOCORE_DLL" /D "TIXML_USE_STL" /D "CYBIOCORE_LOG_IN_FILE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                                                          ..\Exec\CybioCore.lib                                                                        ..\ExportDll_VC6\lib\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CybioCore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Exec"
# PROP Intermediate_Dir "../Temp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../WpMeasure/inc" /I "." /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpGenetic/inc" /I "../WpGui/inc" /I "../WpTinyXml/inc" /D "_DEBUG" /D "DLL_CYBIOCORE_DLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "TIXML_USE_STL" /D "CYBIOCORE_LOG_IN_FILE" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /map /debug /machine:I386 /def:".\src\CybioCoreD.def" /out:"../Exec/CybioCoreD.dll" /pdbtype:sept /fixed:no
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                                                          ..\Exec\CybioCoreD.lib                                                                         ..\ExportDll_VC6\lib\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CybioCore - Win32 Release"
# Name "CybioCore - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CybioCore.rc2
# End Source File
# End Group
# Begin Group "ProjectDll"

# PROP Default_Filter ""
# Begin Group "IncDll"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE="..\..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\IOS.H"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "SrcDll"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\src\CybioCore.def

!IF  "$(CFG)" == "CybioCore - Win32 Release"

!ELSEIF  "$(CFG)" == "CybioCore - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "WpBehavior"

# PROP Default_Filter ""
# Begin Group "IncBehavior"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrain.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrainAnimal.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CFeelingFear.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CFeelingWelfare.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CGeoMap.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CNeuronMatrix.h
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

SOURCE=..\WpBehavior\inc\CReactionHide.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionNothing.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionRuminate.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionSleep.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionStepBackward.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnHeadLeft.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnHeadRight.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnLeft.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionTurnRight.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReactionVarySpeed.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensor.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorCompass.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorComposite.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorEar.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorFear.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorHormone.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorHunger.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorLibido.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorMemoryAction.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorMemoryFeedback.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorObscurity.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorOrientation.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorPheromone.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorPleasure.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSmell.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSpeed.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorStatic.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorStomachOverload.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSuffering.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTactile.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTaste.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTemperature.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorThirst.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTiredness.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorView.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorViewFar.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorViewIdentify.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorViewIdentifyFar.h
# End Source File
# End Group
# Begin Group "SrcBehavior"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpBehavior\src\CBrain.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CBrainAnimal.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CFeelingFear.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CFeelingWelfare.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CGeoMap.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CNeuronMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CPurpose.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReaction.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionAttack.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionCopulate.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionDrink.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionEat.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionHide.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionNothing.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionRuminate.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionSleep.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionStepBackward.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionTurnHeadLeft.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionTurnHeadRight.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionTurnLeft.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionTurnRight.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CReactionVarySpeed.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensor.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorCompass.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorComposite.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorEar.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorFear.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorHormone.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorHunger.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorLibido.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorMemoryAction.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorMemoryFeedback.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorObscurity.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorOrientation.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorPheromone.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorPleasure.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorSmell.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorSpeed.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorStomachOverload.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorSuffering.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorTactile.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorTaste.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorTemperature.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorThirst.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorTiredness.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorViewFar.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorViewIdentify.cpp
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\src\CSensorViewIdentifyFar.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpEntity"

# PROP Default_Filter ""
# Begin Group "IncEntity"

# PROP Default_Filter ".h"
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

SOURCE=..\WpEntity\inc\CGrass.h
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
# Begin Group "SrcEntity"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpEntity\src\CAnimal.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CAnimMammal.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CBasicEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CGrass.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CLifeStage.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CMineral.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CPhysicalWelfare.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CVegetable.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CVegetSpermatophyta.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEntity\src\CWater.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpEnvironment"

# PROP Default_Filter ""
# Begin Group "IncEnvironment"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpEnvironment\inc\CBiotop.h
# End Source File
# Begin Source File

SOURCE=..\WpEnvironment\inc\CScenarioPlayer.h
# End Source File
# End Group
# Begin Group "SrcEnvironment"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpEnvironment\src\CBiotop.cpp
# End Source File
# Begin Source File

SOURCE=..\WpEnvironment\src\CScenarioPlayer.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpCommon"

# PROP Default_Filter ""
# Begin Group "IncCommon"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpCommon\inc\CCyclicParam.h
# End Source File
# Begin Source File

SOURCE=..\WpCommon\inc\CGenericCaract.h
# End Source File
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
# Begin Group "SrcCommon"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpCommon\src\CCyclicParam.cpp
# End Source File
# Begin Source File

SOURCE=..\WpCommon\src\CGenericCaract.cpp
# End Source File
# Begin Source File

SOURCE=..\WpCommon\src\CGenericParam.cpp
# End Source File
# Begin Source File

SOURCE=..\WpCommon\src\Definitions.cpp
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
# Begin Group "SrcGenetic"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpGenetic\src\CChromosome.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\src\CGene.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\src\CGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGenetic\src\CPairOfChromosome.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpMeasure"

# PROP Default_Filter ""
# Begin Group "IncMeasure"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasure.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasureBiotopParam.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasureEntityParam.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasureFeeling.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasureParam.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasurePopulation.h
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\inc\CMeasureReaction.h
# End Source File
# End Group
# Begin Group "SrcMeasure"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasure.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasureBiotopParam.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasureEntityParam.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasureFeeling.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasureParam.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasurePopulation.cpp
# End Source File
# Begin Source File

SOURCE=..\WpMeasure\src\CMeasureReaction.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpTinyXml"

# PROP Default_Filter ""
# Begin Group "IncTinyXml"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpTinyXml\inc\CybioXmlDef.h
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\inc\targetver.h
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\inc\tinystr.h
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\inc\tinyxml.h
# End Source File
# End Group
# Begin Group "SrcTinyXml"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpTinyXml\src\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\src\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\src\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\WpTinyXml\src\tinyxmlparser.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
