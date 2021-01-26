// clang-format off
#ifndef ApplicationName
# define ApplicationName "SEFMediaPreparer"
#endif
#ifndef ApplicationVersion
# define ApplicationVersion Trim(FileRead(FileOpen("version.txt")))
#endif
#ifndef ApplicationExe
# define ApplicationExe "SEFMediaPreparer.exe"
#endif
#ifndef ApplicationSource
# define ApplicationSource "."
#endif
#ifndef Publisher
# define Publisher "SuperEpicFuntime"
#endif
#ifndef PublisherURL
# define PublisherURL "https://superepicfuntime.com/"
#endif

[Setup]
AppId={{B1879C24-6E65-4460-BD30-A67ED9C9D191}
AppName={#ApplicationName}
AppVersion={#ApplicationVersion}
AppVerName={#ApplicationName} {#ApplicationVersion}
AppPublisher={#Publisher}
AppPublisherURL={#PublisherURL}
AppSupportURL={#PublisherURL}
AppUpdatesURL={#PublisherURL}
DefaultDirName={userappdata}\SuperEpicFuntime\{#ApplicationName}
DisableDirPage=yes
DefaultGroupName={#ApplicationName}
DisableProgramGroupPage=yes
DisableWelcomePage=no
OutputDir={#ApplicationSource}/
OutputBaseFilename=SEFMediaPreparer-{#ApplicationVersion}-Setup
SetupIconFile={#ApplicationSource}/seflogo.ico
PrivilegesRequired=lowest
Compression=lzma
SolidCompression=yes
UsePreviousAppDir=False
AppCopyright=2020
VersionInfoCompany={#Publisher}
VersionInfoCopyright=2020
VersionInfoProductName={#ApplicationName}
VersionInfoProductVersion={#ApplicationVersion}
UninstallDisplayName=Uninstall {#ApplicationName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "bin\*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs createallsubdirs replacesameversion; Components: Core; Excludes: "ffmpeg.exe,ffprobe.exe"
Source: "bin\ff*.exe"; DestDir: "{app}\bin"; Components: ffmpeg

[Icons]
Name: "{group}\{#ApplicationName}"; Filename: "{app}\bin\{#ApplicationExe}"
Name: "{commondesktop}\{#ApplicationName}"; Filename: "{app}\bin\{#ApplicationExe}"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\{#ApplicationExe}"; Description: "{cm:LaunchProgram,{#StringChange(ApplicationName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Messages]
WelcomeLabel1=Thank you for downloading {#ApplicationName} {#ApplicationVersion}!
WelcomeLabel2=Developed by SuperEpicFuntime%n%nWe appreciate your support!

[UninstallDelete]
Type: filesandordirs; Name: "{app}"
Type: dirifempty; Name: "{userappdata}\SuperEpicFuntime"

[Components]
Name: "core"; Description: "SEFMediaPreparer Binaries"; Types: full minimal; Flags: fixed; Languages: english
Name: "ffmpeg"; Description: "FFmpeg Binaries"; Types: full; Languages: english

[Types]
Name: "Full"; Description: "Full Installation"; Languages: english
Name: "Minimal"; Description: "Minimal Installation (No FFmpeg)"
