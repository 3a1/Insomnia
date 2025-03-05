#pragma once
#include "pch.h"

struct __declspec(align(1)) args_t
{
    uint64_t address;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
};