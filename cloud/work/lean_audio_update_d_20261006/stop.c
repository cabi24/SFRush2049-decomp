/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_stop(voice) -- release a linked list of voices (next at +0x3C). For each voice: find it in the
 * active pool D_80149450[0..D_80149788) (linear search, i = count when absent), mark its 32-byte
 * pad_config[voice->slot] record's byte 22 = 2 (released), then swap-remove it: count--,
 * pool[i] = pool[count], pool[count] = voice. (Names are historical labels; N64 audio code, no arcade
 * ancestor identified.)
 *
 * Shaping (w12e, closes w5a/w10f's 12 rows after ~150 earlier variants):
 *  - the compiled-out `if (i) {}` right after the search loop is what puts the i*4 shift (`sll t7,v0,2`)
 *    first, in the exit path and the beql delay slot, before the slot load;
 *  - the slot is a named local `s` (a register web, lhu into v0 as in retail);
 *  - no compiled-out pool check (w5a's `if (D_80149450[i] != voice) {}`): that check made
 *    &D_80149450 the first address web, so phase 2 handed out a3/t0/t1/t2 in the wrong order.
 *    Without it the address webs are numbered pad_config, 2, count, pool as in retail.
 * Also MATCH at -O2. Leaf, no frame. The caller-less stub func_800B3584 before it is left as locked.
 */
typedef unsigned char u8; typedef unsigned short u16; typedef int s32;
typedef struct Voice { u8 pad0[0x34]; u16 slot; u8 pad36[6]; struct Voice *next; } Voice;
typedef struct PadConfig { u8 pad0[22]; u8 state; u8 pad17[9]; } PadConfig;
extern PadConfig pad_config[];
extern Voice *D_80149450[];
extern s32 D_80149788;

void sound_stop(Voice *voice) {
    s32 i;
    s32 s;

    while (voice != 0) {
        for (i = 0; i < D_80149788; i++) {
            if (voice == D_80149450[i]) break;
        }
        if (i) {}
        s = voice->slot;
        pad_config[s].state = 2;
        D_80149788--;
        D_80149450[i] = D_80149450[D_80149788];
        D_80149450[D_80149788] = voice;
        voice = voice->next;
    }
}
