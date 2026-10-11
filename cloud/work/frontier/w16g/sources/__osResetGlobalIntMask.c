/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
/* __osResetGlobalIntMask (static, boot 0x8000FDA0, 96 B, segment 0x109a0 -> src/rom/lib_109a0.c, unconverted)
 * Conveyor inventory target_id: osEPiRawWriteIo (stale pre-S12 label; same address and object).
 * Origin: reference/repos/ultralib (decompals e24c836) src/os/resetglobalintmask.c (all versions), -O1 like libultra os/.
 * Project names: ultralib __OSGlobalIntMask is this project's __osGlobalIntMask (0x8002C370). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef u32 OSHWIntr;
#define OS_IM_RCP 0x00000401

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern u32 __osGlobalIntMask;

void __osResetGlobalIntMask(OSHWIntr mask) {
    register u32 saveMask = __osDisableInt();

    __osGlobalIntMask &= ~(mask & ~OS_IM_RCP);
    __osRestoreInt(saveMask);
}
