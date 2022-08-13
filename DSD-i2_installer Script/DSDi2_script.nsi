!define PRODUCT_NAME "DSD-i2"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "EAP"
!define PRODUCT_WEB_SITE "http://www.eap.gr"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\DSDi2_FPGA_tool.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

!include nsDialogs.nsh
!include LogicLib.nsh
!include MUI2.nsh
!include WinVer.nsh
!include x64.nsh

!define DOT_MAJOR "4"
!define DOT_MINOR "7.2"

Name "DSD-i2 Upload Tools"
OutFile DSD-i2_Installer.exe
RequestExecutionLevel user
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Var Dialog
Var ArduinoInstDir
Var StmHardVer
Var StmToolVer
Var EspHardVer
Var EspToolVer


!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "License.txt"
Page custom FpgaPageCreate FpgaPageLeave
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

RequestExecutionLevel admin

Function FpgaPageCreate
    !insertmacro MUI_HEADER_TEXT "DSD-i2 FPGA upload tool for DSD-i2 board" "Provide DSD-i2 FPGA upload tool directory"

    nsDialogs::Create 1018
    Pop $Dialog

    ${If} $Dialog == error
        Abort
    ${EndIf}

    ${NSD_CreateGroupBox} 5% 86u 90% 34u "DSD-i2 FPGA upload tool Install Path"
    Pop $0

        ${NSD_CreateDirRequest} 15% 100u 49% 12u "$LOCALAPPDATA\DSD-i2 FPGA Upload Tool"
        Pop $1
        
        ${NSD_CreateBrowseButton} 65% 100u 20% 12u "Browse..."
        Pop $0
        ${NSD_OnClick} $0 FpgaDirBrowse

    nsDialogs::Show
FunctionEnd

Function FpgaDirBrowse
    ${NSD_GetText} $1 $0
    nsDialogs::SelectFolderDialog "Select DSD-i2 FPGA upload tool directory" "$0"
    Pop $0
    ${If} $0 != error
    StrCpy $2 "$0"
        ${NSD_SetText} $1 "$0"
    ${EndIf}
FunctionEnd

Function FpgaPageLeave
    ${NSD_GetText} $1 $0
    StrCpy $INSTDIR "$0"
FunctionEnd

Section
       SetOutPath "$INSTDIR"
       FileOpen $0 $INSTDIR\info.ini w
       FileClose $0

       #STM32
       #find the version of stm hardware path
       Push $ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\*.*
       Call VersionSearch
       Pop $StmHardVer
           WriteINIStr $INSTDIR\info.ini "STMPaths" "Hardware Path" "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer" ; add path to ini
       #Check if the folder exists
       StrCpy $0 "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)\*.*"
       IfFileExists $0 +3 0
       MessageBox mb_iconStop "L452R(C-E)(I-T-Y)_L462RE(I-T-Y) folder not found in STMicroelectronics folder. Reinstalling the STM32 toolchain may be required.$\n$\r Setup will now exit."
       Quit
       SetOutPath "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)\"
       File "STMicroelectronics\hardware\stm32\2.2.0\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)\ldscript.ld"
       File "STMicroelectronics\hardware\stm32\2.2.0\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)\variant_DSD_i2.cpp"
       File "STMicroelectronics\hardware\stm32\2.2.0\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)\variant_DSD_i2.h"

       #Check if boards.txt exist
       StrCpy $0 "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\boards.txt"
       IfFileExists $0 +3 0
       MessageBox mb_iconStop "boards.txt file not found in STMicroelectronics folder. Reinstalling the STM32 toolchain may be required.$\n$\r Setup will now exit."
       Quit
       #Create a backup
       Delete "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\boards.BAK"
       CopyFiles "$0" "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\boards.BAK" ;test
       #Check if platform.txt exist
       StrCpy $0 "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\platform.txt"
       IfFileExists $0 +3 0
       MessageBox mb_iconStop "platform.txt file not found in STMicroelectronics folder. Reinstalling the STM32 toolchain may be required.$\n$\r Setup will now exit."
       Quit
       #Create a backup
       Delete "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\platform.BAK"
       CopyFiles "$0" "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\platform.BAK" ;test
       FileClose "$0"
       #extract the boards and platform files in TEMP
       SetOutPath "$LOCALAPPDATA\Temp\DSDi2\STMicroelectronics"
       File "STMicroelectronics\hardware\stm32\2.2.0\boards.txt"
       File "STMicroelectronics\hardware\stm32\2.2.0\platform.txt"
       #Debug::Stack
       #Check if append is needed on boards.txt
       Push "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\boards.txt"
       Push "#DSD-i2$\n"
       Call SearchInFile
       Pop $0
       StrCmp "false" $0 0 +6
              Exch
              #append the extracted board.txt
              Push $LOCALAPPDATA\Temp\DSDi2\STMicroelectronics\boards.txt
              Call AppendTxtFile
              Pop $0 ;remove read board.txt from the stack.
              Exch
              Exch
              Pop $0 ;remove appended board.txt from the stack.
              #Debug::Stack
       #Check if append is needed on platform.txt
       Push "$ArduinoInstDir\packages\STMicroelectronics\hardware\stm32\$StmHardVer\platform.txt"
       Exch
       Call SearchInFile
       Pop $0
       StrCmp "false" $0 0 +6
              Exch
              #append the extracted platform.txt
              Push $LOCALAPPDATA\Temp\DSDi2\STMicroelectronics\platform.txt
              Call AppendTxtFile
              Pop $0 ;remove read platform.txt from the stack
              Exch
              Exch
              Pop $0 ;remove appended platform.txt from the stack. String #DSD-i2$\n" should remaain for later use
              #Debug::Stack
       #find the version of stm tool path
       Push  $ArduinoInstDir\packages\STMicroelectronics\tools\STM32Tools\*.*
       Call VersionSearch
       Pop $StmToolVer
           WriteINIStr $INSTDIR\info.ini "STMPaths" "Tool Path" "$ArduinoInstDir\packages\STMicroelectronics\tools\STM32Tools\$StmToolVer\win" ; add path to ini
       #Check if the folder exists
       SetOutPath "$ArduinoInstDir\packages\STMicroelectronics\tools\STM32Tools\$StmToolVer\win\"
       File "STMicroelectronics\tools\STM32Tools\2.1.1\win\dsdi2_stm_upload_tool.exe"
       
       #Debug::Stack
       #DetailPrint "ArduinoIDE DSD-i2 tools for STM32, installation completed."

       #ESP32
       #find the version of esp hardware path
       Push $ArduinoInstDir\packages\esp32\hardware\esp32\*.*
       Call VersionSearch
       Pop $EspHardVer
           WriteINIStr $INSTDIR\info.ini "ESPPaths" "Hardware Path" "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer" ; add path to ini
       #create the required folder
       CreateDirectory "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\variants\DSD-i2"
       SetOutPath "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\variants\DSD-i2\"
       File "esp32\hardware\esp32\2.0.2\variants\DSD-i2\pins_arduino.h"

       #Check if boards.txt exist
       StrCpy $0 "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\boards.txt"
       IfFileExists $0 +3 0
       MessageBox mb_iconStop "boards.txt file not found in esp32 folder. Reinstalling the STM32 toolchain may be required.$\n$\r Setup will now exit."
       Quit
       #Create a backup
       Delete "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\boards.BAK"
       CopyFiles "$0" "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\boards.BAK" ;test
       #Check if platform.txt exist
       StrCpy $0 "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\platform.txt"
       IfFileExists $0 +3 0
       MessageBox mb_iconStop "platform.txt file not found in esp32 folder. Reinstalling the STM32 toolchain may be required.$\n$\r Setup will now exit."
       Quit
       #Create a backup
       Delete "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\platform.BAK"
       CopyFiles "$0" "$ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\platform.BAK" ;test
       
       #extract the boards and platform files in TEMP
       SetOutPath "$LOCALAPPDATA\Temp\DSDi2\esp32"
       File "esp32\hardware\esp32\2.0.2\boards.txt"
       File "esp32\hardware\esp32\2.0.2\platform.txt"
       #Debug::Stack
       #Check if append is needed on boards.txt
       Push $ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\boards.txt
       Exch
       Call SearchInFile
       Pop $0
       StrCmp "false" $0 0 +6
              #append the extracted board.txt
              Exch
              Push $LOCALAPPDATA\Temp\DSDi2\esp32\boards.txt
              Call AppendTxtFile
              Pop $0 ;remove read boards.txt
              Exch
              Exch
              Pop $0 ;remove appended boards.txt
              #Debug::Stack
       #Check if append is needed on platform.txt
       Push $ArduinoInstDir\packages\esp32\hardware\esp32\$EspHardVer\platform.txt
       Exch
       Call SearchInFile
       Pop $0
       StrCmp "false" $0 0 +4
              #append the extracted platform.txt
              Pop $0 ;remove the string
              Push $LOCALAPPDATA\Temp\DSDi2\esp32\platform.txt
              Call AppendTxtFile
              Pop $0
              Pop $0 ;empty the stack
              #Debug::Stack
       #find the version of esp tool path
       Push  $ArduinoInstDir\packages\esp32\tools\esptool_py\*.*
       Call VersionSearch
       Pop $EspToolVer
           WriteINIStr $INSTDIR\info.ini "ESPPaths" "Tool Path" "$ArduinoInstDir\packages\esp32\tools\esptool_py\$EspToolVer" ; add path to ini
       SetOutPath "$ArduinoInstDir\packages\esp32\tools\esptool_py\$EspToolVer\"
       File "esp32\tools\esptool_py\3.1.0\dsdi2_esp_upload_tool.exe"

       #Cyclone 10

       CreateDirectory "$INSTDIR\DSD-i2 Board Bootloader"
       SetOutPath "$INSTDIR\DSD-i2 Board Bootloader"
       File "DSD-i2 FPGA Upload Tool\DSD-i2 Board Bootloader\DSD-i2_Bootloader.bin"
       SetOutPath "$INSTDIR"
       File "DSD-i2 FPGA Upload Tool\10CL32.dll"
       File "DSD-i2 FPGA Upload Tool\DSDi2_FPGA_tool.exe"
       File "DSD-i2 FPGA Upload Tool\LOGO_EAP_GR.ico"
       CreateShortCut "$DESKTOP\DSD-i2 FPGA Upload Tool.lnk" "$INSTDIR\DSDi2_FPGA_tool.exe" "" "$INSTDIR\LOGO_EAP_GR.ico" 0
       WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\DSDi2_FPGA_tool.exe"
       WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Path" "$INSTDIR"
       
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "DSD-i2_FPGA_tool"
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\LOGO_EAP_GR.ico"
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
       WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
       WriteUninstaller "$INSTDIR\uninst.exe"
       
SectionEnd

Function AppendTxtFile
         Exch $R0 ;file to read
         Exch
         Exch $R1 ;file to append
         Push $R2
         Push $R3
         Push $R4
         Push $R5
         Push $R6
         Push $R7
         Push $R8
         Push $R9
         ClearErrors
         FileOpen $R2 $R1 a
         FileOpen $R3 "$R0" r
         IfErrors +10
         FileSeek $R2 0 END
         FileWrite $R2 "$\r$\n"
         FileWrite $R2 "$\r$\n"
              FileRead $R3 $R4
              FileWrite $R2 "$R4"
              IfErrors +2
              Goto -3
         FileClose $R3
         FileClose $R2

         Pop $R9
         Pop $R8
         Pop $R7
         Pop $R6
         Pop $R5
         Pop $R4
         Pop $R3
         Pop $R2
         Exch $R1
         Exch
         Exch $R0
FunctionEnd


Function SearchInFile
         Exch $R0 ;text to look for
         Exch
         Exch $R1 ;file path
         Push $R2
         Push $R3
         Push $R4
         Push $R5
         Push $R6
         Push $R7
         Push $R8
         Push $R9

         ClearErrors
         FileOpen $R2 $R1 r
         IfErrors 0 +3
         StrCpy $0 "false"
         Goto +9
              ClearErrors
              FileRead $R2 $R3
              IfErrors 0 +3
              StrCpy $0 "false"
              Goto +3
              StrCmp $R0 $R3 0 -5
              StrCpy $0 "true"
         FileClose $R2

         Pop $R9
         Pop $R8
         Pop $R7
         Pop $R6
         Pop $R5
         Pop $R4
         Pop $R3
         Pop $R2
         Exch $R1
         Exch
         Exch $R0
         Push $0
FunctionEnd

Function VersionSearch
         Exch $R0 ;current folder
         #Debug::Stack
         Push $R1
         Push $R2
         Push $R3
         Push $R4
         Push $R5
         Push $R6
         Push $R7
         Push $R8
         Push $R9

         FindFirst $R2 $R1 $R0
              StrCmp $R1 "." +3
              StrCmp $R1 ".." +2
         Goto +3
              FindNext $R2 $R1
         Goto -4
         FindClose $R2
         StrCpy $R0 $R1
         #Debug::Stack
         Pop $R9
         Pop $R8
         Pop $R7
         Pop $R6
         Pop $R5
         Pop $R4
         Pop $R3
         Pop $R2
         Pop $R1
         Exch $R0 ;folder that has the version name.
FunctionEnd


#Test for windows 10, arduinoIDE and net framework installation as soon as the user starts the the installer

Function .onInit
         ClearErrors
         ${IfNot} ${AtLeastWin10}
         ${OrIfNot} ${RunningX64}
              MessageBox mb_iconStop "Windows 10-x64 and above required.$\n$\r Setup will now exit."
              Abort
         ${EndIf}
         #Call IsDotNetInstalled ; don't use it for now since I don't have the netframework installed but the program still works.
         ClearErrors
         SetRegView 64
         #test if ArduinoIde is installed.
         ReadRegStr $0 HKLM "SOFTWARE\WOW6432Node\Arduino" "Install_Dir"
         IfFileExists $0 +3 0
         MessageBox mb_iconStop "ArduinoIDE is required.$\n$\r Setup will now exit."
         Abort
         #test for ArduinoIDE Arduino15 folder
         StrCpy $0 "$LOCALAPPDATA\Arduino15\*.*"
         IfFileExists $0 +3 0
         MessageBox mb_iconStop "Arduino15 directory not found.$\n$\r Setup will now exit."
         Abort
         StrCpy $ArduinoInstDir "$LOCALAPPDATA\Arduino15"
         #test for STM32duino toolchain
         StrCpy $0 "$ArduinoInstDir\packages\STMicroelectronics\*.*"
         IfFileExists $0 +3 0
         MessageBox mb_iconStop "STM32 for ArduinoIde was not found.$\n$\r Setup will now exit."
         Abort
         #test for esp32 toolchain
         StrCpy $0 "$ArduinoInstDir\packages\esp32\*.*"
         IfFileExists $0 +3 0
         MessageBox mb_iconStop "ESP32 for ArduinoIde was not found.$\n$\r Setup will now exit."
         Abort
FunctionEnd

#https://nsis.sourceforge.io/How_to_insure_a_required_version_of_.NETFramework_is_installed
; Usage
; Define in your script two constants:
;   DOT_MAJOR "(Major framework version)"
;   DOT_MINOR "{Minor framework version)"
;
; Call IsDotNetInstalled
; This function will abort the installation if the required version
; or higher version of the .NET Framework is not installed.  Place it in
; either your .onInit function or your first install section before
; other code.
Function IsDotNetInstalled

  StrCpy $0 "0"
  StrCpy $1 "SOFTWARE\Microsoft\.NETFramework" ;registry entry to look in.
  StrCpy $2 0

  StartEnum:
    ;Enumerate the versions installed.
    EnumRegKey $3 HKLM "$1\policy" $2

    ;If we don't find any versions installed, it's not here.
    StrCmp $3 "" noDotNet notEmpty

    ;We found something.
    notEmpty:
      ;Find out if the RegKey starts with 'v'.
      ;If it doesn't, goto the next key.
      StrCpy $4 $3 1 0
      StrCmp $4 "v" +1 goNext
      StrCpy $4 $3 1 1

      ;It starts with 'v'.  Now check to see how the installed major version
      ;relates to our required major version.
      ;If it's equal check the minor version, if it's greater,
      ;we found a good RegKey.
      IntCmp $4 ${DOT_MAJOR} +1 goNext yesDotNetReg
      ;Check the minor version.  If it's equal or greater to our requested
      ;version then we're good.
      StrCpy $4 $3 1 3
      IntCmp $4 ${DOT_MINOR} yesDotNetReg goNext yesDotNetReg

    goNext:
      ;Go to the next RegKey.
      IntOp $2 $2 + 1
      goto StartEnum

  yesDotNetReg:
    ;Now that we've found a good RegKey, let's make sure it's actually
    ;installed by getting the install path and checking to see if the
    ;mscorlib.dll exists.
    EnumRegValue $2 HKLM "$1\policy\$3" 0
    ;$2 should equal whatever comes after the major and minor versions
    ;(ie, v1.1.4322)
    StrCmp $2 "" noDotNet
    ReadRegStr $4 HKLM $1 "InstallRoot"
    ;Hopefully the install root isn't empty.
    StrCmp $4 "" noDotNet
    ;build the actuall directory path to mscorlib.dll.
    StrCpy $4 "$4$3.$2\mscorlib.dll"
    IfFileExists $4 yesDotNet noDotNet

  noDotNet:
    ;Nope, something went wrong along the way.  Looks like the
    ;proper .NET Framework isn't installed.
    MessageBox MB_OK "You must have v${DOT_MAJOR}.${DOT_MINOR} or greater of the .NET Framework installed."
    Abort

  yesDotNet:
    ;Everything checks out.  Go on with the rest of the installation.
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "DSD-i2 was successfully removed from your computer."
FunctionEnd

Function un.onInit
         SetRegView 64
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove DSD-i2 and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall

       ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\DSDi2_FPGA_tool.exe" "Path"
       
       #Remove fpga stuff
       Delete "$DESKTOP\DSD-i2 FPGA Upload Tool.lnk"
       Delete "$0\LOGO_EAP_GR.ico"
       Delete "$0\DSDi2_FPGA_tool.exe"
       Delete "$0\10CL32.dll"
       Delete "$0\DSD-i2 Board Bootloader\DSD-i2_Bootloader.bin"
       RMDir "$0\DSD-i2 Board Bootloader"
       StrCpy $3 "$0"
       #remove esp stuff
       #remove upload tool
       ReadINIStr $1 $0\info.ini "ESPPaths" "Tool Path"
       Delete "$1\dsdi2_esp_upload_tool.exe"
       #restore original boards.txt
       ReadINIStr $1 $0\info.ini "ESPPaths" "Hardware Path"
       Delete "$1\boards.txt"
       Copyfiles "$1\boards.BAK" "$1\boards.txt"
       Delete "$1\boards.BAK"
       #restore original platform.txt
       Delete "$1\platform.txt"
       Copyfiles "$1\platform.BAK" "$1\platform.txt"
       Delete "$1\platform.BAK"
       #remove misc related files
       Delete "$1\variants\DSD-i2\pins_arduino.h"
       RMDir "$1\variants\DSD-i2"
       #remove stm stuff
       #remove upload tool
       ReadINIStr $1 $0\info.ini "STMPaths" "Tool Path"
       Delete "$1\dsdi2_stm_upload_tool.exe"
       #restore original boards.txt
       ReadINIStr $1 $0\info.ini "STMPaths" "Hardware Path"
       Delete "$1\boards.txt"
       Copyfiles "$1\boards.BAK" "$1\boards.txt"
       Delete "$1\boards.BAK"
       #restore original platform.txt
       Delete "$1\platform.txt"
       Copyfiles "$1\platform.BAK" "$1\platform.txt"
       Delete "$1\platform.BAK"
       #remove misc related files
       StrCpy $2 "$1\variants\STM32L4xx\L452R(C-E)(I-T-Y)_L462RE(I-T-Y)"
       Delete "$2\ldscript.ld"
       Delete "$2\variant_DSD_i2.cpp"
       Delete "$2\variant_DSD_i2.h"
       Delete "$3/info.ini"
       DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
       DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
       Delete "$3\uninst.exe"

  #SetAutoClose true
SectionEnd





