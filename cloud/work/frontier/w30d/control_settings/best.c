/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
typedef struct { u32 first, second; } Pair;
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
extern u32 game_loop_tick;
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
extern void func_800DCD58(s32 arg0, s32 arg1);
extern void func_800DCCE0(s32 arg1, s32 arg2);
extern void func_800DD0C0(s32 arg0, s32 arg1);
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

void pair_set_DC(u32 first, u32 second)
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

    pair_set_DC(1, 1);
    font_set_DC(11);
    dispatch_handler(1);
    c = &D_80153FD8[row][col];
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, text);
    fh = object_bytes_sum_global();
    row = c->w / 2 + c->x;
    y = c->h + c->y - fh * 3;
    font_set_DC(sel1 ? 10 : 11);
    dispatch_handler(sel1 ? 22 : 1);
    state_utility(row, y - (sel1 ? 1 : 0), s1);
    font_set_DC(sel2 ? 10 : 11);
    dispatch_handler(sel2 ? 22 : 1);
    state_utility(row, y - (sel2 ? 1 : 0) + fh, s2);
    font_set_DC((sel1 || sel2) ? 11 : 10);
    dispatch_handler((sel1 || sel2) ? 1 : 22);
    state_utility(row, y - ((sel1 || sel2) ? 0 : 1) + fh * 2, s3);
    pair_set_DC(0, 3);
}

void control_cell_text2(s32 sel, s32 row, s32 col, char *a, char *b, s32 text)
{
    Cell *c;
    s32 y;

    dispatch_handler(1);
    font_set_DC(11);
    c = &D_80153FD8[row][col];
    pair_set_DC(1, 1);
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, (u8 *)text);
    col = object_bytes_sum_global();
    text = c->w / 2 + c->x;
    y = c->h + c->y - col * 3;
    font_set_DC(sel ? 11 : 10);
    dispatch_handler(sel ? 1 : 22);
    state_utility(text, y - (sel ? 0 : 1) + col, D_8017A4E0.msg[944 / 4]);
    font_set_DC(sel ? 10 : 11);
    dispatch_handler(sel ? 22 : 1);
    state_utility(text, y - (sel ? 1 : 0), D_8017A4E0.strings[D_8017A4E0.lang[25]]);
    pair_set_DC(0, 3);
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
    FONT_SET(11);
    dispatch_handler(1);
    FONT_SET(11);
    dispatch_handler(1);
    sprintf(num, D_80121060, row + 1);
    state_utility((s16)(C.w / 2 + C.x) - (object_manager_update(num, -1) >> 1), C.y, num);
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
        if (C.timeout < game_loop_tick) {
            C.done = 1;
        }
        control_cell_text(row, col, (u8 *)D_8017A4E0.msg[476 / 4]);
        break;
    case 12:
        if (C.timeout < game_loop_tick) {
            C.done = 1;
        }
        control_cell_text(row, col, (u8 *)D_8017A4E0.msg[480 / 4]);
        break;
    case 13:
        if (C.timeout < game_loop_tick) {
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
