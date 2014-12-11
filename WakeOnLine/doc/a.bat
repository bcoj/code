@echo off&setlocal enabledelayedexpansion
for /f "delims=" %%a in ('ipconfig/all') do call:a "%%a"
for /f "tokens=2 delims=:" %%a in ('ipconfig /all^|find "Physical Address"') do set MAC=%%a
ipconfig /all>"%temp%\ipList.txt"
for /f "tokens=1 delims=:" %%a in ('findstr /n "Ethernet adapter" "%temp%\ipList.txt"') do (
    set SkipRow=%%a
    goto :DoSkip
)

:DoSkip
for /f "usebackq skip=%SkipRow% tokens=1,15" %%a in ("%temp%\ipList.txt") do (
    if "%%a" equ "IP" (
        set IP=%%b
        goto :ShowResult
    )
)

:ShowResult
echo;%ip%£º%e%
pause
:a
set "a=%~1"
if defined a if not "!a:~0,1!"==" " set "b=!a:~0,1!"
for /f "tokens=1,2* delims=:. " %%b in ("%~1") do if /i "%%b %%c"=="Physical Address" set %b%=%%d