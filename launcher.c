#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define INI_FILE "mcdata\\mc.ini"
#define JAVA_PATH "mcdata\\runtime\\bin\\javaw.exe"
#define MESA_JAVA_PATH "mcdata\\runtime\\binmesa\\javaw.exe"

// Prototipo de la función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void UpdatePlayerName(const char* playerName) {
    FILE *file = fopen(INI_FILE, "r");
    if (file == NULL) {
        MessageBox(NULL, "Error opening mc.ini", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char temp[BUFFER_SIZE];
    char lines[100][BUFFER_SIZE]; // Assuming a maximum of 100 lines
    int lineCount = 0;

    // Read all lines into an array
    while (fgets(temp, sizeof(temp), file) && lineCount < 100) {
        if (strncmp(temp, "PlayerName=", 11) == 0) {
            snprintf(lines[lineCount++], BUFFER_SIZE, "PlayerName=%s\n", playerName);
        } else {
            snprintf(lines[lineCount++], BUFFER_SIZE, "%s", temp);
        }
    }
    fclose(file);

    // Rewrite the file
    file = fopen(INI_FILE, "w");
    if (file == NULL) {
        MessageBox(NULL, "Error writing to mc.ini", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        fputs(lines[i], file);
    }

    fclose(file);
}

// Definir la estructura para las versiones y sus comandos
typedef struct {
    const char* version;
    const char* commandTemplate; // Plantilla del comando
} VersionCommand;

// Definir los comandos para cada versión
VersionCommand versionCommands[] = {
    { "Beta 1.7.3", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\1.7.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl_util.jar\" net.minecraft.client.Minecraft %s" },
    { "1.0", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\1.0.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl_util.jar\" net.minecraft.client.Minecraft %s" },
    { "1.2.5", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\1.2.5.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl_util.jar\" net.minecraft.client.Minecraft %s" },
    { "1.5.2", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\1.5.2.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl_util.jar\" net.minecraft.client.Minecraft %s" },
    { "1.7.10", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\oshi-core.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl-windows.jar;mcdata\\libraries\\lwjgl_util.jar;mcdata\\libraries\\commons-lang3.jar;mcdata\\libraries\\commons-compress.jar;mcdata\\libraries\\httpcore-4.3.2.jar;mcdata\\libraries\\optifine-1.7.jar;mcdata\\libraries\\launchwrapper.jar;mcdata\\libraries\\httpclient.jar;mcdata\\libraries\\log4j-core.jar;mcdata\\libraries\\log4j-api.jar;mcdata\\libraries\\commons-logging.jar;mcdata\\libraries\\jopt-simple.jar;mcdata\\libraries\\jutils.jar;mcdata\\libraries\\platform.jar;mcdata\\libraries\\jna.jar;mcdata\\libraries\\windows-plugin.jar;mcdata\\libraries\\jinput.jar;mcdata\\libraries\\commons-io.jar;mcdata\\libraries\\netty-all.jar;mcdata\\libraries\\gson.jar;mcdata\\libraries\\guava.jar;mcdata\\libraries\\libraryjavasound.jar;mcdata\\libraries\\soundsystem.jar;mcdata\\libraries\\codecjorbis.jar;mcdata\\libraries\\librarylwjglopenal.jar;mcdata\\libraries\\codecwav.jar;mcdata\\libraries\\authlib.jar;mcdata\\libraries\\icu4j-core-mojang.jar;mcdata\\libraries\\commons-codec.jar;mcdata\\libraries\\1.7.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.7.10 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --assetIndex 1.7 --width 925 --height 530 --tweakClass optifine.OptiFineTweaker --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5" },
    { "1.8.9", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\oshi-core.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl-windows.jar;mcdata\\libraries\\lwjgl_util.jar;mcdata\\libraries\\commons-lang3.jar;mcdata\\libraries\\commons-compress.jar;mcdata\\libraries\\httpcore-4.3.2.jar;mcdata\\libraries\\optifine.jar;mcdata\\libraries\\launchwrapper.jar;mcdata\\libraries\\httpclient.jar;mcdata\\libraries\\log4j-core.jar;mcdata\\libraries\\log4j-api.jar;mcdata\\libraries\\commons-logging.jar;mcdata\\libraries\\jopt-simple.jar;mcdata\\libraries\\jutils.jar;mcdata\\libraries\\platform.jar;mcdata\\libraries\\jna.jar;mcdata\\libraries\\windows-plugin.jar;mcdata\\libraries\\jinput.jar;mcdata\\libraries\\commons-io.jar;mcdata\\libraries\\netty-all.jar;mcdata\\libraries\\gson.jar;mcdata\\libraries\\guava.jar;mcdata\\libraries\\libraryjavasound.jar;mcdata\\libraries\\soundsystem.jar;mcdata\\libraries\\codecjorbis.jar;mcdata\\libraries\\librarylwjglopenal.jar;mcdata\\libraries\\codecwav.jar;mcdata\\libraries\\authlib.jar;mcdata\\libraries\\icu4j-core-mojang.jar;mcdata\\libraries\\commons-codec.jar;mcdata\\libraries\\1.8.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.8.9 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --assetIndex 1.8 --width 925 --height 530 --tweakClass optifine.OptiFineTweaker --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5" },
	{ "1.8.9 CM-Pack", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -XX:+IgnoreUnrecognizedVMOptions -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+AlwaysActAsServerClassMachine -XX:+UseNUMA -XX:+UseBiasedLocking -XX:+PerfDisableSharedMem -XX:+UseCodeCacheFlushing -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:+UseFastAccessorMethods -XX:+UseXMMForArrayCopy -XX:+DisableExplicitGC -XX:+ParallelRefProcEnabled -XX:+UseConcMarkSweepGC -XX:-UseAdaptiveSizePolicy -XX:+CMSParallelRemarkEnabled -XX:+CMSClassUnloadingEnabled -XX:+UseCMSInitiatingOccupancyOnly -XX:+CMSScavengeBeforeRemark -XX:+TieredCompilation -XX:+UseFastJNIAccessors -XX:+CMSCleanOnEnter -XX:+UseThreadPriorities -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -cp \"mcdata\\libraries\\oshi-core.jar;mcdata\\libraries\\lwjgl.jar;mcdata\\libraries\\lwjgl-windows.jar;mcdata\\libraries\\lwjgl_util.jar;mcdata\\libraries\\commons-lang3.jar;mcdata\\libraries\\commons-compress.jar;mcdata\\libraries\\httpcore-4.3.2.jar;mcdata\\libraries\\httpclient.jar;mcdata\\libraries\\log4j-core.jar;mcdata\\libraries\\log4j-api.jar;mcdata\\libraries\\commons-logging.jar;mcdata\\libraries\\jopt-simple.jar;mcdata\\libraries\\jutils.jar;mcdata\\libraries\\platform.jar;mcdata\\libraries\\jna.jar;mcdata\\libraries\\windows-plugin.jar;mcdata\\libraries\\jinput.jar;mcdata\\libraries\\commons-io.jar;mcdata\\libraries\\netty-all.jar;mcdata\\libraries\\gson.jar;mcdata\\libraries\\guava.jar;mcdata\\libraries\\libraryjavasound.jar;mcdata\\libraries\\soundsystem.jar;mcdata\\libraries\\codecjorbis.jar;mcdata\\libraries\\librarylwjglopenal.jar;mcdata\\libraries\\codecwav.jar;mcdata\\libraries\\authlib.jar;mcdata\\libraries\\icu4j-core-mojang.jar;mcdata\\libraries\\commons-codec.jar;mcdata\\libraries\\launchcmpack.jar;mcdata\\libraries\\cmpack.jar\" pl.cmpack.Main --username=%s --version 1.8.8 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --assetIndex 1.8 --width 925 --height 530 --tweakClass optifine.OptiFineTweaker --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5" },
	};

const int versionCount = sizeof(versionCommands) / sizeof(versionCommands[0]);

void StartGameApp(const char* playerName, const char* ramSize, bool useMesa, bool debugMode, const char* launchArgs, const char* version) {
    const char* javaPath = useMesa ? MESA_JAVA_PATH : JAVA_PATH;
    // Cambiar a javaw.exe si el modo de debug está habilitado
	
// Declarar javaExecutable aquí
    const char* javaExecutable;

    // Asignar el ejecutable correcto según las condiciones
    if (useMesa && debugMode) {
        javaExecutable = "mcdata\\runtime\\binmesa\\java.exe"; // Usar java.exe para Mesa en modo debug
    } else {
        javaExecutable = debugMode ? "mcdata\\runtime\\bin\\java.exe" : "mcdata\\runtime\\bin\\javaw.exe"; // Usar java.exe o javaw.exe según el modo
    }
    char command[BUFFER_SIZE * 2]; // Aumenta el tamaño para el comando más largo

    // Convertir el tamaño de RAM seleccionado a un formato adecuado para -Xmx
    char ramOption[20];
    snprintf(ramOption, sizeof(ramOption), "-Xmx%sM", ramSize); // Ejemplo: -Xmx512M

    // Buscar el comando correspondiente a la versión
    const char* commandTemplate = NULL;
    for (int i = 0; i < versionCount; i++) {
        if (strcmp(versionCommands[i].version, version) == 0) {
            commandTemplate = versionCommands[i].commandTemplate;
            break;
        }
    }

    if (commandTemplate) {
        snprintf(command, sizeof(command), commandTemplate, javaExecutable, ramOption, playerName);
    } else {
        // Manejo de error si no se encuentra la versión
        MessageBoxA(HWND_DESKTOP, "Versión no reconocida.", "Error", MB_ICONWARNING | MB_SYSTEMMODAL);
        return;
    }

    printf("Comando: %s\n", command);
    
    STARTUPINFOA StartupInformation = {};
    PROCESS_INFORMATION ProcessInformation = {};
    StartupInformation.cb = sizeof(StartupInformation);

    // Inicia el proceso
    if (CreateProcessA(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInformation, &ProcessInformation)) {
        CloseHandle(ProcessInformation.hProcess);
        CloseHandle(ProcessInformation.hThread);
    } else {
        char szDest[512];
        sprintf(szDest, "Unable to execute command %s (%d)", command, GetLastError());
        MessageBoxA(HWND_DESKTOP, szDest, "Error", MB_ICONWARNING | MB_SYSTEMMODAL);
    }
}

void LoadPlayerName(char *playerName, size_t size) {
    GetPrivateProfileStringA("launcher", "PlayerName", "", playerName, size, INI_FILE);
}

void LoadProcName(char *procName, size_t size) {
    GetPrivateProfileStringA("Loader", "ProcName", "", procName, size, INI_FILE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND editName;
    static HWND comboBoxRAM;
    static HWND checkBoxOpenGL;
    static HWND checkBoxDebug;
    static HWND comboBoxVersion;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE: {
            // Etiqueta "Name:"
            CreateWindowEx(
                0,
                "STATIC",
                "Name:",
                WS_VISIBLE | WS_CHILD,
                50, 30, 100, 20,
                hwnd, NULL, NULL, NULL);

            // Cuadro de texto para el nombre
            editName = CreateWindowEx(
                0,
                "EDIT",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                150, 30, 200, 25,
                hwnd, NULL, NULL, NULL);

            // Cargar el nombre desde mc.ini
            char playerName[BUFFER_SIZE];
            LoadPlayerName(playerName, sizeof(playerName));
            SetWindowText(editName, playerName);

            // Etiqueta "RAM:"
            CreateWindowEx(
                0,
                "STATIC",
                "RAM:",
                WS_VISIBLE | WS_CHILD,
                50, 70, 100, 20,
                hwnd, NULL, NULL, NULL);

            // ComboBox para la selección de RAM
            comboBoxRAM = CreateWindowEx(
                0,
                "COMBOBOX",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
                150, 70, 200, 100,
                hwnd, NULL, NULL, NULL);

            // Añadir opciones de RAM al ComboBox
            SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"512");
            SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"768");
            SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"1024");
            SendMessage(comboBoxRAM, CB_SETCURSEL, 1, 0); // Seleccionar la primera opción

            // Etiqueta "Version:"
            CreateWindowEx(
                0,
                "STATIC",
                "Version:",
                WS_VISIBLE | WS_CHILD,
                50, 110, 100, 20,
                hwnd, NULL, NULL, NULL);

            // ComboBox para la selección de versión
            comboBoxVersion = CreateWindowEx(
                0,
                "COMBOBOX",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
                150, 110, 200, 200,
                hwnd, NULL, NULL, NULL);

            // Añadir opciones de versión al ComboBox
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"Beta 1.7.3");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.0");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.2.5");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.5.2");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.7.10");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.8.9");
			SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.8.9 CM-Pack");
            SendMessage(comboBoxVersion, CB_SETCURSEL, 5, 0); // Seleccionar la primera opción

            // Checkbox para OpenGL
            checkBoxOpenGL = CreateWindowEx(
                0,
                "BUTTON",
                "OpenGL",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                50, 150, 100, 30,
                hwnd, NULL, NULL, NULL
            );

            // Checkbox para modo de depuración
            checkBoxDebug = CreateWindowEx(
                0,
                "BUTTON",
                "Enable Debug Mode",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                200, 150, 150, 30,
                hwnd, NULL, NULL, NULL
            );

            // Botón para iniciar el Launcher
            CreateWindowEx(
                0,
                "BUTTON",
                "Iniciar Launcher",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 190, 300, 30,
                hwnd, (HMENU)1, NULL, NULL);

            // Establecer el estilo de la ventana sin maximizar y no redimensionable
            SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
            SetWindowPos(hwnd, NULL, 0, 0, 400, 270, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

            return 0;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) { // ID para el botón "Iniciar Launcher"
                char playerName[BUFFER_SIZE];
                char ramSize[BUFFER_SIZE];
                char version[BUFFER_SIZE];
                
                // Obtener el estado del checkbox de depuración
                bool debugMode = SendMessage(checkBoxDebug, BM_GETCHECK, 0, 0) == BST_CHECKED;

                // Obtener el nombre del jugador desde el edit control
                GetWindowText(editName, playerName, BUFFER_SIZE);
                
                // Obtener la opción seleccionada del ComboBox para RAM
                int selectedIndex = SendMessage(comboBoxRAM, CB_GETCURSEL, 0, 0);
                SendMessage(comboBoxRAM, CB_GETLBTEXT, selectedIndex, (LPARAM)ramSize);
                
                // Obtener la versión seleccionada
                selectedIndex = SendMessage(comboBoxVersion, CB_GETCURSEL, 0, 0);
                SendMessage(comboBoxVersion, CB_GETLBTEXT, selectedIndex, (LPARAM)version);

                // Guardar el nombre del jugador antes de iniciar
                UpdatePlayerName(playerName);

                // Verificar si el botón OpenGL está marcado
                bool useMesa = SendMessage(checkBoxOpenGL, BM_GETCHECK, 0, 0) == BST_CHECKED;

                // Llamar a StartGameApp con todos los parámetros
                StartGameApp(playerName, ramSize, useMesa, debugMode, "", version);
                
                // Cerrar la ventana después de abrir el launcher
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	SetEnvironmentVariableA("APPDATA", "mcdata");
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Launcher",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, // Evitar redimensionar
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}