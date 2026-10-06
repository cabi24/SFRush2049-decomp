/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
        s = voice->slot;
        pad_config[s].state = 2;
        if (i) {}
        D_80149788--;
        if (D_80149450[i] != voice) {
        }
        D_80149450[i] = D_80149450[D_80149788];
        D_80149450[D_80149788] = voice;
        voice = voice->next;
    }
}
