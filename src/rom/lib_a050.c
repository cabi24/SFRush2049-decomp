/* GENERATED ROM-aligned TU — segment 0xa050 (rom/lib_a050)
 * layout map d5022908837d5caae1c6f954ab92b583231ad91b890aac766d00d270cadc95ee; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a050/__osContBuildPacket.s")
/* PROMOTED 2026-10-01 — __osContGetStatus
 * Source:   cloud/work/static_C7/__osContGetStatus.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C7/__osContGetStatus.c:__osContGetStatus (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osContGetStatus(u8* pattern, OSContStatus* data) {
    u8* ptr;
    __OSContRequesFormat requestHeader;
    int i;
    u8 bits = 0;

    ptr = (u8*)__osSiDmaBuffer.ramarray;
    for (i = 0; i < __osPfsRequestType2; i++, ptr += sizeof(requestHeader), data++) {
        requestHeader = *(__OSContRequesFormat*)ptr;
        data->errno = CHNL_ERR(requestHeader);

        if (data->errno != 0) {
            continue;
        }

        data->type = requestHeader.typel << 8 | requestHeader.typeh;
        data->status = requestHeader.status;
        bits |= 1 << i;
    }
    *pattern = bits;
}

/* PROMOTED 2026-10-01 — __osContRamReset
 * Source:   cloud/work/static_C7/__osContRamReset.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C7/__osContRamReset.c:__osContRamReset (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osContRamReset(u8 cmd) {
    u8* ptr;
    __OSContRequesFormat requestHeader;
    s32 i;

    for (i = 0; i < ARRLEN(__osSiDmaBuffer.ramarray); i++) {
        __osSiDmaBuffer.ramarray[i] = 0;
    }

    __osSiDmaBuffer.pifstatus = CONT_CMD_EXE;
    ptr = (u8*)__osSiDmaBuffer.ramarray;
    requestHeader.dummy = CONT_CMD_NOP;
    requestHeader.txsize = CONT_CMD_RESET_TX;
    requestHeader.rxsize = CONT_CMD_RESET_RX;
    requestHeader.cmd = cmd;
    requestHeader.typeh = CONT_CMD_NOP;
    requestHeader.typel = CONT_CMD_NOP;
    requestHeader.status = CONT_CMD_NOP;
    requestHeader.dummy1 = CONT_CMD_NOP;

    for (i = 0; i < __osPfsRequestType2; i++) {
        *(__OSContRequesFormat*)ptr = requestHeader;
        ptr += sizeof(requestHeader);
    }
    *ptr = CONT_CMD_END;
}

