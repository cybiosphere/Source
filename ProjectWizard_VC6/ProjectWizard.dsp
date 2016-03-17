# Microsoft Developer Studio Project File - Name="ProjectWizard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ProjectWizard - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizard.mak" CFG="ProjectWizard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProjectWizard - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ProjectWizard - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProjectWizard - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Exec"
# PROP Intermediate_Dir "../Temp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../WpGui/inc" /I "../ProjectWizard" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpBehavior/inc" /I "../WpGenetic/inc" /I "../WpWizardGui/inc" /I "../WpWizardProc/inc" /I "../WpMeasure/inc" /I "./" /I "../WpTinyXml/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\ExportDll_VC6\lib\CybioCore.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ProjectWizard - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Exec"
# PROP Intermediate_Dir "../Temp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../WpGui/inc" /I "../ProjectWizard" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpBehavior/inc" /I "../WpGenetic/inc" /I "../WpWizardGui/inc" /I "../WpWizardProc/inc" /I "../WpMeasure/inc" /I "./" /I "../WpTinyXml/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\ExportDll_VC6\lib\CybioCoreD.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Exec/ProjectWizardD.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ProjectWizard - Win32 Release"
# Name "ProjectWizard - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GenomeIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProjectWizard.ico
# End Source File
# Begin Source File

SOURCE=.\ProjectWizard.rc
# End Source File
# Begin Source File

SOURCE=.\res\ProjectWizard.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ProjectWizardDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\TabsIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "WpBehavior"

# PROP Default_Filter ""
# Begin Group "IncBehavior"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrain.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CBrainAnimal.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CPurpose.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CReaction.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensor.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorSmell.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorTaste.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorView.h
# End Source File
# Begin Source File

SOURCE=..\WpBehavior\inc\CSensorViewFar.h
# End Source File
# End Group
# End Group
# Begin Group "WpEntity"

# PROP Default_Filter ""
# Begin Group "IncEntity"

# PROP Default_Filter ""
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
# End Group
# End Group
# Begin Group "WpGenetic"

# PROP Default_Filter ""
# Begin Group "IncGenetic"

# PROP Default_Filter ""
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
# Begin Group "WpCommon"

# PROP Default_Filter ""
# Begin Group "IncCommon"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE="..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
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
# End Group
# Begin Group "WpWizardGui"

# PROP Default_Filter ""
# Begin Group "IncWizardGui"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpWizardGui\inc\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\DialogTab1.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\DialogTab2.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\DialogTab3.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\DialogTab4.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\FileSelectButton.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\GeneButton.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\ProjectWizard.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\ProjectWizardDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\ProjectWizardView.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\resource.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\inc\WizardTabCtrl.h
# End Source File
# End Group
# Begin Group "SrcWizardGui"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpWizardGui\src\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\DialogTab1.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\DialogTab2.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\DialogTab3.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\DialogTab4.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\FileSelectButton.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\GeneButton.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\ProjectWizard.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\ProjectWizardDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\ProjectWizardView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardGui\src\WizardTabCtrl.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpWizardProc"

# PROP Default_Filter ""
# Begin Group "IncWizardProc"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpWizardProc\inc\CWizardBrain.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardProc\inc\CWizardPurpose.h
# End Source File
# Begin Source File

SOURCE=..\WpWizardProc\inc\CWizardSensor.h
# End Source File
# End Group
# Begin Group "ScrWizardProc"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpWizardProc\src\CWizardBrain.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardProc\src\CWizardPurpose.cpp
# End Source File
# Begin Source File

SOURCE=..\WpWizardProc\src\CWizardSensor.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpGui"

# PROP Default_Filter ""
# Begin Group "IncGui"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpGui\inc\GenomeTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\ParameterSlider.h
# End Source File
# End Group
# Begin Group "SrcGui"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpGui\src\GenomeTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\ParameterSlider.cpp
# End Source File
# End Group
# End Group
# Begin Group "WpEnvironment"

# PROP Default_Filter ""
# Begin Group "IncEnvironment"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\WpEnvironment\inc\CBiotop.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
