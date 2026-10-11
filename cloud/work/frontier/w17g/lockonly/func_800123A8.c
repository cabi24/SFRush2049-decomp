/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800123A8 (static, boot 0x800123A8, 628 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 74 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_task_scheduler_800123A8.
 * Body + declarations verbatim from cloud/work/boot_tail/BT02-audio-pair/nonmatch/func_800123A8.c (Astra / BT02-audio-pair).
 * w17g edit: the D10 file called the cache routine by its address label func_800084E0; the target relocation (and
 * symbol_addrs) name it osInvalDCache (0x800084E0), so the call uses that name. IDO 5.3 diff count above is D10's.
 * Shaping devices: none. */
typedef struct AudioCacheNode {
    struct AudioCacheNode *next;
    struct AudioCacheNode *previous;
    unsigned int unknown08;
    unsigned int unknown0C;
    unsigned int unknown10;
    unsigned char *buffer;
} AudioCacheNode;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osInvalDCache(void *, int);
extern unsigned char *D_8003834C;
extern AudioCacheNode *D_80038350;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
void func_800123A8(unsigned short count)
{
    int i;
    D_8003834C = D_80038018(count * 1536, 0);
    osInvalDCache(D_8003834C, count * 1536);
    D_80038350 = D_80038018(count * sizeof(AudioCacheNode), 0);
    D_80038354 = 0;
    D_80038358 = 0;
    D_8003835C = D_80038350;
    D_80038350[0].previous = 0;
    D_80038350[0].buffer = D_8003834C;
    for (i = 1; i < count; i++) {
        D_80038350[i - 1].next = &D_80038350[i];
        D_80038350[i].previous = &D_80038350[i - 1];
        D_80038350[i].buffer = &D_8003834C[i * 1536];
    }
    D_80038350[i - 1].next = 0;
}
