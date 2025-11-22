Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$base = 'C:\MinGW\bin'
Write-Host "Checking folder: $base"
if (-not (Test-Path $base)) { Write-Host "Folder not found: $base"; exit 2 }

$dlls = @('raylib.dll','libwinpthread-1.dll','libgcc_s_seh-1.dll','libstdc++-6.dll')
$root = (Resolve-Path '..\' ).Path.TrimEnd('\')
$dist = Join-Path $root 'installer\dist'
if (Test-Path $dist) { Remove-Item $dist -Recurse -Force }
New-Item -ItemType Directory -Path $dist | Out-Null

$found = @(); $missing = @()
foreach ($d in $dlls) {
    $p = Join-Path $base $d
    if (Test-Path $p) {
        Copy-Item $p -Destination $dist -Force
        Write-Host "COPIED: $d"
        $found += $p
    } else {
        Write-Host "MISSING: $d"
        $missing += $d
    }
}

if ($found.Count -gt 0) {
    $zip = Join-Path $root 'installer\Bankov-portable.zip'
    if (Test-Path $zip) { Remove-Item $zip -Force }
    Compress-Archive -Path (Join-Path $dist '*') -DestinationPath $zip
    Write-Host "ZIP created: $zip"
} else { Write-Host "No DLLs copied; zip not created" }

Write-Host "Summary Found: $($found -join ', ') Missing: $($missing -join ', ')"
