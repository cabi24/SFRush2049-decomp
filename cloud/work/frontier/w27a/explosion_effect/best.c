/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Car952 { u8 pad000[239]; s8 mode; u8 pad0F0[8]; s16 speed; u8 pad0FA[952 - 250]; } Car952;
typedef struct Game2056 { u8 pad000[1990]; s16 slot; u8 pad7C8[2056 - 1992]; } Game2056;
typedef struct Pair { s32 first, second; } Pair;
typedef struct Units { u8 pad0[808]; char *mph; char *kmh; } Units;
typedef struct Bank { u8 pad0[2]; u8 b2; u8 b3; } Bank;

extern s8 D_8015F72C;
extern Pair D_80118E20;
extern s16 D_80151AD0;
extern Game2056 D_8014A250[];
extern Car952 player_array[];
extern s8 D_801613AA;
extern s32 D_80115A68[][4][2];
extern Units *D_8017A4E4;
extern Bank *D_801497F0;
extern u8 D_801461D0[];

void render_helper(f32 arg0);
void dispatch_handler(s32);
void state_utility(s16, s16, char *);
void sound_update_channel(s32 force);
u8 camera_shake_update(s32 arg0);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osSendMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);

static void font_lock(void) {
    osRecvMesg(D_801461D0, 0, 1);
}

static void font_unlock(void) {
    osSendMesg(D_801461D0, 0, 0);
}

static s32 font_select(s32 sel) {
    s32 old;
    font_lock();
    old = slot_state_setup(sel);
    font_unlock();
    return old;
}

static void pair_set(s32 first, s32 second) {
    Pair *p = &D_80118E20;
    p->first = first;
    p->second = second;
}

static u8 bank_byte2(void) {
    sound_update_channel(0);
    return D_801497F0->b2;
}

static u8 bank_byte3(void) {
    sound_update_channel(0);
    return D_801497F0->b3;
}

s32 explosion_effect(void *arg0) {
    char digits[4];
    char str[2];
    s32 y;
    s32 i;
    s32 x;
    s32 charw;
    f32 speed;
    char *p;
    Game2056 *g;

    if (D_8015F72C == 0) {
        return 1;
    }
    render_helper(0.0f);
    pair_set(1, 3);
    if (D_80151AD0 == 1) {
        font_select(4);
    } else {
        font_select(1);
    }
    charw = camera_shake_update(56) + 1;
    g = D_8014A250;
    for (i = 0; i < D_80151AD0; i++, g++) {
        if (player_array[g->slot].mode == 1) {
            continue;
        }
        speed = (player_array[i].speed >> 2) * 1.2f;
        if (D_801613AA) {
            speed *= 1.61f;
        }
        if (speed < 0.0f) {
            speed = -speed;
        }
        x = D_80115A68[D_80151AD0 - 1][i][0] - charw;
        y = D_80115A68[D_80151AD0 - 1][i][1];
        digits[0] = (s32)(speed / 100.0f) % 10 + '0';
        digits[1] = (s32)(speed / 10.0f) % 10 + '0';
        digits[2] = (s32)speed % 10 + '0';
        if (D_80151AD0 == 1) {
            font_select(4);
        } else {
            font_select(1);
        }
        for (p = digits; p != &digits[3]; p++, x += charw) {
            str[1] = '\0';
            if (*p != ':' && *p != '.') {
                str[0] = '8';
                dispatch_handler(0);
                state_utility(x, y, str);
            }
            str[0] = *p;
            dispatch_handler(14);
            state_utility(x, y, str);
        }
        x = D_80115A68[D_80151AD0 - 1][i][0];
        y += (s16)(bank_byte2() + bank_byte3()) - 4;
        font_select(12);
        if (D_801613AA) {
            dispatch_handler(0);
            state_utility(x + 1, y + 1, D_8017A4E4->kmh);
            dispatch_handler(1);
            state_utility(x, y, D_8017A4E4->kmh);
        } else {
            dispatch_handler(0);
            state_utility(x + 1, y + 1, D_8017A4E4->mph);
            dispatch_handler(1);
            state_utility(x, y, D_8017A4E4->mph);
        }
    }
    pair_set(0, 3);
    render_helper(-1.0f);
    return 1;
}
