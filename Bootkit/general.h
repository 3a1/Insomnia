#pragma once
#include "pch.h"

namespace global
{
    EXTERN_C
    {
        inline EFI_SYSTEM_TABLE*       SystemTable;
        inline EFI_RUNTIME_SERVICES*   RuntimeServices;
        inline EFI_BOOT_SERVICES*      BootServices;

        inline EFI_EXIT_BOOT_SERVICES  ExitBootServices;

        inline uint64_t                winload;
        inline uint64_t                ntoskrnl;

        inline uint64_t                BlpArchSwitchContext;
        inline uint64_t                OslLoaderBlock;

        inline uint64_t                KiSystemStartup;
        inline uint64_t                KiInitializeKernel;
        inline uint64_t                KiServiceTable;
    }
}
