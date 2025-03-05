#include "NtShutdownSystem.h"

uint64_t NtShutdownSystemHook(uint64_t a1)
{
    uint64_t     rsp;
    uint16_t     magic;
    args_t       args;

    rsp = __read_gs(0x10 /* ->UserRsp */);
    if (!rsp)
    {
        goto skip;
    }

    magic = *(uint16_t*)(uint8_t*)(rsp + 30 /* ->magic */ + 8 /* call (push rax) */);
    if (magic != 0xDEAD) 
    {
        goto skip;
    }

    args = **(args_t**)(uint8_t*)(rsp + 22 /* ->args */ + 8 /* call (push rax) */);
    if (!args.address) 
    {
        goto skip;
    }

    return ((uint64_t(*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))args.address)(args.a1, args.a2, args.a3, args.a4, args.a5, args.a6, args.a7);

skip:
    uint64_t NtShutdownSystem = 0;
    for (int32_t i = 0; ; ++i)
    {
        if (*(uint16_t*)((uint64_t)NtShutdownSystemHook - i) == 0xDEAD)
        {
            NtShutdownSystem = *(uint64_t*)(((uint64_t)NtShutdownSystemHook - i) + sizeof(uint16_t));
            break;
        }
    }

    return ((uint64_t(*)(uint32_t))NtShutdownSystem)(a1);
}
