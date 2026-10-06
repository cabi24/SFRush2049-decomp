/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * NEAR-MISS (wave 10, w10d): 12 of 360 words in the whole-program unit; provisional (stand-in dead caller).
 *   blob_unit --tag w10d score func_800E0050 func_800E0048 --with best.c --internal func_800E0050
 *
 * func_800E0050(car): per-frame engine-loop sound update for one car (N64 sound bookkeeping, the sibling of
 * the locked func_800D5524 that releases the same records).  car = MODELDAT (slot +0x7C6, s8 mode +0x7CC,
 * s16 rpm +0x7D0, f32 load +0x404, position +0x22C).  D_80140420[slot] is the car's SndSet: +0 points at a
 * table of 0x20-byte engine samples (u16 r0/r1 at +4/+6 for the volume curve, u16 rpm breakpoints b0..b2 at
 * +8/+A/+C, f32 pitch points p0..p2 at +10/+14/+18); rec[0..1] are the two engine-loop voices (handle,
 * pitch +8, volume +0xC), rec[3] an occasional one-shot.  rpm = |car->rpm|, clamped to 890 unless
 * (state_word_a & 0x400000) or mode 2.  Per voice: volume = (r0/r1)*(rpm/r0 - 1) + 1 clamped to [0,2];
 * pitch = piecewise-linear over the breakpoints, times (0.85 + 0.15*(load+200)/900).  Mode 2 (multiplayer
 * split): pitch *= 0.8 - 0.05*players and the voice is updated through entity_hierarchy_update / client_sync
 * only when the values change; otherwise pitch *= 0.75 and both go to camera_clip_planes (positional), with a
 * random one-shot (Random(5)==0, sample 98+Random(3)) started through camera_target_track when D_80152032 >
 * 0.75 and the previous one-shot finished (leaderboard_update == 0).
 *
 * Shaping that the words prove: the engine-volume message is an inlined static (osRecvMesg /
 * entity_transform_calc(h,-2,-2,-2,v) / osJamMesg -- retail calls entity_transform_calc with f22/f24/f26 = -2);
 * Random(max) is the locked arcade func_8008B2E4 (inlined, result in f0); D_80140420[slot] is held in a pointer
 * hoisted before the loop and the voices are indexed set->rec[i]; &D_801141B0 is a pointer local assigned
 * before the loop (retail keeps it in s6); the pitch scale is written inline as `pitch *= 0.85f + ...`;
 * each segment is `pitch = frac; pitch = (p1 - p0) * pitch + p0`.
 *
 * Residual (12 words):
 *  - 8 words: the float conversions of t->b0 and t->b1 tie in colouring priority (save 15, 2 blocks each,
 *    traced uopt proc 80 webs w76/w85); retail colours b1 first (b0 -> f12, b1 -> f2).  Forcing the swap gives
 *    exactly the retail words.  Tried: reversed compares, nested/goto ladder, (f32) casts, float locals, single
 *    expression per segment: no change.
 *  - 4 words: as1 order in the mode-2 block (`addiu a0` for &D_80142728 in the bne delay slot; the f20 reload
 *    after the inlined osJamMesg argument set-up).
 * Quirk: `f32 unused[2]` supplies the 8 bytes between load (sp+64) and slot (sp+52); retail frame 72.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[4];
    u16 r0;          /* 0x04 */
    u16 r1;          /* 0x06 */
    u16 b0;          /* 0x08 */
    u16 b1;          /* 0x0A */
    u16 b2;          /* 0x0C */
    u8 padE[2];
    f32 p0;          /* 0x10 */
    f32 p1;          /* 0x14 */
    f32 p2;          /* 0x18 */
    u8 pad1C[4];
} EngTbl; /* 0x20 */

typedef struct {
    s32 handle;
    f32 unk4[4];
} SndRec; /* 0x14 */

typedef struct {
    EngTbl *tbl;
    SndRec rec[4];
} SndSet; /* 0x54 */

typedef struct {
    u8 pad0[0x22C];
    f32 pos[3];      /* 0x22C */
    u8 pad238[0x404 - 0x238];
    f32 f404;        /* 0x404 */
    u8 pad408[0x7C6 - 0x408];
    s16 slot;        /* 0x7C6 */
    u8 pad7C8[4];
    s8 unk7CC;       /* 0x7CC */
    u8 pad7CD[3];
    s16 rpm;         /* 0x7D0 */
} MODELDAT;

extern s8 D_8010FFC0;
extern u32 state_word_a;
extern SndSet D_80140420[];
extern int D_8011735C;
extern s32 D_801141B0;
extern s32 gameplay_mode;
extern s16 active_player_count;
extern s32 D_80142728;
extern s16 D_80152032;

s32 osRecvMesg(void *, void *, s32);
s32 osJamMesg(void *, void *, s32);
void entity_transform_calc(s32 h, f32 x, f32 y, f32 z, f32 w);
void client_sync(s32 h, f32 opacity);
void results_screen_update(s32 handle);
void player_conditional_call(SndRec *rec);
void scheduler_recv(s32 handle);
s32 leaderboard_update(s32 handle);
s32 camera_target_track(void *a0, s32 a1, f32 a2, f32 a3, f32 a4, f32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
void camera_clip_planes(s32 handle, s32 pos, s32 ref, f32 a, f32 b);

static void eng_volume(s32 h, f32 value) {
    osRecvMesg(&D_80142728, 0, 1);
    entity_transform_calc(h, -2.0f, -2.0f, -2.0f, value);
    osJamMesg(&D_80142728, 0, 0);
}

f32 func_8008B2E4(f32 max);

void player_conditional_check(SndRec *rec, s32 arg1) {
    if (arg1 != 0) {
        results_screen_update(rec->handle);
    } else {
        scheduler_recv(rec->handle);
    }
    player_conditional_call(rec);
}

void func_800E0050(MODELDAT *m) {
    f32 rpm;
    f32 load;
    f32 unused[2];
    s32 slot;
    s32 i;
    SndSet *set;
    EngTbl *t;
    f32 vol;
    f32 pitch;
    f32 k;
    f32 *pos;
    s32 *ref;

    slot = m->slot;
    if (D_8010FFC0 == 0) {
        return;
    }
    if (m->rpm < 0) {
        rpm = -m->rpm;
    } else {
        rpm = m->rpm;
    }
    load = m->f404;
    if (!(state_word_a & 0x400000) && m->unk7CC != 2 && !(rpm < 890.0f)) {
        rpm = 890.0f;
    }
    set = &D_80140420[slot];
    ref = &D_801141B0;
    for (i = 0; i < 2; i++) {
        if (set->rec[i].handle == -1) {
            return;
        }
        t = &set->tbl[i];
        vol = ((f32) t->r0 / (f32) t->r1) * (rpm / t->r0 - 1.0f) + 1.0f;
        vol = vol < 0.0f ? 0.0f : vol > 2.0f ? 2.0f : vol;
        if (rpm < t->b0) {
            pitch = t->p0;
        } else if (rpm < t->b1) {
            pitch = (rpm - t->b0) / (t->b1 - t->b0);
            pitch = (t->p1 - t->p0) * pitch + t->p0;
        } else if (rpm < t->b2) {
            pitch = (rpm - t->b1) / (t->b2 - t->b1);
            pitch = (t->p2 - t->p1) * pitch + t->p1;
        } else {
            pitch = t->p2;
        }
        pitch *= 0.85f + ((load + 200.0f) / 900.0f) * 0.15f;
        if (m->unk7CC == 2) {
            pitch *= 0.8f - 0.05f * (gameplay_mode == 2 ? 1 : active_player_count);
            if (vol != set->rec[i].unk4[1]) {
                set->rec[i].unk4[1] = vol;
                eng_volume(set->rec[i].handle, vol);
            }
            if (pitch != set->rec[i].unk4[0]) {
                set->rec[i].unk4[0] = pitch;
                client_sync(set->rec[i].handle, pitch);
            }
        } else {
            pitch *= 0.75f;
            pos = m->pos;
            if (state_word_a & 0x400000) {
                if (set->rec[3].handle != -1) {
                    player_conditional_check(&set->rec[3], 1);
                }
            } else if (0.75f < D_80152032) {
                if (set->rec[3].handle == -1 || leaderboard_update(set->rec[3].handle) == 0) {
                    if ((s32) func_8008B2E4(5.0f) == 0) {
                        set->rec[3].handle = camera_target_track(ref, (s32) ref, 400.0f, 0.0f, 1.0f, 0.0f,
                            (s32) (func_8008B2E4(3.0f) + 98.0f), slot, 0, 128);
                        camera_clip_planes(set->rec[3].handle, (s32) pos, (s32) ref, 0.8f, -2.0f);
                    }
                }
            }
            if (vol != set->rec[i].unk4[1]) {
                set->rec[i].unk4[1] = vol;
            } else {
                vol = -2.0f;
            }
            camera_clip_planes(set->rec[i].handle, (s32) pos, (s32) ref, pitch, vol);
        }
    }
}

/*
 * STAND-IN (hypothesis): func_800E0048 is the caller-less `jr ra; nop` stub directly before this function.
 * Retail has no reference to func_800E0050 at all, yet the body is an internal procedure (parameter read
 * from the caller's home slot 72(sp) without a store, s0-s8 and f20-f30 used unsaved).  Two call sites
 * that are compiled out after IPA reproduce exactly that, and leave func_800E0048 as `jr ra; nop`.
 */
void func_800E0048(void) {
    MODELDAT *m;

    if (0) {
        func_800E0050(m);
        func_800E0050(m);
    }
}
