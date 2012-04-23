; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppID={{A1415808-331E-47D8-8FD3-0BB62A5B76BF}
AppName=Gravit (32bit)
AppVersion=0.5.0
AppVerName=Gravit 0.5.0 (32bit)
AppPublisher=Gravit Development Team
AppPublisherURL=http://gravit.slowchop.com/
AppSupportURL=http://gravit.slowchop.com/
AppUpdatesURL=http://gravit.slowchop.com/
DefaultDirName={pf32}\Gravit
DefaultGroupName=Gravit
AllowNoIcons=true
LicenseFile=.\files\COPYING
InfoAfterFile=.\files\README.txt
OutputDir=.\
OutputBaseFilename=setup-gravit-0.5.0-32bit
Compression=lzma/Max
SolidCompression=true
WizardSmallImageFile=.\SmallImage.bmp
WizardImageFile=.\Image.bmp
ShowLanguageDialog=no
; minimum windows version: Windows XP SP1
MinVersion=,5.1.2600sp1
SetupIconFile=.\files\gravit.ico
SourceDir=.\

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Components]
Name: "gravit"; Description: "Gravit"; Types: full compact custom; Flags: fixed
Name: "screensaver"; Description: "Gravit Screensaver"; Types: full custom

[Tasks]
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: ".\32bit\gravit.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\32bit\gravit.exe"; DestDir: "{syswow64}"; DestName: "gravit.scr"; Flags: ignoreversion; Components: screensaver
Source: ".\files\cfg\*.cfg"; DestDir: "{app}\cfg"; Flags: ignoreversion; Permissions: users-modify
Source: ".\files\COPYING"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\files\data\*"; DestDir: "{app}\data"; Flags: ignoreversion
Source: ".\32bit\*.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\32bit\*.DLL"; DestDir: "{syswow64}"; Flags: sharedfile; Components: screensaver
Source: ".\files\spawn\*"; DestDir: "{app}\spawn"; Flags: ignoreversion ; Permissions: users-modify
Source: ".\files\stderr.txt"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist; Permissions: users-modify
Source: ".\files\stdout.txt"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist; Permissions: users-modify
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
; include Microsoft VC 2008 SP1 Redistributable Package for x86
Source: .\vc_redist\vcredist_x86.exe; DestDir: {tmp}; Permissions: everyone-full; Flags: ignoreversion overwritereadonly;

[Dirs]
Name: "{app}\cfg"; Permissions: users-modify; 
Name: "{app}\spawn"; Permissions: users-modify; 
Name: "{app}\save"; Flags: setntfscompression; Permissions: users-modify; 
Name: "{app}\screenshots"; Permissions: users-modify; 

[INI]
Filename: "{app}\gravit.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://gravit.slowchop.com"

[Registry]
; register software setting for gravit
Root: "HKLM"; Subkey: "Software\Gravit"; Flags: uninsdeletekeyifempty
Root: "HKLM"; Subkey: "Software\Gravit"; ValueType: string; ValueName: "path"; ValueData: "{app}"; Flags: uninsdeletekey
Root: "HKCU"; Subkey: "Software\Gravit"; Flags: uninsdeletekeyifempty
Root: "HKCU"; Subkey: "Software\Gravit"; ValueType: string; ValueName: "path"; ValueData: "{app}"; Flags: uninsdeletekey
; register application path
Root: "HKLM"; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\gravit.exe"; ValueType: string; ValueData: "{app}\gravit.exe"; Flags: uninsdeletekey
Root: "HKLM"; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\gravit.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app}"; Flags: uninsdeletekey
; register screensaver dll search path
Root: "HKLM"; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\gravit.scr"; ValueType: string; ValueData: "{syswow64}\gravit.scr"; Flags: uninsdeletekey; Components: screensaver
Root: "HKLM"; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\gravit.scr"; ValueType: string; ValueName: "Path"; ValueData: "{app}"; Flags: uninsdeletekey; Components: screensaver

[Icons]
Name: "{group}\Gravit"; Filename: "{app}\gravit.exe"; WorkingDir: "{app}";
Name: "{group}\Gravit (Compatible Mode)"; Filename: "{app}\gravit.exe"; Parameters: """particlerendermode 0"""; WorkingDir: "{app}";
Name: "{group}\Activate Gravit Screen Saver"; Filename: "{app}\gravit.exe"; WorkingDir: "{app}"; Parameters: "installscreensaver quit"; Components: screensaver
Name: "{group}\Configure\Customise Gravit (gravit.cfg)"; Filename: "{win}\notepad.exe"; Parameters: "{app}\cfg\gravit.cfg";
Name: "{group}\Configure\Customise Screen Saver (screensaver.cfg)"; Filename: "{win}\notepad.exe"; Parameters: "{app}\cfg\screensaver.cfg"; Components: screensaver
Name: "{group}\Visit Gravit on the Internet"; Filename: "{app}\gravit.url"
Name: "{group}\{cm:UninstallProgram,Gravit}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Gravit"; Filename: "{app}\gravit.exe"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Gravit"; Filename: "{app}\gravit.exe"; WorkingDir: "{app}"; Tasks: quicklaunchicon

[Run]
; install vcredist_x86.exe
Filename: {tmp}\vcredist_x86.exe; Parameters: "/q:a /c:""vcredist_x86.exe /q:a /c:""""msiexec /i vcredist.msi /qn"""" """; StatusMsg: "Installing MS Visual C++ 2008 SP1 Redistributable Package (x86) ..."; Flags: waituntilterminated;
; post-install tasks
Filename: "{win}\notepad.exe"; Parameters: "{app}\cfg\gravit.cfg"; Description: "Modify gravit.cfg (Recommended)"; Flags: nowait postinstall skipifsilent
;Filename: "{app}\gravit.exe"; Parameters: "installscreensaver quit"; Flags: nowait postinstall skipifsilent; Description: "Activate Gravit Screensaver"; Components: screensaver
Filename: "{app}\gravit.exe"; Flags: nowait postinstall unchecked skipifsilent; Description: "{cm:LaunchProgram,Gravit}"

[UninstallDelete]
Type: files; Name: "{app}\gravit.url"
