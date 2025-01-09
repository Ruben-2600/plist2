#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

void printHelp() {
    printf("[Passe en revue les diverses commandes prises en compte]\n");
    printf("  -h                : Afficher l'aide\n");
    printf("  [nom processus]   : Filtrer par nom de processus\n");
    printf("  -d [PID]          : Afficher les threads d'un processus donné\n");
}

void listProcesses(const wchar_t* filterName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Impossible de créer un instantané des processus.\n");
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        printf("Erreur lors de la récupération des processus.\n");
        CloseHandle(hSnapshot);
        return;
    }

    printf("Name                Pid  Pri  Thd  Hnd  Priv   CPU Time       Elapsed Time\n");
    printf("------------------------------------------------------------------------\n");

    do {
        if (filterName == NULL || _wcsicmp(pe32.szExeFile, filterName) == 0) {
            // Simulez les informations de temps CPU et temps écoulé
            SYSTEMTIME cpuTime = { 0 };
            SYSTEMTIME elapsedTime = { 0 };
            cpuTime.wMinute = rand() % 60; // Génère des valeurs aléatoires pour l'exemple
            elapsedTime.wMinute = rand() % 60;

            wprintf(L"%-20s %-4lu %-4d %-4lu %-4lu %-6lu %02d:%02d:%02d       %02d:%02d:%02d\n",
                pe32.szExeFile,
                pe32.th32ProcessID,
                8, // Priorité fictive
                pe32.cntThreads,
                500, // Nombre de handles fictif
                2048, // Mémoire privée fictive (en Ko)
                cpuTime.wHour, cpuTime.wMinute, cpuTime.wSecond,
                elapsedTime.wHour, elapsedTime.wMinute, elapsedTime.wSecond
            );
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
}

void listThreads(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Impossible de créer un instantané des threads.\n");
        return;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hSnapshot, &te32)) {
        printf("Erreur lors de la récupération des threads.\n");
        CloseHandle(hSnapshot);
        return;
    }

    printf("Tid     Pri  Cswtch  State        User Time       Kernel Time    Elapsed Time\n");
    printf("----------------------------------------------------------------------------\n");

    do {
        if (te32.th32OwnerProcessID == pid) {
            // Simulez les informations de temps CPU et état
            SYSTEMTIME userTime = { 0 };
            SYSTEMTIME kernelTime = { 0 };
            SYSTEMTIME elapsedTime = { 0 };
            userTime.wMinute = rand() % 60; // Génère des valeurs aléatoires pour l'exemple
            kernelTime.wMinute = rand() % 60;
            elapsedTime.wMinute = rand() % 60;

            printf("%-7lu %-4d %-7d %-12s %02d:%02d:%02d   %02d:%02d:%02d   %02d:%02d:%02d\n",
                te32.th32ThreadID,
                8, // Priorité fictive
                rand() % 10000, // Context switches fictifs
                "Wait:UserReq", // État fictif
                userTime.wHour, userTime.wMinute, userTime.wSecond,
                kernelTime.wHour, kernelTime.wMinute, kernelTime.wSecond,
                elapsedTime.wHour, elapsedTime.wMinute, elapsedTime.wSecond
            );
        }
    } while (Thread32Next(hSnapshot, &te32));

    CloseHandle(hSnapshot);
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printHelp();
    }
    else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
        DWORD pid = atoi(argv[2]);
        listThreads(pid);
    }
    else if (argc == 2) {
        wchar_t filterName[MAX_PATH];
        size_t convertedChars = 0;
        mbstowcs_s(&convertedChars, filterName, MAX_PATH, argv[1], _TRUNCATE);
        listProcesses(filterName);
    }
    else {
        listProcesses(NULL);
    }

    return 0;
}
