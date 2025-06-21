@echo off
:: emsdk_setup.bat - Windows version of emsdk setup

:: Path to emsdk_env.bat in the Pixi environment
set "EMSDK_ENV=%CONDA_EMSDK_DIR%\emsdk_env.bat"

echo Checking emsdk_env.bat...
:: Check if emsdk_env.bat exists
if not exist "%EMSDK_ENV%" (
  echo Error: emsdk_env.bat not found at %EMSDK_ENV%
  exit /b 0
)

echo Sourcing Emscripten environment...
:: Run the environment script to set up PATH and other variables
call "%EMSDK_ENV%"

:: Check if emcc is available
echo Checking for emcc...
where emcc >nul 2>&1
if %ERRORLEVEL% equ 0 (
  for /f "delims=" %%i in ('where emcc') do echo emcc found at: %%i
) else (
  echo emcc not found in PATH
)

:: Check if emrun is available
echo Checking for emrun...
where emrun >nul 2>&1
if %ERRORLEVEL% equ 0 (
  for /f "delims=" %%i in ('where emrun') do echo emrun found at: %%i
) else (
  echo emrun not found in PATH
  exit /b 1
)