/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef float f32;
typedef struct Sound {
    u8 pad0[0x10];
    s32 state;
    s32 mode;
    u8 pad18[1];
    s8 looped;
    u8 refs;
    u8 pad1B[5];
    f32 start;
    f32 level;
} Sound;
typedef struct Cmd {
    s16 id;
    s8 kind;
    s8 used;
    Sound *sound;
    u8 pad8[0x10];
} Cmd;
typedef struct Voice {
    u8 pad0[8];
    Sound *sound;
} Voice;
extern f32 D_80152748;
extern s32 D_80146200;
extern s32 D_80110250;
extern s32 D_8011024C;
extern Cmd *D_80143CF0[];
extern Cmd *D_80143AE8[];
Cmd *func_80091B00(void);

s32 func_80095198(f32 x, f32 delta) {
    f32 sum = x + delta;
    if (D_80152748 < x) sum -= 14400.0f;
    return sum < D_80152748;
}

void sound_position_update(Voice *v, s32 dist) {
    Cmd *c;

    if (v->sound->state == 2) {
    } else if (v->sound->state == 1) {
        return;
    }
    if (dist >= D_80146200 * 0.75f
        || (v->sound->looped && v->sound->state == 2 && v->sound->level <= 0.0f && func_80095198(v->sound->start, 0.2f))) {
        if (v->sound->state != 2) {
            return;
        }
        c = func_80091B00();
        D_80143CF0[D_80110250++] = c;
        c->kind = 5;
        c->sound = v->sound;
        c->sound->refs++;
        return;
    }
    if (v->sound->state == 2) {
        return;
    }
    if (v->sound->level > 0.1f || (!v->sound->looped && v->sound->mode == 1)) {
        c = func_80091B00();
        D_80143AE8[D_8011024C++] = c;
        c->kind = 3;
        c->sound = v->sound;
        c->sound->refs++;
    }
}
