call nbtstat ¨Ca %1 |find "MAC Address">nul
if %ERRORLEVEL%==0 goto nbt 
:nbt¡¡
@ echo Currednt ipaddress is: %1 >>allpc.txt
ntstat ¨Ca %1 >>allpc.txt
:end 