/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * countdown_handler (0x800FBC30, 496 B; internal in the unit: s0-s5 unsaved). "Waiting for player N"
 * screen: finds the first not-ready player slot (D_80156CF0[i].ready, 16-byte records; one player in
 * gameplay mode 2, else active_player_count); all ready -> D_801613B0 = count, timer D_801146F8 = 320.
 * Otherwise formats "N" and the language strings (D_8017A4E0.strings +496..+512) into a 256-byte text,
 * selects font 11 (font_set = credits_scroll_grp gr3_c.c's inlined static around slot_state_setup),
 * dispatch_handler(22), draws via state_utility(timer, 8 or 205, text) unless state_word_a bit 0/1, then
 * scrolls the timer: --timer < -object_manager_update(text, -1) -> 320.
 * STATE: draft. unit (--internal countdown_handler) 103/124 words (rows words 80 ops 15 norm 15), frame 344/336.
 * Residual class: shaping. Retail keeps no register for &D_801146F8 (ours CSEs it into s1), the ready-scan
 * loop's bnezl/b shape differs, frame 8 bytes short (one inline level missing; font_set with nested
 * gfx_lock/gfx_unlock statics gives 352). One variable (n) carries count, font_set's result and the timer.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { s32 opaque[6]; } OSMesgQueue;
typedef struct { s8 ready; u8 other1[15]; } Slot16;
typedef struct { u8 other0[496]; char *s496; char *s500; char *s504; char *s508; char *s512; } Strings;
typedef struct { u8 other0[4]; Strings *strings; } Lang;
extern OSMesgQueue D_801461D0;
extern s32 gameplay_mode;
extern s16 active_player_count;
extern Slot16 D_80156CF0[];
extern s32 D_801613B0;
extern s32 D_801146F8;
extern s32 state_word_a;
extern Lang D_8017A4E0;
extern char D_80120308[], D_8012030C[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 sprintf(char *, const char *, ...);
extern u8 *func_800A4770(u8 *arg0, u8 *arg1);
extern s32 slot_state_setup(s32 selection);
extern void dispatch_handler(s32);
extern void state_utility(s16, s32, void *);
extern s32 object_manager_update(void *, s32);

static s32 font_set(s32 font)
{
    s32 old;

    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

void countdown_handler(void)
{
    char text[256];
    char num[8];
    s32 n;
    s32 i;

    if (gameplay_mode == 2) {
        n = 1;
    } else {
        n = active_player_count;
    }
    for (i = 0; i < n; i++) {
        if (!D_80156CF0[i].ready) break;
    }
    if (i >= n) {
        D_801613B0 = i;
        n = 320;
        D_801146F8 = n;
        return;
    }
    D_801613B0 = i;
    sprintf(num, D_80120308, i + 1);
    sprintf(text, D_8012030C, D_8017A4E0.strings->s496, num, D_8017A4E0.strings->s500, num,
            D_8017A4E0.strings->s504, n, D_8017A4E0.strings->s508);
    func_800A4770((u8 *)text, (u8 *)D_8017A4E0.strings->s512);
    n = font_set(11);
    dispatch_handler(22);
    if (!(state_word_a & 1) && !(state_word_a & 2)) {
        if ((state_word_a & 0x40000000) || (state_word_a & 4) || (state_word_a & 8)) {
            n = D_801146F8;
            state_utility(n, 8, text);
        } else {
            n = D_801146F8;
            state_utility(n, 205, text);
        }
    }
    n = D_801146F8;
    if (--n < -object_manager_update(text, -1)) {
        n = 320;
    }
    D_801146F8 = n;
}
