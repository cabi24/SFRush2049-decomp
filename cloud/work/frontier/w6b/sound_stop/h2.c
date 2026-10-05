typedef unsigned char u8; typedef unsigned short u16; typedef int s32;
typedef struct Voice { u8 pad0[0x34]; u16 slot; u8 pad36[6]; struct Voice *next; } Voice;
typedef struct PadConfig { u8 pad0[22]; u8 state; u8 pad17[9]; } PadConfig;
extern PadConfig pad_config[];
extern Voice *D_80149450[];
extern s32 D_80149788;

void func_800B3584(Voice *voice) {
    s32 i;
    for (i = 0; i < D_80149788; i++) {
        if (voice == D_80149450[i]) break;
    }
    D_80149788--;
    D_80149450[i] = D_80149450[D_80149788];
    D_80149450[D_80149788] = voice;
}

void sound_stop(Voice *voice) {
    while (voice != 0) {
        pad_config[voice->slot].state = 2;
        func_800B3584(voice);
        voice = voice->next;
    }
}
