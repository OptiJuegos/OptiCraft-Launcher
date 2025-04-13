#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define UPLAY_EXPORT extern __declspec(dllexport)

HANDLE fileuplay = 0;
void* DirectoryBuffer = 0;

bool created = false;

int val = 0;

struct Chunks {
    ULONG_PTR d1;
    void* d2;
};

struct Overmapped {
    signed char pd[4];
    int32_t f4;
    int32_t f8;
};

struct FileOpen {
    DWORD d1;
};

struct FileOpenTwo {
    DWORD addr1;
    DWORD addr2;
    DWORD addr3;
};

struct FileRead {
    DWORD addr1;
    DWORD addr2;
    DWORD addr3;
};

struct FileList {
    DWORD num;
    void* bufferstring;
    DWORD pointer;
};

struct MyFileRef {
    HANDLE hFile;
    int numfile;
    char nameoffile[4024];
};

struct MyFileRef FileVector[1000];
int FileVectorSize = 0;

struct UplayConfig {
    char UserName[0x200];
    char UserEmail[0x200];
    char password[0x200];
    char GameLanguage[0x200];
    char SaveDir[MAX_PATH];
    char UserId[1024];
    char CdKey[1024];
    char TickedId[1024];
    bool Offline;
    bool appowned;
    int cdkey1;
    int cdkey2;
    uint32_t gameAppId;
} Uplay_Configuration = {
    "Rat",
    "UplayEmu@rat43.com",
    "UplayPassword74",
    "en-US",
    "Default",
    "c91c91c9-1c91-c91c-91c9-1c91c91c91c9",
    "1111-2222-3333-4444",
    "noT456umPqRt",
    false,
    true,
    0,
    0,
    0
};

void* tempbuffer;

char* AppIdtoString(int appid) {
    static char buffer[32];  // Buffer estático para almacenar el resultado
    sprintf(buffer, "%d", appid);
    return buffer;
}

DWORD GetFilePointer(HANDLE hFile) {
    return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
}

void CreatePath(const char* Path) {
    if (!created) {
        CHAR data[MAX_PATH] = { 0 };
        lstrcpyA(data, Path);
        int size = lstrlenA(data) + 1;
        CHAR out[MAX_PATH] = { 0 };
        int i = 0;
        for (;;) {
            if (data[i] == '\0')
                break;

            memcpy(&out[i], &data[i], 1);
            if (out[i] == '\\') {
                if (GetFileAttributesA(out) == INVALID_FILE_ATTRIBUTES) {
                    CreateDirectoryA(out, NULL);
                }
            }
            i++;
        }
        if (GetFileAttributesA(out) == INVALID_FILE_ATTRIBUTES) {
            CreateDirectoryA(out, NULL);
        }
        created = true;
    }
}

static char datapp[1024];

const char* AttachDirFile(const char* Path, const char* file) {
    memset(datapp, 0, sizeof(datapp));
    sprintf(datapp, "%s\\%s", Path, file);
    return datapp;
}

char dataqq[1024] = { 0 };
const char* FormatDir(const char* dir, bool setslashes) {
    if (dataqq[0] == 0) {
        if (strcmp(Uplay_Configuration.SaveDir, "Default") == 0) {
            char tmppath[MAX_PATH];
            GetModuleFileNameA(NULL, tmppath, MAX_PATH);
            int size = lstrlenA(tmppath);
            for (int i = size; i > 0; i--) {
                if (tmppath[i] == '\\') {
                    memset(&tmppath[i], 0, size - i);
                    break;
                }
            }
            sprintf(dataqq, "%s\\%s\\%s\\%u", tmppath, dir, Uplay_Configuration.UserName, Uplay_Configuration.gameAppId);
            return dataqq;
        }
        sprintf(dataqq, "%s\\%s\\%s\\%s", Uplay_Configuration.SaveDir, dir, Uplay_Configuration.UserName, AppIdtoString(Uplay_Configuration.gameAppId));
        return dataqq;
    }
    return dataqq;
}

bool IsTargetExist(LPCSTR path) {
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

// Exports implementations
UPLAY_EXPORT int UPLAY_ACH_EarnAchievement() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ACH_GetAchievementImage() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ACH_GetAchievements() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ACH_ReleaseAchievementImage() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ACH_ReleaseAchievementList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ACH_Write() {
    return 0;
}

UPLAY_EXPORT int UPLAY_AVATAR_Get(void* buf1) {
    return 1;
}

UPLAY_EXPORT int UPLAY_AVATAR_GetAvatarIdForCurrentUser() {
    return 1;
}

UPLAY_EXPORT int UPLAY_AVATAR_GetBitmap() {
    return 1;
}

UPLAY_EXPORT int UPLAY_AVATAR_Release() {
    return 0;
}

UPLAY_EXPORT int UPLAY_ClearGameSession() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_AddPlayedWith() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_AddToBlackList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_DisableFriendMenuItem() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_EnableFriendMenuItem() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_GetFriendList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_Init() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_InviteToGame() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_IsBlackListed() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_IsFriend() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_RemoveFriendship() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_RemoveFromBlackList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_RequestFriendship() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_RespondToGameInvite() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_ShowFriendSelectionUI() {
    return 0;
}

UPLAY_EXPORT int UPLAY_FRIENDS_ShowInviteFriendsToGameUI() {
    return 0;
}

UPLAY_EXPORT int UPLAY_GetLastError() {
    return 0;
}

UPLAY_EXPORT int UPLAY_GetNextEvent() {
    return 0;
}

UPLAY_EXPORT int UPLAY_GetOverlappedOperationResult(void* buf1, int* buf2) {
    struct Overmapped* ovr = (struct Overmapped*)buf1;
    if (!ovr->f4) {
        return 0;
    } else {
        *buf2 = ovr->f8;
        return 1;
    }
}

UPLAY_EXPORT int UPLAY_HasOverlappedOperationCompleted(void* buf1) {
    struct Overmapped* ovr = (struct Overmapped*)buf1;
    return ovr->f4;
}

UPLAY_EXPORT int UPLAY_INSTALLER_AreChunksInstalled() {
    return 0;
}

UPLAY_EXPORT int UPLAY_INSTALLER_GetChunkIdsFromTag() {
    return 0;
}

struct Chunks* chunks = NULL;

UPLAY_EXPORT int UPLAY_INSTALLER_GetChunks(void* buf1) {
    chunks = (struct Chunks*)VirtualAlloc(0, sizeof(struct Chunks), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    chunks->d1 = 1;
    chunks->d2 = VirtualAlloc(0, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#ifdef _WIN64
    memcpy(buf1, &chunks, sizeof(chunks));
#else
    memcpy(buf1, &chunks, sizeof(chunks));
#endif
    return 1;
}

UPLAY_EXPORT const char* UPLAY_INSTALLER_GetLanguageUtf8() {
    return Uplay_Configuration.GameLanguage;
}

UPLAY_EXPORT int UPLAY_INSTALLER_Init() {
    return 1;
}

UPLAY_EXPORT int UPLAY_INSTALLER_ReleaseChunkIdList() {
    VirtualFree((void*)chunks, 0, MEM_DECOMMIT);
    return 1;
}

UPLAY_EXPORT int UPLAY_INSTALLER_UpdateInstallOrder() {
    return 1;
}

UPLAY_EXPORT int UPLAY_Init() {
    return 1;
}

UPLAY_EXPORT int UPLAY_METADATA_ClearContinuousTag() {
    return 1;
}

UPLAY_EXPORT int UPLAY_METADATA_SetContinuousTag() {
    return 1;
}

UPLAY_EXPORT int UPLAY_METADATA_SetSingleEventTag() {
    return 1;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Apply() {
    return 1;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Close() {
    return 1;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Enumerate() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Get() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Open() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OPTIONS_ReleaseKeyValueList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OPTIONS_Set() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OPTIONS_SetInGameState() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_SetShopUrl() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_Show() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_ShowBrowserUrl() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_ShowFacebookAuthentication() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_ShowNotification() {
    return 0;
}

UPLAY_EXPORT int UPLAY_OVERLAY_ShowShopUrl() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_DisablePartyMemberMenuItem() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_EnablePartyMemberMenuItem() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_GetFullMemberList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_GetId() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_GetInGameMemberList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_Init() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_InvitePartyToGame() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_InviteToParty() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_IsInParty() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_IsPartyLeader() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_PromoteToLeader() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_RespondToGameInvite() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_SetGuest() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PARTY_SetUserData() {
    return 1;
}

UPLAY_EXPORT int UPLAY_PARTY_ShowGameInviteOverlayUI() {
    return 0;
}

UPLAY_EXPORT int UPLAY_PRESENCE_SetPresence() {
    return 1;
}

UPLAY_EXPORT int UPLAY_PeekNextEvent() {
    return 1;
}

UPLAY_EXPORT int UPLAY_Quit() {
    return 1;
}

UPLAY_EXPORT int UPLAY_Release() {
    return 1;
}

const char* savefile = "";
long position = 0;
long firstpos = 0;

UPLAY_EXPORT int UPLAY_SAVE_Close(HANDLE hFiledata) {
    SetFilePointer(hFiledata, 0, 0, FILE_BEGIN);
    DWORD data = 0;
    DWORD datat = 0;
    DWORD tt = 0x224;  // Declarado aquí
    ReadFile(hFiledata, &data, sizeof(data), &datat, NULL);
    if (data == 0) {
        SetFilePointer(hFiledata, 0, 0, FILE_BEGIN);
        WriteFile(hFiledata, &tt, sizeof(tt), &data, NULL);  // Asegúrate de pasar NULL para OVERLAPPED
        SetFilePointer(hFiledata, 0x28, 0, FILE_BEGIN);
        for (int f = 0; f < FileVectorSize; f++) {  // Search through initialized files
            if (FileVector[f].hFile == hFiledata) {
                WriteFile(hFiledata, FileVector[f].nameoffile, sizeof(FileVector[f].nameoffile), &data, NULL);  // Asegúrate de pasar NULL para OVERLAPPED
                break;
            }
        }
        SetFilePointer(hFiledata, 0, 0, FILE_BEGIN);
    }
    CloseHandle(hFiledata);
    return 1;
}

const char* dir = NULL;

UPLAY_EXPORT int UPLAY_SAVE_GetSavegames(void* listpointer, void* mystruct) {
    void* FirstPointer = VirtualAlloc(NULL, 0x80000, MEM_COMMIT, PAGE_READWRITE);
    void* SecondPointer = VirtualAlloc(NULL, 0x80000, MEM_COMMIT, PAGE_READWRITE);
    ULONG_PTR SecondPointerAddr = (ULONG_PTR)SecondPointer;
    const char* myconstant = AttachDirFile(dir, "*.save");
    ULONG_PTR valueofiles = 0;
    WIN32_FIND_DATAA fd = { 0 };
    HANDLE firstFile = FindFirstFileA(myconstant, &fd);

    if (firstFile != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
                void* ThirdPointer = VirtualAlloc(NULL, 0x3000, MEM_COMMIT, PAGE_READWRITE);
                int filecounternm = 0;
                char bytesdat[270] = { 0 };
                strcpy(bytesdat, (char*)fd.cFileName);
                int size = strlen(bytesdat);
                for (int i = size; i > 0; i--) {
                    if (bytesdat[i] == '.') {
                        memset(&bytesdat[i], 0, size - i);
                        filecounternm = (int)strtoul(bytesdat, NULL, 16);
                        break;
                    }
                }
                HANDLE hFoundFile = CreateFileA(
                    AttachDirFile(dir, (char*)fd.cFileName),
                    GENERIC_READ,
                    0,
                    NULL,  // lpSecurityAttributes
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL   // hTemplateFile
                );

                if (hFoundFile != INVALID_HANDLE_VALUE) {
                    DWORD bytesreaded;
                    char* datar2 = (char*)VirtualAlloc(NULL, 0x200, MEM_COMMIT, PAGE_READWRITE);
                    ReadFile(hFoundFile, datar2, 0x200, &bytesreaded, NULL);  // Asegúrate de pasar NULL para OVERLAPPED
                    struct FileList* flst = (struct FileList*)ThirdPointer;
                    flst->bufferstring = datar2;
                    flst->num = (DWORD)filecounternm;
                    flst->pointer = (DWORD)(GetFileSize(hFoundFile, NULL) - 0x228);

#ifdef _WIN64
                    memcpy((void*)SecondPointerAddr, &ThirdPointer, sizeof(ThirdPointer));
                    SecondPointerAddr += sizeof(ThirdPointer);
#else
                    memcpy((void*)SecondPointerAddr, &ThirdPointer, sizeof(ThirdPointer));
                    SecondPointerAddr += sizeof(ThirdPointer);
#endif

                    CloseHandle(hFoundFile);
                    valueofiles++;
                }
            }
        } while (FindNextFileA(firstFile, &fd) == TRUE);
        FindClose(firstFile);

#ifdef _WIN64
        memcpy(FirstPointer, &valueofiles, sizeof(valueofiles));
        memcpy((void*)((ULONG_PTR)FirstPointer + 8), &SecondPointer, sizeof(SecondPointer));
#else
        memcpy(FirstPointer, &valueofiles, sizeof(valueofiles));
        memcpy((void*)((ULONG_PTR)FirstPointer + 4), &SecondPointer, sizeof(SecondPointer));
#endif
    }

#ifdef _WIN64
    memcpy(listpointer, &FirstPointer, sizeof(FirstPointer));
#else
    memcpy(listpointer, &FirstPointer, sizeof(FirstPointer));
#endif

    struct FileRead* flr = (struct FileRead*)mystruct;
    flr->addr1++;
    flr->addr2 = 1;
    flr->addr3 = 0;

    return 1;
}

UPLAY_EXPORT int UPLAY_SAVE_Open(int dp1, int dp2, void* buf1, void* buf2) {
    char file[1024] = { 0 };
    sprintf(file, "0x%x.save", dp1);
    savefile = AttachDirFile(dir, file);
    tempbuffer = VirtualAlloc(0, 0x228, MEM_COMMIT, PAGE_READWRITE);
    struct MyFileRef ref = { 0 };
    if (dp2 == 0) {
        struct FileOpen* fp = (struct FileOpen*)buf1;
        struct FileOpenTwo* fp2 = (struct FileOpenTwo*)buf2;
        HANDLE fileuplayt = CreateFileA(savefile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileuplayt == INVALID_HANDLE_VALUE) {
            fp2->addr1++;
            fp2->addr2 = 1;
            fp2->addr3 = 0;
            return 0;
        }
        fp->d1 = (DWORD)fileuplayt;
        fp2->addr1++;
        fp2->addr2 = 1;
        fp2->addr3 = 0;
        ref.hFile = fileuplayt;
        ref.numfile = dp1;
        FileVector[FileVectorSize++] = ref;
        return 1;
    } else {
        struct FileOpen* fp = (struct FileOpen*)buf1;
        struct FileOpenTwo* fp2 = (struct FileOpenTwo*)buf2;
        HANDLE fileuplayt = CreateFileA(savefile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileuplayt == INVALID_HANDLE_VALUE) {
            fp2->addr1++;
            fp2->addr2 = 1;
            fp2->addr3 = 0;
            return 0;
        }
        DWORD ld;
        WriteFile(fileuplayt, tempbuffer, 0x228, &ld, NULL);  // Asegúrate de pasar NULL para OVERLAPPED
        fp->d1 = (DWORD)fileuplayt;
        fp2->addr1++;
        fp2->addr2 = 1;
        fp2->addr3 = 0;
        ref.hFile = fileuplayt;
        ref.numfile = dp1;
        FileVector[FileVectorSize++] = ref;
        return 1;
    }
    return 0;
}

UPLAY_EXPORT int UPLAY_SAVE_Read(HANDLE hFile, SIZE_T datatoread, long offset, void* dt1, DWORD* bytesreaded, void* mystruct) {
    void* ReadedData = 0;
    if (hFile != INVALID_HANDLE_VALUE) {
#ifdef _WIN64
        memcpy(&ReadedData, dt1, sizeof(ReadedData));
#else
        memcpy(&ReadedData, dt1, sizeof(ReadedData));
#endif
        SetFilePointer(hFile, 0x228 + offset, NULL, FILE_BEGIN);
        DWORD wrote;
        if (!ReadFile(hFile, ReadedData, datatoread, bytesreaded, NULL)) {
            struct FileRead* flr = (struct FileRead*)mystruct;
            flr->addr1++;
            flr->addr2 = 1;
            flr->addr3 = 0;
            return 0;
        }
        struct FileRead* flr = (struct FileRead*)mystruct;
        flr->addr1++;
        flr->addr2 = 1;
        flr->addr3 = 0;
        return 1;
    }
    struct FileRead* flr = (struct FileRead*)mystruct;
    flr->addr1++;
    flr->addr2 = 1;
    flr->addr3 = 0;
    return 0;
}

UPLAY_EXPORT int UPLAY_SAVE_ReleaseGameList(void* listpointer) {
    VirtualFree(listpointer, 0, MEM_DECOMMIT);
    return 1;
}

UPLAY_EXPORT int UPLAY_SAVE_Remove(int fileindex, void* filestruct) {
    char file[1024] = { 0 };
    sprintf(file, "%d.save", fileindex);
    savefile = AttachDirFile(dir, file);

    if (!DeleteFileA(savefile)) {
        struct FileRead* flr = (struct FileRead*)filestruct;
        flr->addr1++;
        flr->addr2 = 1;
        flr->addr3 = 0;
        return 0;
    }
    struct FileRead* flr = (struct FileRead*)filestruct;
    flr->addr1++;
    flr->addr2 = 1;
    flr->addr3 = 0;
    return 1;
}

UPLAY_EXPORT int UPLAY_SAVE_SetName(HANDLE hFileData, const char* namefile) {
    for (int f = 0; f < FileVectorSize; f++) { // Search through initialized files
        if (FileVector[f].hFile == hFileData) {
            strcpy(FileVector[f].nameoffile, namefile);
            break;
        }
    }
    return 1;
}

UPLAY_EXPORT int UPLAY_SAVE_Write(HANDLE hFile, SIZE_T datatowrite, void* buf1, void* buf2) {
    if (hFile != INVALID_HANDLE_VALUE) {
        void* WriteData = 0;
#ifdef _WIN64
        memcpy(&WriteData, buf1, sizeof(WriteData));
#else
        memcpy(&WriteData, buf1, sizeof(WriteData));
#endif
        DWORD dat = 0;
        struct FileRead* flr = (struct FileRead*)buf2;
        if (!WriteFile(hFile, WriteData, datatowrite, &dat, NULL)) {
            flr->addr1++;
            flr->addr2 = 1;
            flr->addr3 = 0;
            return 0;
        }

        flr->addr1++;
        flr->addr2 = 1;
        flr->addr3 = 0;
        return 1;
    }
    struct FileRead* flr = (struct FileRead*)buf2;
    flr->addr1++;
    flr->addr2 = 1;
    flr->addr3 = 0;
    return 0;
}

// Implementaciones de la tienda
UPLAY_EXPORT int UPLAY_STORE_Checkout() {
    return 0;
}

UPLAY_EXPORT int UPLAY_STORE_GetPartner() {
    return 0;
}

UPLAY_EXPORT int UPLAY_STORE_GetProducts() {
    return 0;
}

UPLAY_EXPORT int UPLAY_STORE_ReleaseProductsList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_STORE_ShowProductDetails() {
    return 0;
}

UPLAY_EXPORT int UPLAY_STORE_ShowProducts() {
    return 0;
}

UPLAY_EXPORT int UPLAY_SetGameSession() {
    return 1;
}

UPLAY_EXPORT int UPLAY_SetLanguage(const char* language) {
    strcpy(Uplay_Configuration.GameLanguage, language);
    return 1;
}

UPLAY_EXPORT int UPLAY_Start() {
    CHAR INI[MAX_PATH] = { 0 }; // Get ini directory
    GetModuleFileNameA(NULL, INI, MAX_PATH);
    int Size = lstrlenA(INI);
    for (int i = Size; i > 0; i--) {
        if (INI[i] == '\\') {
            lstrcpyA(&INI[i], "\\Uplay.ini");
            break;
        }
    }
    if (!IsTargetExist(INI)) {
        MessageBoxA(0, "Couldn't find Uplay.ini.", "Uplay", MB_ICONERROR);
        ExitProcess(0);
    }

    Uplay_Configuration.appowned = GetPrivateProfileIntA("Uplay", "IsAppOwned", 0, INI) == TRUE; // Read ini informations
    Uplay_Configuration.Offline = GetPrivateProfileIntA("Uplay", "UplayConnection", 0, INI) == TRUE;
    Uplay_Configuration.gameAppId = GetPrivateProfileIntA("Uplay", "AppId", 0, INI);
    
    GetPrivateProfileStringA("Uplay", "Username", 0, Uplay_Configuration.UserName, sizeof(Uplay_Configuration.UserName), INI);
    GetPrivateProfileStringA("Uplay", "Email", 0, Uplay_Configuration.UserEmail, sizeof(Uplay_Configuration.UserEmail), INI);
    GetPrivateProfileStringA("Uplay", "Password", 0, Uplay_Configuration.password, sizeof(Uplay_Configuration.password), INI);
    GetPrivateProfileStringA("Uplay", "SavePath", 0, Uplay_Configuration.SaveDir, sizeof(Uplay_Configuration.SaveDir), INI);
    GetPrivateProfileStringA("Uplay", "Language", 0, Uplay_Configuration.GameLanguage, sizeof(Uplay_Configuration.GameLanguage), INI);
    GetPrivateProfileStringA("Uplay", "CdKey", 0, Uplay_Configuration.CdKey, sizeof(Uplay_Configuration.CdKey), INI);
    GetPrivateProfileStringA("Uplay", "UserId", 0, Uplay_Configuration.UserId, sizeof(Uplay_Configuration.UserId), INI);
    GetPrivateProfileStringA("Uplay", "TickedId", 0, Uplay_Configuration.TickedId, sizeof(Uplay_Configuration.TickedId), INI);

    dir = FormatDir("ApiLocalStorage", 0); // Storage init
    CreatePath(dir);
    return 0;
}

UPLAY_EXPORT int UPLAY_StartCN() {
    return UPLAY_Start();
}

UPLAY_EXPORT int UPLAY_Startup() {
    return UPLAY_Start();
}

UPLAY_EXPORT int UPLAY_USER_ClearGameSession() {
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_ConsumeItem() {
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_GetAccountId() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetAccountIdUtf8() {
    return Uplay_Configuration.UserId;
}

UPLAY_EXPORT int UPLAY_USER_GetCPUScore() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetCdKeyUtf8() {
    return Uplay_Configuration.CdKey;
}

UPLAY_EXPORT int UPLAY_USER_GetCdKeys() {
    return 0;
}

UPLAY_EXPORT int UPLAY_USER_GetConsumableItems(void* buf1) {
#ifdef _WIN64
    memset(buf1, 0, 8);
#else
    memset(buf1, 0, 4);
#endif
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_GetCredentials() {
    return 0;
}

UPLAY_EXPORT int UPLAY_USER_GetEmail() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetEmailUtf8() {
    return Uplay_Configuration.UserEmail;
}

UPLAY_EXPORT int UPLAY_USER_GetGPUScore() {
    return 0;
}

UPLAY_EXPORT int UPLAY_USER_GetGPUScoreConfidenceLevel() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetNameUtf8() {
    return Uplay_Configuration.UserName;
}

UPLAY_EXPORT int UPLAY_USER_GetPassword() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetPasswordUtf8() {
    return Uplay_Configuration.password;
}

UPLAY_EXPORT int UPLAY_USER_GetProfile() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetTicketUtf8() {
    return Uplay_Configuration.TickedId;
}

UPLAY_EXPORT int UPLAY_USER_GetUsername() {
    return 0;
}

UPLAY_EXPORT const char* UPLAY_USER_GetUsernameUtf8() {
    return Uplay_Configuration.UserName;
}

UPLAY_EXPORT int UPLAY_USER_IsConnected() {
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_IsInOfflineMode() {
    return Uplay_Configuration.Offline;
}

UPLAY_EXPORT int UPLAY_USER_IsOwned(int data) {
    return Uplay_Configuration.appowned;
}

UPLAY_EXPORT int UPLAY_USER_ReleaseCdKeyList() {
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_ReleaseConsumeItemResult() {
    return 1;
}

UPLAY_EXPORT int UPLAY_USER_ReleaseProfile() {
    return 0;
}

UPLAY_EXPORT int UPLAY_USER_SetGameSession() {
    return 0;
}

UPLAY_EXPORT int UPLAY_Update() {
    return 1;
}

UPLAY_EXPORT int UPLAY_WIN_GetActions() {
    return 0;
}

UPLAY_EXPORT int UPLAY_WIN_GetRewards() {
    return 0;
}

UPLAY_EXPORT int UPLAY_WIN_GetUnitBalance() {
    return 0;
}

UPLAY_EXPORT int UPLAY_WIN_RefreshActions() {
    return 1;
}

UPLAY_EXPORT int UPLAY_WIN_ReleaseActionList() {
    return 1;
}

UPLAY_EXPORT int UPLAY_WIN_ReleaseRewardList() {
    return 1;
}

UPLAY_EXPORT int UPLAY_WIN_SetActionsCompleted() {
    return 1;
}

UPLAY_EXPORT int UPLAY_CHAT_GetHistory() {
    return 0;
}

UPLAY_EXPORT int UPLAY_CHAT_Init() {
    return 0;
}

UPLAY_EXPORT int UPLAY_CHAT_ReleaseHistoryList() {
    return 0;
}

UPLAY_EXPORT int UPLAY_CHAT_SendMessage() {
    return 0;
}

UPLAY_EXPORT int UPLAY_CHAT_SentMessagesRead() {
    return 0;
}