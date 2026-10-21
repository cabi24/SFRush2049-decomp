/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * countdown_handler = _animScrollText@game.c (MAT3 GameSource/src/game.c _animScrollText, GC 0x80015A24, 364 B),
 * N64 form (0x800FBC30, 496 B), called once per frame by GameLoop (game_loop) as _DrawNoControllers' text: the
 * "controller N removed" scroller drawn in immediate mode.  Internal (private IPA: s0/s1/s2/s4/s5 unsaved); its only
 * caller is game_loop.
 * numPlayers = gRace == kGhost ? 1 : gNumPlayers; first player whose gControllerData[i].attached is clear; none ->
 * x = 320 and return.  Else the message is built from text-table pieces: sprintf(num, "%d", i + 1);
 * sprintf(str, "%s%s%s%s%s%d%s", single[124], num, single[125], num, single[126], numPlayers, single[127]);
 * func_800A4770(str, single[128]); HAL_SetFont(11) (inlined font_set_cdh); TB_SetTextColor(22) = dispatch_handler;
 * TB_DrawText(x, 8 or 205, str) = state_utility unless gGameState & 1/2 (y 8 for 0x40000000/4/8); then
 * `if (--x < -width) x = 320` with HAL_GetTextWidth = object_manager_update (N64 has no vsync gate).
 * Statics as in MAT3: `static int x = 320` (0x801146F8, own .data in game.c's TU) and `static int i`
 * (0x801613B0; MAT3 leaves it uninitialised, here a local i plus the two explicit stores, which is the form
 * uopt's promotion of the static produces).  gControllerData (D_80156CF0, 16-byte records) is declared volatile:
 * with a plain extern uopt folds the "skip the break" jump (`beqzl t7`); retail kept cfe's `bnez t7 / b exit`
 * shape (listing oracle countdown_handler/o1.s).  font_set_cdh nests gfx_lock_cdh only (frame 344; both lock and
 * unlock nested give 352, none 336).  Helpers non-static, not on keep (w17j).  Base: w27b draft (103/124).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { s32 opaque[6]; } OSMesgQueue;
typedef struct { s8 attached; u8 pad1[15]; } ControllerData;
typedef struct { void *unk0; char **single; } TextTable;
extern OSMesgQueue D_801461D0;
extern s32 gameplay_mode;
extern s16 D_8014A108;
extern volatile ControllerData D_80156CF0[];   /* gControllerData */
extern s32 state_word_a;
extern TextTable countdown_state;

extern s32 D_801613B0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 sprintf(char *, const char *, ...);
char *func_800A4770(char *, char *);
s32 slot_state_setup(s32 selection);
void dispatch_handler(s32);
void state_utility(s16, s16, char *);
s32 object_manager_update(char *, s32);

void gfx_lock_cdh(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

s32 font_set_cdh(s32 font)
{
    s32 old;

    gfx_lock_cdh();
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

void countdown_handler(void)
{
    char str[256];
    char num[8];
    s32 numPlayers;
    s32 i;
    static s32 x = 320;

    numPlayers = gameplay_mode == 2 ? 1 : D_8014A108;
    for (i = 0; i < numPlayers; i++) {
        if (!D_80156CF0[i].attached) break;
    }

    if (i >= numPlayers) {
        D_801613B0 = i;
        x = 320;
        return;
    }
    D_801613B0 = i;
    sprintf(num, "%d", i + 1);
    sprintf(str, "%s%s%s%s%s%d%s", countdown_state.single[124], num, countdown_state.single[125], num,
            countdown_state.single[126], numPlayers, countdown_state.single[127]);
    func_800A4770(str, countdown_state.single[128]);
    font_set_cdh(11);
    dispatch_handler(22);
    if (!(state_word_a & 1) && !(state_word_a & 2)) {
        if ((state_word_a & 0x40000000) || (state_word_a & 4) || (state_word_a & 8)) {
            state_utility(x, 8, str);
        } else {
            state_utility(x, 205, str);
        }
    }
    if (--x < -object_manager_update(str, -1)) {
        x = 320;
    }
}
