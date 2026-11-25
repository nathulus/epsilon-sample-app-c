# Ouvre la page officielle NumWorks et copie le chemin du .nwa dans le presse-papiers.
$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
$nwa = Join-Path $root "output\app.nwa"

if (-not (Test-Path $nwa)) {
    Write-Host "Fichier absent. Compilation en cours..."
    & (Join-Path $PSScriptRoot "build.ps1")
}

$full = (Resolve-Path $nwa).Path
Set-Clipboard -Value $full
Write-Host "Chemin copie dans le presse-papiers :"
Write-Host $full
Write-Host ""
Write-Host "Ouverture de https://my.numworks.com/apps (Chrome ou Edge obligatoire)"
Start-Process "https://my.numworks.com/apps"