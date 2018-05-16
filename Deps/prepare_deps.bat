@echo off

REM TODO:
REM  * Discover CPU count

echo | set /p=Checking for cmake... 
for %%X in (cmake.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
    echo FOUND
) else (
    echo NOT FOUND
    echo     Make sure you download CMake before using this script.
    pause
    exit /b
)

echo | set /p=Checking for msbuild... 
for %%X in (msbuild.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
    echo FOUND
) else (
    echo NOT FOUND
    echo     Make sure you set MSBuild on your PATH variable before using this script.
    pause
    exit /b
)


pushd .

cd %~dp0
echo Current directory is %cd%

REM ========== googletest build ===========

cd googletest

mkdir build
cd build
mkdir x86
mkdir x64

cd x86
cmake ..\.. -DCMAKE_INSTALL_PREFIX="%cd%" -G "Visual Studio 15 2017"
msbuild /property:Configuration=Debug /maxcpucount:8 INSTALL.vcxproj
msbuild /property:Configuration=Release /maxcpucount:8 INSTALL.vcxproj

cd ..\x64
cmake ..\.. -DCMAKE_INSTALL_PREFIX="%cd%"  -G "Visual Studio 15 2017 Win64"
msbuild /property:Configuration=Debug /maxcpucount:8 INSTALL.vcxproj
msbuild /property:Configuration=Release /maxcpucount:8 INSTALL.vcxproj

echo.
echo Script is done
echo.

popd
