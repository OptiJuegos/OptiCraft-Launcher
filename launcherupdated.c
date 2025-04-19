#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 8192
#define INI_FILE "mcdata\\mc.ini"
#define OPTI_JAVA_PATH "mcdata\\runtime\\bin\\javaw.exe"
#define OPTI_MESA_JAVA_PATH "mcdata\\runtime\\binmesa\\javaw.exe"

// Prototipo de la función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool IsSystem64Bit() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    
    return (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || 
            systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64);
}

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
    { "1.21.0 Sodium", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\intermediary-1.21.jar;mcdata\\libraries\\sponge-mixin-0.13.3+mixin.0.8.5.jar;mcdata\\libraries\\fabric-loader-0.15.11.jar;mcdata\\libraries\\asm-util-9.6.jar;mcdata\\libraries\\asm-tree-9.6.jar;mcdata\\libraries\\asm-commons-9.6.jar;mcdata\\libraries\\asm-analysis-9.6.jar;mcdata\\libraries\\asm-9.6.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\oshi-core-6.4.10.jar;mcdata\\libraries\\jna-5.14.0.jar;mcdata\\libraries\\jna-platform-5.14.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.9.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.97.Final.jar;mcdata\\libraries\\netty-buffer-4.1.97.Final.jar;mcdata\\libraries\\netty-codec-4.1.97.Final.jar;mcdata\\libraries\\netty-handler-4.1.97.Final.jar;mcdata\\libraries\\netty-resolver-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.97.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.5.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.0.jar\" net.fabricmc.loader.impl.launch.knot.KnotClient --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --assetIndex 17 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft" },
    { "1.21.0 OptiFine", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\OptiFine-1.21_HD_U_J1_pre9.jar;mcdata\\libraries\\launchwrapper-of-2.3.jar;mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\oshi-core-6.4.10.jar;mcdata\\libraries\\jna-5.14.0.jar;mcdata\\libraries\\jna-platform-5.14.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.9.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.97.Final.jar;mcdata\\libraries\\netty-buffer-4.1.97.Final.jar;mcdata\\libraries\\netty-codec-4.1.97.Final.jar;mcdata\\libraries\\netty-handler-4.1.97.Final.jar;mcdata\\libraries\\netty-resolver-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.97.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.5.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.0.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft --assetIndex 17 --tweakClass optifine.OptiFineTweaker" },
	{ "1.21.1 OptiFine", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\OptiFine-1.21.1_HD_U_J1.jar;mcdata\\libraries\\launchwrapper-of-2.3.jar;mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\oshi-core-6.4.10.jar;mcdata\\libraries\\jna-5.14.0.jar;mcdata\\libraries\\jna-platform-5.14.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.9.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.97.Final.jar;mcdata\\libraries\\netty-buffer-4.1.97.Final.jar;mcdata\\libraries\\netty-codec-4.1.97.Final.jar;mcdata\\libraries\\netty-handler-4.1.97.Final.jar;mcdata\\libraries\\netty-resolver-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.97.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.5.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.1.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft --assetIndex 17 --tweakClass optifine.OptiFineTweaker" },
	{ "1.21.3 OptiFine", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\OptiFine_1.21.3_HD_U_J2.jar;mcdata\\libraries\\launchwrapper-of-2.3.jar;mcdata\\libraries\\jtracy-1.0.29.jar;mcdata\\libraries\\jtracy-1.0.29-natives-windows.jar;mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\oshi-core-6.4.10.jar;mcdata\\libraries\\jna-5.14.0.jar;mcdata\\libraries\\jna-platform-5.14.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.9.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.97.Final.jar;mcdata\\libraries\\netty-buffer-4.1.97.Final.jar;mcdata\\libraries\\netty-codec-4.1.97.Final.jar;mcdata\\libraries\\netty-handler-4.1.97.Final.jar;mcdata\\libraries\\netty-resolver-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.97.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.5.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.3.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --assetIndex 17 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft --tweakClass optifine.OptiFineTweaker" },
	{ "1.21.4 OptiFine", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\OptiFine-1.21.4_HD_U_J3_pre3.jar;mcdata\\libraries\\launchwrapper-of-2.3.jar;mcdata\\libraries\\jtracy-1.0.29.jar;mcdata\\libraries\\jtracy-1.0.29-natives-windows.jar;mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\oshi-core-6.4.10.jar;mcdata\\libraries\\jna-5.14.0.jar;mcdata\\libraries\\jna-platform-5.14.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.9.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.97.Final.jar;mcdata\\libraries\\netty-buffer-4.1.97.Final.jar;mcdata\\libraries\\netty-codec-4.1.97.Final.jar;mcdata\\libraries\\netty-handler-4.1.97.Final.jar;mcdata\\libraries\\netty-resolver-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.97.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.97.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.5.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.4.jar\" net.minecraft.launchwrapper.Launch --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --assetIndex 17 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft --tweakClass optifine.OptiFineTweaker" },   	
	{ "1.21.5 Sodium", "\"%s\" -Djava.library.path=\"mcdata\\natives\" %s -cp \"mcdata\\libraries\\jorbis-0.0.17.jar;mcdata\\libraries\\intermediary-1.21.5.jar;mcdata\\libraries\\sponge-mixin-0.15.4+mixin.0.8.7.jar;mcdata\\libraries\\jtracy-1.0.29.jar;mcdata\\libraries\\jtracy-1.0.29-natives-windows.jar;mcdata\\libraries\\fabric-loader-0.16.10.jar;mcdata\\libraries\\asm-util-9.6.jar;mcdata\\libraries\\asm-tree-9.7.1.jar;mcdata\\libraries\\asm-commons-9.7.1.jar;mcdata\\libraries\\asm-analysis-9.7.1.jar;mcdata\\libraries\\asm-9.7.1.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-stb-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-openal-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3-natives-windows.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows-x86.jar;mcdata\\libraries\\lwjgl-3.3.3-natives-windows.jar;mcdata\\libraries\\guava-33.3.1-jre.jar;mcdata\\libraries\\logging-1.1.1.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\jackson-databind-2.13.4.2.jar;mcdata\\libraries\\oshi-core-6.6.5.jar;mcdata\\libraries\\jackson-core-2.13.4.jar;mcdata\\libraries\\jackson-annotations-2.13.4.jar;mcdata\\libraries\\patchy-2.2.10.jar;mcdata\\libraries\\gson-2.11.0.jar;mcdata\\libraries\\jna-5.15.0.jar;mcdata\\libraries\\jna-platform-5.15.0.jar;mcdata\\libraries\\blocklist-1.0.10.jar;mcdata\\libraries\\slf4j-api-2.0.16.jar;mcdata\\libraries\\log4j-slf4j2-impl-2.19.0.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\netty-common-4.1.118.Final.jar;mcdata\\libraries\\netty-buffer-4.1.118.Final.jar;mcdata\\libraries\\netty-codec-4.1.118.Final.jar;mcdata\\libraries\\netty-handler-4.1.118.Final.jar;mcdata\\libraries\\netty-resolver-4.1.118.Final.jar;mcdata\\libraries\\netty-transport-4.1.118.Final.jar;mcdata\\libraries\\netty-transport-native-unix-common-4.1.118.Final.jar;mcdata\\libraries\\netty-transport-classes-epoll-4.1.118.Final.jar;mcdata\\libraries\\netty-all-4.0.23.Final.jar;mcdata\\libraries\\lz4-java-1.8.0.jar;mcdata\\libraries\\failureaccess-1.0.1.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\commons-lang3-3.13.0.jar;mcdata\\libraries\\commons-io-2.13.0.jar;mcdata\\libraries\\commons-codec-1.16.0.jar;mcdata\\libraries\\brigadier-1.2.9.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\datafixerupper-8.0.16.jar;mcdata\\libraries\\gson-2.10.1.jar;mcdata\\libraries\\authlib-6.0.54.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\commons-compress-1.26.0.jar;mcdata\\libraries\\httpclient-4.5.13.jar;mcdata\\libraries\\commons-logging-1.2.jar;mcdata\\libraries\\fastutil-8.5.12.jar;mcdata\\libraries\\httpcore-4.4.16.jar;mcdata\\libraries\\log4j-api-2.22.1.jar;mcdata\\libraries\\log4j-core-2.22.1.jar;mcdata\\libraries\\jopt-simple-5.0.4.jar;mcdata\\libraries\\icu4j-core-mojang-51.2.jar;mcdata\\libraries\\guava-32.1.2-jre.jar;mcdata\\libraries\\accessors-smart-2.5.1.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\joml-1.10.8.jar;mcdata\\libraries\\lwjgl-3.3.3.jar;mcdata\\libraries\\lwjgl-stb-3.3.3.jar;mcdata\\libraries\\lwjgl-jemalloc-3.3.3.jar;mcdata\\libraries\\lwjgl-openal-3.3.3.jar;mcdata\\libraries\\lwjgl-opengl-3.3.3.jar;mcdata\\libraries\\lwjgl-tinyfd-3.3.3.jar;mcdata\\libraries\\lwjgl-glfw-3.3.3.jar;mcdata\\libraries\\lwjgl-freetype-3.3.3.jar;mcdata\\libraries\\text2speech-1.17.9.jar;mcdata\\libraries\\1.21.5.jar\" net.fabricmc.loader.impl.launch.knot.KnotClient --username=%s --version 1.21 --accessToken [random] --userProperties {} --gameDir \"mcdata\" --assetsDir \"mcdata\\assets\" --width 925 --height 530 --assetIndex 17 --uuid e4ff1ea62ec43e1ca0ceb206a0d472b5 --clientId --xuid -userType legacy --versionType OptiCraft" },
	};
	
const int versionCount = sizeof(versionCommands) / sizeof(versionCommands[0]);

void StartGameApp(const char* playerName, const char* ramSize, bool useMesa, bool debugMode, const char* launchArgs, const char* version) {
    const char* javaPath = useMesa ? OPTI_MESA_JAVA_PATH : OPTI_JAVA_PATH;
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
    char ramOption[8000];
    snprintf(ramOption, sizeof(ramOption), "-Xms1024M -Xmx%sM -XX:+UnlockExperimentalVMOptions -XX:+UseG1GC -XX:MaxGCPauseMillis=37 -XX:G1HeapRegionSize=16M -XX:G1NewSizePercent=23 -XX:G1ReservePercent=20 -XX:SurvivorRatio=32 -XX:G1MixedGCCountTarget=3 -XX:G1HeapWastePercent=20 -XX:InitiatingHeapOccupancyPercent=10 -XX:G1RSetUpdatingPauseTimePercent=0 -XX:G1SATBBufferEnqueueingThresholdPercent=30 -XX:G1ConcMarkStepDurationMillis=5.0 -XX:G1ConcRSHotCardLimit=16 -XX:G1ConcRefinementServiceIntervalMillis=150 -XX:GCTimeRatio=99 -XX:MaxTenuringThreshold=1 -XX:MaxNodeLimit=240000 -XX:NodeLimitFudgeFactor=8000 -XX:+AlwaysActAsServerClassMachine -XX:+DisableExplicitGC -XX:+DisableAttachMechanism -XX:+ParallelRefProcEnabled -XX:+PerfDisableSharedMem -XX:+UseFastJNIAccessors -XX:+UseFastUnorderedTimeStamps -XX:+UseNUMA -XX:-UsePerfData -XX:+UseVectorCmov -XX:+EnableJVMCI -XX:+UseJVMCICompiler -XX:+EagerJVMCI -Dgraal.TuneInlinerExploration=1 -Dgraal.UsePriorityInlining=true -Dgraal.Vectorization=true -Dgraal.OptDuplication=true -Dgraal.DetectInvertedLoopsAsCounted=true -Dgraal.LoopInversion=true -Dgraal.VectorizeHashes=true -Dgraal.TuneInlinerExploration=1 -Dgraal.LoopRotation=true -Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true -Djava.net.useSystemProxies=true -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dsun.rmi.dgc.server.gcInterval=2147483646 -Djava.net.preferIPv4Stack=true", ramSize); // Ejemplo: -Xmx512M

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
            SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"2048");
            SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"4096");
			SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"6144");
			SendMessage(comboBoxRAM, CB_ADDSTRING, 0, (LPARAM)"8192");
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
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.0 Sodium");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.0 OptiFine");
			SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.1 OptiFine");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.3 OptiFine");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.4 OptiFine");
            SendMessage(comboBoxVersion, CB_ADDSTRING, 0, (LPARAM)"1.21.5 Sodium");
            SendMessage(comboBoxVersion, CB_SETCURSEL, 0, 0); // Seleccionar la primera opción

            // Checkbox para OpenGL
            checkBoxOpenGL = CreateWindowEx(
                0,
                "BUTTON",
                "OpenGL Mesa",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                50, 150, 111, 30,
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
                "Start OptiCraft",
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
    const char CLASS_NAME[] = "OptiCraft";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "OptiCraft Launcher",
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