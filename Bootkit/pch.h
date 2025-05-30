#pragma once

#define EXTERN_C extern "C"
#include "struct.h"

EXTERN_C
{
	#include <Uefi.h>

	uint64_t __readgsqword(uint64_t);
	#pragma intrinsic(__readgsqword)
	#define __read_gs(offset) __readgsqword(offset)

	uint64_t _ReturnAddress();
	#pragma intrinsic(_ReturnAddress)
	#define __get_rax() _ReturnAddress()

	uint64_t _AddressOfReturnAddress();
	#pragma intrinsic(_AddressOfReturnAddress)
	#define __get_rax_ptr() _AddressOfReturnAddress()
}

#include "general.h"
#include "utils.h"
#include "memory.h"
#include "debug.h"

#include "NtShutdownSystem.h"
#include "ExitBootServices.h"

//       |\      _,,,---,,_
// Zzz   /,`.-'`'    -.  ;-;;,_
//      |,4-  ) )-,_. ,\ (  `'-'
//     '---''(_/--'  `-\_)
