/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_stop (w5a, NOT a match): 12/40 words differ (standalone score.py fn, -O3).
 * Releases a linked list of voices (next at +0x3C): find each in the pool D_80149450[0..count),
 * mark pad_config[voice->slot] (32-byte entries, byte 22) = 2, swap-remove with the last entry.
 * Residual: retail computes i*4 first after the search loop, loads the slot into v0 (i's
 * register: slot is a register web) and keeps &pool[i] in a1 (the search loop's pointer
 * register) while the pool count stays promoted in v1 across the outer loop.
 *  - `p = &pool[i]; i = voice->slot; ...; *p = ...` reproduces the post-loop block exactly but
 *    the store through the named pointer breaks the count promotion (count reloaded per voice);
 *  - a compiled-out `if (D_80149450[i] != voice) {}` makes &pool[i] an unnamed web in a1 and
 *    keeps the promotion (yy/z1.c, 27 rows) but the slot stays a temp.
 * Best next hypothesis: a compiled-out check that both keeps &pool[i] as an unnamed web and
 * makes the slot a web (e.g. a check on the slot before the pad_config store), with i reused.
 */
typedef unsigned char u8; typedef unsigned short u16; typedef int s32;
typedef struct Voice { u8 pad0[0x34]; u16 slot; u8 pad36[6]; struct Voice *next; } Voice;
typedef struct PadConfig { u8 pad0[22]; u8 state; u8 pad17[9]; } PadConfig;
extern PadConfig pad_config[];
extern Voice *D_80149450[];
extern s32 D_80149788;

void sound_stop(Voice *voice) {
    s32 i;
    s32 n;
    s32 s;

    while (voice != 0) {
        for (i = 0; i < D_80149788; i++) {
            if (voice == D_80149450[i]) break;
        }
        if (D_80149450[i] != voice) {
        }
        s = voice->slot;
        pad_config[s].state = 2;
        n = --D_80149788;
        D_80149450[i] = D_80149450[n];
        D_80149450[n] = voice;
        voice = voice->next;
    }
}
