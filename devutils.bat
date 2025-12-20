@echo off

uv --version >nul 2>&1
if %errorlevel% equ 0 (
    echo Found uv
    goto UV_INSTALLED
) else (
    echo uv not found, installing it now
    echo This may take a while...
)

powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"

echo Refreshing environment variables...
call :RefreshEnv

uv python install 3.14

:UV_INSTALLED

if not exist .venv (
    echo Creating virtual environment...
    uv venv
    echo Installing requirements...
    uv pip install -r requirements.txt
) else (
    echo Virtual environment found, skipping setup
)

uv run devutils %*

pause
exit /b

:RefreshEnv
:: Refresh PATH from registry
for /f "tokens=2*" %%a in ('reg query "HKLM\System\CurrentControlSet\Control\Session Manager\Environment" /v PATH 2^>nul') do set "SysPath=%%b"
for /f "tokens=2*" %%a in ('reg query "HKCU\Environment" /v PATH 2^>nul') do set "UserPath=%%b"
set "PATH=%SysPath%;%UserPath%"
exit /b
