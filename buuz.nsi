# vim: set et ts=2 sw=2:

!define VERSION "0.9.3"

!define BIN_DIR_X86 ".\objfre_w2k_x86\i386"
!define BIN_DIR_X64 ".\objfre_wnet_amd64\amd64"

!define REG_BUUZ "Software\Buuz"
!define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\Buuz"

Name "Buuz v${VERSION}"
OutFile "Buuz_v${VERSION}.exe"

InstallDir "$PROGRAMFILES\Buuz"
InstallDirRegKey HKLM ${REG_BUUZ} ""

# Request admin privileges
RequestExecutionLevel admin

# Included Headers
!include "MUI2.nsh"
!include "Library.nsh"
!include "LogicLib.nsh"
!include "Memento.nsh"
!include "WinVer.nsh"
!include "WordFunc.nsh"

# Memento Settings
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY ${REG_UNINSTALL}

# Interface Settings
!define MUI_ABORTWARNING

# Remember the user's language choice
!define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
!define MUI_LANGDLL_REGISTRY_KEY ${REG_BUUZ}
!define MUI_LANGDLL_REGISTRY_VALUENAME "InstallerLanguage"

# Installer Pages
!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

# Uninstaller Pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

# Languages
!insertmacro MUI_LANGUAGE "English"

#
# Init Funtions
#

Function .onInit

  ${IfNot} ${IsWin2000}
  ${AndIfNot} ${IsWinXP}
  ${AndIfNot} ${IsWin2003}
    MessageBox MB_OK|MB_ICONSTOP "Sorry, Buuz ${VERSION} works only on the following Windows versions: Windows 2000, Windows XP, Windows 2003"
    Abort
  ${EndIf}

  ClearErrors
  UserInfo::GetAccountType
  ${IfNot} ${Errors}
    Pop $1
    ${If} $1 != "Admin"
      MessageBox MB_OK|MB_ICONSTOP "You must have Administrator privileges to install Buuz."
      Abort
    ${EndIf}
  ${EndIf}

  ReadRegStr $R0 HKLM ${REG_BUUZ} "Version"
  ${IfNot} ${Errors}
    ${VersionCompare} ${VERSION} $R0 $R0
    ${If} $R0 == 2
      MessageBox MB_OK|MB_ICONEXCLAMATION "A newer version of Buuz is already installed! It is not recommended that you install an older version. If you really want to install this older version, You must uninstall the current version first."
      Abort
    ${EndIf}
  ${EndIf}
  
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "NSIS_Buuz") i .r1 ?e'
  Pop $R0
  ${If} $R0 <> 0
    MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
    Abort
  ${EndIf}

  ${MementoSectionRestore}

FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}

FunctionEnd


#
# Installer
#

Function InstallIme

  StrCpy $R9 0

  !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED ${BIN_DIR_X86}\buuz.ime $SYSDIR\buuz.ime $SYSDIR

  ${If} ${RunningX64}
    !define LIBRARY_X64
    !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED ${BIN_DIR_X64}\buuz.ime $SYSDIR\buuz.ime $SYSDIR
    !undef LIBRARY_X64
  ${Endif}

  WriteRegStr HKLM "System\CurrentControlSet\Control\Keyboard Layouts\E0800450" "IME File" "buuz.ime"
  WriteRegStr HKLM "System\CurrentControlSet\Control\Keyboard Layouts\E0800450" "Layout Display Name" "Buuz (Mongolian)"
  WriteRegStr HKLM "System\CurrentControlSet\Control\Keyboard Layouts\E0800450" "Layout File" "kbdus.dll"
  WriteRegStr HKLM "System\CurrentControlSet\Control\Keyboard Layouts\E0800450" "Layout Text" "Buuz (Mongolian)"

  System::Call "kernel32::LoadLibraryA(t 'input.dll') i .r0"
  ${If} $0 <> 0
    System::Call "kernel32::GetProcAddress(i r0., i 102) i .r1"
    ${If} $1 <> 0
      System::Call "kernel32::GetProcAddress(i r0., i 103) i .r2"
      ${If} $2 <> 0
        System::Call "::$1(i 0x0450, i 0xE0800450, i 0, i 0, i 0, i 0) i .."
        System::Call "::$1(i 0x0450, i 0xE0800450, i 0, i 0, i 1, i 0) i .."
        System::Call "::$2(i 0x0450, i 0x00000450, i 0) i .."
        System::Call "::$2(i 0x0450, i 0x00000450, i 1) i .."
      ${EndIf}
    ${EndIf}
    System::Call "kernel32::FreeLibrary(i r0.) i .."
  ${EndIf}

  System::Call "user32::LoadKeyboardLayoutA(t '00000450', i 0) i .r0"
  ${If} $0 <> 0
    System::Call "user32::UnloadKeyboardLayout(i r0.) i .."
  ${EndIf}

FunctionEnd

${MementoSection} "Buuz (required)" SecBuuz
  
  SectionIn RO

  SetOutPath "$INSTDIR"
  File "license.txt"

  SetOutPath "$INSTDIR\docs"
  File "docs\configure.html"
  File "docs\configure1.png"
  File "docs\configure2.png"
  File "docs\configure3.png"
  File "docs\style.css"
  File "docs\table.html"

  Call InstallIme
  AddSize 300
  
  WriteRegStr HKLM ${REG_BUUZ} "" "$INSTDIR"
  WriteRegStr HKLM ${REG_BUUZ} "Version" ${VERSION}

  WriteUninstaller "$INSTDIR\uninstall.exe"

  WriteRegExpandStr HKLM ${REG_UNINSTALL} "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegExpandStr HKLM ${REG_UNINSTALL} "InstallLocation" "$INSTDIR"
  WriteRegStr       HKLM ${REG_UNINSTALL} "DisplayName"     "Buuz ${VERSION}"
  WriteRegStr       HKLM ${REG_UNINSTALL} "DisplayVersion"  "${VERSION}"
  WriteRegStr       HKLM ${REG_UNINSTALL} "URLInfoAbout"    "http://buuz.sourceforge.net/"
  WriteRegStr       HKLM ${REG_UNINSTALL} "HelpLink"        "http://buuz.sourceforge.net/"
  WriteRegDWORD     HKLM ${REG_UNINSTALL} "NoModify"        "1"
  WriteRegDWORD     HKLM ${REG_UNINSTALL} "NoRepair"        "1"

${MementoSectionEnd}

${MementoSection} "Start Menu Group" SecStartMenu

    CreateDirectory "$SMPROGRAMS\Buuz"
    CreateShortCut "$SMPROGRAMS\Buuz\Conversion Table.lnk" "$INSTDIR\docs\table.html"
    CreateShortCut "$SMPROGRAMS\Buuz\Configuration Guide.lnk" "$INSTDIR\docs\configure.html"
    CreateShortCut "$SMPROGRAMS\Buuz\License.lnk" "$INSTDIR\license.txt"
    CreateShortCut "$SMPROGRAMS\Buuz\Uninstall.lnk" "$INSTDIR\uninstall.exe"

${MementoSectionEnd}

${MementoSectionDone}

#
# Uninstaller
#

Function un.RemoveIme
 
  System::Call "kernel32::LoadLibraryA(t 'input.dll') i .r0"
  ${If} $0 <> 0
    System::Call "kernel32::GetProcAddress(i r0., i 102) i .r1"
    ${If} $1 <> 0
      System::Call "kernel32::GetProcAddress(i r0., i 103) i .r2"
      ${If} $2 <> 0
        System::Call "::$1(i 0x0450, i 0x00000450, i 0, i 0, i 0, i 0) i .r0"
        System::Call "::$1(i 0x0450, i 0x00000450, i 0, i 0, i 1, i 0) i .r0"
        System::Call "::$2(i 0x0450, i 0xE0800450, i 0) i .r0"
        System::Call "::$2(i 0x0450, i 0xE0800450, i 1) i .r0"
      ${EndIf}
    ${EndIf}
  ${EndIf}

  System::Call "user32::LoadKeyboardLayoutA(t 'E0800450', i 0) i .r0"
  ${If} $0 <> 0
    System::Call "user32::UnloadKeyboardLayout(i r0.) i .."
  ${EndIf}

  DeleteRegKey HKLM "System\CurrentControlSet\Control\Keyboard Layouts\E0800450"

  !insertmacro UninstallLib DLL NOTSHARED REBOOT_NOTPROTECTED $SYSDIR\buuz.ime

  ${If} ${RunningX64}
    !define LIBRARY_X64
    !insertmacro UninstallLib DLL NOTSHARED REBOOT_NOTPROTECTED $SYSDIR\buuz.ime
    !undef LIBRARY_X64
  ${Endif}

FunctionEnd

Function un.RemoveStartMenu

  Delete "$SMPROGRAMS\Buuz\Conversion Table.lnk"
  Delete "$SMPROGRAMS\Buuz\Configuration Guide.lnk"
  Delete "$SMPROGRAMS\Buuz\License.lnk"
  Delete "$SMPROGRAMS\Buuz\Uninstall.lnk"
  RMDir "$SMPROGRAMS\Buuz"

FunctionEnd

Section "Uninstall"

  Call un.RemoveIme

  Call un.RemoveStartMenu

  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\uninstall.exe"
  RMDir /r "$INSTDIR\docs"
  RMDir "$INSTDIR"

  DeleteRegKey HKLM ${REG_BUUZ}
  DeleteRegKey HKLM ${REG_UNINSTALL}

SectionEnd
