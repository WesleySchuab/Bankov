<#
  package_for_installer.ps1
  Uso: execute este script no PowerShell (pode usar pwsh) a partir da raiz do projeto.
  Ele vai criar a pasta `dist/` contendo `jogo.exe` e as DLLs necessárias para distribuir.

  O script tenta localizar as DLLs do MinGW/MSYS2 em caminhos comuns. Se não encontrar,
  pedirá que você informe o caminho onde estão as DLLs (ex.: C:\msys64\mingw64\bin).
#>

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$root = Resolve-Path .
Write-Host "Project root: $root"

# Local do executável (ajuste se usado outro nome/pasta)
$exeCandidates = @("jogo.exe", "bin/Bankov.exe", "Release/Bankov.exe", "./Release/Bankov.exe")
$exePath = $null
foreach ($c in $exeCandidates) {
    $p = Join-Path $root $c
    if (Test-Path $p) { $exePath = $p; break }
}
if (-not $exePath) {
    Write-Host "Executável não encontrado nas localizações padrão. Informe o caminho completo para o executável (ex: C:\Users\wesley\Bankov\jogo.exe):"
    $exePath = Read-Host "Caminho do executável"
    if (-not (Test-Path $exePath)) { throw "Executável não encontrado em $exePath" }
}

Write-Host "Usando executável: $exePath"

# DLLs que normalmente são necessárias ao usar MinGW/w64 (ajuste conforme necessário)
$dllNames = @(
    'raylib.dll',
    'libwinpthread-1.dll',
    'libgcc_s_seh-1.dll',
    'libstdc++-6.dll'
)

# Tentar localizar em caminhos comuns
$commonPaths = @(
    'C:\msys64\mingw64\bin',
    'C:\msys64\mingw32\bin',
    'C:\msys64\usr\bin',
    'C:\Program Files\raylib\bin',
    'C:\Program Files (x86)\raylib\bin'
)

$foundDlls = @{}
foreach ($p in $commonPaths) {
    foreach ($d in $dllNames) {
        $full = Join-Path $p $d
        if ((-not $foundDlls.ContainsKey($d)) -and (Test-Path $full)) {
            $foundDlls[$d] = $full
        }
    }
}

# Se alguma DLL não foi encontrada, pedir ao usuário o diretório das DLLs
$missing = $dllNames | Where-Object { -not $foundDlls.ContainsKey($_) }
if ($missing.Count -gt 0) {
    Write-Host "Algumas DLLs não foram encontradas em caminhos comuns: $($missing -join ', ')"
    $dllFolder = Read-Host "Informe a pasta onde as DLLs do MinGW/MSYS2 estão (ex: C:\msys64\mingw64\bin) ou deixe vazio para procurar manualmente"
    if (-not [string]::IsNullOrWhiteSpace($dllFolder)) {
        foreach ($d in $missing) {
            $candidate = Join-Path $dllFolder $d
            if (Test-Path $candidate) { $foundDlls[$d] = $candidate }
        }
    }
}

# Final check
$stillMissing = $dllNames | Where-Object { -not $foundDlls.ContainsKey($_) }
if ($stillMissing.Count -gt 0) {
    Write-Warning "Ainda faltam DLLs: $($stillMissing -join ', '). Você pode prosseguir, mas o instalador pode não funcionar em máquinas alvo sem essas DLLs."
    $proceed = Read-Host "Deseja continuar mesmo assim? (S/n)"
    if ($proceed -eq 'n' -or $proceed -eq 'N') { throw "Cancelado pelo usuário" }
}

# Criar dist/
$dist = Join-Path $root 'installer\dist'
if (Test-Path $dist) { Remove-Item $dist -Recurse -Force }
New-Item -ItemType Directory -Path $dist | Out-Null

# Copiar executável
Copy-Item $exePath -Destination $dist -Force

# Copiar DLLs encontradas
foreach ($kv in $foundDlls.GetEnumerator()) {
    $dst = Join-Path $dist $kv.Key
    Copy-Item $kv.Value -Destination $dst -Force
    Write-Host "Copiado: $($kv.Key)"
}

Write-Host "Conteúdo de dist/"
Get-ChildItem $dist | ForEach-Object { Write-Host $_.Name }

Write-Host "Criando arquivo zip 'installer\Bankov-portable.zip'..."
if (Test-Path (Join-Path $root 'installer\Bankov-portable.zip')) { Remove-Item (Join-Path $root 'installer\Bankov-portable.zip') -Force }
Compress-Archive -Path (Join-Path $dist '*') -DestinationPath (Join-Path $root 'installer\Bankov-portable.zip')
Write-Host "Zip criado: installer\Bankov-portable.zip"

Write-Host "Pronto. Agora você pode usar 'installer/BankovInstaller.nsi' com o NSIS (makensis) para gerar um instalador .exe a partir do conteúdo de 'installer/dist/'."
