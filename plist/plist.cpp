#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

void listProcesses() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        return;
    }

    printf("PID      Process Name                    Threads\n");
    printf("------------------------------------------------\n");

    do {
        printf("%-8lu %-30s %lu\n",
            pe32.th32ProcessID,
            pe32.szExeFile,
            pe32.cntThreads);
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
}

int main() {
    listProcesses();
    return 0;
}