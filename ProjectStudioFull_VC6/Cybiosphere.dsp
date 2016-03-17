# Microsoft Developer Studio Project File - Name="Cybiosphere" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Cybiosphere - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Cybiosphere.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cybiosphere.mak" CFG="Cybiosphere - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cybiosphere - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Cybiosphere - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cybiosphere - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./inc" /I "../WpOpenGL/inc" /I "../ExportDll/inc" /I "../ProjectDll/inc" /I "." /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpGenetic/inc" /I "../WpMeasure/inc" /I "../WpGui/inc" /I "../WpTinyXml/inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "TIXML_USE_STL" /D "CYBIOCORE_LOG_IN_FILE" /FR /FD /c
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

!ELSEIF  "$(CFG)" == "Cybiosphere - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Exec"
# PROP Intermediate_Dir "../Temp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./inc" /I "../WpOpenGL/inc" /I "../ExportDll/inc" /I "../ProjectDll/inc" /I "." /I "../WpBehavior/inc" /I "../WpEntity/inc" /I "../WpEnvironment/inc" /I "../WpCommon/inc" /I "../WpGenetic/inc" /I "../WpMeasure/inc" /I "../WpGui/inc" /I "../WpTinyXml/inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "TIXML_USE_STL" /D "CYBIOCORE_LOG_IN_FILE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\ExportDll_VC6\lib\CybioCoreD.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"../Exec/CybiosphereD.exe" /pdbtype:sept /fixed:no
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Cybiosphere - Win32 Release"
# Name "Cybiosphere - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\WpGui\res\anim.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim0.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim1.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim2.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim3.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim4.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim5.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\anim6.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\AttributeIcon.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\BiotopDoc.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cybiosphere.ico
# End Source File
# Begin Source File

SOURCE=.\WpGui\res\Cybiosphere.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\Cybiosphere.ico
# End Source File
# Begin Source File

SOURCE=.\WpGui\res\Cybiosphere.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CybiosphereDoc.ico
# End Source File
# Begin Source File

SOURCE=.\WpGui\res\CybiosphereDoc.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\form_ico.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\FormIcons.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\GenomeIcons.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icoLess.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icoMore.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon_col.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icon_red.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icoOpen.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\icoSave.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\mineral.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\OdorIcon.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\PheromonIcon.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\red.ico
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\SexIcons.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\TasteIcon.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\TemperColors.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\TextureIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\WpGui\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\WpGui\res\veget.bmp
# End Source File
# End Group
# Begin Group "Project2D"

# PROP Default_Filter ""
# Begin Group "Src2D"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Cybiosphere.rc
# End Source File
# End Group
# Begin Group "Inc2D"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File

# End Group
# End Group
# Begin Group "WpGui"

# PROP Default_Filter ""
# Begin Group "IncGui"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpGui\inc\ActionButton.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BioCtrlDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BioCtrlView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BiotopDisplay.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BiotopDisplayMFC.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BiotopDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BiotopEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BiotopView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BrainDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BrainEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BrainGridCtrl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\BrainView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\CellRange.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\ColorButton.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\Cybiosphere.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\EntityCreatorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\EntityDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\EntityEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\EntityFindDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\EntityView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GeneticDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GeneticView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GenomeEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GenomeTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GridCell.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\mainfrm.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\MapConfigDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\MapConfigView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\MeasureEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\MemDC.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\ParameterCrtl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\ParameterSlider.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\SensWeightListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\StatisticDoc.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\StatisticView.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\TitleTip.h
# End Source File
# Begin Source File

SOURCE=..\WpGui\inc\TrainingDlg.h
# End Source File
# End Group
# Begin Group "SrcGui"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpGui\src\ActionButton.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BioCtrlDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BioCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BiotopDisplayMFC.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BiotopDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BiotopEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BiotopView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BrainDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BrainEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\BrainGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\brainview.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\Cybiosphere.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\CybiosphereSet.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\EntityCreatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\EntityDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\EntityEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\EntityFindDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\EntityView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GeneticDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GeneticView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GenomeEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GenomeTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\MapConfigDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\MapConfigView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\MeasureEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\ParameterCrtl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\ParameterSlider.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\SensWeightListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\StatisticDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\StatisticView.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=..\WpGui\src\TrainingDlg.cpp
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
# End Group
# Begin Group "WpOpenGL"

# PROP Default_Filter ""
# Begin Group "IncOpenGL"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\WpOpenGL\inc\AnimatedSprite.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\BiotopDisplayGL.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\CSortedArray.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\Map.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\Sprite.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\StdAfxGL.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\Texture.h
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\inc\Tile.h
# End Source File
# End Group
# Begin Group "SrcOpenGL"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\WpOpenGL\src\AnimatedSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\BiotopDisplayGL.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\CSortedArray.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\Map.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\WpOpenGL\src\Tile.cpp
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
# Begin Group "WpMeasure No. 1"

# PROP Default_Filter ""
# Begin Group "IncMeasure No. 1"

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
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
