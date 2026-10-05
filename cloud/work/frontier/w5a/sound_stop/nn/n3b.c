typedef unsigned char u8; typedef unsigned short u16; typedef int s32;
typedef struct Voice { u8 pad0[0x34]; u16 slot; u8 pad36[6]; struct Voice *next; } Voice;
typedef struct PadConfig { u8 pad0[22]; u8 state; u8 pad17[9]; } PadConfig;
extern PadConfig pad_config[];
extern Voice *D_80149450[];
extern s32 D_80149788;

void sound_stop(Voice *voice) {
    s32 i;
    s32 n;
    Voice **p;

    while (voice != 0) {
        n = D_80149788;
        for (i = 0; i < n; i++) {
            if (voice == D_80149450[i]) break;
        }
        p = &D_80149450[i];
        i = voice->slot;
        pad_config[i].state = 2;
        D_80149788 = n = n - 1;
        *p = D_80149450[n];
        D_80149450[n] = voice;
        voice = voice->next;
    }
}
