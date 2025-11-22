# INSTALL — Como gerar o instalador do jogo (Bankov)

Este documento reúne, passo a passo, tudo que foi feito para gerar o instalador e o pacote portátil do jogo. Use-o para recriar o processo após uma formatação ou em outra máquina.

Pré-requisitos
- Windows (com privilégios para instalar softwares).
- MSYS2 (recomendo instalar de https://www.msys2.org).
- PowerShell (pwsh) para rodar scripts do repositório.
- Opcional: NSIS (para gerar instalador .exe) ou 7-Zip (para SFX).

1) Preparar o repositório
- Clone o repositório em `C:\Users\<seu-usuário>\Bankov` (aqui usaremos `C:\Users\wesley\Bankov`).
- Verifique que existem: `installer/package_for_installer.ps1`, `installer/BankovInstaller.nsi`.

2) Escolher variante MSYS2
- Use o atalho **MSYS2 UCRT64** (ou **MSYS2 MinGW 64-bit** para MinGW).
- No shell MSYS2 (bash): `echo $MSYSTEM` — deve retornar `UCRT64` ou `MINGW64`.

3) Atualizar MSYS2 e instalar Raylib (UCRT64)
- No shell **MSYS2 UCRT64**:
  ```bash
  pacman -Syu
  # se pedir reiniciar o shell, feche e reabra o atalho (UCRT64) e rode:
  pacman -Su

  pacman -S mingw-w64-ucrt-x86_64-raylib
  ```
- Se precisar compilar o raylib do source (fallback):
  ```bash
  pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-gcc
  cd /c/raylib/raylib
  mkdir -p build && cd build
  cmake -G "MinGW Makefiles" .. -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/ucrt64
  mingw32-make
  mingw32-make install
  ```
- Após instalar, `raylib.dll` normalmente ficará em `C:\msys64\ucrt64\bin\raylib.dll`.

4) Garantir compatibilidade
- Assegure que o executável do jogo foi compilado com a mesma toolchain (UCRT64/MINGW64) do `raylib.dll`.

5) Localizar e copiar DLLs necessárias
- DLLs comumente necessárias para distribuição (ex.: MinGW runtimes):
  - `raylib.dll`
  - `libwinpthread-1.dll`
  - `libgcc_s_seh-1.dll`
  - `libstdc++-6.dll`
- Exemplo de comandos PowerShell na raiz do repositório:
  ```powershell
  New-Item -ItemType Directory -Path .\installer\dist -Force | Out-Null
  Copy-Item "C:\msys64\ucrt64\bin\raylib.dll" -Destination ".\installer\dist\" -Force
  Copy-Item "C:\msys64\ucrt64\bin\libwinpthread-1.dll" -Destination ".\installer\dist\" -Force
  Copy-Item "C:\msys64\ucrt64\bin\libgcc_s_seh-1.dll" -Destination ".\installer\dist\" -Force
  Copy-Item "C:\msys64\ucrt64\bin\libstdc++-6.dll" -Destination ".\installer\dist\" -Force
  ```

6) Incluir o executável do jogo
- Coloque o `.exe` do jogo (ex.: `Bankov.exe` ou `jogo.exe`) dentro de `installer\dist\` ou deixe que o script solicite o caminho.

7) Rodar o script de empacotamento
- O repositório inclui `installer/package_for_installer.ps1` que automatiza a cópia e cria o zip.
- Execute no PowerShell (na raiz do repositório):
  ```powershell
  powershell -NoProfile -ExecutionPolicy Bypass -File .\installer\package_for_installer.ps1
  ```
- O script procura por DLLs em caminhos comuns e pedirá o diretório se algo estiver faltando (por exemplo, informe `C:\msys64\ucrt64\bin`).
- O resultado será `installer/Bankov-portable.zip` contendo o `.exe` e as DLLs.

8) Verificar o pacote
- Listar conteúdo da pasta `dist`:
  ```powershell
  Get-ChildItem .\installer\dist\ | Select-Object Name,Length
  Test-Path .\installer\Bankov-portable.zip
  ```
- Para inspecionar o ZIP localmente:
  ```powershell
  Expand-Archive -LiteralPath .\installer\Bankov-portable.zip -DestinationPath .\installer\_check -Force
  Get-ChildItem .\installer\_check\ | Select-Object Name,Length
  ```

9) Gerar instalador NSIS (.exe) — opcional
- Instale NSIS (se desejar um instalador `.exe`):
  - Manual: https://nsis.sourceforge.io/Download
  - Ou via `winget`/`choco` se disponível.
- Depois de instalado, rode (PowerShell):
  ```powershell
  & 'C:\Program Files (x86)\NSIS\makensis.exe' .\installer\BankovInstaller.nsi
  ```
- Observação: se `makensis` não estiver no PATH, use o caminho completo do executável.

10) Testar em máquina limpa/VM
- Copie `installer/Bankov-portable.zip` para uma máquina sem MSYS2 e execute o `.exe` do jogo.
- Se usar NSIS-installer, instale via `.exe` e teste.
- Se faltar DLLs, use `Dependency Walker` ou `dumpbin` para identificar dependências faltantes e inclua-as em `installer/dist`.

11) Checklist para recuperação após formatação
- Clonar o repositório em `C:\Users\<you>\Bankov`.
- Abrir MSYS2 UCRT64 e instalar `raylib` via `pacman` (ou reutilizar DLL do backup).
- Copiar DLLs para `installer/dist` ou executar `package_for_installer.ps1` e informar `C:\msys64\ucrt64\bin`.
- Gerar `installer/Bankov-portable.zip`.
- (Opcional) Instalar NSIS e gerar `.exe` com `makensis`.

12) Notas de manutenção técnica
- Algumas mudanças feitas no projeto durante o processo:
  - Pequenas refatorações em `Stock` e `FII` (mover métodos para `.cpp`).
  - Correção em `installer/package_for_installer.ps1` para tratar arrays vazios.
  - Inclusão de scripts auxiliares em `installer/` para busca/cópia de DLLs.

13) Problemas comuns e soluções rápidas
- `cmake: command not found` → instale `mingw-w64-ucrt-x86_64-cmake` no shell UCRT64 com `pacman -S`.
- `makensis: command not found` → instale NSIS manualmente ou via package manager.
- Executável não inicia na máquina alvo → verifique runtimes MinGW faltantes (`libstdc++-6.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`).

Se você quiser que eu crie este arquivo no repositório, eu já gerei `INSTALL.md` na raiz com este conteúdo.

---
*Gerado automaticamente com base no processo realizado no repositório Bankov.*
