;--------------------------------
;
; SpChat.Ru Chat Informer Install Script
; Copyright (C) 2013 SpChat.Ru Project

;--------------------------------
; Include section

    !include "MUI2.nsh"
    !include "LogicLib.nsh"
    !include "WinMessages.nsh"

;--------------------------------
; Custom defines

    !define CHAT_NAME "SpChat.Ru"
    !define PUB_NAME "SpChat.Ru Project"
    !define PUB_WEBPAGE "http://informer.spchat.ru"
    !define PUB_COPYRIGHT "© 2005, 2006, 2013 ${PUB_NAME}"

    !define APP_VERSION "2.0.0"
    !define APP_FILE_VERSION "2.0.0.0"
    !define APP_NAME "${CHAT_NAME} Chat Informer"

    !define EXE_NAME "Chat_Informer.exe"
    !define UNINSTALL_NAME "Uninstall.exe"

    !define RUN_APPLICATION "Запустить ${APP_NAME}"
    !define DELETE_APPLICATION "Удалить ${APP_NAME}"

    !define PROJECT_DIR ".."
    !define SETUP_DIR "${PROJECT_DIR}\setup"
    !define BIN_DIR "${PROJECT_DIR}\bin\release"

    !define REG_PUBKEY "Software\${PUB_NAME}"
    !define REG_APPKEY "Software\${PUB_NAME}\${APP_NAME}"
    !define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

;--------------------------------
; Old version defines

    !define APP_NAME_OLD "${CHAT_NAME} Informer"
    !define REG_APPKEY_OLD "Software\${CHAT_NAME}\Chat_Informer"
    !define REG_UNINSTALL_OLD "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME_OLD}"

;--------------------------------
; General settings

    ; Setup name
    Name "${APP_NAME}"

    ; Custom branding text
    BrandingText "${PUB_NAME}"

    ; Compressor options
    SetCompressor /SOLID /FINAL lzma

    ; Enable Windows XP style
    XPStyle on

    ; Hide "Show Details" button
    ShowInstDetails nevershow
    ShowUninstDetails nevershow

    ; Do not allow install into root folder
    AllowRootDirInstall false

    ; We do not need admin rights to install
    RequestExecutionLevel user

    ; Do not exit after installation finished
    AutoCloseWindow false

    ; Enable compiler optimizations
    SetDataBlockOptimize on

    ; Overwrite files if possible
    SetOverwrite try

    ; The output setup file name
    OutFile "informer_setup_${APP_VERSION}.exe"

    ; The default installation directory
    InstallDir "$PROGRAMFILES\${APP_NAME}"

;--------------------------------
; Interface configuration

    ; Installer and Uninstaller icons
    !define MUI_ICON "${SETUP_DIR}\Installer.ico"
    !define MUI_UNICON "${SETUP_DIR}\Uninstaller.ico"

    ; Wizard has header image
    !define MUI_HEADERIMAGE
    !define MUI_HEADERIMAGE_RIGHT

    ; Installer/Uninstaller header bitmaps
    !define MUI_HEADERIMAGE_BITMAP "${SETUP_DIR}\WizSmallImage.bmp"
    !define MUI_HEADERIMAGE_UNBITMAP "${SETUP_DIR}\WizSmallImage.bmp"

    ; Installer/Uninstaller welcome page bitmaps
    !define MUI_WELCOMEFINISHPAGE_BITMAP "${SETUP_DIR}\WizardImage.bmp"
    !define MUI_UNWELCOMEFINISHPAGE_BITMAP "${SETUP_DIR}\WizardImage.bmp"

    ; Do not close GUI after installation
    !define MUI_FINISHPAGE_NOAUTOCLOSE

    ; "Run application" checkbox after installation
    !define MUI_FINISHPAGE_RUN
    !define MUI_FINISHPAGE_RUN_TEXT "${RUN_APPLICATION}"
    !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchApplication"

    ; Show warning message on setup abort
    !define MUI_ABORTWARNING

    ; Installer pages
    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    ; Uninstaller pages
    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH

    ; Languages
    !insertmacro MUI_LANGUAGE "English"
    !insertmacro MUI_LANGUAGE "Russian"

;--------------------------------
; Version information

    VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${APP_NAME} Client"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${PUB_NAME}"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${APP_NAME} Setup"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${APP_FILE_VERSION}"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "${PUB_COPYRIGHT}"

    VIProductVersion "${APP_FILE_VERSION}"

;--------------------------------
; System plugin function definitions

!define sysGetVersion 'kernel32::GetVersion() i'
!define sysGetModuleFileName 'kernel32::GetModuleFileNameA(i, t, i) i'

;--------------------------------
; Custom macroses

; Uninstalls old application version (pre-2.0.0)
!macro UninstallOldVersion
/*
    Push $R0

    ReadRegStr $R0 HKLM "${REG_UNINSTALL_OLD}" "UninstallString"
    StrCmp $R0 "" done

    MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
    "${APP_NAME} is already installed. $\n$\nClick `OK` to remove the \
    previous version or `Cancel` to cancel this upgrade." \
    IDOK uninst
    Abort
    
    ExecWait '"$INSTDIR\Uninstall.exe /S"'
    
        Pop $R0
*/
!macroend

; Closes running application instance
!macro CloseRunningInstance
/*

*/
!macroend

;--------------------------------
; Callback functions

Function .onInit

    ; Check operating system version
    System::Call "${sysGetVersion} () .r0"

    ${If} $0 < 0x80000000
        MessageBox MB_OK|MB_ICONSTOP "Приложение ${APP_NAME} не может быть установлено на Windows 95/98/ME!"
        Abort
    ${EndIf}

    ; Get installation directory from registry if exists

    ReadRegStr $0 HKLM "${REG_APPKEY}" "InstallDir"

    ${If} $0 != ""
        StrCpy $INSTDIR $0
    ${EndIf}

FunctionEnd

; If installation is silent run installed
; application and delete the installer
Function .OnInstSuccess

    IfSilent 0 NotSilent

    ; Run installed application
    Exec "$INSTDIR\${EXE_NAME}"

    System::Alloc 1024
    Pop $0

    ; Get installer executable filename
    System::Call "${sysGetModuleFileName} (i 0, t .r0, i 1024) .r1"

    ; Remove installer file
    Delete /REBOOTOK $0

    ; Do not ask for reboot after installation
    SetRebootFlag false

    System::Free $0

NotSilent:

FunctionEnd

;--------------------------------
; Custom functions

Function LaunchApplication
    ExecShell "open" "$INSTDIR\${EXE_NAME}"
FunctionEnd

;--------------------------------
; Sections

Section "Install"

    SetShellVarContext all

    ; Uninstall old application version (pre-2.0.0)
    !insertmacro UninstallOldVersion

    ; Close running instance of application
    !insertmacro CloseRunningInstance

    ; Set output path to the installation directory.
    SetOutPath $INSTDIR

    ; Store installation path in registry
    WriteRegStr HKLM "${REG_APPKEY}" "InstallDir" $INSTDIR

    ; Add uninstall information to Add/Remove Programs
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "UninstallString" "$INSTDIR\${UNINSTALL_NAME}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayIcon" "$INSTDIR\${EXE_NAME}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "Publisher" "${PUB_NAME}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "HelpLink" "${PUB_WEBPAGE}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayVersion" "${APP_VERSION}"
    WriteRegDWord HKLM "${REG_UNINSTALL}" "NoModify" 1
    WriteRegDWord HKLM "${REG_UNINSTALL}" "NoRepair" 1

    ; Create application directories
    CreateDirectory "$INSTDIR\data\icons\links"
    CreateDirectory "$INSTDIR\data\icons\users"
    CreateDirectory "$INSTDIR\data\photos"

    ; Add application files into package
    File "${BIN_DIR}\${EXE_NAME}"
    File "${PROJECT_DIR}\CHANGES"

    ; Create uninstaller
    WriteUninstaller "$INSTDIR\${UNINSTALL_NAME}"

    ; Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${RUN_APPLICATION}.lnk" "$INSTDIR\${EXE_NAME}" "" "$INSTDIR\${EXE_NAME}" 0 SW_SHOWNORMAL "" "${RUN_APPLICATION}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${DELETE_APPLICATION}.lnk" "$INSTDIR\${UNINSTALL_NAME}" "" "$INSTDIR\${UNINSTALL_NAME}" 0 SW_SHOWNORMAL "" "${DELETE_APPLICATION}"

    ; Create Quick Launch shortcut
    CreateShortCut "$QUICKLAUNCH\${APP_NAME}.lnk" "$INSTDIR\${EXE_NAME}" "" "$INSTDIR\${EXE_NAME}" 0 SW_SHOWNORMAL "" "${RUN_APPLICATION}"

SectionEnd

Section "Uninstall"

    SetShellVarContext all

    ; Close running instance of application
    !insertmacro CloseRunningInstance

    ; Remove application files
    Delete "$INSTDIR\${EXE_NAME}"
    Delete "$INSTDIR\CHANGES"

    ; Check if installation is silent
    IfSilent SilentMode 0

    ; Remove application registry keys
    DeleteRegKey HKLM "${REG_APPKEY}"
    DeleteRegKey /ifempty HKLM "${REG_PUBKEY}"

    ; Remove cached favorite links icons
    Delete "$INSTDIR\data\icons\links\*.ico"

    ; Remove cached user icons
    Delete "$INSTDIR\data\icons\users\*.ico"

    ; Remove cached thumbnails
    Delete "$INSTDIR\data\photos\*.jpg"

    ; Remove news template file
    Delete "$INSTDIR\data\news.html"

SilentMode:

    ; Remove log file
    Delete "$INSTDIR\chat_informer.log"

    ; Remove uninstaller itself
    Delete "$INSTDIR\${UNINSTALL_NAME}"

    ; Remove application folders if empty
    RmDir "$INSTDIR\data\icons\links"
    RmDir "$INSTDIR\data\icons\users"
    RmDir "$INSTDIR\data\icons"
    RmDir "$INSTDIR\data\photos"
    RmDir "$INSTDIR\data"
    RmDir "$INSTDIR"

    ; Remove Start Menu shortcuts
    RmDir /r "$SMPROGRAMS\${APP_NAME}"

    ; Remove QuickLaunch shortcut
    Delete "$QUICKLAUNCH\${APP_NAME}.lnk"

    ; Remove uninstall information from Add/Remove Programs
    DeleteRegKey HKLM "${REG_UNINSTALL}"

SectionEnd
