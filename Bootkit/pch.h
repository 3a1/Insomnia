#pragma once

#define EXTERN_C extern "C"
#include "struct.h"

EXTERN_C
{
	#include <Uefi.h>
	#include <Library/UefiLib.h>
	#include <Library/DebugLib.h>
	#include <Library/MemoryAllocationLib.h>

	#include <Library/UefiBootServicesTableLib.h>
	#include <Library/UefiRuntimeServicesTableLib.h>

	#include <Uefi/UefiBaseType.h>
	#include <Protocol/LoadedImage.h>
	#include <Library/PrintLib.h>

	uint64_t __readgsqword(uint64_t);
	#pragma intrinsic(__readgsqword)
	#define __read_gs(offset) __readgsqword(offset)

	uint64_t _ReturnAddress();
	#pragma intrinsic(_ReturnAddress)
	#define __get_rax() _ReturnAddress()

	uint64_t _AddressOfReturnAddress();
	#pragma intrinsic(_AddressOfReturnAddress)
	#define __get_rsp() _AddressOfReturnAddress()
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
