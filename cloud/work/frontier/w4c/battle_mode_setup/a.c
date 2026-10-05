typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef f32 F32; typedef s32 S32; typedef s16 S16; typedef s8 S08;
typedef struct MODELDAT {
    u8 pad0[76];
    F32 W[3];               /* 76 */
    u8 pad58[544 - 88];
    F32 RWV[3];             /* 544 */
    F32 RWR[3];             /* 556 */
    u8 pad238[748 - 568];
    F32 UV[3][3];           /* 748 */
    u8 pad310[1812 - 784];
    F32 lasttime;           /* 1812 */
    u8 pad718[1940 - 1816];
    F32 reckonRWR[3];       /* 1940 */
    F32 reckonUV[3][3];     /* 1952 */
    u8 pad7C4[1992 - 1988];
    S16 in_game;            /* 1992 */
    u8 pad7CA[2036 - 1994];
    F32 time_fudge;         /* 2036 */
    u8 pad7F8[2056 - 2040];
} MODELDAT;
typedef struct CAR_DATA {
    u8 pad0[857];
    S08 state;              /* 857 */
    u8 pad35A[952 - 858];
} CAR_DATA;
extern MODELDAT D_8014A250[6];
extern CAR_DATA D_80152818[6];
void math_utility(void *src, void *dst);
void sound_position_set(F32 *rv, void *uvs);

static void scalmul(F32 *a, F32 s, F32 *out) {
    out[0] = a[0] * s;
    out[1] = a[1] * s;
    out[2] = a[2] * s;
}

static void vecadd(F32 *a, F32 *b, F32 *out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void battle_mode_setup(F32 time) {
    MODELDAT *m;
    S32 i;
    F32 deltime, temp[3];

    for (i = 0; i < 6; i++) {
        m = &D_8014A250[i];
        if (m->in_game && D_80152818[i].state < 2) {
            deltime = (time - m->lasttime) * m->time_fudge;
            scalmul(m->RWV, deltime, temp);
            vecadd(temp, m->RWR, m->reckonRWR);
            math_utility(m->UV, m->reckonUV);
            scalmul(m->W, deltime, temp);
            sound_position_set(temp, m->reckonUV);
        }
    }
}
