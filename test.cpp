#include<windows.h>
#include<stdio.h>
#include <TlHelp32.h>

// 简化版：获取目标进程中模块的基地址（仅保留核心逻辑）
DWORD64 GetModuleBaseAddress(DWORD dwProcessId, LPCWSTR szModuleName) {
    DWORD64 dwModuleBaseAddr = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
    MODULEENTRY32W me32 = { sizeof(MODULEENTRY32W) }; // 直接初始化dwSize

    if (Module32FirstW(hSnapshot, &me32)) {
        do {
            if (_wcsicmp(me32.szModule, szModuleName) == 0) {
                dwModuleBaseAddr = (DWORD64)me32.modBaseAddr;
                break;
            }
        } while (Module32NextW(hSnapshot, &me32));
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddr;
}

DWORD64 RET_BASE_ADDRESS()
{
    HWND hWnd = FindWindowA(NULL, "AssaultCube");//获取目标窗口句柄
    DWORD dwPID = 0;
    GetWindowThreadProcessId(hWnd, &dwPID);//获取进程pid
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);//获取目标进程句柄

    // 2. 获取模块基地址
    DWORD64 dwBaseAddr = GetModuleBaseAddress(dwPID, L"ac_client.exe");
    printf("AssaultCube基地址：0x%llX\n", dwBaseAddr);
    return dwBaseAddr;
}
HANDLE RET_hprocess()
{
    HWND hWnd = FindWindowA(NULL, "AssaultCube");//获取目标窗口句柄
    DWORD dwPID = 0;
    GetWindowThreadProcessId(hWnd, &dwPID);//获取进程pid
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);//获取目标进程句柄
    return hProcess;
}

DWORD R(DWORD address)
{
    DWORD addr = address;
    DWORD temp = 0;

    ReadProcessMemory(RET_hprocess(), (LPVOID)addr, &temp, 4, 0);
    printf("%08x\n", temp);

    return temp;
}

int  main()
{
    HANDLE hProcess = RET_hprocess();
    int value = 500;
    WriteProcessMemory(RET_hprocess(), (LPVOID)R(R(R((RET_BASE_ADDRESS() + 0x195404)) + 0x364) + 0x14), &value, 4, 0);//0x364,0x14,0
    printf("修改后的值为 ：%d\n", value);

    CloseHandle(hProcess);
    return 0;
}