#
# remove any generated build files
#
rm -rf ..\gmake
rm -rf ..\vs2015
rm -rf ..\vs2017
rm -rf ..\obj
#
# remove any compiled libraries
#
rm -rf ..\..\lib\*.lib
#
# create new build files files
#
./premake5-alpha12 --os=linux --file=premake5.lua gmake

