/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_lerp_position (historical label): resets the level-object system before objects are
 * instantiated. Initialises the node pool D_80143FC8 (130 nodes of 112 bytes at D_8014D280; the
 * pool type is pool_linked_list_init's), copies D_80114650 to D_80150DD0, clears the D_8013C300
 * table (func_800BB7F4), the per-category counts D_80150E28[8], D_80150B64, D_80151408 and four
 * 20-byte records at D_80151528; when D_8014A110 == 6 it allocates (audio_dma_sync, historical label;
 * only when D_80150DD0 is clear) a 120-byte block for a second pool (15 x 8 bytes, D_80151AA8), sets
 * category 5's count to 16 and calls the overlay function 0x803914A8; finally it zeroes the counters
 * D_80150F78, D_80151688, D_801515F0, D_80151964, D_80151610 (incremented by transmission_ratio_get).
 * No arcade ancestor identified. The argument is unused (retail still stores a0 to its home slot).
 * Shaping quirks: the two pool set-ups are one inlined static helper (pool_init; argument order
 * mem, count, size sets the t6/t7/t8 order, and its pointer parameter puts the second pool's memory in
 * v0); D_80151688 and D_80151964 are cleared with `*= 0` (read-modify-write folded by uopt, which
 * leaves the address in v0/v1 as retail does; `= 0`, `&= 0`, `-= x`, volatile all differ).
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32;
typedef struct { u8 b0; u8 pad1[3]; s32 count; s32 size; u8 *mem; s32 f16; u8 *free; } Pool;
typedef struct { s32 w[5]; } Rec20;
extern Pool D_80143FC8;
extern u8 D_8014D280[];
extern s8 D_80114650;
extern s8 D_80150DD0;
extern s32 D_80150B64;
extern u8 D_80150E28[8];
extern s32 D_80151408;
extern Rec20 D_80151528[4];
extern s32 D_8014A110;
extern void *D_80151A70;
extern Pool D_80151AA8;
extern s32 D_80150F78;
extern s32 D_80151688;
extern s32 D_801515F0;
extern s32 D_80151964;
extern s32 D_80151610;
void pool_linked_list_init(Pool *p);
void func_800BB7F4(void);
void *audio_dma_sync(s32 arg0, s32 arg1);
void func_803914A8(void);

static void pool_init(Pool *p, void *mem, s32 count, s32 size) {
    p->mem = mem;
    p->count = count;
    p->size = size;
    p->b0 = 0;
    pool_linked_list_init(p);
}

void camera_lerp_position(s32 arg0) {
    s16 i;

    pool_init(&D_80143FC8, D_8014D280, 130, 112);
    D_80150DD0 = D_80114650;
    func_800BB7F4();
    D_80150B64 = 0;
    for (i = 0; i < 8; i++) {
        D_80150E28[i] = 0;
    }
    D_80151408 = 0;
    for (i = 0; i < 4; i++) {
        D_80151528[i].w[0] = 0;
        D_80151528[i].w[1] = 0;
        D_80151528[i].w[2] = 0;
        D_80151528[i].w[3] = 0;
        D_80151528[i].w[4] = 0;
    }
    if (D_8014A110 == 6) {
        if (D_80150DD0 == 0) {
            D_80151A70 = audio_dma_sync(0, 120);
        }
        D_80150E28[5] = 16;
        func_803914A8();
        pool_init(&D_80151AA8, D_80151A70, 15, 8);
    }
    D_80150F78 = 0;
    D_80151688 *= 0;
    D_801515F0 = 0;
    D_80151964 *= 0;
    D_80151610 = 0;
}
