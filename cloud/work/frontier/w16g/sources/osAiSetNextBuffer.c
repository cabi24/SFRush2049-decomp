/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* osAiSetNextBuffer (static, boot 0x8000BE70, 144 B, segment 0xca70 -> src/rom/lib_ca70.c, converted)
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/aisetnextbuf.c, BUILD_VERSION >= VERSION_J
 *         (__osAiDeviceBusy check first).
 * DATA: the body only matches with ultralib's function-local "static u8 hdwrBugFlag = FALSE;"
 *   (the project's data symbol __osAiNeedsAlign, 0x8002C3C0). An extern u8 / extern volatile u8 /
 *   tentative or initialised file-scope definition all CSE the address (26 objdump lines differ).
 *   The static puts a 0x10-byte .data section in lib_ca70.o, so promotion needs an owned .data
 *   slot (rom_owned_data.json, data.bin offset 0x4BF0, 16 zero bytes) - see RESULTS.md. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define TRUE 1
#define FALSE 0
#define IO_WRITE(addr,value) (*(volatile u32 *)(addr)=(value))
#define AI_DRAM_ADDR_REG 0xA4500000
#define AI_LEN_REG 0xA4500004

extern s32 __osAiDeviceBusy(void);
extern u32 osVirtualToPhysical(void *vaddr);

s32 osAiSetNextBuffer(void* bufPtr, u32 size) {
    static u8 hdwrBugFlag = FALSE;
    char* bptr;

    if (__osAiDeviceBusy()) {
        return -1;
    }

    bptr = bufPtr;

    if (hdwrBugFlag) {
        bptr = (u8*)bufPtr - 0x2000;
    }

    if ((((u32)bufPtr + size) & 0x1fff) == 0) {
        hdwrBugFlag = TRUE;
    } else {
        hdwrBugFlag = FALSE;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
