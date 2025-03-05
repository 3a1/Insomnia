#include "pch.h"

/*
    U can use the overload call function in two ways

    current_process = bootkit.call("PsGetCurrentProcess");

    or

    uint64_t PsGetCurrentProcess = bootkit.get_kernel_export("PsGetCurrentProcess");
    current_process = bootkit.call(PsGetCurrentProcess);
*/

int main() 
{
    Bootkit bootkit;
    
    uint64_t current_process = bootkit.call("PsGetCurrentProcess");
    std::cout << "PsGetCurrentProcess -> 0x" << std::hex << current_process << std::endl;

    uint64_t current_pid = bootkit.call("PsGetProcessId", current_process);
    std::cout << "PsGetProcessId -> " << std::dec << current_pid << std::endl;

    system("pause");
}