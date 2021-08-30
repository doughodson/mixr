#
# remove any generated build files
#
rm -rf ..\gmake
rm -rf ..\obj
#
# remove any compiled libraries
#
rm -rf ..\..\lib\*.lib
#
# create new build files files
#
./premake5-alpha16 --os=linux --file=premake5.lua gmake

