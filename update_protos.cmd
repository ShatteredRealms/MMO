@echo off
setlocal enabledelayedexpansion

pushd Plugins\TurboLink\Tools
set TOOLS_DIR=%cd%
set SCRIPT=%TOOLS_DIR%\generate_code.cmd
popd

pushd Protos
set PROTOS_DIR=%cd%
popd

pushd Plugins\TurboLink\Source\TurboLinkGrpc
set OUTPUT_DIR=%cd%
popd

for /F "delims=" %%f in ('dir "%PROTOS_DIR%\*.proto" /b /s') do (
    call "%SCRIPT%" "%%f" "%OUTPUT_DIR%" "%PROTOS_DIR%"
)