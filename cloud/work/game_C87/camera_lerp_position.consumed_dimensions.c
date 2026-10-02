/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef struct PoolDescriptor {
    s8 mode;
    s32 count;
    s32 stride;
    void *data;
    u32 allocated;
    void *free_list;
} PoolDescriptor;
typedef struct InitRecord { u32 a, b, c, d, e; } InitRecord;
extern PoolDescriptor D_80143FC8, D_80151AA8;
extern unsigned char D_8014D280[];
extern s8 D_80114650, D_80150DD0, D_80150E28[];
extern s32 D_80150B64, D_80151408, D_80150F78, D_801515F0, D_80151610, gameplay_mode;
typedef struct ListFirstWord { u32 word; } ListFirstWord;
extern ListFirstWord D_80151688, D_80151964;
extern InitRecord D_80151528[];
extern void *D_80151A70;
extern void pool_linked_list_init(PoolDescriptor *);
extern void func_800BB7F4(void);
extern void *audio_dma_sync(void *, s32);
extern void func_803914a8(void);
void camera_lerp_position(s32 argument) {
    s16 i;
    void *small_pool_data;
    void *large_pool_data;
    s32 large_pool_count = 130;
    s32 large_pool_stride = 112;
    large_pool_data = D_8014D280;
    D_80143FC8.stride = large_pool_stride;
    D_80143FC8.count = large_pool_count;
    D_80143FC8.data = large_pool_data;
    D_80143FC8.mode = 0;
    pool_linked_list_init(&D_80143FC8);
    D_80150DD0 = D_80114650;
    func_800BB7F4();
    D_80150B64 = 0;
    for (i = 0; i < 8; i++) D_80150E28[i] = 0;
    D_80151408 = 0;
    for (i = 0; i < 4; i++) {
        D_80151528[i].a = 0;
        D_80151528[i].b = 0;
        D_80151528[i].c = 0;
        D_80151528[i].d = 0;
        D_80151528[i].e = 0;
    }
    if (gameplay_mode == 6) {
        if (D_80150DD0 == 0) D_80151A70 = audio_dma_sync((void *)0, 120);
        D_80150E28[5] = 16;
        func_803914a8();
        small_pool_data = D_80151A70;
        D_80151AA8.data = small_pool_data;
        D_80151AA8.stride = 8;
        D_80151AA8.count = 15;
        D_80151AA8.mode = 0;
        pool_linked_list_init(&D_80151AA8);
    }
    D_80150F78 = 0;
    D_80151688.word = 0;
    D_801515F0 = 0;
    D_80151964.word = 0;
    D_80151610 = 0;
}
