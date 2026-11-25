# Preparer l'installation (build + fichier pret + ouvre Chrome)
$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
& (Join-Path $PSScriptRoot "build.ps1")

$nwa = Join-Path $root "output\app.nwa"
$desk = [Environment]::GetFolderPath("Desktop")
$dest = Join-Path $desk "Autre.nwa"
Copy-Item -Force $nwa $dest
Set-Clipboard -Value $dest

$chrome = @(
    "$env:ProgramFiles\Google\Chrome\Application\chrome.exe",
    "${env:ProgramFiles(x86)}\Google\Chrome\Application\chrome.exe"
) | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $chrome) {
    Write-Host "Installez Google Chrome, puis ouvrez https://my.numworks.com/apps"
    exit 1
}

Start-Process $chrome @(
    "--new-window",
    "https://my.numworks.com/apps"
)

Write-Host ""
Write-Host "=== Autre pret ==="
Write-Host "Fichier : $dest"
Write-Host "Chemin copie dans le presse-papiers."
Write-Host ""
Write-Host "Sur la page Chrome : Connect -> NumWorks Calculator -> ajouter Autre.nwa -> Install"
Write-Host "(Gardez aussi les autres .nwa que vous voulez conserver.)"