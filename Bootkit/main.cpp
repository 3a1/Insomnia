#include "pch.h"

__declspec(noinline) void swap_execution(uint64_t new_base)
{
    int32_t current_location = (int32_t)((uint64_t)__get_rax() - global::ImageBase);
    *(uint64_t*)(__get_rax_ptr()) = new_base + current_location;
}

EXTERN_C EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    global::RuntimeServices = SystemTable->RuntimeServices;
    global::BootServices    = SystemTable->BootServices;
    global::SystemTable     = SystemTable;

    /* get image base and size */
    global::ImageBase = memory::get_image_base((uint64_t)UefiMain);
    global::ImageSize = memory::get_image_size(global::ImageBase);

    uint32_t new_location = 0;
    uint32_t pages_count = EFI_SIZE_TO_PAGES(global::ImageSize);

    /* allocate new pages */
    if (global::BootServices->AllocatePages(AllocateAnyPages, EfiLoaderCode, pages_count, (EFI_PHYSICAL_ADDRESS*)&new_location) == EFI_SUCCESS)
    {
        /* copy image to new location */
        memory::copy((uint64_t*)global::ImageBase, (uint64_t*)new_location, global::ImageSize);

        /* swap execution to new image */
        swap_execution((uint32_t)new_location);

        /* remove the old image */
        memory::zero((uint8_t*)global::ImageBase, global::ImageSize);

        /* update new image base */
        global::ImageBase = new_location;

        /* place ExitBootServices hook */
        global::ExitBootServices = global::BootServices->ExitBootServices;
        global::BootServices->ExitBootServices = ExitBootServicesHook;

        Log(L"Insomnia has been successfully loaded");
        SLEEP(5000);
        return EFI_SUCCESS;
    }

    Log(L"Insomnia loading failed!");
    SLEEP(5000);
    return EFI_LOAD_ERROR;
}
