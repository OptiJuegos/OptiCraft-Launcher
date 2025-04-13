#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>
#include <psapi.h>
#include <string.h>
#include <shlwapi.h>
#include <stdbool.h>
#include <gdiplus.h>

// Set checksum
#define SET_CHECKSUM 0

// Set Welcome 
#define ENABLE_WELCOME 1

// Set Hex Modding 
#define HEX_MODDING 1

// Set Registry Change
#define REGISTRY_CHANGE 0

// Set Extract Crack
#define EXTRACT_CRACK 0

// Set Startup Animation
#define STARTUP_ANIMATION 0

// Create directories servers
void createDirectoryIfNeeded(const char* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // Directory created or already exists
    } else {
        //printf("Error creating directory: %s\n", path);
    }
}


// Check appdata
void getAppDataPath(char* appDataPath, size_t size) {
    if (GetEnvironmentVariable("APPDATA", appDataPath, size) == 0) {
        //printf("Error getting APPDATA environment variable.\n");
    }
}

////////////////////////////////////////////////////////// WELCOME MESSAGE //////////////////////////////////////////////////////////////////

__declspec(dllexport) void Opti(void) {
    #if ENABLE_WELCOME
    char appDataPath[MAX_PATH];
    getAppDataPath(appDataPath, sizeof(appDataPath));
    char dirPath[MAX_PATH];
    snprintf(dirPath, sizeof(dirPath), "%s\\OptiProjects", appDataPath);
    createDirectoryIfNeeded(dirPath);

    // Nombre del archivo de verificación definido por el usuario
    const char *checkFileName = "check_MinecraftStory.txt"; // Cambia esto según sea necesario
    char checkFilePath[MAX_PATH];
    snprintf(checkFilePath, sizeof(checkFilePath), "%s\\%s", dirPath, checkFileName);

    // Verificar si el archivo ya existe
    if (GetFileAttributes(checkFilePath) != INVALID_FILE_ATTRIBUTES) {
        printf("El archivo de verificación '%s' ya existe. No se realizarán verificaciones.\n", checkFileName);
        return;
    }
    
    // Crear el archivo
    FILE *checkFile = fopen(checkFilePath, "wb");
    if (checkFile) {
        // Mensaje según el idioma
        LANGID langID = GetUserDefaultUILanguage();
        LANGID primaryLang = langID & 0x3FF;  // Extraer el idioma primario
        const char *verificationMessage;

        // Mensajes según el idioma
        switch (primaryLang) {
            case 0x0A:  // LANG_SPANISH
                verificationMessage = "Este archivo se utiliza para verificar si ya se inició la aplicación anteriormente.";
                break;
            case 0x16:  // LANG_PORTUGUESE
                verificationMessage = "Este arquivo é usado para verificar se a aplicação já foi iniciada anteriormente.";
                break;
            case 0x19:  // LANG_RUSSIAN
                verificationMessage = "Этот файл используется для проверки, была ли запущена программа ранее.";
                break;
            default:
                verificationMessage = "This file is used to check if the application has been started before.";
                break;
        }

        // Escribir el mensaje en el archivo
        fwrite(verificationMessage, sizeof(char), strlen(verificationMessage), checkFile);
        fclose(checkFile);
        printf("Archivo de verificación creado: %s\n", checkFilePath);
        
        // Mostrar mensaje basado en el idioma
        const wchar_t *message;
        switch (primaryLang) {
            case 0x0A:  // LANG_SPANISH
                message = L"Bienvenido al Proyecto!\n\n"
                          L"Si encuentras algún problema con el proyecto, por favor consulta el archivo 'Information.txt', "
                          L"Este archivo proporciona soluciones para los problemas más comunes, así como preguntas frecuentes.\n\n"
                          L"Más proyectos interesantes en: https://optijuegos.github.io!";
                break;
            case 0x16:  // LANG_PORTUGUESE
                message = L"Bem-vindo ao Projeto!\n\n"
                          L"Se você encontrar algum problema com o projeto, por favor consulte o arquivo 'Information.txt', "
                          L"Este arquivo fornece soluções para os problemas mais comuns, assim como perguntas frequentes.\n\n"
                          L"Confira mais projetos legais em: https://optijuegos.github.io!";
                break;
            case 0x19:  // LANG_RUSSIAN
                message = L"Добро пожаловать в проект!\n\n"
                          L"Если вы столкнетесь с какими-либо проблемами в проекте, пожалуйста, обратитесь к файлу 'Information.txt', "
                          L"В нем содержатся решения наиболее распространенных проблем, а также часто задаваемые вопросы.\n\n"
                          L"Смотрите больше классных проектов на: https://optijuegos.github.io!";
                break;
            default:
                message = L"Welcome to the Project!\n\n"
                          L"If you encounter any issues with the project, please refer to the 'Information.txt' file. "
                          L"It provides solutions for the most common problems, as well as FAQ.\n\n"
                          L"Check out more cool projects at: https://optijuegos.github.io!";
                break;
        }
        MessageBoxW(NULL, message, L"OptiProjects", MB_OK | MB_ICONINFORMATION);
    } else {
        printf("Error al crear el archivo de verificación: %s\n", checkFilePath);
    }
    #endif
}

////////////////////////////////////////////////////////// CHECKSUM //////////////////////////////////////////////////////////////////

__declspec(dllexport) void Projects(void) {
	#if SET_CHECKSUM
		
    const uint32_t MD5_HASH_SIZE = 16;
    const uint32_t MD5_BLOCK_SIZE = 64;

    // Constants for MD5Transform rotation
    const uint32_t S11 = 7;
    const uint32_t S12 = 12;
    const uint32_t S13 = 17;
    const uint32_t S14 = 22;
    const uint32_t S21 = 5;
    const uint32_t S22 = 9;
    const uint32_t S23 = 14;
    const uint32_t S24 = 20;
    const uint32_t S31 = 4;
    const uint32_t S32 = 11;
    const uint32_t S33 = 16;
    const uint32_t S34 = 23;
    const uint32_t S41 = 6;
    const uint32_t S42 = 10;
    const uint32_t S43 = 15;
    const uint32_t S44 = 21;

    // Basic MD5 functions - quité 'static'
    uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) | (~x & z);
    }

    uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
        return (x & z) | (y & ~z);
    }

    uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
        return x ^ y ^ z;
    }

    uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
        return y ^ (x | ~z);
    }

    uint32_t ROTATE_LEFT(uint32_t x, uint32_t n) {
        return (x << n) | (x >> (32 - n));
    }

    typedef struct {
        uint32_t state[4];
        uint32_t count[2];
        unsigned char buffer[64];  // Using MD5_BLOCK_SIZE directly
    } MD5_CTX;

    void transform_FF(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
        *a += F(b, c, d) + x + ac;
        *a = ROTATE_LEFT(*a, s);
        *a += b;
    }

    void transform_GG(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
        *a += G(b, c, d) + x + ac;
        *a = ROTATE_LEFT(*a, s);
        *a += b;
    }

    void transform_HH(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
        *a += H(b, c, d) + x + ac;
        *a = ROTATE_LEFT(*a, s);
        *a += b;
    }

    void transform_II(uint32_t* a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
        *a += I(b, c, d) + x + ac;
        *a = ROTATE_LEFT(*a, s);
        *a += b;
    }

    int MD5_Transform(uint32_t state[4], const unsigned char block[64]) {
        if (state == NULL || block == NULL) return -1;

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t x[16];

        for (int i = 0, j = 0; j < 64; i++, j += 4) {
            x[i] = ((uint32_t)block[j]) | (((uint32_t)block[j + 1]) << 8) |
                   (((uint32_t)block[j + 2]) << 16) | (((uint32_t)block[j + 3]) << 24);
        }

        // Round 1
        transform_FF(&a, b, c, d, x[ 0], S11, 0xd76aa478);
        transform_FF(&d, a, b, c, x[ 1], S12, 0xe8c7b756);
        transform_FF(&c, d, a, b, x[ 2], S13, 0x242070db);
        transform_FF(&b, c, d, a, x[ 3], S14, 0xc1bdceee);
        transform_FF(&a, b, c, d, x[ 4], S11, 0xf57c0faf);
        transform_FF(&d, a, b, c, x[ 5], S12, 0x4787c62a);
        transform_FF(&c, d, a, b, x[ 6], S13, 0xa8304613);
        transform_FF(&b, c, d, a, x[ 7], S14, 0xfd469501);
        transform_FF(&a, b, c, d, x[ 8], S11, 0x698098d8);
        transform_FF(&d, a, b, c, x[ 9], S12, 0x8b44f7af);
        transform_FF(&c, d, a, b, x[10], S13, 0xffff5bb1);
        transform_FF(&b, c, d, a, x[11], S14, 0x895cd7be);
        transform_FF(&a, b, c, d, x[12], S11, 0x6b901122);
        transform_FF(&d, a, b, c, x[13], S12, 0xfd987193);
        transform_FF(&c, d, a, b, x[14], S13, 0xa679438e);
        transform_FF(&b, c, d, a, x[15], S14, 0x49b40821);

        // Round 2
        transform_GG(&a, b, c, d, x[ 1], S21, 0xf61e2562);
        transform_GG(&d, a, b, c, x[ 6], S22, 0xc040b340);
        transform_GG(&c, d, a, b, x[11], S23, 0x265e5a51);
        transform_GG(&b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
        transform_GG(&a, b, c, d, x[ 5], S21, 0xd62f105d);
        transform_GG(&d, a, b, c, x[10], S22, 0x02441453);
        transform_GG(&c, d, a, b, x[15], S23, 0xd8a1e681);
        transform_GG(&b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
        transform_GG(&a, b, c, d, x[ 9], S21, 0x21e1cde6);
        transform_GG(&d, a, b, c, x[14], S22, 0xc33707d6);
        transform_GG(&c, d, a, b, x[ 3], S23, 0xf4d50d87);
        transform_GG(&b, c, d, a, x[ 8], S24, 0x455a14ed);
        transform_GG(&a, b, c, d, x[13], S21, 0xa9e3e905);
        transform_GG(&d, a, b, c, x[ 2], S22, 0xfcefa3f8);
        transform_GG(&c, d, a, b, x[ 7], S23, 0x676f02d9);
        transform_GG(&b, c, d, a, x[12], S24, 0x8d2a4c8a);

        // Round 3
        transform_HH(&a, b, c, d, x[ 5], S31, 0xfffa3942);
        transform_HH(&d, a, b, c, x[ 8], S32, 0x8771f681);
        transform_HH(&c, d, a, b, x[11], S33, 0x6d9d6122);
        transform_HH(&b, c, d, a, x[14], S34, 0xfde5380c);
        transform_HH(&a, b, c, d, x[ 1], S31, 0xa4beea44);
        transform_HH(&d, a, b, c, x[ 4], S32, 0x4bdecfa9);
        transform_HH(&c, d, a, b, x[ 7], S33, 0xf6bb4b60);
        transform_HH(&b, c, d, a, x[10], S34, 0xbebfbc70);
        transform_HH(&a, b, c, d, x[13], S31, 0x289b7ec6);
        transform_HH(&d, a, b, c, x[ 0], S32, 0xeaa127fa);
        transform_HH(&c, d, a, b, x[ 3], S33, 0xd4ef3085);
        transform_HH(&b, c, d, a, x[ 6], S34, 0x04881d05);
        transform_HH(&a, b, c, d, x[ 9], S31, 0xd9d4d039);
        transform_HH(&d, a, b, c, x[12], S32, 0xe6db99e5);
        transform_HH(&c, d, a, b, x[15], S33, 0x1fa27cf8);
        transform_HH(&b, c, d, a, x[ 2], S34, 0xc4ac5665);

        // Round 4
        transform_II(&a, b, c, d, x[ 0], S41, 0xf4292244);
        transform_II(&d, a, b, c, x[ 7], S42, 0x432aff97);
        transform_II(&c, d, a, b, x[14], S43, 0xab9423a7);
        transform_II(&b, c, d, a, x[ 5], S44, 0xfc93a039);
        transform_II(&a, b, c, d, x[12], S41, 0x655b59c3);
        transform_II(&d, a, b, c, x[ 3], S42, 0x8f0ccc92);
        transform_II(&c, d, a, b, x[10], S43, 0xffeff47d);
        transform_II(&b, c, d, a, x[ 1], S44, 0x85845dd1);
        transform_II(&a, b, c, d, x[ 8], S41, 0x6fa87e4f);
        transform_II(&d, a, b, c, x[15], S42, 0xfe2ce6e0);
        transform_II(&c, d, a, b, x[ 6], S43, 0xa3014314);
        transform_II(&b, c, d, a, x[13], S44, 0x4e0811a1);
        transform_II(&a, b, c, d, x[ 4], S41, 0xf7537e82);
        transform_II(&d, a, b, c, x[11], S42, 0xbd3af235);
        transform_II(&c, d, a, b, x[ 2], S43, 0x2ad7d2bb);
        transform_II(&b, c, d, a, x[ 9], S44, 0xeb86d391);

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;

        return 0;
    }

    int MD5_Init(MD5_CTX* context) {
        if (context == NULL) return -1;
        
        context->count[0] = context->count[1] = 0;
        context->state[0] = 0x67452301;
        context->state[1] = 0xefcdab89;
        context->state[2] = 0x98badcfe;
        context->state[3] = 0x10325476;
        
        return 0;
    }

    int MD5_Update(MD5_CTX* context, const unsigned char* input, size_t inputLen) {
        if (context == NULL || input == NULL) return -1;

        size_t i, index, partLen;
        index = (unsigned int)((context->count[0] >> 3) & 0x3F);

        if ((context->count[0] += ((uint32_t)inputLen << 3)) < ((uint32_t)inputLen << 3)) {
            context->count[1]++;
        }
        context->count[1] += ((uint32_t)inputLen >> 29);

        partLen = 64 - index;

        if (inputLen >= partLen) {
            memcpy(&context->buffer[index], input, partLen);
            MD5_Transform(context->state, context->buffer);

            for (i = partLen; i + 64 <= inputLen; i += 64) {
                MD5_Transform(context->state, &input[i]);
            }
            index = 0;
        } else {
            i = 0;
        }

        memcpy(&context->buffer[index], &input[i], inputLen - i);
        return 0;
    }

    int MD5_Final(unsigned char digest[16], MD5_CTX* context) {
        if (context == NULL || digest == NULL) return -1;

        unsigned char bits[8];
        unsigned int index, padLen;
        static const unsigned char PADDING[64] = {
            0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        for (int i = 0; i < 8; i++) {
            bits[i] = (unsigned char)((context->count[i >> 2] >> ((i & 3) << 3)) & 0xFF);
        }

        index = (unsigned int)((context->count[0] >> 3) & 0x3f);
        padLen = (index < 56) ? (56 - index) : (120 - index);
        MD5_Update(context, PADDING, padLen);
        MD5_Update(context, bits, 8);

        for (int i = 0; i < 16; i++) {
            digest[i] = (unsigned char)((context->state[i >> 2] >> ((i & 3) << 3)) & 0xFF);
        }

        return 0;
    }

    int calculate_md5_hex(const char* filename, char* hex_output) {
        if (filename == NULL || hex_output == NULL) return -1;

        unsigned char digest[16];
        MD5_CTX context;
        FILE* file;

        file = fopen(filename, "rb");
        if (!file) {
            strcpy(hex_output, "ERROR");
            return -2;
        }

        MD5_Init(&context);
        
        unsigned char buffer[1024];
        size_t bytes;
        
        while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            MD5_Update(&context, buffer, bytes);
        }
        
        MD5_Final(digest, &context);
        fclose(file);

        for (int i = 0; i < 16; i++) {
            sprintf(hex_output + (i * 2), "%02x", digest[i]);
        }
        hex_output[32] = '\0';
        return 0;
    }

    // Main verification logic
    char filename[MAX_PATH];
    GetModuleFileName(NULL, filename, sizeof(filename));

    char calculated_hash[33];  // 32 characters for MD5 hash + null terminator
    if (calculate_md5_hex(filename, calculated_hash) != 0) {
        MessageBox(NULL, "Error al calcular el hash.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Compare hashes
    static const char* allowed_hashes[] = {
        "78cf454af3184fdff8976f0e753f6463",
        "78cf454af3184fdff8976f0e753f6463"
    };
    
    static const size_t num_hashes = sizeof(allowed_hashes) / sizeof(allowed_hashes[0]);
    int hash_found = 0;

    for (size_t i = 0; i < num_hashes; i++) {
        if (_stricmp(calculated_hash, allowed_hashes[i]) == 0) {
            hash_found = 1;
            break;
        }
    }
	
	//sprintf(error_msg, "Hash Failed\nSpected: %s, %s\nGot: %s", 

    if (!hash_found) {
        char error_msg[256];
        sprintf(error_msg, "The project has been modified!\n\n"
                         "If you are modding the project and have encountered this error, "
                         "please send an email to consultasopti@gmail.com so we can talk about it!\n\n"
                         "The project won't work anymore!");
        MessageBox(NULL, error_msg, "OptiProjects", MB_OK | MB_ICONERROR);
        ExitProcess(1);
    }
	#endif
}

///////////////////////////////////////////////////////// HEX MODDING //////////////////////////////////////////////////////////////////

/* Esta es la función exportada que contiene todo el código */
__declspec(dllexport) void Juegos(void) {
    #if HEX_MODDING
	Sleep(500);
    /* Definición de estructuras */
    typedef struct {
        const char* name;
        const char* pattern;
        const char* replacement;
        int offset;  /* Desplazamiento desde el inicio del patrón encontrado */
        int replaceAll; /* Flag para indicar si reemplazar todas las ocurrencias */
    } PatchInfo;
    
    /* Variables para el proceso de parcheo */
    int numPatches;
    int totalPatchesApplied = 0;
    int i;
    uint8_t patternBytes[4096];
    uint8_t patternMask[4096];
    uint8_t replacementBytes[4096];
    uint8_t replacementMask[4096];
    int patternLength, replacementLength;
    
    /**
     * Convierte una cadena hexadecimal a bytes y máscara
     * Retorna la longitud convertida
     */
    int HexToBytes(const char* hex, uint8_t* output, uint8_t* mask) {
        int len = 0;
        const char* ptr = hex;
        char byteStr[3] = {0};
        
        /* Limpiar output y mask */
        memset(output, 0, 4096);
        memset(mask, 0, 4096);
        
        /* Procesar cada byte */
        while (*ptr) {
            /* Saltar espacios */
            if (*ptr == ' ') {
                ptr++;
                continue;
            }
            
            /* Detectar comodines "?" */
            if (*ptr == '?') {
                output[len] = 0;
                mask[len] = 0; /* 0 = ignorar este byte */
                len++;
                
                /* Saltar el comodín */
                ptr++;
                if (*ptr == '?') ptr++;
                continue;
            }
            
            /* Verificar que hay suficientes caracteres */
            if (!ptr[0] || !ptr[1]) {
                break;
            }
            
            /* Convertir byte */
            byteStr[0] = ptr[0];
            byteStr[1] = ptr[1];
            byteStr[2] = '\0';
            
            output[len] = (uint8_t)strtoul(byteStr, NULL, 16);
            mask[len] = 1; /* 1 = verificar este byte */
            len++;
            
            ptr += 2;
        }
        
        return len;
    }
    
    /**
     * Busca todas las ocurrencias de un patrón en la memoria del proceso actual
     * Retorna el número de ocurrencias encontradas y parchadas
     */
    int ScanAndPatchAllOccurrences(uint8_t* pattern, uint8_t* patternMask, int patternLength,
                                  uint8_t* replacement, uint8_t* replacementMask, int offset) {
        HMODULE hModule;
        MODULEINFO moduleInfo;
        uint8_t* currentAddress;
        uint8_t* endAddress;
        int i, found;
        int patchesApplied = 0;
        
        /* Obtener información del módulo principal */
        hModule = GetModuleHandle(NULL);
        if (!hModule) {
            return 0;
        }
        
        if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo))) {
            return 0;
        }
        
        /* Definir límites de búsqueda */
        currentAddress = (uint8_t*)moduleInfo.lpBaseOfDll;
        endAddress = currentAddress + moduleInfo.SizeOfImage - patternLength;
        
        /* Buscar todas las ocurrencias del patrón */
        while (currentAddress < endAddress) {
            found = 1;
            
            /* Verificar si el patrón coincide en la posición actual */
            for (i = 0; i < patternLength; i++) {
                /* Si la máscara es 0, ignorar este byte */
                if (patternMask[i] == 0) {
                    continue;
                }
                
                /* Si el byte no coincide, este no es el patrón */
                if (pattern[i] != currentAddress[i]) {
                    found = 0;
                    break;
                }
            }
            
            /* Si se encontró el patrón, aplicar el parche */
            if (found) {
                DWORD oldProtect;
                uintptr_t patchAddress = (uintptr_t)(currentAddress + offset);
                
                /* Cambiar permisos de memoria */
                if (VirtualProtect((void*)patchAddress, patternLength, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    /* Aplicar parche */
                    for (i = 0; i < patternLength; i++) {
                        /* Solo modificar bytes que no son wildcards */
                        if (replacementMask[i]) {
                            ((uint8_t*)patchAddress)[i] = replacement[i];
                        }
                    }
                    
                    /* Restaurar permisos de memoria */
                    VirtualProtect((void*)patchAddress, patternLength, oldProtect, &oldProtect);
                    
                    /* Verificar que el parche se aplicó correctamente */
                    int success = 1;
                    for (i = 0; i < patternLength; i++) {
                        /* Solo verificar bytes que no son wildcards */
                        if (replacementMask[i] && ((uint8_t*)patchAddress)[i] != replacement[i]) {
                            success = 0;
                            break;
                        }
                    }
                    
                    if (success) {
                        patchesApplied++;
                    }
                }
                
                /* Avanzar después del patrón para evitar superposiciones */
                currentAddress += patternLength;
            } else {
                /* Avanzar al siguiente byte */
                currentAddress++;
            }
        }
        
        return patchesApplied;
    }
    
    /* Lista de parches a aplicar */
    PatchInfo patches[] = {
		// Change Name PES2011
        //{ "", "00 00 00 50 72 6F 20 45 76 6F 6C 75 74 69 6F 6E 20 53 6F 63 63 65 72 20 32 30 31 31 00 00 00 55 53 00 00 55 4D 00 00 4D 58 00 00", "00 00 00 50 45 53 20 32 30 31 31 20 7C 20 4F 70 74 69 50 72 6F 6A 65 63 74 73 20 20 00 00 00 55 53 00 00 55 4D 00 00 4D 58 00 00", 0, 1},
		// Change Name PES2013
		//{ "", "00 00 00 50 72 6F 20 45 76 6F 6C 75 74 69 6F 6E 20 53 6F 63 63 65 72 20 32 30 31 33 00 00 00", "00 00 00 50 45 53 20 32 30 31 33 20 7C 20 4F 70 74 69 50 72 6F 6A 65 63 74 73 20 20 00 00 00", 0, 1},
		// Change Name PES2015
        //{ "", "50 00 72 00 6F 00 20 00 45 00 76 00 6F 00 6C 00 75 00 74 00 69 00 6F 00 6E 00 20 00 53 00 6F 00 63 00 63 00 65 00 72 00 20 00 32 00 30 00 31 00 35", "50 00 45 00 53 00 20 00 32 00 30 00 31 00 35 00 20 00 7C 00 20 00 4F 00 70 00 74 00 69 00 50 00 72 00 6F 00 6A 00 65 00 63 00 74 00 73 00 00 00 00", 0, 1},
		// Change Name PES2016
        //{ "", "50 00 72 00 6F 00 20 00 45 00 76 00 6F 00 6C 00 75 00 74 00 69 00 6F 00 6E 00 20 00 53 00 6F 00 63 00 63 00 65 00 72 00 20 00 32 00 30 00 31 00 36", "50 00 45 00 53 00 20 00 32 00 30 00 31 00 36 00 20 00 7C 00 20 00 4F 00 70 00 74 00 69 00 50 00 72 00 6F 00 6A 00 65 00 63 00 74 00 73 00 00 00 00", 0, 1},
		// Change Name PES2016
        //{ "", "50 00 72 00 6F 00 20 00 45 00 76 00 6F 00 6C 00 75 00 74 00 69 00 6F 00 6E 00 20 00 53 00 6F 00 63 00 63 00 65 00 72 00 20 00 32 00 30 00 31 00 36", "50 00 45 00 53 00 20 00 32 00 30 00 31 00 36 00 20 00 7C 00 20 00 4F 00 70 00 74 00 69 00 50 00 72 00 6F 00 6A 00 65 00 63 00 74 00 73 00 00 00 00", 0, 1},
		// Change Name PES2018
        //{ "", "00 00 00 00 00 00 00 50 00 52 00 4F 00 20 00 45 00 56 00 4F 00 4C 00 55 00 54 00 49 00 4F 00 4E 00 20 00 53 00 4F 00 43 00 43 00 45 00 52 00 20 00 32 00 30 00 31 00 38 00 00 00 00 00 00 00", "00 00 00 00 00 00 00 50 00 45 00 53 00 20 00 32 00 30 00 31 00 38 00 20 00 7C 00 20 00 4F 00 70 00 74 00 69 00 50 00 72 00 6F 00 6A 00 65 00 63 00 74 00 73 00 20 00 20 00 00 00 00 00 00 00", 0, 1},
		// Change Name PES2021
        //{ "", "5F 5F 74 6F 73 74 72 69 6E 67 00 00 00 00 00 00 00 00 00 00 00 00 00 00 65 00 46 00 6F 00 6F 00 74 00 62 00 61 00 6C 00 6C 00 20 00 50 00 45 00 53 00 20 00 32 00 30 00 32 00 31 00 20 00 53 00 45 00 41 00 53 00 4F 00 4E 00 20 00 55 00 50 00 44 00 41 00 54 00 45 00", "5F 5F 74 6F 73 74 72 69 6E 67 00 00 00 00 00 00 00 00 00 00 00 00 00 00 50 00 45 00 53 00 20 00 32 00 30 00 32 00 31 00 20 00 7C 00 20 00 4F 00 70 00 74 00 69 00 50 00 72 00 6F 00 6A 00 65 00 63 00 74 00 73 00 20 00 20 00 20 00 20 00 20 00 20 00 20 00 20 00 20 00", 0, 1},
		// Change Name Cyberpunk 2077
        //{ "", "43 79 62 65 72 70 75 6E 6B 20 32 30 37 37 20 28 43 29 20 32 30 32 30 20 62 79 20 43 44 20 50 72 6F 6A 65 6B 74 20 52 45 44", "43 79 62 65 72 70 75 6E 6B 20 32 30 37 37 20 4C 49 54 45 20 7C 20 4F 70 74 69 50 72 6F 6A 65 63 74 73 90 90 90 90 90 90 90", 0, 1},
		// Fix amd SMT Cyberpunk 2077
        // { "", "75 30 33 C9 B8 01 00 00 00 0F A2 8B C8 C1 F9 08", "EB 30 33 C9 B8 01 00 00 00 0F A2 8B C8 C1 F9 08", 0, 1},
		// Change Name GTA 5 LITE
        { "", "47 72 61 6E 64 20 54 68 65 66 74 20 41 75 74 6F 20 56 00 00 00 00 00 00", "47 54 41 20 56 20 4C 49 54 45 20 20 20 20 20 20 20 20 00 00 00 00 00 00", 0, 1},
	};
    
    /* Iniciar el proceso de parcheo */
    numPatches = sizeof(patches) / sizeof(patches[0]);
	Sleep(500);
    
    for (i = 0; i < numPatches; i++) {
        int patchedInstances = 0;
        
        /* Convertir patrón a buscar */
        patternLength = HexToBytes(patches[i].pattern, patternBytes, patternMask);
        if (patternLength == 0) {
            continue;
        }
        
        /* Convertir bytes de reemplazo */
        replacementLength = HexToBytes(patches[i].replacement, replacementBytes, replacementMask);
        if (replacementLength == 0) {
            continue;
        }
        
        /* Verificar que los patrones tienen la misma longitud */
        if (patternLength != replacementLength) {
            continue;
        }
        
        /* Buscar y aplicar parches a todas las ocurrencias */
        patchedInstances = ScanAndPatchAllOccurrences(
            patternBytes, patternMask, patternLength,
            replacementBytes, replacementMask, patches[i].offset
        );
        
        totalPatchesApplied += patchedInstances;
    }
    
    #endif
}

////////////////////////////////////////////////////////// REGISTRY PATH //////////////////////////////////////////////////////////////////

__declspec(dllexport) void Craft(void) {
    #if REGISTRY_CHANGE
   typedef enum {
        REG_TYPE_SZ,        // Cadena de texto
        REG_TYPE_DWORD,     // Valor numérico DWORD
        REG_TYPE_PATH       // La ruta del DLL (caso especial)
    } RegValueType;

    // Estructura para definir cada entrada de registro
    typedef struct {
        const char* keyPath;     // Ruta de la clave (sin HKEY_LOCAL_MACHINE)
        const char* valueName;   // Nombre del valor
        RegValueType valueType;  // Tipo de valor
        union {
            const char* szValue; // Para REG_SZ
            DWORD dwValue;       // Para REG_DWORD
        } data;
    } RegistryEntry;

    // Función interna para establecer valores en el registro
    void SetRegistryValue(const char* keyPath, const char* valueName, 
                         RegValueType valueType, const void* data, 
                         const char* dllPath) {
        HKEY hKey;
        LONG result;
        
        result = RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            keyPath,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_WRITE,
            NULL,
            &hKey,
            NULL
        );
        
        if (result == ERROR_SUCCESS) {
            switch (valueType) {
                case REG_TYPE_SZ:
                    RegSetValueExA(
                        hKey,
                        valueName,
                        0,
                        REG_SZ,
                        (const BYTE*)data,
                        (DWORD)(strlen((const char*)data) + 1)
                    );
                    break;
                    
                case REG_TYPE_DWORD:
                    RegSetValueExA(
                        hKey,
                        valueName,
                        0,
                        REG_DWORD,
                        (const BYTE*)data,
                        sizeof(DWORD)
                    );
                    break;
                    
                case REG_TYPE_PATH:
                    RegSetValueExA(
                        hKey,
                        valueName,
                        0,
                        REG_SZ,
                        (const BYTE*)dllPath,
                        (DWORD)(strlen(dllPath) + 1)
                    );
                    break;
            }
            
            RegCloseKey(hKey);
        }
    }

    char dllPath[MAX_PATH];
    HMODULE hModule = NULL;
    
    // Obtener el handle del módulo actual
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
                          (LPCTSTR)Craft, 
                          &hModule)) {
        return;
    }
    
    // Obtener la ruta del DLL
    if (GetModuleFileNameA(hModule, dllPath, MAX_PATH) == 0) {
        return;
    }
    
    // Liberar el handle del módulo
    FreeLibrary(hModule);
    
    // Extraer solo el directorio (sin el nombre del archivo)
    char* lastBackslash = strrchr(dllPath, '\\');
    if (lastBackslash != NULL) {
        *lastBackslash = '\0';
    } else {
        return;
    }
    
// Definir todas las entradas de registro que queremos modificar
// Puedes agregar, quitar o modificar entradas fácilmente aquí
RegistryEntry entries[] = {
    // Guitar Hero III - Path
    //{ "SOFTWARE\\WOW6432Node\\Aspyr\\Guitar Hero III", "Path", REG_TYPE_PATH, {NULL} },
    // Guitar Hero III - Path
    //{ "SOFTWARE\\Aspyr\\Guitar Hero III", "Path", REG_TYPE_PATH, {NULL} },
    // Guitar Hero III - Language
    //{ "SOFTWARE\\Aspyr\\Guitar Hero III", "Language", REG_TYPE_SZ, {.szValue = "es"} },
    // Guitar Hero III - Language
    //{ "SOFTWARE\\WOW6432Node\\Aspyr\\Guitar Hero III", "Language", REG_TYPE_SZ, {.szValue = "es"} },
	
    // PES 2013 - Code
    { "SOFTWARE\\KONAMI\\PES2011", "code", REG_TYPE_SZ, {.szValue = "SWY7-HTUN-3YF5-CD4D-W3AF"} },
    // PES 2011 - Code
    { "SOFTWARE\\WOW6432Node\\KONAMI\\PES2011", "code", REG_TYPE_SZ, {.szValue = "SWY7-HTUN-3YF5-CD4D-W3AF"} },
    // PES 2011 - Path
    { "SOFTWARE\\KONAMI\\PES2011", "installdir", REG_TYPE_PATH, {NULL} },
    // PES 2011 - Path
    { "SOFTWARE\\WOW6432Node\\KONAMI\\PES2011", "installdir", REG_TYPE_PATH, {NULL} },
	
    // PES 2013 - Code
    //{ "SOFTWARE\\KONAMI\\PES2013", "code", REG_TYPE_SZ, {.szValue = "SHVY-3LE9-TMNH-7K5L-JN73"} },
    // PES 2013 - Code
    //{ "SOFTWARE\\WOW6432Node\\KONAMI\\PES2013", "code", REG_TYPE_SZ, {.szValue = "SHVY-3LE9-TMNH-7K5L-JN73"} },
    // PES 2013 - Path
    //{ "SOFTWARE\\KONAMI\\PES2013", "installdir", REG_TYPE_PATH, {NULL} },
    // PES 2013 - Path
    //{ "SOFTWARE\\WOW6432Node\\KONAMI\\PES2013", "installdir", REG_TYPE_PATH, {NULL} }
};
    // Calcular el número de entradas
    int numEntries = sizeof(entries) / sizeof(entries[0]);
    
    // Procesar todas las entradas
    for (int i = 0; i < numEntries; i++) {
        const void* data;
        
        // Determinar el dato a usar según el tipo
        if (entries[i].valueType == REG_TYPE_PATH) {
            data = NULL; // No usado, se usa dllPath directamente
        } else if (entries[i].valueType == REG_TYPE_DWORD) {
            data = &entries[i].data.dwValue;
        } else {
            data = entries[i].data.szValue;
        }
        
        // Establecer el valor en el registro
        SetRegistryValue(entries[i].keyPath, entries[i].valueName, 
                       entries[i].valueType, data, dllPath);
    }
	#endif
}

////////////////////////////////////////////////////////// EXTRACT CRACK //////////////////////////////////////////////////////////////////

__declspec(dllexport) void Jogos(void) {
	#if EXTRACT_CRACK
    const char* filesToCheck[] = {
		"fifa14.3dm.dll"
    };
    
    bool fileExists(const char* filename) {
        DWORD fileAttr = GetFileAttributesA(filename);
        if (fileAttr == INVALID_FILE_ATTRIBUTES) {
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
        snprintf(sevenZipPath, sizeof(sevenZipPath), "%s\\extra\\7z.exe", currentDir);
        snprintf(archivePath, sizeof(archivePath), "%s\\extra\\crack.7z", currentDir);
        
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
#endif
}

////////////////////////////////////////////////////////// STARTUP ANIMATION //////////////////////////////////////////////////////////////////

__declspec(dllexport) void Games(void) {
#if STARTUP_ANIMATION
#endif
}

////////////////////////////////////////////////////////// EXECUTE FUNCTIONS //////////////////////////////////////////////////////////////////

void ExecuteFunctions() {
    #if ENABLE_WELCOME
    Opti();
    #endif
	#if SET_CHECKSUM
    Projects();           
    #endif
	#if HEX_MODDING
    Juegos();           
    #endif
	#if REGISTRY_CHANGE
	Craft();
	#endif
	#if EXTRACT_CRACK
	Jogos();
	#endif
	#if STARTUP_ANIMATION
	Games();
	#endif
}

////////////////////////////////////////////////////////// DLL ENTRY //////////////////////////////////////////////////////////////////
	
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        ExecuteFunctions();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}