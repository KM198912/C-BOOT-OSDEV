/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022-2025, All rights reserved.
*/
#include "boot.h"

struct multiboot_header_loader multiboot_header __attribute__((section(".multiboot"))) = {
    .magic = LOADER_MAGIC,
    .flags = {
        // align=1, mmap=1, video=1 if VBE_MODE else 0, rest 0
        #ifdef VBE_MODE
        .align = 1,
        .mmap = 1,
        .video = 1,
        #else
        .align = 1,
        .mmap = 1,
        .video = 0,
        #endif
    },
    #ifdef VBE_MODE
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))),
    #else
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1))),
    #endif
    .reserved = {0},
    .vbe_info = {
        .video_mode = 0,
        .width = 800,
        .height = 600,
        .depth = 32
    }
};

void *mboot;
void __init()
{
    /**
     * Welcome to the Kernel, your journey begins here.
     * This is the entry point of the kernel, where you can initialize your kernel.
     * a good first step now would be to call kmain() or similar,
     * but if i where in your shoes, i would not do that yet.
     * Instead, i would now setup the GDT and IDT, some boot.asm do that for you,
     * but the boot.asm would become long and messy, you have the magic of C, setup the GDT and IDT in Seperate Filesm,
     * include the headers here, and call the init functions, like gdt_init() and idt_init(), enable interrupts, etc.
     * Then, you can call kmain() or similar, and start your kernel.
     */
    for(;;);
}

void _start()
{
    // Disable interrupts so our kernel later can set them up properly
    asm volatile("cli");

    // Set the multiboot pointer to the multiboot header, we use ebx because thats where per multiboot spec
    // the bootloader should pass the multiboot header pointer. using a void pointer here prevents the use of 
    // of the multiboot header in the kernel, and makes the bootloader standalone to be easily integrated into other projects.
    asm("mov %%ebx, %0" : "=m"(mboot));
    /**
     * Here you would now setup the stack top and bottom, and the heap.
     * we have struct members for this, BUT, you would cast this to results from your heap manager, etc.
     * This is just a simple example, so we will not do that here.
     * Also, in theory you are now still in the preboot stage, on a typical x86, 
     * you would now be at the point where you define "extern kmain" and call it.
     **/

     //example stack setup
    memory_regions_t memory_regions;
    memory_regions.kernel_heap = (uint8_t *)0x100000; // 1MB
    memory_regions.kernel_stack = (uint8_t *)0x200000; // 2MB
    memory_regions.common_heap = (uint8_t *)0x300000; // 3MB
    memory_regions.lf_heap = (uint8_t *)0x400000; // 4MB lf_heap is actually a local function heap, used for temporary allocations.
    memory_regions.end = 0x500000; // 5MB
    // Actually set the stack pointer (example - validate your addresses first!)
    asm volatile("mov %0, %%esp" : : "r"(memory_regions.kernel_stack));
    // The above is just an example, you would typically use a heap manager to allocate these regions.
    // If you use it AS IS, you will have a stack at 2MB, a kernel heap at 1MB, a common heap at 3MB, and a lf_heap at 4MB.
    //But, this can and WILL break like this, so you should use a heap manager to allocate these regions properly.

    __init();
}
