/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022-2025, All rights reserved.
*/
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#define VBE_MODE // Define this to enable VBE mode, otherwise it will be disabled, in theory, you can ommit this, and pass -DVBE_MODE during compile time if you want it

#define LOADER_MAGIC 0x1BADB002 // "BADBOOT", the Grub 0.6.96 Magic Number per Spec

/*
* Below is what the boot.asm would regulary do, but we do it here in C and for simplicity use a struct
* ASM in AS Systax would look like this:
* .section .multiboot, "a"
* .align 4
* .global multiboot_header
* multiboot_header:
* .long LOADER_MAGIC
* .long (1 << 0) | (1 << 1) | (1 << 2) // align=1, mmap=1, video=1 if VBE_MODE else 0
* .long -(LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))) // checksum
* etc.
*/

struct multiboot_header_loader
{
    uint32_t magic;
    struct
    {
        bool align: 1;
        bool mmap: 1;
        bool video: 1;
        uint32_t : 29;
    } flags;
    uint32_t checksum;
    uint8_t  reserved[20];
    struct
    {
        uint32_t video_mode;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    } vbe_info;
};
struct memory_regions
{
    uint8_t *kernel_stack;
    uint8_t *kernel_heap ;
    uint8_t *common_heap ;
    uint8_t *lf_heap     ;
    uintptr_t end;
};

typedef struct multiboot_header_loader multiboot_header_t;
typedef struct memory_regions memory_regions_t;
