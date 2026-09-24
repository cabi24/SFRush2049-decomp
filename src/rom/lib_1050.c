/* GENERATED ROM-aligned TU — segment 0x1050 (rom/lib_1050)
 * layout map ccc136428ff2b5d1bcf76ca5c9828dfe26cb8ebd62f7eeaafde5ae2db3a770e8; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/osCreateScheduler.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/osScAddClient.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scMain.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scSchedule.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scHandleRetrace.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scHandleRSP.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scHandleRDP.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scTaskReady.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scExecTask.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scAppendList.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scExec.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scHandlePreNMI.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/__scScheduleCore.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viTickStart.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viEnableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viDisableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viUpdateTime.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viScheduleTick.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viAddTicks.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viGetTimeToDeadline.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viDeadlinePassed.s")
/* PROMOTED 2026-09-24 — viStub
 * Source:   work/auto/viStub/matched.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/auto/viStub/matched.c:viStub (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void viStub(void)
{
  int new_var2;
  unsigned short new_var;
 new_var2 = 1; new_var2 = 0; new_var = new_var2; if (new_var & (0xFFFF ^ new_var2)) { } if (new_var) { } if (new_var) { } if (new_var) { }
}

