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

set script_path="%~dp0"
set target=""
set config=Release
set rebuild=0
set cmake_generator=""
set msbuild_platform=""

:argparse_loop
if NOT "%1" == "" (
    if "%1" == "Rebuild" (
        set target=":Rebuild"
        set rebuild=1
    ) else if "%1" == "Debug" (
        set config=Debug
    ) else if "%1" == "Clean" (
        set target=":Clean"
    ) else if "%1" == "x64" (
        set msbuild_platform="x64"
        set cmake_generator="Visual Studio 15 2017 Win64"
    ) else if "%1" == "Win32" (
        set msbuild_platform="Win32"
        set cmake_generator="Visual Studio 15 2017"
    )

    shift
    goto :argparse_loop
)

pushd .

cd "%script_path%"
echo Current directory is %cd%

REM ========== lkCommon deps build ===========

set build_dir=build-%config%

REM Create solutions for dependencies
call :cmake_create lkCommonDeps
cd %build_dir%

REM zlib
cd zlib
call :ms_build zlib
cd ..

REM libpng
cd libpng
call :ms_build libpng png_static
cd ..


echo.
echo Script is done
echo.

popd

exit /b 0



REM ========== Functions ==========

:ms_build
if NOT "%2"=="" ( 
    set project=%2
    set solution=%1.sln
) else (
    set project=%1
    set solution=%project%.sln
)

msbuild %solution% /t:%project%%target% /p:Configuration=%config%;Platform=%msbuild_platform%
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Build failed
    echo.
    exit /b
)

exit /b 0



:cmake_create
set project=%1
mkdir %build_dir%
cd %build_dir%

cmake .. -DCMAKE_BUILD_TYPE=%config% -G %cmake_generator%
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo CMake generation failed
    echo.
    exit /b
)

exit /b %ERRORLEVEL%
