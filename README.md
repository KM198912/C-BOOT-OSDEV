# C Multiboot Loader

A simple, beginner-friendly C-based multiboot loader that replaces traditional assembly boot code for 32-bit kernel development.

## Why This Exists

Most OS development tutorials throw beginners into complex x86 assembly code right from the start. The traditional `boot.asm` file is often the first major hurdle that stops aspiring kernel developers before they even reach the interesting parts of OS development.

This library provides a clean, readable C alternative to assembly bootloaders while maintaining full multiboot compliance.

## What This Does

- **Replaces boot.asm**: No more cryptic assembly magic numbers and manual checksum calculations
- **Multiboot compliant**: Works with GRUB and other standard bootloaders
- **VBE graphics support**: Optional video mode setup for graphical kernels
- **Educational**: Clear documentation showing what traditional assembly would look like
- **Minimal**: Focused purely on the boot transition problem

## What This Doesn't Do

This is intentionally **not** a kernel framework. It doesn't include:
- Memory management
- Heap allocators
- Device drivers
- File systems

It's a focused library that solves one problem well: getting from bootloader to kernel cleanly.

## Usage

### Basic Setup

1. Include the header in your kernel:
```c
#include "boot.h"
```

2. Implement your kernel initialization:
```c
void __init()
{
    // Setup GDT, IDT, etc.
    gdt_init();
    idt_init();
    
    // Enable interrupts
    asm volatile("sti");
    
    // Call your kernel main
    kmain();
}
```

3. Compile with the loader source file and link with appropriate flags.

### Graphics Mode

To enable VBE graphics mode, define `VBE_MODE` before including the header:
```c
#define VBE_MODE
#include "boot.h"
```

Or pass `-DVBE_MODE` during compilation.

### Memory Layout Example

The loader provides a structure for organizing memory regions:
```c
memory_regions_t memory_regions;
memory_regions.kernel_heap = (uint8_t *)0x100000; // 1MB
memory_regions.kernel_stack = (uint8_t *)0x200000; // 2MB
memory_regions.common_heap = (uint8_t *)0x300000; // 3MB
memory_regions.lf_heap = (uint8_t *)0x400000; // 4MB - Local function heap
memory_regions.end = 0x500000; // 5MB
```

**Warning**: The above is just an example. In practice, you should use proper memory management and validate these addresses against the multiboot memory map.

## Building

### Requirements
- GCC or compatible C compiler with multiboot support
- GNU Make (optional)
- GRUB or compatible bootloader for testing

### Example Compilation
```bash
gcc -m32 -ffreestanding -nostdlib -lgcc -T linker.ld -o kernel.bin loader.c your_kernel.c
```

### Linker Script
Make sure your linker script places the `.multiboot` section at the beginning:
```ld
SECTIONS
{
    . = 1M;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    /* ... rest of your sections */
}
```

## Learning Path

If you're new to OS development, here's the recommended progression after using this loader:

1. **Set up basic infrastructure**: GDT, IDT, interrupt handling
2. **Implement basic I/O**: VGA text mode, keyboard input
3. **Memory management**: Paging, heap allocation
4. **Process management**: Task switching, scheduling
5. **File systems and drivers**: Storage, networking, etc.

## Comparison with Traditional Assembly

Instead of writing this in assembly:
```asm
.section .multiboot, "a"
.align 4
multiboot_header:
    .long 0x1BADB002
    .long (1 << 0) | (1 << 1) | (1 << 2)
    .long -(0x1BADB002 + ((1 << 0) | (1 << 1) | (1 << 2)))
```

You get this readable C structure:
```c
struct multiboot_header_loader multiboot_header = {
    .magic = LOADER_MAGIC,
    .flags = { .align = 1, .mmap = 1, .video = 1 },
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))),
    // ...
};
```

## Contributing

This library is designed to stay simple and focused. If you have suggestions for:
- Better documentation
- Bug fixes
- Improved compatibility
- Cleaner API design

Please open an issue or submit a pull request.

## License

See License file for License.

## Authors

- Nicola Leone Ciardi
- Kevin Meerts

## Resources

- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [OSDev Wiki](https://wiki.osdev.org/)
- [Little OS Book](https://littleosbook.github.io/)

---

**Happy kernel hacking!** 🚀
