#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <psapi.h>
#include <string.h>
#include <shlwapi.h>
#include <strsafe.h>

// Prototipos de funciones
void checkWindowsVersion(void);
void getAppDataPath(char* appDataPath, size_t size);
void getLocalAppDataPath(char* localAppDataPath, size_t size);
void createDirectoryIfNeeded(const char* path);
void getCurrentGamePath(char* gamePath, size_t size);
void CheckAndInstallRequirements(void);
void showMessageBasedOnLanguage(LANGID primaryLang);
void CheckAndReinstallIfNeeded(void);
void StartGame(void);

void checkWindowsVersion() {
    OSVERSIONINFOEX versionInfo;
    ZeroMemory(&versionInfo, sizeof(OSVERSIONINFOEX));
    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    // Usar RtlGetVersion que no está sujeto a restricciones de manifiesto
    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll) {
        RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hNtdll, "RtlGetVersion");
        if (RtlGetVersion) {
            RtlGetVersion((PRTL_OSVERSIONINFOW)&versionInfo);
        }
    }
    
    // Verificar versión
    if (versionInfo.dwMajorVersion < 10) {
        MessageBoxA(NULL, "Tu sistema operativo no está soportado.", "Error", MB_ICONERROR | MB_OK);
        exit(1);
    } 
    else if (versionInfo.dwMajorVersion == 10) {
        // Windows 10 y 11 tienen la misma versión mayor (10)
        if (versionInfo.dwBuildNumber < 10240) {
            MessageBoxA(NULL, "Tu sistema operativo no está soportado.", "Error", MB_ICONERROR | MB_OK);
            exit(1);
        }
        else if (versionInfo.dwBuildNumber < 14393) {
            MessageBoxA(NULL, "Tu sistema operativo no está soportado. Necesitas Windows 10 versión 1607 o superior.", "Error", MB_ICONERROR | MB_OK);
            exit(1);
        }
        else if (versionInfo.dwBuildNumber < 22000) {
        }
        else {
        }
    } 
    else {
    }
}

// Función para ejecutar comandos de PowerShell y capturar su salida
char* ExecutePowerShellCommand(const char* command) {
    char psCommand[1024];
    StringCbPrintfA(psCommand, sizeof(psCommand), "powershell -Command \"%s\"", command);
    
    FILE* pipe = _popen(psCommand, "r");
    if (!pipe) {
        return NULL;
    }
    
    char* result = (char*)malloc(4096);
    if (!result) {
        _pclose(pipe);
        return NULL;
    }
    
    result[0] = '\0';
    char buffer[128];
    
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        StringCbCatA(result, 4096, buffer);
    }
    
    _pclose(pipe);
    return result;
}

// Función para desinstalar un paquete AppX
int UninstallAppxPackage(const char* appName) {
    char command[512];
    StringCbPrintfA(command, sizeof(command), "Get-AppxPackage -Name *%s* | Remove-AppxPackage", appName);
    
    char* result = ExecutePowerShellCommand(command);
    if (!result) {
        return 0;
    }
    
    // Comprobamos si hubo errores en la desinstalación
    int success = (strstr(result, "error") == NULL);
    free(result);
    
    return success;
}

// Función para comprobar si una aplicación UWP está instalada
int IsAppInstalled(const char* appName) {
    char command[512];
    StringCbPrintfA(command, sizeof(command), "Get-AppxPackage -Name *%s* | Select-Object -ExpandProperty Name", appName);
    
    char* result = ExecutePowerShellCommand(command);
    if (!result) {
        return 0;
    }
    
    int installed = (strstr(result, appName) != NULL);
    free(result);
    
    return installed;
}

// Función para registrar un paquete AppX
int RegisterAppxPackage(const char* packagePath) {
    char command[512];
    StringCbPrintfA(command, sizeof(command), "Add-AppxPackage -register \"%s\" -ForceApplicationShutdown", packagePath);
    
    char* result = ExecutePowerShellCommand(command);
    if (!result) {
        return 0;
    }
    
    // Comprobamos si hubo errores en la instalación
    int success = (strstr(result, "error") == NULL);
    free(result);
    
    return success;
}

// Función para instalar un paquete AppX
int InstallAppxPackage(const char* packagePath) {
    char command[512];
    StringCbPrintfA(command, sizeof(command), "Add-AppxPackage -Path \"%s\" -ForceApplicationShutdown", packagePath);
    
    char* result = ExecutePowerShellCommand(command);
    if (!result) {
        return 0;
    }
    
    // Comprobamos si hubo errores en la instalación
    int success = (strstr(result, "error") == NULL);
    free(result);
    
    return success;
}

// Definición de funciones auxiliares
void createDirectoryIfNeeded(const char* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // Directory created or already exists
    } else {
        printf("Error creating directory: %s\n", path);
    }
}

void getAppDataPath(char* appDataPath, size_t size) {
    if (GetEnvironmentVariable("APPDATA", appDataPath, size) == 0) {
        printf("Error getting APPDATA environment variable.\n");
    }
}

void getLocalAppDataPath(char* localAppDataPath, size_t size) {
    if (GetEnvironmentVariable("LOCALAPPDATA", localAppDataPath, size) == 0) {
        printf("Error getting LOCALAPPDATA environment variable.\n");
    }
}

void getCurrentGamePath(char* gamePath, size_t size) {
    // Obtener el directorio actual
    if (GetCurrentDirectory(size, gamePath) == 0) {
        printf("Error al obtener el directorio actual.\n");
    }
}

void CheckAndInstallRequirements(void) {
    printf("Instalando requisitos...\n");
    
    // Comprobar si VCLibs está instalado
    printf("Comprobando si VCLibs está instalado...\n");
    if (!IsAppInstalled("Microsoft.VCLibs")) {
        printf("VCLibs no está instalado. Instalando VCLibs...\n");
        
        // Definir la ruta al paquete VCLibs
        const char* vcLibsPath = ".\\runtimes\\VCLibs.appx";
        
        // Instalar VCLibs
        if (!InstallAppxPackage(vcLibsPath)) {
            printf("Error al instalar VCLibs. Abortando.\n");
            return;
        }
        
        printf("VCLibs instalado correctamente.\n");
    } else {
        printf("VCLibs ya está instalado.\n");
    }

    // Instalar Microsoft.CraftsmanUWP1616
    printf("Instalando Microsoft.CraftsmanUWP16...\n");
    
    // Definir la ruta al manifiesto de la aplicación
    const char* appManifestPath = ".\\AppxManifest.xml";
    
    // Instalar la aplicación
    if (!RegisterAppxPackage(appManifestPath)) {
        printf("Error al instalar Microsoft.CraftsmanUWP16. Abortando.\n");
        return;
    }
    
    printf("Microsoft.CraftsmanUWP16 instalado correctamente.\n");
}

void showMessageBasedOnLanguage(LANGID primaryLang) {
    const char *message;

    switch (primaryLang) {
        case 0x0A:  // LANG_SPANISH
            message = "Bienvenido al Proyecto!\n\n"
                      "Si encuentras algún problema con el proyecto, por favor consulta el archivo 'Information.txt', "
                      "Este archivo proporciona soluciones para los problemas más comunes, así como preguntas frecuentes.\n\n"
                      "Más proyectos interesantes en: https://optijuegos.github.io!";
            break;
        case 0x16:  // LANG_PORTUGUESE
            message = "Bem-vindo ao Projeto!\n\n"
                      "Se você encontrar algum problema com o projeto, por favor consulte o arquivo 'Information.txt', "
                      "Este arquivo fornece soluções para os problemas mais comuns, assim como perguntas frequentes.\n\n"
                      "Confira mais projetos legais em: https://optijuegos.github.io!";
            break;
        case 0x19:  // LANG_RUSSIAN
            message = "Добро пожаловать в проект!\n\n"
                      "Если вы столкнетесь с какими-либо проблемами в проекте, пожалуйста, обратитесь к файлу 'Information.txt', "
                      "В нем содержатся решения наиболее распространенных проблем, а также часто задаваемые вопросы.\n\n"
                      "Смотрите больше классных проектов на: https://optijuegos.github.io!";
            break;
        default:
            message = "Welcome to the Project!\n\n"
                      "If you encounter any issues with the project, please refer to the 'Information.txt' file. "
                      "It provides solutions for the most common problems, as well as FAQ.\n\n"
                      "Check out more cool projects at: https://optijuegos.github.io!";
            break;
    }
    MessageBoxA(NULL, message, "OptiProjects", MB_OK | MB_ICONINFORMATION);
}

// Función principal para comprobar e instalar los requisitos
void CheckAndReinstallIfNeeded() {
    char appDataPath[MAX_PATH];
    char dirPath[MAX_PATH];
    
    getAppDataPath(appDataPath, sizeof(appDataPath));
    
    snprintf(dirPath, sizeof(dirPath), "%s\\OptiProjects", appDataPath);
    createDirectoryIfNeeded(dirPath);
    
    // Nombre del archivo de verificación
    const char *checkFileName = "check_Craftsman1.16v1.txt"; 
    char verificationFilePath[MAX_PATH];
    snprintf(verificationFilePath, sizeof(verificationFilePath), "%s\\%s", dirPath, checkFileName);
    
    // Obtener la ruta actual del juego
    char currentGamePath[MAX_PATH];
    getCurrentGamePath(currentGamePath, sizeof(currentGamePath));
    
    // Verificar si el juego está instalado
    BOOL isGameInstalled = IsAppInstalled("Microsoft.CraftsmanUWP16");
    
    // Verificar si el archivo ya existe
    if (GetFileAttributes(verificationFilePath) != INVALID_FILE_ATTRIBUTES) {
        // Verificar si el juego está instalado
        if (!isGameInstalled) {
            printf("El juego no está instalado a pesar de existir el archivo de verificación.\n");
            printf("Reinstalando Microsoft.CraftsmanUWP16...\n");
            CheckAndInstallRequirements();
            
            // Actualizar el archivo de verificación con la ruta actual
            FILE *checkFile = fopen(verificationFilePath, "wb");
            if (checkFile) {
                fwrite(currentGamePath, sizeof(char), strlen(currentGamePath), checkFile);
                fclose(checkFile);
                printf("Archivo de verificación actualizado con la ruta actual.\n");
            }
            return;
        }
        
        // Leer la ruta guardada anteriormente
        FILE *checkFile = fopen(verificationFilePath, "rb");
        if (checkFile) {
            char savedGamePath[MAX_PATH] = {0};
            fread(savedGamePath, sizeof(char), MAX_PATH - 1, checkFile);
            fclose(checkFile);
            
            // Comparar las rutas
            if (strcmp(savedGamePath, currentGamePath) != 0) {
                printf("La ruta de instalación ha cambiado:\n");
                printf("Ruta anterior: %s\n", savedGamePath);
                printf("Ruta actual: %s\n", currentGamePath);
                
                // Desinstalar la aplicación existente
                printf("Desinstalando Microsoft.CraftsmanUWP16...\n");
                if (!UninstallAppxPackage("Microsoft.CraftsmanUWP16")) {
                    printf("Error al desinstalar Microsoft.CraftsmanUWP16.\n");
                } else {
                    printf("Microsoft.CraftsmanUWP16 desinstalado correctamente.\n");
                }
                
                // Reinstalar la aplicación
                printf("Reinstalando Microsoft.CraftsmanUWP16...\n");
                CheckAndInstallRequirements();
                
                // Actualizar el archivo de verificación con la nueva ruta
                checkFile = fopen(verificationFilePath, "wb");
                if (checkFile) {
                    fwrite(currentGamePath, sizeof(char), strlen(currentGamePath), checkFile);
                    fclose(checkFile);
                    printf("Archivo de verificación actualizado con la nueva ruta.\n");
                }
            } else {
                printf("La ruta de instalación no ha cambiado.\n");
                
                // Verificar si el juego está correctamente instalado aunque la ruta no haya cambiado
                if (!isGameInstalled) {
                    printf("El juego no está correctamente instalado. Reinstalando...\n");
                    CheckAndInstallRequirements();
                } else {
                    printf("No es necesario reinstalar.\n");
                }
            }
        }
    } else {
        // Mostrar mensaje basado en el idioma
        showMessageBasedOnLanguage(GetUserDefaultUILanguage() & 0x3FF);
        
        // Crear el archivo por primera vez
        FILE *checkFile = fopen(verificationFilePath, "wb");
        if (checkFile) {
            // Escribir la ruta del juego en el archivo
            fwrite(currentGamePath, sizeof(char), strlen(currentGamePath), checkFile);
            fclose(checkFile);
            printf("Archivo de verificación creado: %s\n", verificationFilePath);
            printf("Ruta del juego guardada: %s\n", currentGamePath);
            
            // Instalar la aplicación por primera vez
            CheckAndInstallRequirements();
        } else {
            printf("Error al crear el archivo de verificación: %s\n", verificationFilePath);
        }
    }
}

// Iniciar juego
void StartGame() {
    // Comando para abrir la aplicación UWP
    const char *appId = "shell:AppsFolder\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe!App";
    
    // Ejecutar el comando
    HINSTANCE result = ShellExecute(NULL, "open", appId, NULL, NULL, SW_SHOWNORMAL);
    
    // Verificar el resultado (opcional)
    if ((INT_PTR)result <= 32) {
        printf("Error al abrir la aplicación. Código: %d\n", (int)(INT_PTR)result);
    } else {
        printf("Aplicación iniciada correctamente.\n");
    }
}

int main() {
    char dirPath[MAX_PATH];
    
    checkWindowsVersion();
    CheckAndReinstallIfNeeded();

    ////////////////////////////////////////////////////////// CUSTOM NAME //////////////////////////////////////////////////////////////////

    // Inicializar la semilla para números aleatorios
    srand((unsigned int)time(NULL));

    // Lista de nombres predefinidos
    const char *names[] = {
        "Player", "Axolotl", "Allay", "Chorus", "Cat",
        "Homer", "Stick", "Vex", "Piglin", "Star",
        "Turtle", "Wolf", "Shulker", "Phantom", "Parrot",
        "Illusioner", "Giant", "Camel", "Armadillo"		
    };
    int numNames = sizeof(names) / sizeof(names[0]);

    // Seleccionar un nombre aleatorio de la lista
    const char *randomName = names[rand() % numNames];

    // Generar un número aleatorio entre 0 y 8999
    int randomNumber = rand() % 9000;

    // Crear el nombre completo con el número aleatorio
    char username[256];
    snprintf(username, sizeof(username), "%s%d", randomName, randomNumber);

    char localDataPath[MAX_PATH];
    getLocalAppDataPath(localDataPath, sizeof(localDataPath));

    // Crear directorios necesarios
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\com.mojang\\minecraftpe", localDataPath);
    createDirectoryIfNeeded(dirPath);

    // Ruta del archivo options.txt
    char optionsFilePath[MAX_PATH]; // Cambiado el nombre
    snprintf(optionsFilePath, sizeof(optionsFilePath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\com.mojang\\minecraftpe\\options.txt", localDataPath);

    // Ruta del archivo de verificación
    char checkFilePath3[MAX_PATH]; // Cambiado el nombre
    snprintf(checkFilePath3, sizeof(checkFilePath3), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\check3.txt", localDataPath);

    // Verificar si el archivo de verificación ya existe
    if (GetFileAttributes(checkFilePath3) == INVALID_FILE_ATTRIBUTES) {
        // Verificar si options.txt existe y eliminarlo
        if (GetFileAttributes(optionsFilePath) != INVALID_FILE_ATTRIBUTES) {
            if (!DeleteFile(optionsFilePath)) {
                printf("Error deleting the file 'options.txt'.\n");
            } else {
                printf("Existing file 'options.txt' deleted.\n");
            }
        }

        // Crear el archivo de verificación
        FILE *checkFile3 = fopen(checkFilePath3, "wb");
        if (checkFile3) {
            fclose(checkFile3);
            printf("Third verification file created: %s\n", checkFilePath3);
        } else {
            printf("Error creating the third verification file.\n");
        }

        // Crear el archivo options.txt y escribir el nombre de usuario
        FILE *outputFile = fopen(optionsFilePath, "wb");
        if (outputFile) {
            fprintf(outputFile, "mp_username:%s\n", username);
            fclose(outputFile);
            printf("Custom name '%s' written to file.\n", username);
        } else {
            printf("Error creating options.txt\n");
        }
    } else {
        printf("The third verification file already exists. No operations will be performed.\n");
    }

    ////////////////////////////////////////////////////////// EXTERNAL SERVERS //////////////////////////////////////////////////////////////////

    // Check external servers and create directory
    char externalDataPath[MAX_PATH];
    getLocalAppDataPath(externalDataPath, sizeof(externalDataPath));
    
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages", externalDataPath);
    createDirectoryIfNeeded(dirPath);
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe", externalDataPath);
    createDirectoryIfNeeded(dirPath);
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState", externalDataPath);
    createDirectoryIfNeeded(dirPath);
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games", externalDataPath);
    createDirectoryIfNeeded(dirPath);
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\com.mojang", externalDataPath);
    createDirectoryIfNeeded(dirPath);
    snprintf(dirPath, sizeof(dirPath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\com.mojang\\minecraftpe", externalDataPath);
    createDirectoryIfNeeded(dirPath);

    char serverFilePath[MAX_PATH]; // Cambiado el nombre
    snprintf(serverFilePath, sizeof(serverFilePath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\com.mojang\\minecraftpe\\external_servers.txt", externalDataPath);
    char checkFilePath[MAX_PATH]; // Manteniendo el nombre original
    snprintf(checkFilePath, sizeof(checkFilePath), "%s\\Packages\\Microsoft.CraftsmanUWP16_8wekyb3d8bbwe\\LocalState\\games\\check.txt", externalDataPath);

    if (GetFileAttributes(checkFilePath) == INVALID_FILE_ATTRIBUTES) {
        if (GetFileAttributes(serverFilePath) != INVALID_FILE_ATTRIBUTES) {
            if (DeleteFile(serverFilePath)) {
                printf("Existing file 'external_servers.txt' deleted.\n");
            } else {
                printf("Error deleting the file 'external_servers.txt'.\n");
            }
        }

        FILE *checkFile = fopen(checkFilePath, "wb");
        if (checkFile) {
            fclose(checkFile);
            printf("First verification file created: %s\n", checkFilePath);
        } else {
            printf("Error creating the first verification file.\n");
        }

        FILE *outputFile = fopen(serverFilePath, "wb");
        if (outputFile) {
            const char data[] = "1:GamerLife:gamerlifepe.ddns.net:25551:0\n"
                               "2:MiniGames GamePE:gamepe.ru:19132:0\n"
                               "3:MiniGames BmPE:bmpe.pw:19132:0\n"
                               "4:Nexland Survival:nexland.fun:19132:0\n"
                               "5:MiniGames ModsCraft:play.modscraft.net:19132:0\n"
                               "6:MiniGames SuomiCraft:suomicraftpe.ddns.net:19132:0\n";
            fwrite(data, 1, sizeof(data) - 1, outputFile);
            fclose(outputFile);
            printf("Server list written to file.\n");
        } else {
            printf("Error creating external_servers.txt\n");
        }
    } else {
        printf("The first verification file already exists. No operations will be performed.\n");
    }
	StartGame();
    return 0; // Finalizar correctamente
}