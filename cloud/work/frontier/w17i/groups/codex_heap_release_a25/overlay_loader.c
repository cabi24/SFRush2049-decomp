/*
 * Overlay loader (N64-only; no arcade ancestor).  Historical labels: PrevMaxPath (0x800A11E4),
 * InitMaxPath (0x800A1244), sync_maxpath_to_checkpoint (0x800A133C).  NextMaxPath is the game
 * heap's allocate-at-address (alloc_at.c in this group).
 *
 * PrevMaxPath = load_overlay(vram, name, vramEnd, bssEnd, romStart, bssStart, &loaded, romData):
 * if the overlay is not loaded, reserve [vram, vramEnd) in the game heap, mark the block
 * (0x800962D4), inflate the compressed image to vram, clear its bss and set the flag.
 * InitMaxPath loads overlay "Extra" (flag D_8011ED04), sync_maxpath_to_checkpoint loads "Start"
 * (flag D_8011ED00).  display_enable (0x800C8FA4, unmatched, internal; real caller
 * playgame_state_change unmatched) unloads "Extra", inlines the "Start" load and unloads "Start".
 *
 * PrevMaxPath is internal (IPA register parameters: s0 vram, a2 vramEnd, s1 bssEnd, s2 bssStart,
 * s3 &loaded, s4 romData; name and romStart are stack-passed and unused).
 * Shaping quirks (compile-affecting, may not be original source):
 *  - `if (&name == 0) return;` is code-free; it homes `name` in memory (callers store it at
 *    4(sp)) and keeps umerge from inlining PrevMaxPath into its three callers.
 *  - the four code-free `if (x) {}` reads set the callee-saved colouring order of the register
 *    parameters (uopt orders webs by accumulated read count; workbench lever 9).  Without them the
 *    order is s0 vram, s1 loaded, s2 bssStart, s3 romData, s4 bssEnd.
 */
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

void PrevMaxPath(u8 *dst, char *name, u8 *end, u8 *bssEnd, u8 *romStart, u8 *bssStart, s8 *loaded, u8 *rom) {
    void *p;

    if (*loaded != 0) {
        return;
    }
    if (&name == 0) {
        return;
    }
    if (dst) {}
    if (bssEnd) {}
    if (bssEnd) {}
    if (bssStart) {}
    p = NextMaxPath((u32) dst, end - dst);
    audio_loop_control(p, 0);
    inflate_decompress(rom, dst, 1);
    bzero(bssStart, bssEnd - bssStart);
    *loaded = 1;
}

void InitMaxPath(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Extra", (u8 *) 0x803BB380, D_8039B440, D_00BE4C70, D_80394F70, &D_8011ED04, D_8002B024);
}

void sync_maxpath_to_checkpoint(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Start", (u8 *) 0x803BB380, D_803BAE20, D_00BDA100, D_803B9A50, &D_8011ED00, D_8002B020);
}
