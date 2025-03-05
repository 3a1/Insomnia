#pragma once
#include "pch.h"

class Bootkit 
{
public:
	Bootkit();

	uint64_t get_kernel_base();
	uint64_t get_kernel_export(LPCSTR name);

	uint64_t __fastcall call(uint64_t address, 
							 uint64_t a1 = 0, uint64_t a2 = 0,
							 uint64_t a3 = 0, uint64_t a4 = 0,
						         uint64_t a5 = 0, uint64_t a6 = 0,
						 	 uint64_t a7 = 0);

	uint64_t __fastcall call(LPCSTR name,
							 uint64_t a1 = 0, uint64_t a2 = 0,
							 uint64_t a3 = 0, uint64_t a4 = 0,
							 uint64_t a5 = 0, uint64_t a6 = 0,
							 uint64_t a7 = 0);
};
