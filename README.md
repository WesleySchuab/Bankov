# Ações Imobiliárias

Um jogo desenvolvido em C++ utilizando a biblioteca Raylib.

## 📋 Descrição

[Adicione aqui uma breve descrição do seu jogo]

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C++17
- **Framework:** Raylib
- **Build System:** CMake

## 📁 Estrutura do Projeto

```
Bankov/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── Game.hpp
│   └── assets/
│       └── fonts/
└── README.md
```

## 🚀 Como Compilar

### Pré-requisitos
- CMake (versão 3.10 ou superior)
- Compilador C++17
- Raylib instalado

### Passos para compilar

1. Clone o repositório:
```bash
git clone [URL_DO_SEU_REPOSITORIO]
cd Bankov
```

2. Crie o diretório de build:
```bash
mkdir build
cd build
```

3. Configure com CMake:
# Bankov — Ações Imobiliárias

Jogo desenvolvido em C++ que simula operações de compra/venda de propriedades e investimentos (Ações/Stocks). A interface gráfica usa a biblioteca Raylib e o projeto é construído com CMake.

**Status:** Código fonte disponível. Instruções de compilação e execução abaixo.

**Dica:** este README está em Português — ajuste conforme preferir.

**Funcionalidades principais**
- Comprar e vender propriedades
- Investimentos em ações/ativos
- Turnos de jogadores e eventos aleatórios

**Tecnologias**
- Linguagem: `C++17`
- Biblioteca gráfica: `raylib`
- Sistema de build: `CMake`

**Estrutura do repositório (resumida)**
```
Bankov/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── Game.cpp / Game.hpp
│   ├── Board.cpp / Board.hpp
│   ├── Player.cpp / Player.hpp
│   ├── Property.cpp / Property.hpp
│   └── assets/
│       ├── fonts/
│       └── images/
└── build/    (diretório gerado pelo CMake)
```

**Pré-requisitos**
- `CMake` (3.10+ recomendado)
- Compilador com suporte a C++17 (MSVC, GCC, Clang)
- `raylib` instalado na sua máquina (ou via gerenciador de pacotes)

Se você estiver no Windows, pode instalar o `raylib` via `vcpkg`, `msys2` ou compilá-lo manualmente. No Linux/macOS use o gerenciador de pacotes da sua distro ou compile a partir das fontes.

## Como compilar (instruções rápidas)

Abra um terminal (`pwsh.exe` no Windows) na raiz do projeto `C:\Users\wesley\Bankov`.

1) Clone (se ainda não tiver):
```powershell
git clone --recurse-submodules git@github.com:WesleySchuab/Bankov.git
cd Bankov
```

2) Criar diretório de build e configurar com CMake:
```powershell
mkdir build
cd build
cmake ..
```

Se você usa MSVC no Windows e quer gerar solução Visual Studio, especifique o gerador (exemplo):
```powershell
# Exemplo: usar Visual Studio 2019 x64
cmake -G "Visual Studio 16 2019" -A x64 ..
```

3) Compilar:
```powershell
# build recomendado (todos os geradores):
cmake --build . --config Release
```

4) Executar o binário gerado (exemplo path relativo ao build):
```powershell
# ajuste o caminho se o executável estiver em outra pasta
./bin/Bankov.exe
# ou, no caso de gerar com Visual Studio
./Release/Bankov.exe
```

## Controles e Jogabilidade
- Use as teclas direcionais ou o mouse para navegar nos menus (dependendo da implementação)
- Durante o jogo, siga as instruções na tela para comprar/vender ou terminar o turno

Observação: os controles exatos dependem da implementação em `src/` — consulte `Game.cpp` e `main.cpp` para detalhes.

## Desenvolvimento

Como contribuir localmente:
1. Crie uma branch para sua feature/bugfix:
```powershell
git checkout -b feat/nome-da-feature
```
2. Faça mudanças, compile e teste localmente.
3. Commit e push:
```powershell
git add .
git commit -m "Descrição curta da mudança"
git push -u origin feat/nome-da-feature
```
4. Abra um Pull Request no GitHub.

## Problemas comuns
- Erro de `raylib` não encontrado: verifique se `raylib` está instalado e se `CMAKE_PREFIX_PATH` ou variáveis de sistema apontam para a instalação.
- Erro no `cmake`: tente limpar `build` e rodar `cmake ..` de novo.

## Licença
Adicione aqui a licença do projeto (por exemplo: `MIT`, `GPL-3.0` etc.). Se ainda não escolheu, uma opção comum é a `MIT`.

## Autor
Wesley Schuab — contato: [seu-email@exemplo.com]

---

Se quiser, eu posso:
- ajustar o `README.md` para incluir exemplos de build específicos para `vcpkg`/`msys2` no Windows;
- criar um `CONTRIBUTING.md` com regras de contribuição;
- rodar o build aqui para verificar se compila (preciso que confirme para executar comandos no terminal).

Diga qual dessas opções prefere ou se quer que eu apenas finalize o `README.md` com outras informações específicas.
