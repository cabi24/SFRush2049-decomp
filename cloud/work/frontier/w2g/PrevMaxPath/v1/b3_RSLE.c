typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern s8 D_8011ED00;
extern s8 D_8011ED04;
extern u8 *D_8002B020;
extern u8 *D_8002B024;
extern u8 D_80394F70[];
extern u8 D_8039B440[];
extern u8 D_803B9A50[];
extern u8 D_803BAE20[];
extern u8 D_00BE4C70[];
extern u8 D_00BDA100[];

void *NextMaxPath(u32 addr, u32 size);
void audio_loop_control(void *ptr, s32 flag);
void inflate_decompress(u8 *src, u8 *dst, s32 flag);
void bzero(void *p, u32 n);

void PrevMaxPath(u8 *dst, char *name, u8 *end, u8 *rom, u8 *romStart, u8 *bssStart, s8 *loaded, u8 *bssEnd) {
    void *p;
    char **np = &name;

    if (*loaded != 0) {
        return;
    }
    p = NextMaxPath((u32) dst, end - dst);
    audio_loop_control(p, 0);
    inflate_decompress(rom, dst, 1);
    bzero(bssStart, bssEnd - bssStart);
    *loaded = 1;
    if (np) {
    }
}

void InitMaxPath(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Extra", (u8 *) 0x803BB380, D_8002B024, D_00BE4C70, D_80394F70, &D_8011ED04, D_8039B440);
}

void sync_maxpath_to_checkpoint(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Start", (u8 *) 0x803BB380, D_8002B020, D_00BDA100, D_803B9A50, &D_8011ED00, D_803BAE20);
}
