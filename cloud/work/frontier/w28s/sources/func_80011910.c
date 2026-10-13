/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80011910 (lib_11640, 208 B): configure and submit the audio RSP task.
 *
 * Body: Astra's #377 (cloud/work/frontier/dot_static_11910_20261013/candidate.c),
 * with the descriptor typed as the SDK's OSTask (include/types.h). For a
 * nonzero command count it fills the task header, microcode and command
 * fields, writes back count * 2576 bytes of commands, hands the task to the
 * submit hook and marks completion pending (func_800118C0 waits on that flag).
 * The local pointer whose lifetime starts before the count guard is the
 * load-bearing source lever (#377's causal matrix); no padding, unused
 * locals, false prototypes or shaping devices.
 *
 * Storage (owner decision 2026-10-13, lane w28s): lib_11640's zero-initialised
 * words 0x800382F0..0x8003833F are defined here, in retail address order (IDO
 * 7.1 lays a TU's .bss out in definition order), so that the task's 16-byte
 * aligned window is one owned .bss slot (rom_owned_data.json, owner
 * func_80011910, 0x800382F0 size 0x50). Each is a real object with its retail
 * users' type; those users (func_80010DD8, func_80010E80, func_800110C4,
 * func_80011F60, func_800121BC) are C in the same TU and keep their extern
 * declarations. OWNED-OFFSETS proves every object at its retail address.
 *   D_800382F0  void *            reverb delay-line buffer (func_80010DD8 allocates it)
 *   D_800382F4  unsigned int      its sample count (func_80010DD8)
 *   D_800382F8  OSTask            the audio task configured below
 *   D_80038338  struct AudioNode * node array (func_80011F60 allocates it)
 *   D_8003833C  unsigned char *   node buffers, 256 B each (func_80011F60)
 */
#include "types.h"
void *D_800382F0;
unsigned int D_800382F4;
OSTask D_800382F8;
struct AudioNode *D_80038338;
unsigned char *D_8003833C;
extern unsigned char D_80027DD0[];
extern unsigned char D_80027EA0[];
extern unsigned char D_8002A050[];
extern unsigned char D_8002E210[];
extern void (*D_80038000)(OSTask *);
extern volatile unsigned char D_800382CD;
extern void osWritebackDCache(void *, int);
void func_80011910(void *commands, unsigned short count)
{
    OSTask *task = &D_800382F8;
    if (count != 0) {
        task->t.type = 2;
        task->t.flags = 0;
        task->t.ucode_boot = (u64 *) D_80027DD0;
        task->t.ucode_boot_size = (unsigned int) D_80027EA0 - (unsigned int) D_80027DD0;
        task->t.ucode = (u64 *) D_8002A050;
        task->t.ucode_size = 4096;
        task->t.ucode_data = (u64 *) D_8002E210;
        task->t.ucode_data_size = 2048;
        task->t.data_ptr = commands;
        task->t.data_size = count;
        osWritebackDCache(commands, count * 2576);
        D_80038000(task);
        D_800382CD = 1;
    }
}
