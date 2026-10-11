/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80011F60 (static, boot 0x80011F60, 604 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 74 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_task_scheduler_80011F60.
 * Body + declarations verbatim from cloud/work/boot_tail/BT02-audio-pair/nonmatch/func_80011F60.c (Astra / BT02-audio-pair).
 * w17g edit: the D10 file called the cache routine by its address label func_800084E0; the target relocation (and
 * symbol_addrs) name it osInvalDCache (0x800084E0), so the call uses that name. IDO 5.3 diff count above is D10's.
 * Shaping devices: none. */
typedef struct AudioBufferNode {
    struct AudioBufferNode *next;
    struct AudioBufferNode *previous;
    unsigned char *buffer;
    unsigned int unknown0C;
    unsigned int unknown10;
} AudioBufferNode;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osInvalDCache(void *, int);
extern AudioBufferNode *D_80038338;
extern unsigned char *D_8003833C;
extern unsigned int D_80038340;
extern AudioBufferNode *D_80038344;
extern AudioBufferNode *D_80038348;
void func_80011F60(int count)
{
    int i;
    D_80038338 = D_80038018(count * sizeof(AudioBufferNode), 0);
    D_8003833C = D_80038018(count * 256, 0);
    osInvalDCache(D_8003833C, count * 256);
    D_80038344 = 0;
    D_80038348 = D_80038338;
    D_80038338[0].buffer = D_8003833C;
    D_80038338[0].previous = 0;
    for (i = 1; i < count; i++) {
        D_80038338[i].previous = &D_80038338[i - 1];
        D_80038338[i - 1].next = &D_80038338[i];
        D_80038338[i].buffer = &D_8003833C[i * 256];
    }
    D_80038338[i - 1].next = 0;
    D_80038340 = 0;
}
