#
# remove any generated build files
#
rm -rf ..\gmake
rm -rf ..\vs2017
rm -rf ..\vs2019
rm -rf ..\obj
#
# remove any compiled libraries
#
rm -rf ..\..\lib\*.lib
#
# create new build files files
#
./premake5-alpha15 --os=linux --file=premake5.lua gmake

