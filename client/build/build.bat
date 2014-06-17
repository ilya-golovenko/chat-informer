@ECHO OFF

pushd %~DP0\..
SET INFORMER_ROOT=%CD%
popd

IF NOT EXIST "%INFORMER_ROOT%\Jamroot" (
    ECHO Could not find Jamroot file required to build missio informer
    EXIT /b 1
)

:: Check if BOOST_ROOT is set
IF NOT DEFINED BOOST_ROOT (
    ECHO Required environmental variable BOOST_ROOT is not set
    ECHO Please set it to point to the root of BOOST distribution
    EXIT /b 1
)

:: Remove quotes
SET BOOST_ROOT=%BOOST_ROOT:"=%

:: Remove trailing backslash
IF "%BOOST_ROOT:~-1%" == "\" (
    SET BOOST_ROOT=%BOOST_ROOT:~0,-1%
)

ECHO INFORMER_ROOT = %INFORMER_ROOT%
ECHO BOOST_ROOT = %BOOST_ROOT%

SET BOOST_JAM=%BOOST_ROOT%\bjam.exe
SET BOOST_JAM_LOG=%BOOST_ROOT%\bjam.log
SET BOOST_BUILD_PATH=%BOOST_ROOT%\tools\build\v2

IF NOT EXIST "%BOOST_JAM%" (
    IF NOT EXIST "%BOOST_ROOT%\bootstrap.bat" (
        ECHO Could not find file bootstrap.bat required to build Boost.Jam build engine
        EXIT /b 1
    )

    pushd "%BOOST_ROOT%"

    ECHO Building Boost.Jam build engine
    CALL bootstrap.bat > NUL

    popd

    IF ERRORLEVEL 1 (
        ECHO Error building Boost.Jam build engine
        ECHO See %BOOST_JAM_LOG% for details
        EXIT /b 1
    )
)

pushd "%INFORMER_ROOT%"

SET JOBS_NUM=%NUMBER_OF_PROCESSORS%

SET BOOST_ROOT="%BOOST_ROOT%"
SET BOOST_BUILD_PATH="%BOOST_BUILD_PATH%"

ECHO Starting build of missio informer
"%BOOST_JAM%" -j%JOBS_NUM% toolset=msvc %*

popd
