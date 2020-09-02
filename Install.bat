@echo off
setlocal

if [%~1]==[] (
	@echo Must must provide native host executable path
	pause
	exit /B
)

for %%i in ("%~1") do (
	set filedrive=%%~di
	set filepath=%%~pi
	set filename=%%~ni
	set fileextension=%%~xi
)

set filepath=%filedrive%%filepath%
cd %filepath%

set lcFilename=%filename%
call :LoCase lcFilename

if [%~2]==[-uninstall] (
	@echo Removing native host from system registry...
	%SystemRoot%\System32\reg.exe delete "HKCU\Software\Google\Chrome\NativeMessagingHosts\%lcFilename%" /f

	@echo Deleting json for %filename%...
	del %lcFilename%.json

	pause
	exit
)

@echo Adding native host to system registry...
%SystemRoot%\System32\reg.exe add "HKCU\Software\Google\Chrome\NativeMessagingHosts\%lcFilename%" /ve /t REG_SZ /d "%filepath%%lcFilename%.json" /f

::TODO: Handle special characters, see https://developer.chrome.com/extensions/nativeMessaging
@echo Creating json for %filename%...
@echo {> %lcFilename%.json
@echo "name": "%lcFilename%",>> %lcFilename%.json
@echo "description": " ",>> %lcFilename%.json
@echo "path": "%filepath:\=//%%filename%%fileextension%",>> %lcFilename%.json
@echo "type": "stdio",>> %lcFilename%.json
@echo "allowed_origins": [>> %lcFilename%.json
@echo "chrome-extension:///">> %lcFilename%.json
@echo ]>> %lcFilename%.json
@echo }>> %lcFilename%.json

echo Installation successful
pause
exit

:LoCase
:: From https://www.robvanderwoude.com/battech_convertcase.php
:: Subroutine to convert a variable VALUE to all lower case.
:: The argument for this subroutine is the variable NAME.
FOR %%i IN ("A=a" "B=b" "C=c" "D=d" "E=e" "F=f" "G=g" "H=h" "I=i" "J=j" "K=k" "L=l" "M=m" "N=n" "O=o" "P=p" "Q=q" "R=r" "S=s" "T=t" "U=u" "V=v" "W=w" "X=x" "Y=y" "Z=z") DO CALL SET "%1=%%%1:%%~i%%"
GOTO:EOF