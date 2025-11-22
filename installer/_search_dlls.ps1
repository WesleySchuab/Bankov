Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$dlls = @('raylib.dll','libwinpthread-1.dll','libgcc_s_seh-1.dll','libstdc++-6.dll')
$found = @{}

Function Check-Paths([string[]]$paths) {
    foreach ($p in $paths) {
        if (-not (Test-Path $p)) { continue }
        foreach ($d in $dlls) {
            $f = Join-Path $p $d
            if (Test-Path $f) {
                $found[$d] = $f
                Write-Host "FOUND: $d -> $f"
            }
        }
    }
}

Write-Host "Checking PATH entries and common locations..."
$paths = $env:PATH -split ';' | ForEach-Object { $_.Trim() } | Where-Object { $_ -ne '' } | Select-Object -Unique
$common = @('C:\msys64\mingw64\bin','C:\msys64\mingw32\bin','C:\w64devkit\mingw64\bin','C:\w64devkit\mingw32\bin','C:\Program Files\raylib\bin','C:\Program Files (x86)\raylib\bin','C:\msys64\usr\bin')
$all = $paths + $common | Select-Object -Unique
Check-Paths $all

if ($found.Count -eq 0) {
    Write-Host "No results in PATH/common. Will attempt recursive search in C:\msys64 and C:\w64devkit (may take time)..."
    if (Test-Path 'C:\msys64') {
        foreach ($d in $dlls) {
            Get-ChildItem -Path 'C:\msys64' -Filter $d -Recurse -ErrorAction SilentlyContinue -Force | ForEach-Object { $found[$d] = $_.FullName; Write-Host "FOUND recursive: $d -> $($_.FullName)" }
        }
    }
    if (Test-Path 'C:\w64devkit') {
        foreach ($d in $dlls) {
            Get-ChildItem -Path 'C:\w64devkit' -Filter $d -Recurse -ErrorAction SilentlyContinue -Force | ForEach-Object { $found[$d] = $_.FullName; Write-Host "FOUND recursive: $d -> $($_.FullName)" }
        }
    }
}

if ($found.Count -gt 0) {
    Write-Host "\nSummary found:";
    $found.GetEnumerator() | ForEach-Object { Write-Host " $($_.Key) => $($_.Value)" }
} else {
    Write-Host "\nNenhuma DLL encontrada nos caminhos verificados."
}
