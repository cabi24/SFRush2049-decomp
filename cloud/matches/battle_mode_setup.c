/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * battle_mode_setup (historical label): game-task dead reckoning of every model, the "we control"
 * branch of arcade game_reckon_all() (game/reckon.c), N64 form: for each of the 6 MODELDAT records
 * (0x8014A250, 0x808 bytes) that is in game and whose CAR_DATA (0x80152818, 0x3B8 bytes) state byte
 * at +857 is < 2: deltime = (time - lasttime) * time_fudge (time is a float argument here);
 * reckon.RWR = RWR + RWV * deltime; reckon.UV = UV (math_utility = fmatcopy, 9 floats);
 * rotateuv(W * deltime, reckon.UV) (sound_position_set, historical label).
 * The arcade scalmul/vecadd calls are written out per component; the add is RWR + temp (temp + RWR
 * reorders the schedule). Field names taken from the arcade function; offsets are N64 evidence.
 * No compile-shaping quirks. Also MATCH at -O2.
 */
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

void battle_mode_setup(F32 time) {
    MODELDAT *m;
    S32 i;
    F32 deltime, temp[3];

    for (i = 0; i < 6; i++) {
        m = &D_8014A250[i];
        if (m->in_game && D_80152818[i].state < 2) {
            deltime = (time - m->lasttime) * m->time_fudge;
            temp[0] = m->RWV[0] * deltime;
            temp[1] = m->RWV[1] * deltime;
            temp[2] = m->RWV[2] * deltime;
            m->reckonRWR[0] = m->RWR[0] + temp[0];
            m->reckonRWR[1] = m->RWR[1] + temp[1];
            m->reckonRWR[2] = m->RWR[2] + temp[2];
            math_utility(m->UV, m->reckonUV);
            temp[0] = m->W[0] * deltime;
            temp[1] = m->W[1] * deltime;
            temp[2] = m->W[2] * deltime;
            sound_position_set(temp, m->reckonUV);
        }
    }
}
