# Compilation Windows (sans GNU make)
$ErrorActionPreference = "Stop"
Set-Location (Split-Path $PSScriptRoot -Parent)

$CC = "arm-none-eabi-gcc"
$NWLINK = "npx --yes -- nwlink@0.0.19"
$BUILD = "output"

if (-not (Get-Command $CC -ErrorAction SilentlyContinue)) {
    Write-Error "arm-none-eabi-gcc introuvable. Installez Arm.GnuArmEmbeddedToolchain (winget)."
}

$cflagsRaw = Invoke-Expression "$NWLINK eadk-cflags-device"
$cflags = $cflagsRaw -split '\s+' | Where-Object { $_ }

$sources = @(
    "src/main.c",
    "src/app.c",
    "src/menu/menu.c",
    "src/ui/ui.c",
    "src/modules/formules/formules.c"
)

# Flags communs (PAS de -fwhole-program : plusieurs .c, sinon plantage sur calculette)
$common = @(
    "-Isrc", "-Os", "-Wall", "-ggdb", "-std=c99",
    "-fdata-sections", "-ffunction-sections"
)

New-Item -ItemType Directory -Force -Path $BUILD | Out-Null

$objects = @()
foreach ($src in $sources) {
    $rel = $src.Substring(4) -replace '\.c$', '.o'
    $obj = Join-Path $BUILD ($rel -replace '\\', '/')
    $objDir = Split-Path $obj -Parent
    if ($objDir) { New-Item -ItemType Directory -Force -Path $objDir | Out-Null }

    Write-Host "CC $src"
    & $CC @cflags @common -c $src -o $obj
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    $objects += $obj
}

$iconSrc = "assets/icon/icon.png"
$iconObj = Join-Path $BUILD "icon.o"
Write-Host "ICON $iconSrc"
& cmd /c "$NWLINK png-icon-o $iconSrc $iconObj"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$nwa = Join-Path $BUILD "app.nwa"
Write-Host "LD $nwa"
& $CC @cflags @common `
    "-Wl,--relocatable" "-nostartfiles" "--specs=nano.specs" `
    "-Wl,-e,main" "-Wl,-u,eadk_app_name" "-Wl,-u,eadk_app_icon" "-Wl,-u,eadk_api_level" `
    "-Wl,--gc-sections" `
    @objects $iconObj -o $nwa

if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Write-Host "OK -> $nwa"