@echo off

REM TODO:
REM  * Discover CPU count

call :path_check cmake.exe
call :path_check msbuild.exe

set script_path=%~dp0
set target=""
set config=Release
set clean=0
set cmake_generator="Visual Studio 16 2019"
set cmake_architecture=""
set msbuild_platform=""
set msbuild_rtlib=""

:argparse_loop
if NOT "%1" == "" (
    if "%1" == "Debug" (
        set config=Debug
        set msbuild_rtlib="d"
    ) else if "%1" == "Clean" (
        set clean=1
    ) else if "%1" == "x64" (
        set msbuild_platform=x64
        set cmake_architecture="x64"
    ) else if "%1" == "Win32" (
        set msbuild_platform=Win32
        set cmake_architecture="Win32"
    )

    shift
    goto :argparse_loop
)

pushd .

cd "%script_path%"
echo Current directory is %cd%

REM ========== lkCommon deps build ===========

REM Prepare build tree
call :setup_build_tree
if %ERRORLEVEL% GEQ 1 (
    if %ERRORLEVEL% EQU 1 (
        exit /b 0
    ) else (
        echo.
        echo Error while setting up build tree
        echo.
        exit /b %ERRORLEVEL%
    )
)

REM Create solutions for dependencies
call :cmake_create
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Failed to create build files from CMake
    echo.
    exit /b %ERORLEVEL%
)

cd %cmake_dir%

REM zlib
cd zlib
call :ms_build zlib zlibstatic
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Failed to build zlib
    echo.
    exit /b %ERORLEVEL%
)
cd ..

REM libpng
cd libpng
call :ms_build libpng png_static
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Failed to build libpng
    echo.
    exit /b %ERORLEVEL%
)
cd ..

REM fix zlib's artifacts
call :ms_build lkCommonDeps PostDepsBuild
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Failed to perform post deps build operations
    echo.
    exit /b %ERRORLEVEL%
)

echo.
echo Script is done
echo.

popd

exit /b 0



REM ========== Functions ==========

REM Checks if executable exists in PATH
REM   %1 - name of execuable to check
:path_check
echo | set /p=Checking for %1... 
for %%X in (%1) do (set FOUND=%%~$PATH:X)
if defined FOUND (
    echo FOUND
) else (
    echo NOT FOUND
    echo     Make sure %1 is visible in PATH env variable before using this script.
    exit /b 1
)

exit /b 0



REM Sets up build tree for dependencies. Returns 1 if tree is already done
:setup_build_tree
echo | set /p=Setting up build tree... 
set build_dir=%script_path%build
set bin_dir=%script_path%Bin
set cmake_dir=%build_dir%\%msbuild_platform%\%config%
set output_dir=%bin_dir%\%msbuild_platform%\%config%
if %clean% EQU 1 (
    REM if we cleanin, remove the build dir so it gets recreated later on
    >nul 2>nul rmdir /s /q "%cmake_dir%"
    >nul 2>nul dir /a-d /s "%build_dir%"
    if %ERRORLEVEL% EQU 1 (
        REM FIXME this does not enter when run through VS
        rmdir /s /q "%build_dir%"
    )
    >nul 2>nul rmdir /s /q "%output_dir%"
    >nul 2>nul dir /a-d /s "%bin_dir%"
    if %ERRORLEVEL% EQU 1 (
        REM FIXME this does not enter when run through VS
        rmdir /s /q "%bin_dir%"
    )
    echo CLEAN
    exit /b 1
)

REM check if build tree already exists and exit early if it does
REM TODO add check for build artifacts
if EXIST "%output_dir%" (
    echo ALREADY BUILT
    exit /b 1
)

mkdir "%cmake_dir%"
mkdir "%output_dir%"
echo DONE

exit /b 0




REM Builds given project from solution using msbuild. Forces output to predefined output dir.
REM   %1 - Solution to select project from.
REM   %2 - Project to be built. Can be omitted - then assumes same project name as solution.
:ms_build
if NOT "%2"=="" ( 
    set project=%2
    set solution=%1.sln
) else (
    set project=%1
    set solution=%project%.sln
)

echo %output_dir%
msbuild %solution% /t:%project%%target% /p:Configuration=%config%;Platform=%msbuild_platform% /p:OutDir="%output_dir%\\"
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Build failed
    echo.
    exit /b %ERRORLEVEL%
)

exit /b 0



REM Create build files from cmake
:cmake_create
cd %cmake_dir%

cmake ..\..\.. -DCMAKE_BUILD_TYPE=%config% -G %cmake_generator% -A %cmake_architecture%
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo CMake generation failed
    echo.
    exit /b %ERRORLEVEL%
)

exit /b 0
