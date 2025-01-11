# Microsoft Developer Studio Project File - Name="stretch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=stretch - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "stretch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "stretch.mak" CFG="stretch - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "stretch - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "stretch - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "stretch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\include" /I "./" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib dinput.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "stretch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "./" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib dinput.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "stretch - Win32 Release"
# Name "stretch - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stretch.cpp
# End Source File
# Begin Source File

SOURCE=.\stretch.rc
# End Source File
# Begin Source File

SOURCE=.\winjoy.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Data\16x16_sprites.bmp
# End Source File
# Begin Source File

SOURCE=.\Data\32x32_sprites.bmp
# End Source File
# Begin Source File

SOURCE=.\directx.ico
# End Source File
# Begin Source File

SOURCE=.\DONUT.BMP
# End Source File
# Begin Source File

SOURCE=.\Data\MapTiles.bmp
# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\GCam.c
# End Source File
# Begin Source File

SOURCE=.\Graphics\GCam.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\GFont.c
# End Source File
# Begin Source File

SOURCE=.\Graphics\GFont.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\GMap.c
# End Source File
# Begin Source File

SOURCE=.\Graphics\GMap.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\graphics.h
# End Source File
# End Group
# Begin Group "Levels"

# PROP Default_Filter ""
# Begin Group "Level0"

# PROP Default_Filter ""
# Begin Group "L0_Entities"

# PROP Default_Filter ""
# Begin Group "L0_MapEnt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_MapEnt\L0_SmallWall.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_MapEnt\L0_SmallWall.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Bacterias.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Bacterias.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Bouncer.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Bouncer.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_CommonBlast.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_CommonBlast.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Cross.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Cross.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Entities.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Estrellita.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Estrellita.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Sphere.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Sphere.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Sphere2.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Sphere2.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Turret.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Turret.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Wave.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_Wave.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_World.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_World.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_ZigZag.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\L0_Entities\L0_ZigZag.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Levels\Level0\Level0.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Level0\Level0.h
# End Source File
# End Group
# Begin Group "Entities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Levels\Entities\Blaster.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Blaster.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\CommonEntity.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\CommonEntity.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Entity.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Entity.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Explosion.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Explosion.h
# End Source File
# Begin Source File

SOURCE=.\Levels\Level.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Player.c
# End Source File
# Begin Source File

SOURCE=.\Levels\Entities\Player.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Levels\Level.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Input\Input.c
# End Source File
# Begin Source File

SOURCE=.\Input\Input.h
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "HUD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Game\HUD\HUD.c
# End Source File
# Begin Source File

SOURCE=.\Game\HUD\HUD.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Game\Game.c
# End Source File
# Begin Source File

SOURCE=.\Game\Game.h
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\Common\Common.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "LevelData"

# PROP Default_Filter ""
# Begin Group "Level0Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Sect0.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Sect1.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Sect2.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Sect3.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Sect4.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Data\Levels\Level0\MAP0_Tiles.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Group
# Begin Group "DemoData"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\Demo\demo.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Data\Ship.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utils\FMath.c
# End Source File
# Begin Source File

SOURCE=.\Utils\FMath.h
# End Source File
# End Group
# Begin Group "Menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Menu\Menu.c
# End Source File
# Begin Source File

SOURCE=.\Menu\Menu.h
# End Source File
# End Group
# Begin Group "Demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Demo\Demo.c
# End Source File
# Begin Source File

SOURCE=.\Demo\Demo.h
# End Source File
# End Group
# End Target
# End Project
