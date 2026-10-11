/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w17e func_800D91A0 (0x800D91A0, 965 w) -- NEAR-MISS, not a match. Whole-program unit: 8 ops rows,
 * 212 word rows, 966 vs 965 words, frame 240 = retail. See cloud/work/frontier/w17e/RESULTS.md.
 *
 * Semantics (no arcade ancestor; N64-only): per-player 10-character code/name entry screen. Cursor 0..9 =
 * characters (0..25, func_800D8078 validates), 10 = three-way option byte save->opt (+0x2B), 11 = done.
 * Look-alike / duplicate characters (21~8, 22~9; 19 and 25 exempt) are flagged in D_80149DA8[p][] and
 * D_80149DD4[p]; scroll window of D_8014A10A rows; first entry copies the name from the controller-pak save
 * and calls the pause-menu setup drone_throttle_calc(p); confirm compares with func_800A1910 and marks
 * D_80139300[slot] dirty.
 *
 * Unit context needed (all in w17e/base): func_800D816C (Astra draft, NOT matched), render_replay_ui,
 * render_results_screen, func_800D8078, drone_throttle_calc (provisional bodies, all EQUAL here);
 * func_800D8154 redefined as `s32 func_800D8154(s8 player, s32 item)` (internal + inline blocker);
 * func_800DA0B4 (the caller-less stub after this function) defined below as the inlined getter
 * (internal; its own body compiles to the retail 2-word stub).
 *
 * Shaping devices: none (no unused locals, no compiled-out reads). Source choices that the score depends on
 * are listed in RESULTS.md (D_80149DD4[p] = 0 inside the clear loops, s8 D_80139300, slot2 s32,
 * `(u8 *)D_8013C068 + slot2 * 10` with a u32 size parameter for func_800A1910).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u8 pad0[16]; u8 idx; } SaveR;
typedef struct { SaveR *r; } SaveQ;
typedef struct {
    u8 pad0[8];
    SaveQ *q;          /* 0x08 */
    u8 padC[21];
    s8 name[10];       /* 0x21 */
    s8 opt;            /* 0x2B */
} Save;
typedef struct {
    u8 pad0;
    u8 slot;           /* 0x01 */
    u8 pad2[2];
    u32 buttons;       /* 0x04 */
    u8 pad8[64];
    Save **save;       /* 0x48 */
} InputRec;            /* 0x4C */
typedef struct { s32 x, y, w, h; u8 pad10[8]; } Rect;   /* 0x18 */
typedef struct { u8 pad0; s8 f1; u8 pad2[4]; s8 f6; u8 pad7[765]; } SlotInfo; /* 0x304 */

s32 *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
void drone_target_update(s32);
s32 entity_flags_apply(s32, s32, s32, u8);
s32 func_800A1910(u8 *, u8 *, u32);
void func_800B4FB0(s32);
void func_800C77C4(void *);
s32 func_800D8078(s8);
s32 func_800D8154(s8, s32);
void func_800D9058(void);
void *memcpy(void *, const void *, u32);
void render_results_screen(void);
void resource_type_select(s32);
void *sound_control(s16, s16, void *, s16);
void sound_handles_clear(s32);
void drone_throttle_calc(s8);
void func_800D816C(s8);
void render_replay_ui(s8);

extern s32 state_word_a;
extern InputRec input_rec0[4];
extern s16 D_80151AD0;
extern s8 D_8015418C[4];
extern Rect D_80113EE0[4][4];
extern s8 D_8014A10C[4];
extern s16 D_8014A100[4];
extern s16 D_8014A0F8[4];
extern s16 D_8014A10A;
extern s8 D_80149DA8[4][10];
extern s8 D_80149DD4[4];
extern s8 D_8013C068[][10];
extern Save *D_80146150[];
extern SlotInfo D_80144030[];
extern s8 D_80139300[];
extern s8 D_80157244;
extern s8 D_80113ED4;
extern void *D_80113ED8;
extern s32 D_80113EDC;
extern f32 D_80112A98, D_80112A9C, D_80112AA0, D_80112AA4, D_80112AA8, D_80112AAC, D_80112AB0, D_80112AB4;
extern u8 D_80112AB8[], D_8011361C[], D_80113910[];

s8 func_800DA0B4(u8 slot);

void func_800D91A0(void) {
    s32 done;
    s32 p;
    s32 i;
    s32 j;
    s32 count;
    s32 last;
    s8 *name;
    s8 dup;
    u8 slot;
    s32 slot2;
    Save *s;
    s8 v;

    if (D_80113ED4 == 0) {
        if (state_word_a & 0x7C03FFFE) {
            i = 1;
            for (p = 0; p < D_80151AD0; p++) {
                if (D_8015418C[p] == 1) {
                    i = 0;
                }
            }
            if (i == 1) {
                return;
            }
        } else {
            for (p = 0; p < D_80151AD0; p++) {
                ambient_sound_set(D_80113EE0[D_80151AD0 - 1][p].x, D_80113EE0[D_80151AD0 - 1][p].y,
                                  D_80113EE0[D_80151AD0 - 1][p].x + D_80113EE0[D_80151AD0 - 1][p].w,
                                  D_80113EE0[D_80151AD0 - 1][p].y + D_80113EE0[D_80151AD0 - 1][p].h, 176, 0, 0, 0);
            }
            for (p = D_80151AD0; p < 4; p++) {
                D_8014A10C[p] = 0;
            }
        }
        func_800D9058();
        if (state_word_a & 0x7C03FFFE) {
            D_80113ED8 = sound_control(0, 0, D_80112AB8, 81);
        } else if (D_80151AD0 == 1) {
            D_80113ED8 = sound_control(0, 0, D_8011361C, 21);
        } else {
            D_80113ED8 = sound_control(0, 0, D_80113910, 41);
        }
        if (D_80151AD0 == 1) {
            D_80112A98 = 1.0f;
            D_80112A9C = 0.9f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 120.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        } else if (D_80151AD0 == 2) {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        } else {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = -30.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        }
        D_80113EDC = 0;
        D_80113ED4 = 1;
    }

    for (p = 0; p < 4; p++) {
        slot = input_rec0[p].slot;
        if (slot == 5) {
            continue;
        }
        if (D_8015418C[p] == 0) {
            if ((state_word_a & 0x7C0000) && (input_rec0[p].buttons & 2)) {
                D_8015418C[p] = 1;
            }
        } else {
        last = 0;
        done = 0;
        count = 0;
        if (D_8014A10C[p] == 0) {
            D_8014A0F8[p] = 0;
            D_8014A100[p] = 0;
            for (i = 0; i < 10; i++) {
                D_80149DA8[p][i] = 0;
                D_80149DD4[p] = 0;
            }
            if (input_rec0[p].save != 0) {
                memcpy(D_8013C068[slot], (*input_rec0[p].save)->name, 10);
            } else {
                memcpy(D_8013C068[slot], D_80146150[slot]->name, 10);
            }
            if (state_word_a & 0x7C03FFFE) {
                drone_throttle_calc(p);
            }
        }
        func_800D816C(p);
        name = D_8013C068[input_rec0[p].slot];
#define BAD(c) ((D_80149DD4[p] == 1 && D_80149DA8[p][c] == 0 && (c) != 11) || !func_800D8154(p, c) || \
                (func_800DA0B4(input_rec0[p].slot) == 0 && (c) == 10))
        while (BAD(D_8014A0F8[p])) {
            D_8014A0F8[p]++;
            if (D_8014A0F8[p] >= 12) {
                D_8014A0F8[p] = 0;
            }
        }
        if (input_rec0[p].buttons & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
            do {
                D_8014A0F8[p]--;
                if (D_8014A0F8[p] < 0) {
                    D_8014A0F8[p] = 11;
                }
            } while (BAD(D_8014A0F8[p]));
        }
        if (input_rec0[p].buttons & 0x800) {
            entity_flags_apply(39, 0, 1, 0);
            do {
                D_8014A0F8[p]++;
                if (D_8014A0F8[p] >= 12) {
                    D_8014A0F8[p] = 0;
                }
            } while (BAD(D_8014A0F8[p]));
        }
        for (i = 11; i >= 0; i--) {
            if (func_800D8154(p, i)) {
                if (++count == D_8014A10A) {
                    last = i;
                    break;
                }
            }
        }
        count = 0;
        for (i = 0; i < D_8014A0F8[p]; i++) {
            if (func_800D8154(p, i)) {
                count++;
            }
        }
        if (D_8014A100[p] < count - D_8014A10A + 2) {
            D_8014A100[p] = count - D_8014A10A + 2;
        }
        if (D_8014A100[p] > count - 1) {
            D_8014A100[p] = count - 1;
        }
        if (D_8014A100[p] > last) {
            D_8014A100[p] = last;
        }
        if (D_8014A100[p] < 0) {
            D_8014A100[p] = 0;
        }
        if (D_8014A0F8[p] == 11) {
            if (input_rec0[p].buttons & 2) {
                entity_flags_apply(43, 0, 1, 0);
                func_800C77C4(name);
            }
        } else if (input_rec0[p].buttons & 0x1000) {
            entity_flags_apply(41, 0, 1, 0);
            if (D_8014A0F8[p] == 10) {
                s = *input_rec0[p].save;
                v = s->opt - 1;
                if (v < 0) {
                    v = 2;
                }
                s->opt = v;
            } else {
                do {
                    name[D_8014A0F8[p]]--;
                    if (name[D_8014A0F8[p]] < 0) {
                        name[D_8014A0F8[p]] = 25;
                    }
                } while (!func_800D8078(p));
            }
        } else if (input_rec0[p].buttons & 0x2000) {
            entity_flags_apply(44, 0, 1, 0);
            if (D_8014A0F8[p] == 10) {
                s = *input_rec0[p].save;
                v = s->opt + 1;
                if (v >= 3) {
                    v = 0;
                }
                s->opt = v;
            } else {
                do {
                    name[D_8014A0F8[p]]++;
                    if (name[D_8014A0F8[p]] >= 26) {
                        name[D_8014A0F8[p]] = 0;
                    }
                } while (!func_800D8078(p));
            }
        }
        for (i = 0; i < 10; i++) {
            D_80149DA8[p][i] = 0;
            D_80149DD4[p] = 0;
        }
        for (i = 0; i < 9; i++) {
            for (j = i + 1; j < 10; j++) {
                if (name[i] == name[j] || (name[i] == 21 && name[j] == 8) || (name[i] == 8 && name[j] == 21) ||
                    (name[i] == 22 && name[j] == 9) || (name[i] == 9 && name[j] == 22)) {
                    if (name[i] != 19 && name[i] != 25) {
                        D_80149DA8[p][i] = 1;
                        D_80149DA8[p][j] = 1;
                        D_80149DD4[p] = 1;
                    }
                }
            }
        }
        if ((state_word_a & 0x7C03FFFE) && input_rec0[p].save != 0 && (*input_rec0[p].save)->q != 0 &&
            D_80144030[(*input_rec0[p].save)->q->r->idx].f1 == 0) {
            done = 1;
        }
        if (done) {
            D_8015418C[p] = 0;
            render_replay_ui(p);
            D_80113EDC = 2;
        } else if (input_rec0[p].buttons & 5) {
            dup = 0;
            for (i = 0; i < 10; i++) {
                if (D_80149DA8[p][i] == 1) {
                    dup = 1;
                }
            }
            if (dup == 1) {
                entity_flags_apply(42, 0, 1, 0);
            } else {
                slot2 = input_rec0[p].slot;
                resource_type_select(input_rec0[p].buttons);
                if (slot2 != 5) {
                    if (input_rec0[p].save != 0) {
                        if (func_800A1910((u8 *)D_8013C068 + slot2 * 10, (u8 *)(*input_rec0[p].save)->name, 10)) {
                            D_80139300[slot2] = 1;
                        }
                    } else {
                        if (func_800A1910((u8 *)D_8013C068 + slot2 * 10, (u8 *)D_80146150[slot2]->name, 10)) {
                            D_80139300[slot2] = 1;
                        }
                    }
                }
                drone_target_update(0);
                D_8015418C[p] = 0;
                render_replay_ui(p);
                D_80113EDC = 2;
            }
        }
        D_8014A10C[p] = D_8015418C[p];
        }
    }

    for (i = 0; i < D_80151AD0; i++) {
        if (D_8015418C[i] == 1) {
            D_80113EDC = 0;
        }
    }
    if (D_80157244 != 0 || D_80113EDC != 0) {
        render_results_screen();
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            func_800B4FB0(1);
        }
    }
}

s8 func_800DA0B4(u8 slot) {
    return D_80144030[slot].f6;
}
