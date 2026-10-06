typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

extern u32 D_8011025C;
extern u32 D_80110260;
extern s32 D_80152770;

s32 osRecvMesg(void *, void *, s32);
s32 osJamMesg(void *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);

__inline void audio_effect_process(u32 address) {
    s32 u0, u1, u2, u3;
    osRecvMesg(&D_80152770, 0, 1);
    audio_reverb_update(address, 0);
    osJamMesg(&D_80152770, 0, 0);
}

void func_800C885C(void) {
    if (D_8011025C) {
        audio_effect_process(D_8011025C);
        D_8011025C = 0;
    }
    if (D_80110260) {
        audio_effect_process(D_80110260);
        D_80110260 = 0;
    }
}
