rem delete old solution/workspace files
rem
rmdir /S /Q ..\gmake
rmdir /S /Q ..\vs2017
rmdir /S /Q ..\vs2019
rmdir /S /Q ..\obj
rem
rem delete old compiled libraries
rem
del ..\..\lib\*.lib
rem
rem create new solution/workspace files
rem
premake5-alpha16.exe --os=windows --file=deps.lua vs2017
premake5-alpha16.exe --os=windows --file=mixr.lua vs2017
premake5-alpha16.exe --os=windows --file=deps.lua vs2019
premake5-alpha16.exe --os=windows --file=mixr.lua vs2019
