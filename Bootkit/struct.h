#pragma once
#include "pch.h"

typedef unsigned long long uint64_t;
typedef unsigned int       uint32_t;
typedef unsigned short     uint16_t;
typedef unsigned char      uint8_t;

typedef signed long long int64_t;
typedef signed int       int32_t;
typedef signed short     int16_t;
typedef signed char      int8_t;

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

constexpr uint8_t BlpArchSwitchContextPattern[] =
{
    0x40, 0x53, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x8B, 0x15
};

constexpr uint8_t OslLoaderBlockPattern[] =
{
    0x48, 0x8B, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x8F, 0x00, 0x00, 0x00, 0x00
};

constexpr uint8_t OslLoaderBlockMask[] =
{
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00
};