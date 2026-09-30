typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[16]; u16 w16; u16 w18; } Src;
typedef struct {
    s32 id;
    void *p4;
    Src *src;
    s16 s12;
    u8 pad14[2];
    s16 s16v;
    s16 s18;
    u16 s20;
    u16 s22;
    u8 b24;
    u8 pad25[3];
    s16 h[5];
} Snd;
extern u8 D_80140BDC;
extern u8 D_80110664[];
Src *func_800B24EC(s32, s16 *, s32, s32, s32);
void collision_sound_play(Snd *s) {
    if (s->id == 0) {
        s->s12 = 0;
        s->src = 0;
        s->s20 = 0;
        s->s22 = 0;
        s->p4 = D_80110664;
    } else if (s->id == -1) {
        s->s12 = 0;
        s->src = 0;
        s->s20 = 0;
        s->s22 = 0;
        s->p4 = 0;
    } else {
        Src *r = func_800B24EC(s->id, &s->s12, 0, (s8)(D_80140BDC - 1), 1);
        s->src = r;
        s->s20 = r->w16;
        s->p4 = 0;
        s->s22 = r->w18;
    }
    s->s18 = 0;
    s->b24 = 255;
    s->h[0] = -1;
    s->h[1] = -1;
    s->h[2] = -1;
    s->h[3] = -1;
    s->h[4] = -1;
}
