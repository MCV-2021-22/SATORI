SET UE4_ROOT="E:/UnrealEngine4/UE_4.27"
SET UE4_UAT=%UE4_ROOT%/Engine/Build/BatchFiles/RunUAT.bat
SET PROJECT_ROOT=D:/Project__UPF
SET UPROJECT=D:/Project__UPF/SATORI/SATORI.uproject
SET CONFIG=Development

%UE4_UAT% ^
  -ScriptsForProject=%UPROJECT% ^
  BuildCookRun ^
  -project=%UPROJECT% ^
  -noP4 ^
  -clientconfig=%CONFIG% ^
  -serverconfig=%CONFIG% ^
  -utf8output ^
  -platform=Win64 ^
  -targetplatform=Win64 ^
  -build ^
  -cook ^
  -map= ^
  -pak ^
  -stage ^
  -distribution ^
  -compressed ^
  -cmdline=" -Messaging" ^
  -stagingdirectory=%PROJECT_ROOT%/Builds/%CONFIG% ^
  -nocompile ^
  -nocompileeditor ^
  -addcmdline="-SessionId=F4A7206C469A8E268084EB99D416D7EF -SessionOwner='AULES' -SessionName='Development With Pak' "

REM   -deploy ^
  -encryptinifiles ^
  -unversionedcookedcontent ^
  -installed ^

