try {
    $null = Get-Command uv -ErrorAction Stop
    Write-Host "Found uv" -ForegroundColor Green
}
catch {
    Write-Host "uv not found, installing it now" -ForegroundColor Yellow
    Write-Host "This may take a while..." -ForegroundColor Yellow

    Invoke-RestMethod https://astral.sh/uv/install.ps1 | Invoke-Expression

    Write-Host "Refreshing environment variables..." -ForegroundColor Yellow
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")

    # Install Python 3.14
    uv python install 3.14
}

if (-not (Test-Path ".venv")) {
    Write-Host "Creating virtual environment..." -ForegroundColor Yellow
    uv venv
    Write-Host "Installing requirements..." -ForegroundColor Yellow
    uv pip install -r requirements.txt
}
else {
    Write-Host "Virtual environment found, skipping setup" -ForegroundColor Green
}

uv run devutils $args

Write-Host "`nPress any key to continue..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
