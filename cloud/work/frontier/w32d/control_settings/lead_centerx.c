/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Control-settings group TU (lane w31d, from w30d best.c): control_cell_text (0x800DC88C), control_cell_text3
 * (0x800DC99C), func_800DCCE0 + func_800DCD58 (LOCKED bodies of codex_control_settings_a4, verbatim m2c text),
 * control_cell_text2 (0x800DCDF4), func_800DD0C0 (0x800DD0C0, NEW draft), control_settings (0x800DD4AC).
 * STATE: draft, NOT a match. Unit (run.sh): control_settings 634/638 body, rows 280, ops 82, FRAME 1896/1896;
 * control_cell_text EQUAL; text2 rows 17; text3 rows 35; func_800DD0C0 rows 22, ops 10, frame 440/440;
 * func_800DCD58/func_800DCCE0 EQUAL; locked bodies differing 0.
 * draft_check (cs1 form): EQUIVALENT over 90 calls / ghost_delete_fail (the w29c/w30d fault came from compiling
 * without func_800DCD58's body: retail DCD58 clobbers s0/s1 unsaved).
 * DEVICES (disclosed, oracle-grade, NOT a claim): compiled-out `if (row) {}` TWICE and `if (col) {}` once in the
 * column-search loop. Traced (ctrace): row's whole web totalsave 25 (35 with one holder) vs s3 cost 37.3 (base
 * 33.1 = nblocks/4 + 4.2 slot_state_setup clobber penalty); col 55 vs s4 59.1. Each loop use adds 10. Retail
 * colours both whole webs (row s3, col s4) - the natural source of their loop weight is still unknown.
 * Frame: one nested font_set_DC site (the first FONT_SET; each inlined call costs 8 B in this frame: font_set +
 * gfx_lock + gfx_unlock = 24 = retail 1896-1872). The other three FONT_SET sites are direct statements, so they
 * lack retail's `move s0,v0` (inlined return copy) - the shape that gives all four sites the copy within 24 B is
 * unknown (see RESULTS.md).
 */
/*
 * control_settings (0x800DD4AC, 2,552 B) + its three IPA callees control_cell_text (0x800DC88C),
 * control_cell_text3 (0x800DC99C), control_cell_text2 (0x800DCDF4). Lane w30d draft (from w29c best.c).
 * STATE: draft, NOT a match. Unit (run.sh): control_settings 623/638 words, rows 283 (was 663), ops 135 (was 164),
 * body 629/638, frame 1872/1896; control_cell_text EQUAL; text2 rows 17; text3 rows 35.
 * w30d changes vs w29c: (1) FONT_SET written as three plain statements (no do {} while (0): each costs uopt 2
 * blocks, base callee-saved cost = nblocks/4: 34.0 -> 32.0); (2) the two TB_SetJust pair stores in case 1 written
 * directly (no inlined pair_set: -32 B inline area, frame 1904 -> 1872); (3) DEVICES (disclosed, oracle-grade):
 * compiled-out `if (row) {}` and `if (col) {}` in the column-search loop. Traced cause (ctrace): col's whole web
 * totalsave 55 vs s4 cost 56.2 (= base 32 + 24.2 save/reload penalty around the s4-clobbering IPA handler calls),
 * row's 25 vs s3 cost 36.4 -> both split. One loop use adds 10 (loop weight): col 65 > 56.7 -> s4 as retail;
 * row 35 < 36.9 (still split, 2 short). Retail must give row/col loop-weighted uses (or have ~8 fewer uopt blocks).
 * Remaining: row in memory (needs +2 totalsave), the inlined font_set return copies (retail `move s0,v0` at all
 * four sites, i.e. an inlined helper, but neither the one-piece wrapper (not inlined at >1 site) nor the nested
 * gfx_lock/gfx_unlock form (+24 B per site, frame 1968) fits retail's frame 1872+24), func_800DD0C0's (row, col)
 * stack-argument convention (retail `sw s3,0(sp); sw s4,4(sp)`: DD0C0 is IPA with memory params; ours calls the
 * asm body o32 a0/a1 -> needs DD0C0 as C in the same unit).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { s32 first, second; } Pair;
typedef struct {
    s8 done;            /* 0 */
    s8 sel1;            /* 1 */
    s8 sel2;            /* 2 */
    u8 pad3;
    s8 state[4];        /* 4 */
    s8 mode[4];         /* 8 */
    u8 pad0C[4];
    s8 alt;             /* 0x10 */
    u8 pad11[3];
    s32 type;           /* 0x14 */
    s16 x, y, w, h;     /* 0x18..0x1E */
    s32 buttons;        /* 0x20 */
    s32 held;           /* 0x24 */
    u32 timeout;        /* 0x28 */
} Cell;                 /* 44 bytes */
typedef struct {
    u8 pad0[4];
    char **msg;         /* +4 */
    u8 pad8[4];
    u16 *lang;          /* +12 */
    char **strings;     /* +16 */
} StrTable;
extern OSMesgQueue D_801461D0;
extern Pair D_80118E20;
extern Cell D_80153FD8[][2];
extern StrTable D_8017A4E0;
extern s32 state_word_a;
extern s32 D_8015694C;
extern s32 D_80156944;
typedef struct Sched { u8 pad0[0x27C]; volatile u32 frameCount; } Sched; /* gScheduler; display_settings.c */
extern Sched D_8002E8E8;
extern char D_80121060[];
extern char D_80121070[];
extern char D_80121074[];
extern char D_80121078[];
extern char D_8012107C[];
extern char D_80121080[];
extern char D_80121088[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 sprintf(char *, const char *, ...);
extern s32 slot_state_setup(s32 selection);
extern void dispatch_handler(s32);
extern s32 object_bytes_sum_global(void);
extern u32 object_manager_update(void *, s32);
extern void state_utility(s16, s16, char *);
extern void camera_auto_follow(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, u8 *arg6);
extern u8 *func_800BE6A4(u8 *arg0, void *arg1);
extern u8 *func_800BE4F0(u8 *arg0, u8 *arg1);
extern void audio_doppler(s32 arg0);
extern void resource_type_select(s32 arg0);
extern void func_800DD45C(s32 arg0);
extern void audio_distance_atten(s32 arg0);
extern s32 entity_flags_apply(s32 arg0, s32 arg1, s32 arg2, u8 arg3);
extern void player_state_set(s32, s32);
extern void player_mode_set(s32, s32);
extern void process_inputs(void);

void gfx_lock_DC(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_DC(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_DC(s32 font)
{
    s32 old;

    gfx_lock_DC();
    old = slot_state_setup(font);
    gfx_unlock_DC();
    return old;
}

s32 font_set1_DC(s32 font)
{
    s32 old;

    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

void pair_set_DC(s32 first, s32 second)
{
    Pair *p = &D_80118E20;

    p->first = first;
    p->second = second;
}

void control_cell_text(s32 row, s32 col, u8 *text)
{
    Cell *c;

    font_set1_DC(11);
    dispatch_handler(1);
    c = &D_80153FD8[row][col];
    pair_set_DC(1, 1);
    camera_auto_follow(c->w / 2 + c->x, c->h / 2 + c->y, c->w, c->h, -1, 0, text);
    pair_set_DC(0, 3);
}

void control_cell_text3(s32 sel1, s32 row, u8 *text, char *s1, char *s2, char *s3, s32 col, s32 sel2)
{
    Cell *c;
    s32 y;
    s32 fh;
    s32 x;

    pair_set_DC(1, 1);
    font_set_DC(11);
    dispatch_handler(1);
    c = &D_80153FD8[row][col];
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, text);
    fh = object_bytes_sum_global();
    x = c->w / 2 + c->x;
    y = c->h + c->y - fh * 3;
    font_set_DC(sel1 ? 10 : 11);
    dispatch_handler(sel1 ? 22 : 1);
    state_utility(x, y - (sel1 ? 1 : 0), s1);
    font_set_DC(sel2 ? 10 : 11);
    dispatch_handler(sel2 ? 22 : 1);
    state_utility(x, y - (sel2 ? 1 : 0) + fh, s2);
    font_set_DC((sel1 || sel2) ? 11 : 10);
    dispatch_handler((sel1 || sel2) ? 1 : 22);
    state_utility(x, y - ((sel1 || sel2) ? 0 : 1) + fh * 2, s3);
    pair_set_DC(0, 3);
}

void func_800DCCE0(s32 arg1, s32 arg2)
{
  u8 *p;
  s32 v;
  p = ((u8 *) &D_80153FD8) + arg1 * 0x58 + arg2 * 0x2C;
  v = *((s32 *) (p + 0x20));
  if ((v & 7) != 0)
  {
    resource_type_select(v);
    *p = 1;
  }
}

void func_800DCD58(s32 arg0, s32 arg1)
{
  s32 temp_s0;
  void *temp_s1;
  temp_s1 = (s32 *) (((arg0 * 0x58) + (arg1 * 0x2C)) + ((u8 *) (&D_80153FD8)));
  temp_s0 = *((s32 *) (((s8 *) temp_s1) + 0x20));
  if (temp_s0 & 0xC00)
  {
    audio_doppler(*((s32 *) (((s8 *) temp_s1) + 0x20)));
    *((s8 *) (((s8 *) temp_s1) + 1)) = (s8)!(*((s8 *) (((s8 *) temp_s1) + 1)));
  }
  if ((*((s32 *) (((s8 *) temp_s1) + 0x20))) & 2)
  {
    entity_flags_apply(0x25, 0, 1, 0U);
    *((s8 *) (((s8 *) temp_s1) + 0)) = 1;
  }
}

void control_cell_text2(s32 sel, s32 row, s32 col, char *a, char *b, s32 text)
{
    Cell *c;
    s32 y;
    s32 fh;
    s32 x;

    dispatch_handler(1);
    font_set_DC(11);
    c = &D_80153FD8[row][col];
    pair_set_DC(1, 1);
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, (u8 *)text);
    fh = object_bytes_sum_global();
    x = c->w / 2 + c->x;
    y = c->h + c->y - fh * 3;
    font_set_DC(sel ? 11 : 10);
    dispatch_handler(sel ? 1 : 22);
    state_utility(x, y - (sel ? 0 : 1) + fh, D_8017A4E0.msg[944 / 4]);
    font_set_DC(sel ? 10 : 11);
    dispatch_handler(sel ? 22 : 1);
    state_utility(x, y - (sel ? 1 : 0), D_8017A4E0.strings[D_8017A4E0.lang[25]]);
    pair_set_DC(0, 3);
}

void func_800DD0C0(s32 row, s32 col)
{
    s32 x;
    s32 y;
    s32 fh;
    u8 buf[256];
    s32 big;

    big = !(state_word_a & 0x7C03FFFE);
    font_set_DC(11);
    dispatch_handler(1);
    func_800BE6A4(buf, D_8017A4E0.msg[416 / 4]);
    func_800BE4F0(buf, (u8 *)(big ? D_8017A4E0.msg[412 / 4] : D_8017A4E0.msg[408 / 4]));
    pair_set_DC(1, 1);
    camera_auto_follow(D_80153FD8[row][col].x + D_80153FD8[row][col].w / 2, D_80153FD8[row][col].y + (D_80153FD8[row][col].h - (big ? 40 : 20)) / 2 + 20, D_80153FD8[row][col].w, D_80153FD8[row][col].h - (big ? 40 : 20),
                       -1, 0, buf);
    if (big) {
        fh = object_bytes_sum_global();
        x = D_80153FD8[row][col].x + D_80153FD8[row][col].w / 2;
        y = D_80153FD8[row][col].y + D_80153FD8[row][col].h - fh * 3;
        font_set_DC(D_80153FD8[row][col].sel1 ? 11 : 10);
        dispatch_handler(D_80153FD8[row][col].sel1 ? 1 : 22);
        state_utility(x, y - (D_80153FD8[row][col].sel1 ? 0 : 1) + fh, D_8017A4E0.msg[944 / 4]);
        font_set_DC(D_80153FD8[row][col].sel1 ? 10 : 11);
        dispatch_handler(D_80153FD8[row][col].sel1 ? 22 : 1);
        state_utility(x, y - (D_80153FD8[row][col].sel1 ? 1 : 0), D_8017A4E0.strings[D_8017A4E0.lang[25]]);
    }
    pair_set_DC(0, 3);
    font_set_DC(11);
}

#define C (D_80153FD8[row][col])
#define FONT_SET(f) osRecvMesg(&D_801461D0, (void *)0, 1); slot_state_setup(f); osSendMesg(&D_801461D0, (void *)0, 0)
void control_settings(s32 row, s8 *sel1, s8 *sel2, s8 *done, s32 (*poll)(void))
{
    s32 col;
    char num[76];
    u8 t1[296];
    u8 t3[328];
    u8 t4[272];
    u8 t15[264];
    u8 t6[264];
    u8 t9[264];
    s32 i;

    if (poll != 0 && poll() != 0) {
        func_800DD45C(row);
    }
    for (col = 1; col >= 0; col--) {
        if (C.type != 0) {
            break;
        }
        if (row) {
        }
        if (col) {
        }
        if (row) {
        }
        if (C.done == 0) {
            C.done = 1;
            *done = 1;
            *sel2 = 0;
            *sel1 = 0;
            goto apply;
        }
    }
    if (col < 0) {
        return;
    }
    C.buttons = D_8015694C;
    C.held = D_80156944;
    font_set_DC(11);
    dispatch_handler(1);
    FONT_SET(11);
    dispatch_handler(1);
    sprintf(num, D_80121060, row + 1);
    i = (s16)(C.w / 2 + C.x);
    state_utility(i - (object_manager_update(num, -1) >> 1), C.y, num);
    switch (C.type) {
    case 1:
        func_800DCD58(row, col);
        FONT_SET(11);
        dispatch_handler(1);
        func_800BE6A4(t1, D_8017A4E0.msg[404 / 4]);
        func_800BE4F0(t1, (u8 *)D_80121070);
        func_800BE4F0(t1, (u8 *)D_8017A4E0.msg[408 / 4]);
        D_80118E20.first = 1; D_80118E20.second = 1;
        camera_auto_follow(C.w / 2 + C.x, (C.h - 20) / 2 + C.y + 20, C.w, C.h - 20, -1, 0, t1);
        D_80118E20.first = 0; D_80118E20.second = 3;
        FONT_SET(11);
        break;
    case 2:
        if (!(state_word_a & 0x7C03FFFE)) {
            func_800DCD58(row, col);
        } else {
            func_800DCCE0(row, col);
        }
        func_800DD0C0(row, col);
        break;
    case 3:
        func_800DCD58(row, col);
        func_800BE6A4(t3, C.alt ? D_8017A4E0.msg[424 / 4] : D_8017A4E0.msg[428 / 4]);
        func_800BE4F0(t3, (u8 *)D_80121074);
        func_800BE4F0(t3, (u8 *)D_8017A4E0.msg[432 / 4]);
        control_cell_text2(C.sel1, row, col, D_8017A4E0.strings[D_8017A4E0.lang[25]],
                              D_8017A4E0.msg[944 / 4], (s32)t3);
        break;
    case 4:
        func_800DCCE0(row, col);
        func_800BE6A4(t4, D_8017A4E0.msg[436 / 4]);
        func_800BE4F0(t4, (u8 *)D_80121078);
        func_800BE4F0(t4, (u8 *)D_8017A4E0.msg[440 / 4]);
        control_cell_text(row, col, t4);
        break;
    case 15:
        func_800DCCE0(row, col);
        func_800BE6A4(t15, D_8017A4E0.msg[444 / 4]);
        func_800BE4F0(t15, (u8 *)D_80121088);
        func_800BE4F0(t15, (u8 *)D_8017A4E0.msg[408 / 4]);
        control_cell_text(row, col, t15);
        break;
    case 5:
    case 7:
        func_800DCD58(row, col);
        control_cell_text2(C.sel1, row, col, D_8017A4E0.strings[D_8017A4E0.lang[26] + 1],
                              D_8017A4E0.msg[944 / 4], (s32)D_8017A4E0.msg[444 / 4]);
        break;
    case 8:
        func_800DCD58(row, col);
        control_cell_text2(C.sel1, row, col, D_8017A4E0.strings[D_8017A4E0.lang[24]],
                              D_8017A4E0.strings[D_8017A4E0.lang[24] + 1], (s32)D_8017A4E0.msg[448 / 4]);
        break;
    case 6:
        func_800DCD58(row, col);
        func_800BE6A4(t6, D_8017A4E0.msg[452 / 4]);
        func_800BE4F0(t6, (u8 *)D_8012107C);
        func_800BE4F0(t6, (u8 *)D_8017A4E0.msg[456 / 4]);
        control_cell_text(row, col, t6);
        break;
    case 9:
        func_800DCCE0(row, col);
        func_800BE6A4(t9, D_8017A4E0.msg[460 / 4]);
        func_800BE4F0(t9, (u8 *)D_80121080);
        func_800BE4F0(t9, (u8 *)D_8017A4E0.msg[440 / 4]);
        control_cell_text(row, col, t9);
        break;
    case 10:
        if (C.buttons & 0x800) {
            audio_distance_atten(C.buttons);
            if (C.sel1) {
                C.sel1 = 0;
                C.sel2 = 1;
            } else if (C.sel2) {
                C.sel2 = 0;
            } else {
                C.sel1 = 1;
            }
        }
        if (C.buttons & 0x400) {
            audio_distance_atten(C.buttons);
            if (C.sel1) {
                C.sel1 = 0;
            } else if (C.sel2) {
                C.sel2 = 0;
                C.sel1 = 1;
            } else {
                C.sel2 = 1;
            }
        }
        if (C.buttons & 2) {
            entity_flags_apply(37, 0, 1, 0);
            C.done = 1;
        }
        control_cell_text3(C.sel1, row, (u8 *)D_8017A4E0.msg[472 / 4], D_8017A4E0.msg[944 / 4],
                             D_8017A4E0.strings[D_8017A4E0.lang[26]], D_8017A4E0.strings[D_8017A4E0.lang[26] + 1],
                             col, C.sel2);
        break;
    case 11:
        if (C.timeout < D_8002E8E8.frameCount) {
            C.done = 1;
        }
        control_cell_text(row, col, (u8 *)D_8017A4E0.msg[476 / 4]);
        break;
    case 12:
        if (C.timeout < D_8002E8E8.frameCount) {
            C.done = 1;
        }
        control_cell_text(row, col, (u8 *)D_8017A4E0.msg[480 / 4]);
        break;
    case 13:
        if (C.timeout < D_8002E8E8.frameCount) {
            C.done = 1;
        }
        control_cell_text(row, col, (u8 *)D_8017A4E0.msg[484 / 4]);
        break;
    case 14:
        C.done = 1;
        break;
    }
    *done = C.done;
    if (C.done) {
        C.type = 0;
        *sel1 = C.sel1;
        *sel2 = C.sel2;
apply:
        for (i = 0; i < 4; i++) {
            player_state_set(i, C.state[i]);
            player_mode_set(i, C.mode[i]);
        }
        process_inputs();
    }
}
