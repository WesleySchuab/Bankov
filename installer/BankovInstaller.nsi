; BankovInstaller.nsi
; Script NSIS para criar instalador do jogo Bankov
; Requer: makensis (NSIS) instalado no sistema

; Informações básicas
!define APP_NAME "Bankov"
!define APP_EXE "jogo.exe"
!define APP_VERSION "1.0"

Name "${APP_NAME} ${APP_VERSION}"
OutFile "${APP_NAME}-Installer-${APP_VERSION}.exe"
InstallDir "$PROGRAMFILES\\${APP_NAME}"
RequestExecutionLevel user

; Páginas
Page directory
Page instfiles

Section "Install"
    SetOutPath "$INSTDIR"
    ; Copiar todos os arquivos da pasta dist para o diretório de instalação
    File /r "dist\*.*"

    ; Criar atalho no Start Menu
    CreateDirectory "$SMPROGRAMS\\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\\${APP_NAME}\\${APP_NAME}.lnk" "$INSTDIR\\${APP_EXE}"

    ; Opcional: criar atalho na área de trabalho
    CreateShortCut "$DESKTOP\\${APP_NAME}.lnk" "$INSTDIR\\${APP_EXE}"
SectionEnd

Section "Uninstall"
    Delete "$SMPROGRAMS\\${APP_NAME}\\${APP_NAME}.lnk"
    RMDir "$SMPROGRAMS\\${APP_NAME}"
    Delete "$DESKTOP\\${APP_NAME}.lnk"
    ; Remover os arquivos instalados
    Delete "$INSTDIR\\*.*"
    RMDir "$INSTDIR"
SectionEnd

; Nota: execute `makensis BankovInstaller.nsi` dentro da pasta installer/ onde existe dist/ com os arquivos.
