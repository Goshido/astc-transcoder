#define APPLICATION_NAME "astc-transcoder"
#define APPLICATION_ID "astc-transcoder"
#define APPLICATION_EXE_FILE_NAME "astc-transcoder.exe"
#define APPLICATION_VERSION GetFileVersion(ASTC_TRANSCODER_OUTPUT_DIR + "\" + APPLICATION_EXE_FILE_NAME)
#define APPLICATION_PUBLISHER "Goshido Inc"
#define APPLICATION_URL "https://github.com/Goshido"
#define YEAR GetDateTimeString("yyyy", '', '')
#define COMPANY "Goshido Inc"
#define COMPANY_BASED_YEAR "2015"
#define ALL_RIGHTS_RESERVED "All right reserved."
#define SETUP_EXE_VERSION_INFO_DESCRIPTION "ASTC transcoder setup"
#define ICON "../astc-transcoder/windows/icon.ico"
#define GOSHIDO_INC_FOLDER "Goshido Inc"

[Setup]
; Note we do not specify copyright explicitly because it was set already.
AppName={#APPLICATION_NAME}
AppId={#APPLICATION_ID}
AppVersion={#APPLICATION_VERSION}
AppVerName={#APPLICATION_NAME}
AppPublisher={#APPLICATION_PUBLISHER}
AppPublisherURL={#APPLICATION_URL}
AppSupportURL={#APPLICATION_URL}
AppUpdatesURL={#APPLICATION_URL}

; Note copyright symbol generates error in define section. So we put it here.
VersionInfoCopyright="© {#COMPANY_BASED_YEAR} - {#YEAR} {#COMPANY}. {#ALL_RIGHTS_RESERVED}"
VersionInfoCompany={#COMPANY}
VersionInfoDescription={#SETUP_EXE_VERSION_INFO_DESCRIPTION}
VersionInfoProductName={#APPLICATION_NAME}
VersionInfoVersion={#APPLICATION_VERSION}
VersionInfoTextVersion={#APPLICATION_VERSION}
VersionInfoProductVersion={#APPLICATION_VERSION}
VersionInfoProductTextVersion={#APPLICATION_VERSION}

DefaultDirName="{commonpf64}\{#GOSHIDO_INC_FOLDER}\{#APPLICATION_NAME}"
DefaultGroupName={#APPLICATION_NAME}

OutputBaseFilename="{#APPLICATION_NAME}.{#APPLICATION_VERSION}.setup"

Compression="lzma"
SolidCompression=yes
SetupIconFile={#ICON}

UninstallDisplayIcon="{app}\{#APPLICATION_EXE_FILE_NAME}"
UninstallDisplayName={#APPLICATION_NAME}

UsePreviousAppDir=no
SetupLogging=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[CustomMessages]
english.errorMessage=To continue, you must enter a value
english.installingVS2019=Installing Microsoft Visual C++ 2019 x64 Redistributables...

russian.errorMessage=Для продолжения, необходимо ввести значение
russian.installingVS2019=Установка зависимостей Microsoft Visual C++ 2019 x64...

[Tasks]
Name: "desktopicon"; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}

[Icons]
Name: "{group}\{#APPLICATION_NAME}"; Filename: "{app}\{#APPLICATION_EXE_FILE_NAME}"
Name: "{commondesktop}\{#APPLICATION_NAME}"; Filename: "{app}\{#APPLICATION_EXE_FILE_NAME}"; Tasks: desktopicon
Name: "{group}\{cm:UninstallProgram,{#APPLICATION_NAME}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\{#APPLICATION_EXE_FILE_NAME}"; Description: {cm:LaunchProgram, {#StringChange(APPLICATION_NAME, '&', '&&')}}; Flags: nowait postinstall skipifsilent
Filename: {tmp}\vc_redist.x64.exe; Parameters: "/install /quiet /norestart"; StatusMsg: {cm:installingVS2019}

[Messages]
russian.WelcomeLabel1=Вас приветствует Мастер установки программы "[name]".

[Files]    
; application core libraries                        
Source: "{#ASTC_TRANSCODER_OUTPUT_DIR}\astc-transcoder.exe"; DestDir: {app}; Flags: ignoreversion

; Visial C++ 2019 x64
Source: "{#VS2019_X64_REDIST}"; DestDir: {tmp}; Flags: deleteafterinstall
