/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800CF06C: per-tire road contact update, N64 port of arcade game/road.c road(MODELDAT *m).
 *
 * Callees (all locked): camera_victory (new N64 pre-pass on the model), entity_update = tiresurf
 * (m, TIRERWR[i], igpos, &roadcode[i], roaduvs[i].fpuvs, i), menu_audio_settings = collision(m).
 * Differences from the arcade: no CENTERFORCE clear, m->(0x648) cleared once before the tire loop,
 * suscomp = tpcomp - igpos[Y] (the arcade adds igpos[ZCOMP]), makesuvs/bodysurf and the body-velocity
 * code are gone, all literals single precision.
 *
 * Literal spellings (uopt constant-web keys, wave 17): the force-clear zero is `0.f` (retail keeps it in
 * $f0, apart from the loop's 0.0f in $f20); the clamp stores `3.f` / `40.f` are separate constants from
 * the compares `3.0f` / `40.0f` (retail holds 3.0 twice, in $f26 and $f28, and rematerialises 40.0 for
 * the store).
 *
 * DISCLOSED unused locals (exact frame residual, owner rule): `BODYRWV[3]` (an arcade global, here a
 * local declared before igpos) and `temp[3]` (arcade road() local, its body-velocity code is gone).
 * Retail frame 192 with igpos at sp+168; without both locals the frame is 168 (igpos at sp+156); with
 * temp only, 176. 12 + 12 bytes = 6 slots (192 - 168 = 24).
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct ROADUVS { f32 fpuvs[3][3]; } ROADUVS;
typedef struct MODELDAT {
    u8 pad0[196];
    f32 BODYFORCE[4][3];        /* 0x0C4 */
    u8 pad244[580 - 244];
    f32 TIRERWR[4][3];          /* 0x244 */
    u8 pad628[784 - 628];
    ROADUVS roaduvs[4];         /* 0x310 */
    u8 pad928[1484 - 928];
    f32 suscomp[4];             /* 0x5CC */
    f32 tpcomp[4];              /* 0x5DC */
    f32 airdist[4];             /* 0x5EC */
    f32 airvel[4];              /* 0x5FC */
    s32 roadcode[4];            /* 0x60C */
    u16 sviscode[4];            /* 0x61C */
    u8 pad1572[1592 - 1572];
    f32 idt;                    /* 0x638 */
    u8 pad1596[1608 - 1596];
    s32 unk648;                 /* 0x648, cleared each frame before the tire loop */
} MODELDAT;
void camera_victory(MODELDAT *m);
void entity_update(MODELDAT *m, f32 *rwr, f32 *igpos, s32 *code, f32 *uvs, s32 i);
void menu_audio_settings(MODELDAT *m);

void func_800CF06C(MODELDAT *m)
{
    f32 BODYRWV[3], igpos[3], temp[3], last_airdist;
    s32 i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 4; j++)
            m->BODYFORCE[j][i] = 0.f;

    camera_victory(m);
    m->unk648 = 0;
    for (i = 0; i < 4; ++i) {
        entity_update(m, &m->TIRERWR[i][0], igpos, &m->roadcode[i], m->roaduvs[i].fpuvs[0], i);
        last_airdist = m->airdist[i];
        m->suscomp[i] = m->tpcomp[i] - igpos[1];
        m->airdist[i] = -m->suscomp[i];
        if (m->suscomp[i] > 3.0f)
            m->suscomp[i] = 3.f;
        else if (m->suscomp[i] < -3.0f)
            m->suscomp[i] = -3.0f;
        m->tpcomp[i] = (m->suscomp[i] < 0.0f) ? 0.0f : m->suscomp[i];
        if (m->suscomp[i] < 0.0f)
            m->roadcode[i] = m->sviscode[i] = 8;
        m->airvel[i] = (m->airdist[i] - last_airdist) * m->idt;
        if (m->airvel[i] < -40.0f)
            m->airvel[i] = -40.0f;
        else if (m->airvel[i] > 40.0f)
            m->airvel[i] = 40.f;
    }
    menu_audio_settings(m);
}
