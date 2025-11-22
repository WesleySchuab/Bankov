<#
  search_and_copy_raylib.ps1
  Procura por raylib.dll em caminhos prováveis e copia para installer/dist
  Uso: pwsh -NoProfile -ExecutionPolicy Bypass -File installer\search_and_copy_raylib.ps1
#>
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$paths = @(
  'C:\raylib',
  'C:\msys64\mingw64\bin',
  'C:\msys64\ucrt64\bin',
  'C:\msys64',
  'C:\w64devkit',
  'C:\Program Files',
  'C:\Program Files (x86)',
  'C:\Users',
  'C:\'
)

$root = (Resolve-Path '.').Path
$target = Join-Path $root 'installer\dist'
if (Test-Path $target) { Remove-Item $target -Recurse -Force }
New-Item -ItemType Directory -Path $target | Out-Null

$dll = 'raylib.dll'
$found = $null

foreach ($p in $paths) {
    if (-not (Test-Path $p)) { continue }
    Write-Host "Searching in: $p"
    try {
        $res = Get-ChildItem -Path $p -Filter $dll -Recurse -ErrorAction SilentlyContinue -Force | Select-Object -First 1
    } catch {
        $res = $null
    }
    if ($res) {
        $found = $res.FullName
        Write-Host "FOUND: $found"
        break
    }
}

if ($found) {
    Copy-Item $found -Destination $target -Force
    Write-Host "COPIED: $found -> $target"
    $zip = Join-Path $root 'installer\Bankov-portable.zip'
    if (Test-Path $zip) { Remove-Item $zip -Force }
    Compress-Archive -Path (Join-Path $target '*') -DestinationPath $zip
    Write-Host "ZIP created: $zip"
    Write-Host "Done."
    exit 0
} else {
    Write-Host "raylib.dll not found in searched locations."
    exit 1
}
