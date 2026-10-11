/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
/* __osSetGlobalIntMask (static, boot 0x800100C0, 80 B, segment 0x10cc0 -> src/rom/lib_10cc0.c, unconverted)
 * Conveyor inventory target_id: __osPiGetCmdQueue (stale pre-S12 label; same address and object).
 * Origin: reference/repos/ultralib (decompals e24c836) src/os/setglobalintmask.c (all versions), -O1 like libultra os/.
 * Project names: ultralib __OSGlobalIntMask is this project's __osGlobalIntMask (0x8002C370). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef u32 OSHWIntr;

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern u32 __osGlobalIntMask;

void __osSetGlobalIntMask(OSHWIntr mask) {
    register u32 saveMask = __osDisableInt();

    __osGlobalIntMask |= mask;
    __osRestoreInt(saveMask);
}
