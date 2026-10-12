/* GENERATED ROM-aligned TU — segment 0x10d60 (rom/lib_10d60)
 * layout map 1eec946940dcffacc041b948c1e7606f7b20389466a7f58bb42d30e32fe0ed79; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — osPfsIsPlug
 * Source:   cloud/work/frontier/w23c/libultra/osPfsIsPlug.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w23c/libultra/osPfsIsPlug.c:osPfsIsPlug (score0); rom_tu.h context: cloud/work/frontier/w23c/ctx53_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType2;
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern void __osPfsRequestData(u8 cmd);
extern void func_800103A0(u8* pattern, OSContStatus* data);
s32 osPfsIsPlug(OSMesgQueue* mq, u8* pattern) {
    s32 ret = 0;
    OSMesg msg;
    u8 bitpattern;
    OSContStatus contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();

    do {
        __osPfsRequestData(0);

        ret = __osSiRawStartDma(1, &__osPfsBuffer);
        osRecvMesg(mq, &msg, 1);

        ret = __osSiRawStartDma(0, &__osPfsBuffer);
        osRecvMesg(mq, &msg, 1);

        func_800103A0(&bitpattern, &contData[0]);

        for (channel = 0; channel < __osPfsRequestType2; channel++) {
            if ((contData[channel].status & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }

        if (channel == __osPfsRequestType2) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);

    for (channel = 0; channel < __osPfsRequestType2; channel++) {
        if ((contData[channel].errno == 0) && ((contData[channel].status & 1) != 0)) {
            bits |= (1 << channel);
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}

/* PROMOTED 2026-10-11 — __osPfsRequestData
 * Source:   cloud/work/frontier/w23c/libultra/__osPfsRequestData.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w23c/libultra/__osPfsRequestData.c:__osPfsRequestData (score0); rom_tu.h context: cloud/work/frontier/w23c/ctx53_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 __osPfsRequestType;
void __osPfsRequestData(u8 cmd) {
    u8* ptr = (u8*)&__osPfsBuffer;
    __OSContRequesFormat requestformat;
    int i;

    __osPfsRequestType = cmd;
    ((OSPifRam*)__osPfsBuffer)->pifstatus = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;

    for (i = 0; i < __osPfsRequestType2; i++) {
        *((__OSContRequesFormat*)ptr) = requestformat;
        ptr += sizeof(__OSContRequesFormat);
    }

    *ptr = 0xFE;
}

/* PROMOTED 2026-10-11 — func_800103A0
 * Source:   cloud/work/frontier/w16g/sources/func_800103A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/func_800103A0.c:func_800103A0 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType2;
void func_800103A0(u8* pattern, OSContStatus* data) {
    u8* ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits = 0;

    ptr = (u8*)&__osPfsBuffer;

    for (i = 0; i < __osPfsRequestType2; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((__OSContRequesFormat*)ptr);
        data->errno = CHNL_ERR(requestformat);

        if (data->errno != 0) {
            continue;
        }

        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}

