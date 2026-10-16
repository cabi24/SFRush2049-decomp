/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * codex_control_settings_a4 group TU (lane w33d, superseding w32d's packet): control_cell_text (0x800DC88C),
 * control_cell_text3 (0x800DC99C), func_800DCCE0 + func_800DCD58 (LOCKED bodies, verbatim m2c text),
 * control_cell_text2 (0x800DCDF4), func_800DD0C0 (0x800DD0C0) and control_settings (0x800DD4AC) -- the N64 pak
 * error dialog: control_settings finds the active cell of a row, draws the header and dispatches on the cell type.
 * STATE: unit EQUAL for all seven (blob_unit, locked bodies differing 0); see cloud/work/frontier/w33d/RESULTS.md.
 * w33d levers (control_settings, from 636/638 rows 277):
 *  - GC-style per-dialog draw/control helpers inlined into control_settings (cs_draw1/3, cs_drawT0..3, cs_wait11..13,
 *    cs_ctl10): each owns `u8 str[256]`, which reproduces retail's buffer offsets and its 24 B of inline areas; the
 *    header = two font sets with return copies (fsS_DC single-level, fsL_DC with the nested gfx lock) + an inlined
 *    GC TB_CenterText (TB_CenterText_DC; s16 return copy `move a0,t8`); pair_set_DC writes D_80118E20 directly (an
 *    inlined `Pair *p` local is one inline-area slot too many, as w30d animPlace).
 *  - control_cell_text2/text3 parameter order (row, col, text, a, b, sel) / (row, col, text, s1, s2, s3, sel1, sel2):
 *    ugen evaluates IPA register arguments in prototype order, and retail loads sel/sel1 last; the callee bodies are
 *    unchanged and stay byte-identical (their IPA colours do not depend on the order).
 *  - (strings + lang[n])[1] for the second of a strings pair (retail CSEs the row address), `C.x + C.w / 2` /
 *    `C.y + (C.h - 20) / 2 + 20` operand order (temp ring), the timeout test written on one line (as1 delay slot).
 *  - Source-line layout is load-bearing (as1 ties on line numbers): fsS_DC/fsL_DC keep `old = ...; osSendMesg(...)`
 *    on one line; TB_CenterText_DC and fsL_DC are defined after control_settings.
 * DEVICE (disclosed): one compiled-out `if (row) {}` in control_settings' column-search loop (w31d/w32d had three).
 * No unused locals. Earlier levers (w32d): TB_SetJust pair takes s32; text2/text3 separate fh/x locals declared
 * c, y, fh, x; func_800DD0C0 indexes the cell directly, declarations x, y, fh, buf, big.
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
    old = slot_state_setup(font); osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

void pair_set_DC(s32 first, s32 second)
{
    D_80118E20.first = first;
    D_80118E20.second = second;
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

void control_cell_text3(s32 row, s32 col, u8 *text, char *s1, char *s2, char *s3, s32 sel1, s32 sel2)
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

void control_cell_text2(s32 row, s32 col, s32 text, char *a, char *b, s32 sel)
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

s32 fsS_DC(s32 font)
{
    s32 old;

    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(font); osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

s16 TB_CenterText_DC(char *text, s16 centerX);
s32 fsL_DC(s32 font);
#define C (D_80153FD8[row][col])
void cs_draw1_DC(s32 row, s32 col)
{
    u8 str[256];

    font_set_DC(11);
    dispatch_handler(1);
    func_800BE6A4(str, D_8017A4E0.msg[404 / 4]);
    func_800BE4F0(str, (u8 *)D_80121070);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[408 / 4]);
    pair_set_DC(1, 1);
    camera_auto_follow(C.x + C.w / 2, C.y + (C.h - 20) / 2 + 20, C.w, C.h - 20, -1, 0, str);
    pair_set_DC(0, 3);
    font_set_DC(11);
}

void cs_draw3_DC(s32 row, s32 col)
{
    u8 str[256];

    func_800BE6A4(str, C.alt ? D_8017A4E0.msg[424 / 4] : D_8017A4E0.msg[428 / 4]);
    func_800BE4F0(str, (u8 *)D_80121074);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[432 / 4]);
    control_cell_text2(row, col, (s32)str, D_8017A4E0.strings[D_8017A4E0.lang[25]], D_8017A4E0.msg[944 / 4], C.sel1);
}

void cs_drawT0_DC(s32 row, s32 col)
{
    u8 str[256];

    func_800BE6A4(str, D_8017A4E0.msg[436 / 4]);
    func_800BE4F0(str, (u8 *)D_80121078);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[440 / 4]);
    control_cell_text(row, col, str);
}

void cs_drawT1_DC(s32 row, s32 col)
{
    u8 str[256];

    func_800BE6A4(str, D_8017A4E0.msg[444 / 4]);
    func_800BE4F0(str, (u8 *)D_80121088);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[408 / 4]);
    control_cell_text(row, col, str);
}

void cs_drawT2_DC(s32 row, s32 col)
{
    u8 str[256];

    func_800BE6A4(str, D_8017A4E0.msg[452 / 4]);
    func_800BE4F0(str, (u8 *)D_8012107C);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[456 / 4]);
    control_cell_text(row, col, str);
}

void cs_drawT3_DC(s32 row, s32 col)
{
    u8 str[256];

    func_800BE6A4(str, D_8017A4E0.msg[460 / 4]);
    func_800BE4F0(str, (u8 *)D_80121080);
    func_800BE4F0(str, (u8 *)D_8017A4E0.msg[440 / 4]);
    control_cell_text(row, col, str);
}

void cs_wait11_DC(s32 row, s32 col)
{
    if (C.timeout < D_8002E8E8.frameCount) C.done = 1;
    control_cell_text(row, col, (u8 *)D_8017A4E0.msg[476 / 4]);
}

void cs_wait12_DC(s32 row, s32 col)
{
    if (C.timeout < D_8002E8E8.frameCount) C.done = 1;
    control_cell_text(row, col, (u8 *)D_8017A4E0.msg[480 / 4]);
}

void cs_wait13_DC(s32 row, s32 col)
{
    if (C.timeout < D_8002E8E8.frameCount) C.done = 1;
    control_cell_text(row, col, (u8 *)D_8017A4E0.msg[484 / 4]);
}

void cs_ctl10_DC(Cell *c)
{
    if (c->buttons & 0x800) {
        audio_distance_atten(c->buttons);
        if (c->sel1) {
            c->sel1 = 0;
            c->sel2 = 1;
        } else if (c->sel2) {
            c->sel2 = 0;
        } else {
            c->sel1 = 1;
        }
    }
    if (c->buttons & 0x400) {
        audio_distance_atten(c->buttons);
        if (c->sel1) {
            c->sel1 = 0;
        } else if (c->sel2) {
            c->sel2 = 0;
            c->sel1 = 1;
        } else {
            c->sel2 = 1;
        }
    }
    if (c->buttons & 2) {
        entity_flags_apply(37, 0, 1, 0);
        c->done = 1;
    }
}

void control_settings(s32 row, s8 *sel1, s8 *sel2, s8 *done, s32 (*poll)(void))
{
    s32 col;
    s32 i;
    char num[72];

    if (poll != 0 && poll() != 0) {
        func_800DD45C(row);
    }
    for (col = 1; col >= 0; col--) {
        if (C.type != 0) {
            break;
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
    fsS_DC(11);
    dispatch_handler(1);
    fsL_DC(11);
    dispatch_handler(1);
    sprintf(num, D_80121060, row + 1);
    state_utility(TB_CenterText_DC(num, C.x + C.w / 2), C.y, num);
    switch (C.type) {
    case 1:
        func_800DCD58(row, col);
        cs_draw1_DC(row, col);
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
        cs_draw3_DC(row, col);
        break;
    case 4:
        func_800DCCE0(row, col);
        cs_drawT0_DC(row, col);
        break;
    case 15:
        func_800DCCE0(row, col);
        cs_drawT1_DC(row, col);
        break;
    case 5:
    case 7:
        func_800DCD58(row, col);
        control_cell_text2(row, col, (s32)D_8017A4E0.msg[444 / 4], (D_8017A4E0.strings + D_8017A4E0.lang[26])[1], D_8017A4E0.msg[944 / 4], C.sel1);
        break;
    case 8:
        func_800DCD58(row, col);
        control_cell_text2(row, col, (s32)D_8017A4E0.msg[448 / 4], D_8017A4E0.strings[D_8017A4E0.lang[24]], (D_8017A4E0.strings + D_8017A4E0.lang[24])[1], C.sel1);
        break;
    case 6:
        func_800DCD58(row, col);
        cs_drawT2_DC(row, col);
        break;
    case 9:
        func_800DCCE0(row, col);
        cs_drawT3_DC(row, col);
        break;
    case 10:
        cs_ctl10_DC(&C);
        control_cell_text3(row, col, (u8 *)D_8017A4E0.msg[472 / 4], D_8017A4E0.msg[944 / 4], D_8017A4E0.strings[D_8017A4E0.lang[26]], (D_8017A4E0.strings + D_8017A4E0.lang[26])[1], C.sel1, C.sel2);
        break;
    case 11:
        cs_wait11_DC(row, col);
        break;
    case 12:
        cs_wait12_DC(row, col);
        break;
    case 13:
        cs_wait13_DC(row, col);
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

s16 TB_CenterText_DC(char *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s32 fsL_DC(s32 font)
{
    s32 old;

    gfx_lock_DC();
    old = slot_state_setup(font); osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}
