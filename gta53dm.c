#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <psapi.h>
#include <process.h>
#include <direct.h>
#include <shellapi.h>

// SI SOS PROGRAMADOR BIEN HECHO EN C SEGURAMENTE VEAS ESTE CODIGO Y DIGAS QUE ES UN PAPELON!
// Y SI LO ES! PERO BUENO ES LO QUE HAY. ASI QUE SI ME QUERES AYUDAR A MEJORARLO, MANDAME UN MAIL!
// CONSULTASOPTI@GMAIL.COM - GRACIAS OPTIJUEGOS

// Función para cargar la DLL objetivo en el proceso actual
BOOL loadTargetDLL(const char* dllPath) {
    if (!dllPath) {
        return FALSE;
    }
    
    // Cargar la DLL directamente en el proceso actual
    HMODULE hModule = LoadLibraryA(dllPath);
    
    if (!hModule) {
        // Si falla, obtener el código de error
        DWORD error = GetLastError();
        // Aquí podrías registrar el error o mostrar un mensaje
        return FALSE;
    }
    
    return TRUE;
}

// Hilo que carga la DLL objetivo después de un pequeño retraso
DWORD WINAPI LoaderThread(LPVOID lpParam) {
    // Esperar un momento para asegurar que el proceso esté completamente inicializado
    Sleep(1000);
    
    char* dllPath = (char*)lpParam;
    return loadTargetDLL(dllPath);
}

void MessageCrackDeleted() {
    // Mensaje según el idioma
    LANGID langID = GetUserDefaultUILanguage();
    LANGID primaryLang = langID & 0x3FF;  // Extraer el idioma primario
    const char *message;
    
    switch (primaryLang) {
        case 0x0A:  // LANG_SPANISH
            message = "Tu antivirus ha eliminado el crack del juego\n\n"
                      "Te recomendamos desactivar el antivirus para evitar problemas al iniciar el proyecto. "
                      "El crack se extraerá nuevamente para poder abrir el ejecutable.\n\n"
                      "Más proyectos interesantes en: https://optijuegos.github.io!";
            break;
        case 0x16:  // LANG_PORTUGUESE
            message = "Seu antivírus removeu o crack do jogo\n\n"
                      "Recomendamos desativar o antivírus para evitar problemas ao iniciar o projeto. "
                      "O crack será extraído novamente para poder abrir o executável.\n\n"
                      "Mais projetos interessantes em: https://optijuegos.github.io!";
            break;
        case 0x19:  // LANG_RUSSIAN
            message = "Ваш антивирус удалил кряк игры\n\n"
                      "Мы рекомендуем отключить антивирус, чтобы избежать проблем при запуске проекта. "
                      "Кряк будет извлечен повторно для возможности открытия исполняемого файла.\n\n"
                      "Больше интересных проектов на: https://optijuegos.github.io!";
            break;
        case 0x05:  // LANG_CZECH
            message = "Váš antivirový program odstranil crack hry\n\n"
                      "Doporučujeme deaktivovat antivirový program, abyste se vyhnuli problémům při spuštění projektu. "
                      "Crack bude znovu extrahován, aby bylo možné otevřít spustitelný soubor.\n\n"
                      "Více zajímavých projektů na: https://optijuegos.github.io!";
            break;
        case 0x07:  // LANG_GERMAN
            message = "Ihr Antivirenprogramm hat den Spielecrack entfernt\n\n"
                      "Wir empfehlen, den Virenschutz zu deaktivieren, um Probleme beim Starten des Projekts zu vermeiden. "
                      "Der Crack wird erneut extrahiert, um die ausführbare Datei öffnen zu können.\n\n"
                      "Weitere interessante Projekte unter: https://optijuegos.github.io!";
            break;
        case 0x0C:  // LANG_FRENCH
            message = "Votre antivirus a supprimé le crack du jeu\n\n"
                      "Nous vous recommandons de désactiver l'antivirus pour éviter les problèmes lors du démarrage du projet. "
                      "Le crack sera à nouveau extrait pour pouvoir ouvrir l'exécutable.\n\n"
                      "Plus de projets intéressants sur : https://optijuegos.github.io!";
            break;
        case 0x10:  // LANG_ITALIAN
            message = "Il tuo antivirus ha rimosso il crack del gioco\n\n"
                      "Ti consigliamo di disattivare l'antivirus per evitare problemi all'avvio del progetto. "
                      "Il crack verrà estratto nuovamente per poter aprire l'eseguibile.\n\n"
                      "Altri progetti interessanti su: https://optijuegos.github.io!";
            break;
        default:  // ENGLISH
            message = "Your antivirus has deleted the game crack\n\n"
                      "We recommend disabling your antivirus to avoid issues when starting the project. "
                      "The crack will be extracted again to open the executable.\n\n"
                      "More interesting projects at: https://optijuegos.github.io!";
            break;
    }
    
    MessageBoxA(NULL, message, "OptiProjects", MB_OK | MB_ICONERROR);
}

// Comprobar si el crack fue eliminado
void CheckCrack() {
	
	const char* filesToCheck[] = {
		"3dmgame.dll"
    };
    
  bool fileExists(const char* filename) {
        DWORD fileAttr = GetFileAttributesA(filename);
        if (fileAttr == INVALID_FILE_ATTRIBUTES) {
            MessageCrackDeleted();
			return false;
        }
        return !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
    }
    
    bool executeHidden(const char* command) {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE; // Ocultar la ventana
        
        ZeroMemory(&pi, sizeof(pi));
        
        // Crear una copia del comando ya que CreateProcessA puede modificar el string
        char cmdCopy[MAX_PATH * 2 + 100];
        strncpy(cmdCopy, command, sizeof(cmdCopy) - 1);
        cmdCopy[sizeof(cmdCopy) - 1] = '\0';
        
        // Crear el proceso
        if (!CreateProcessA(NULL, cmdCopy, NULL, NULL, FALSE, 
                           CREATE_NO_WINDOW, // Evitar crear ventana
                           NULL, NULL, &si, &pi)) {
            return false;
        }
        
        // Esperar a que el proceso termine
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        // Obtener el código de salida
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        // Cerrar los handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        return (exitCode == 0);
    }
    
    void extract7z() {
        char currentDir[MAX_PATH];
        GetModuleFileNameA(NULL, currentDir, sizeof(currentDir));
        
        // Obtener la ruta de la carpeta de la DLL
        char* lastSlash = strrchr(currentDir, '\\');
        if (lastSlash) {
            *lastSlash = '\0'; // Cortar la ruta para obtener solo el directorio
        }
        
        // Construir la ruta completa para 7z.exe y crack.7z
        char sevenZipPath[MAX_PATH];
        char archivePath[MAX_PATH];
        snprintf(sevenZipPath, sizeof(sevenZipPath), "%s\\bonus\\7z.exe", currentDir);
        snprintf(archivePath, sizeof(archivePath), "%s\\bonus\\crack\\crack_123.7z", currentDir);
        
        // Crear el comando para ejecutar
        char command[MAX_PATH * 2 + 100];
        snprintf(command, sizeof(command), "\"%s\" x \"%s\" -p123 -aoa -y -o\"%s\"", 
                 sevenZipPath, archivePath, currentDir);
        
        // Ejecutar 7z.exe sin mostrar ventana
        executeHidden(command);
    }
    
    bool allExist = true;
    for (int i = 0; i < sizeof(filesToCheck) / sizeof(filesToCheck[0]); i++) {
        if (!fileExists(filesToCheck[i])) {
            allExist = false;
            break;
        }
    }
    
    if (!allExist) {
        extract7z();
    }
}

// Función principal para configurar y ejecutar
void setupAndExecute() {
    char currentDir[MAX_PATH];
    char configPath[MAX_PATH];
    char dllPath[MAX_PATH];
    char exeName[MAX_PATH];
    
    // Obtener la ruta del ejecutable actual
    GetModuleFileNameA(NULL, currentDir, MAX_PATH);
    
    // Extraer el directorio
    char* lastBackslash = strrchr(currentDir, '\\');
    if (lastBackslash) {
        *(lastBackslash + 1) = '\0';
    }
    
    // Construir rutas
    sprintf(dllPath, "%s%s", currentDir, "3dmgame.dll"); // La DLL que queremos cargar
    sprintf(configPath, "%s%s", currentDir, "3dmgame.ini"); // Archivo de configuración
    
    // Leer configuración si es necesario
    GetPrivateProfileString("Settings", "ExeName", "GlowStorm", exeName, MAX_PATH, configPath);
    
    // Crear una copia del path para pasar al hilo
    char* dllPathCopy = (char*)malloc(strlen(dllPath) + 1);
    if (dllPathCopy) {
        strcpy(dllPathCopy, dllPath);
        
        HANDLE hThread = CreateThread(NULL, 0, LoaderThread, dllPathCopy, 0, NULL);
        if (hThread) {
            // No esperamos a que termine, solo cerramos el handle
            CloseHandle(hThread);
        } else {
            // Liberar memoria si falla la creación del hilo
            free(dllPathCopy);
        }
    }
}

// (winlator)
void loadAsiLoader() {
    char currentDir[MAX_PATH];
    char socialClubPath[MAX_PATH];
    
    // Obtener la ruta del ejecutable actual
    GetModuleFileNameA(NULL, currentDir, MAX_PATH);
    
    // Extraer el directorio
    char* lastBackslash = strrchr(currentDir, '\\');
    if (lastBackslash) {
        *(lastBackslash + 1) = '\0';
    }
    
    sprintf(socialClubPath, "%s%s", currentDir, "asiloader.dll");
    
    // Cargar la DLL
    HMODULE hModule = LoadLibraryA(socialClubPath);
    
    if (!hModule) {
        // Si falla, mostrar un mensaje de error
        MessageBoxA(NULL, "Error asiloader.dll", "Error", MB_OK | MB_ICONERROR);
    }
}

// Función para mostrar mensajes multilingües
void ShowSocialClubMessage() {
    // Mensaje según el idioma
    LANGID langID = GetUserDefaultUILanguage();
    LANGID primaryLang = langID & 0x3FF;  // Extraer el idioma primario
    const char *message;
    
    switch (primaryLang) {
        case 0x0A:  // LANG_SPANISH
            message = "Rockstar Games Social Club no está instalado en su sistema.\n\n"
                     "Este componente es necesario para iniciar el juego. La instalación continuará.";
            break;
        case 0x16:  // LANG_PORTUGUESE
            message = "Rockstar Games Social Club não está instalado no seu sistema.\n\n"
                     "Este componente é necessário para iniciar o jogo. A instalação prosseguirá.";
            break;
        case 0x19:  // LANG_RUSSIAN
            message = "Rockstar Games Social Club не установлен в вашей системе.\n\n"
                     "Этот компонент необходим для запуска игры. Установка будет продолжена.";
            break;
        case 0x05:  // LANG_CZECH
            message = "Rockstar Games Social Club není nainstalován ve vašem systému.\n\n"
                     "Tato komponenta je nutná pro spuštění hry. Instalace bude pokračovat.";
            break;
        case 0x07:  // LANG_GERMAN
            message = "Rockstar Games Social Club ist auf Ihrem System nicht installiert.\n\n"
                     "Diese Komponente ist erforderlich, um das Spiel zu starten. Die Installation wird fortgesetzt.";
            break;
        case 0x0C:  // LANG_FRENCH
            message = "Rockstar Games Social Club n'est pas installé sur votre système.\n\n"
                     "Ce composant est nécessaire pour démarrer le jeu. L'installation va se poursuivre.";
            break;
        case 0x10:  // LANG_ITALIAN
            message = "Rockstar Games Social Club non è installato sul tuo sistema.\n\n"
                     "Questo componente è necessario per avviare il gioco. L'installazione procederà.";
            break;
        default:  // ENGLISH
            message = "Rockstar Games Social Club is not installed on your system.\n\n"
                     "This component is required to start the game. Installation will proceed.";
            break;
    }
    
    MessageBoxA(NULL, message, "Social Club", MB_OK | MB_ICONERROR);
}

void CheckRGSC() {
    char socialClubPath[MAX_PATH];
    char installPath[MAX_PATH];
    char commandLine[MAX_PATH + 50];
    BOOL is64bit = FALSE;
    
    // Verificar si el sistema es 64 bits
    #ifdef _WIN64
        is64bit = TRUE;
    #else
        // Verificación adicional para sistemas de 32 bits en hardware de 64 bits
        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle("kernel32"), "IsWow64Process");
        
        if (fnIsWow64Process != NULL) {
            fnIsWow64Process(GetCurrentProcess(), &is64bit);
        }
    #endif
    
    // Construir la ruta adecuada según la arquitectura
    if (is64bit) {
        // En 64 bits, usar Program Files
        sprintf(socialClubPath, "%s\\Rockstar Games\\Social Club\\socialclub.dll", 
                getenv("ProgramFiles"));
    } else {
        // En 32 bits, usar Program Files (x86) si existe, o Program Files si no
        if (getenv("ProgramFiles(x86)") != NULL) {
            sprintf(socialClubPath, "%s\\Rockstar Games\\Social Club\\socialclub.dll", 
                    getenv("ProgramFiles(x86)"));
        } else {
            sprintf(socialClubPath, "%s\\Rockstar Games\\Social Club\\socialclub.dll", 
                    getenv("ProgramFiles"));
        }
    }
    
    // Verificar si el archivo existe
    if (GetFileAttributes(socialClubPath) == INVALID_FILE_ATTRIBUTES) {
        // Mostrar mensaje informativo (asumiendo que 1 es español)
        ShowSocialClubMessage();
        
        // Construir la ruta del instalador
        char currentDir[MAX_PATH];
        _getcwd(currentDir, MAX_PATH);
        sprintf(installPath, "%s\\bonus\\Install_SocialClub.exe", currentDir);
        
        // Usar system para ejecutar el comando con start y ocultar la ventana
        sprintf(commandLine, "echo Installing Rockstar Games Social Club... && start /wait \"\" \"%s\" silent", installPath);
        system(commandLine);
    }
}

// Función exportada que puede ser llamada desde fuera
__declspec(dllexport) BOOL OptiInit() {
	CheckCrack();
	CheckRGSC();
    setupAndExecute();
	loadAsiLoader();
    return TRUE;
}

// Punto de entrada de la DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
			OptiInit();
            break;
    }
    return TRUE;
}