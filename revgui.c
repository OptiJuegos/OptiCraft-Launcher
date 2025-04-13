#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 500
#define INI_FILE "rev.ini"

char g_LauncherDir[MAX_PATH] = {};
char g_RevIniName[MAX_PATH] = {};
char g_ProcName[MAX_PATH] = {};
char g_LibraryName[MAX_PATH] = {};
char g_GameAppId[256] = {};

wchar_t **g_Argv = NULL;
char g_AdditionalProcName[MAX_PATH] = {};
int g_NumArgs = 0;

bool GetSteamAppID(char *pszOut) {
    FILE* f = fopen("steam_appid.txt", "r");
    if (!f) {
        *pszOut = '\0';
        return false;
    }

    fread(pszOut, sizeof(char), BUFFER_SIZE, f);
    fclose(f);

    char *psz = strchr(pszOut, ' ');
    if (psz) {
        *psz = '\0'; 
    }

    return true;
}

void CreateSharedMemFile(HANDLE *hMapView, HANDLE *hFileMap, HANDLE *hEvent) {
    char szDest[260];

    *hFileMap = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, "Local\\SteamStart_SharedMemFile");
    if (!*hFileMap) {
        sprintf(szDest, "Unable to CreateFileMapping: %i", GetLastError());
        MessageBoxA(NULL, szDest, "Error", MB_OK);
        return;
    }
    
    *hMapView = MapViewOfFile(*hFileMap, SECTION_ALL_ACCESS, 0, 0, 0);
    if (!*hMapView) {
        sprintf(szDest, "Unable to MapViewOfFile: %i", GetLastError());
        MessageBoxA(NULL, szDest, "Error", MB_OK);
        CloseHandle(*hFileMap);
        return;
    }

    *hEvent = CreateEventA(NULL, FALSE, FALSE, "Local\\SteamStart_SharedMemLock");
    if (!*hEvent) {
        sprintf(szDest, "Unable to CreateEvent: %i", GetLastError());
        MessageBoxA(NULL, szDest, "Error", MB_OK);
        CloseHandle(*hFileMap);
        CloseHandle(*hMapView);
        return;
    }

    SetEvent(*hEvent);
}

void SetActiveProcess(int pid) {
    DWORD dwD;
    HKEY phkResult;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Valve\\Steam\\ActiveProcess", 0, KEY_WRITE, &phkResult) == ERROR_SUCCESS) {
        RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Valve\\Steam\\ActiveProcess", 0, NULL, 0, KEY_WRITE, NULL, &phkResult, &dwD);
    }

    RegSetValueExA(phkResult, "pid", 0, REG_DWORD, (BYTE *)&pid, sizeof(pid));
    RegCloseKey(phkResult);
}

void SetSteamClientDll(char *pszLib) {
    DWORD dwD;
    HKEY phkResult;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Valve\\Steam\\ActiveProcess", 0, KEY_WRITE, &phkResult) == ERROR_SUCCESS) {
        RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Valve\\Steam\\ActiveProcess", 0, NULL, 0, KEY_WRITE, NULL, &phkResult, &dwD);
    }

    RegSetValueExA(phkResult, "SteamClientDll", 0, REG_SZ, (BYTE *)pszLib, strlen(pszLib) + 1);
    RegCloseKey(phkResult);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    if (!GetModuleFileNameA(NULL, g_LauncherDir, sizeof(g_LauncherDir))) {
        MessageBoxA(NULL, "Unable to initialize the process", "Error", MB_ICONWARNING | MB_SYSTEMMODAL);
        return -1;
    }

    char *psz = strrchr(g_LauncherDir, '\\');
    if (psz) {
        *psz = '\0';
    }

    strcpy(g_RevIniName, g_LauncherDir);
    strcat(g_RevIniName, "\\" INI_FILE);

    g_Argv = CommandLineToArgvW(GetCommandLineW(), &g_NumArgs);
    for (int i = 0; i < g_NumArgs; i++) {
        if (_wcsicmp(g_Argv[i], L"-launch") == 0) {
            wcstombs(g_ProcName, g_Argv[++i], sizeof(g_ProcName) - 1);
        } else if (_wcsicmp(g_Argv[i], L"-appid") == 0) {
            wcstombs(g_GameAppId, g_Argv[++i], sizeof(g_GameAppId) - 1);
        } else if (i != 0) {
            char szArg[128];
            wcstombs(szArg, g_Argv[i], sizeof(szArg) - 1);
            strcat(g_AdditionalProcName, szArg);
            strcat(g_AdditionalProcName, " ");
        }
    }

    if (strlen(g_AdditionalProcName) != 0) {
        strcat(g_ProcName, g_AdditionalProcName);
    }

    if (!GetPrivateProfileStringA("Loader", "ProcName", "", g_ProcName, sizeof(g_ProcName), g_RevIniName)) {
        MessageBoxA(NULL, "ProcName value not found on command line or in rev.ini. Please edit the file.", "Error", MB_ICONWARNING | MB_SYSTEMMODAL);
        return -1;
    }

    if (!GetSteamAppID(g_GameAppId)) {
        MessageBoxA(NULL, "No steam_appid.txt detected, the game might not launch correctly", "Warning", MB_ICONWARNING | MB_SYSTEMMODAL);
    }

    if (g_GameAppId[0] != '\0') {
        SetEnvironmentVariableA("SteamGameId", g_GameAppId);
        SetEnvironmentVariableA("SteamAppId", g_GameAppId);
    }

    char szSteamClientDll[MAX_PATH];
    if (GetPrivateProfileStringA("Loader", "SteamClientDll", "", szSteamClientDll, sizeof(szSteamClientDll), g_RevIniName)) {
        if (szSteamClientDll[0] != '\0') {
            strcpy(g_LibraryName, g_LauncherDir);
            strcat(g_LibraryName, "\\");
            strcat(g_LibraryName, szSteamClientDll);

            if (!LoadLibraryA(g_LibraryName)) {
                char szDest[512];
                sprintf(szDest, "Can't find steamclient.dll relative to executable path %s", g_LauncherDir);
                MessageBoxA(NULL, szDest, "Warning", MB_ICONWARNING | MB_SYSTEMMODAL);
                return -1;
            }

            SetSteamClientDll(g_LibraryName);
        }
    }

    strcpy(g_LibraryName, g_LauncherDir);
    strcat(g_LibraryName, "\\steam.dll");

    if (!LoadLibraryA(g_LibraryName)) {
        char szDest[512];
        sprintf(szDest, "Can't find steam.dll relative to executable path %s", g_LauncherDir);
        MessageBoxA(NULL, szDest, "Warning", MB_ICONWARNING | MB_SYSTEMMODAL);
        return -1;
    }
    
    const char CLASS_NAME[] = "GameStartWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Get the screen size
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create the window with a fixed size
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "RevGui",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
        (screenWidth - 400) / 2, (screenHeight - 330) / 2, 400, 330,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void UpdatePlayerName(const char* playerName) {
    FILE *file = fopen(INI_FILE, "r");
    if (file == NULL) {
        MessageBox(NULL, "Error opening rev.ini", "Error", MB_OK | MB_ICONERROR);
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
        MessageBox(NULL, "Error writing to rev.ini", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        fputs(lines[i], file);
    }

    fclose(file);
}

void StartGameApp(HWND hwnd) {
    HANDLE hFileMap = 0;
    HANDLE hMapView = 0;
    HANDLE hSteamMem = 0;
    CreateSharedMemFile(&hMapView, &hFileMap, &hSteamMem);

    STARTUPINFOA StartupInformation = {};
    PROCESS_INFORMATION ProcessInformation = {};

    StartupInformation.cb = sizeof(StartupInformation);

    if (CreateProcessA(NULL, g_ProcName, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInformation, &ProcessInformation)) {
        SetActiveProcess(ProcessInformation.dwProcessId);
        
        // Close the process handle here
        CloseHandle(ProcessInformation.hProcess); // Close the process handle
        CloseHandle(ProcessInformation.hThread);  // Close the thread handle
    } else {
        char szDest[512];
        sprintf(szDest, "Unable to execute command %s (%d)", g_ProcName, GetLastError());
        MessageBoxA(HWND_DESKTOP, szDest, "Error", MB_ICONWARNING | MB_SYSTEMMODAL);
    }

    if (hSteamMem) CloseHandle(hSteamMem);
    if (hMapView) CloseHandle(hMapView);
    if (hFileMap) CloseHandle(hFileMap);

    PostMessage(hwnd, WM_CLOSE, 0, 0); // Close the window after starting Launcher
}

void LoadPlayerName(char *playerName, size_t size) {
    GetPrivateProfileStringA("steamclient", "PlayerName", "", playerName, size, g_RevIniName);
}

void LoadProcName(char *procName, size_t size) {
    GetPrivateProfileStringA("Loader", "ProcName", "", procName, size, g_RevIniName);
}

void UpdateParameters(HWND editName, HWND editParams) {
    char playerName[BUFFER_SIZE];
    char procName[BUFFER_SIZE];

    // Get text box names
    GetWindowText(editName, playerName, BUFFER_SIZE);
    GetWindowText(editParams, procName, BUFFER_SIZE);

    // Update PlayerName in the ini file
    UpdatePlayerName(playerName);

    // Update ProcName in the ini file
    FILE *file = fopen(g_RevIniName, "r+");
    if (file) {
        char temp[BUFFER_SIZE];
        char lines[100][BUFFER_SIZE]; // Assuming a maximum of 100 lines
        int lineCount = 0;

        // Read all lines into an array
        while (fgets(temp, sizeof(temp), file) && lineCount < 100) {
            if (strncmp(temp, "ProcName=", 9) == 0) {
                snprintf(lines[lineCount++], BUFFER_SIZE, "ProcName=%s\n", procName);
            } else {
                snprintf(lines[lineCount++], BUFFER_SIZE, "%s", temp);
            }
        }
        fclose(file);

        // Rewrite the file
        file = fopen(g_RevIniName, "w");
        if (file) {
            for (int i = 0; i < lineCount; i++) {
                fputs(lines[i], file);
            }
            fclose(file);
        } else {
            MessageBox(NULL, "Error writing to rev.ini", "Error", MB_OK | MB_ICONERROR);
        }
    } else {
        MessageBox(NULL, "Error opening rev.ini", "Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND editName;
    static HWND editParams;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE: {
            // Label for "Name:"
            CreateWindow(
                "STATIC",
                "Name:",
                WS_VISIBLE | WS_CHILD,
                50, 50, 100, 20,
                hwnd, NULL, NULL, NULL);

            // Text box for the name
            editName = CreateWindow(
                "EDIT",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                50, 80, 300, 25,
                hwnd, NULL, NULL, NULL);

            // Load the name from rev.ini
            char playerName[BUFFER_SIZE];
            LoadPlayerName(playerName, sizeof(playerName));
            SetWindowText(editName, playerName);

            // Label for "Parameters:"
            CreateWindow(
                "STATIC",
                "Parameters:",
                WS_VISIBLE | WS_CHILD,
                50, 120, 100, 20,
                hwnd, NULL, NULL, NULL);

            // Text box for Parameters
            editParams = CreateWindow(
                "EDIT",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                50, 150, 300, 25,
                hwnd, NULL, NULL, NULL);

            // Load the ProcName from rev.ini
            char procName[BUFFER_SIZE];
            LoadProcName(procName, sizeof(procName));
            SetWindowText(editParams, procName);

            // Button to start Launcher
            CreateWindow(
                "BUTTON",
                "Start Launcher",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 200, 300, 30,
                hwnd, (HMENU)1, NULL, NULL);
            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // ID for the "Start Launcher" button
                UpdateParameters(editName, editParams); // Update parameters automatically
                StartGameApp(hwnd); // Start the game and close the window
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}