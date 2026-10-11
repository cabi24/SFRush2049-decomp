/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w16f group frontier_pause_quit -- PROVISIONAL (stand-in callers), NOT installable as a match.
 *
 * drone_throttle_calc (0x800D8C58, 256 w) is the alias `pause_quit`: the pause-menu set-up for one player
 * (w6d semantics, cloud/work/frontier/w6d/RESULTS.md). It creates the missing models of the player's 17
 * menu items, sets the textures of items 13-15 ("BUTTON_TITLE", "CURSOR", "B1"), positions items 13/15/16
 * and flags D_80154618/D_80154630[player].
 *
 * Its only caller, func_800D91A0 (3,860 B), is unmatched. IPA gives drone_throttle_calc a private
 * convention (no callee saves, the s8 player read from the caller's outgoing slot, `lb 115(sp)`), which
 * needs a C caller in the same unit. zz_caller/zz_caller2 below are STAND-INS that reproduce it (two
 * callers stop umerge inlining). In the whole-program unit without them the function is either kept
 * (standard convention, 251/256 words differ) or internal with no caller (deleted, 2-word stub).
 * The group lands when func_800D91A0 joins it and the stand-ins are removed.
 *
 * Code: identical to retail (score.py group: only the 20 own-.rodata relocations unverified; w15j gate
 * dry run: 0 word diffs, own .rodata verified in two retail regions, see w16f/RESULTS.md).
 *
 * Disclosed shaping (owner review):
 * 1. Unused locals that are the exact frame residual: `k`, `u0`, `u1`, `u2` (4 slots, 16 B). Retail
 *    frame 112; without them 96 (9 words differ: frame and the home slots). They replace w6d's
 *    `char pad[12]` (pad arrays are not acceptable); w16f measured both forms code-identical.
 * 2. `if (player < 0) {}` compiled-out read at the top (w6d): gives `color` a longer live range so it is
 *    spilled to sp+108 as in retail.
 * 3. `scale = 1.0f` held in a local: retail multiplies the stored positions by 1.0 (f20); a literal folds.
 * 4. `s16 none = -1` held in a variable gives retail's sll/sra of the parent argument.
 * Prototypes follow the locked definitions (src/blob/func_8008E26C.c etc.); func_800B24EC's NameEntry *
 * return is declared u32 as in the locked groups.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    u8 pad8[4];
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 pad3C[4];
} Item; /* 0x40 */
typedef struct {
    Item item[17];
} Menu; /* 0x440 */
typedef struct {
    u8 pad0[0x3C];
    s32 color;           /* 0x3C */
    u8 pad40[4];
} Node;

extern s32 D_801140EC;
extern Menu D_80111998[];
extern char *D_8011196C[];
extern f32 D_8011418C[];
extern volatile u8 D_80140BDC;
extern s16 D_80151AD0;
extern Node D_8012E700[];
extern s32 D_80154618[];
extern s32 D_80154630[];

void math_utility(void *dst, void *src);
s32 string_copy_format(char *name, s8 first, s8 last, s8 complain);
s32 func_8008E26C(s32 name, void *mat, s16 parent, s32 flags);
void model_data_load(s32 a, s32 mode, u32 v);
void model_transform_setup(s32 a, s32 mode, u32 v);
u32 func_800B24EC(char *name, s16 *out, s8 lo, s8 hi, s32 err);
void func_8008D870(s16 handle, s32 tex, s32 c);
void func_800B5898(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void particle_velocity_set(void);


void drone_throttle_calc(s8 player) {
    s32 color;
    s32 k;
    s16 tex;
    s32 u0;
    s32 u1;
    s32 u2;
    s32 i;
    s32 mask;
    s16 none;
    Item *e;
    s32 h;
    f32 scale;

    color = D_801140EC;
    scale = 1.0f;
    none = -1;
    if (player < 0) {}
    for (i = 0; i < 17; i++) {
        e = &D_80111998[player].item[i];
        if (e->handle == none) {
            mask = 1 << player;
            math_utility(D_8011418C, e->mat);
            h = func_8008E26C(string_copy_format(D_8011196C[e->type], 0, (s8)(D_80140BDC - 1), 1), e->mat, none,
                                      (e->type == 0 || e->type == 1 || e->type == 2 || e->type == 9 || e->type == 10) ? 0x42000 : 0);
            e->handle = h;
            D_8012E700[h].color = color;
            model_data_load(h, 1, 15);
            model_transform_setup(e->handle, 0, mask);
        }
    }
    func_8008D870(D_80111998[player].item[13].handle, func_800B24EC("BUTTON_TITLE", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    math_utility(D_8011418C, D_80111998[player].item[13].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[13].mat);
    D_80111998[player].item[13].pos[0] = 0.0f;
    if (D_80151AD0 >= 2)
        D_80111998[player].item[13].pos[1] = 105.0f;
    else
        D_80111998[player].item[13].pos[1] = 115.0f;
    D_80111998[player].item[13].pos[2] = 200.0f;
    D_80111998[player].item[13].pos[0] *= scale;
    D_80111998[player].item[13].pos[1] *= scale;
    D_80111998[player].item[13].pos[2] *= scale;
    func_8008D870(D_80111998[player].item[14].handle, func_800B24EC("CURSOR", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    func_8008D870(D_80111998[player].item[15].handle, func_800B24EC("B1", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    math_utility(D_8011418C, D_80111998[player].item[15].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[15].mat);
    D_80111998[player].item[15].pos[0] = -75.1f;
    if (D_80151AD0 >= 2)
        D_80111998[player].item[15].pos[1] = 50.2f;
    else
        D_80111998[player].item[15].pos[1] = 55.2f;
    D_80111998[player].item[15].pos[2] = 100.0f;
    D_80111998[player].item[15].pos[0] *= scale;
    D_80111998[player].item[15].pos[1] *= scale;
    D_80111998[player].item[15].pos[2] *= scale;
    func_8008B32C(D_80111998[player].item[15].mat, D_80111998[player].item[15].mat, 0.5f);
    math_utility(D_8011418C, D_80111998[player].item[16].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[16].mat);
    D_80111998[player].item[16].pos[0] = -87.1f;
    if (D_80151AD0 >= 2)
        D_80111998[player].item[16].pos[1] = 50.2f;
    else
        D_80111998[player].item[16].pos[1] = 55.2f;
    D_80111998[player].item[16].pos[2] = 100.0f;
    D_80111998[player].item[16].pos[0] *= scale;
    D_80111998[player].item[16].pos[1] *= scale;
    D_80111998[player].item[16].pos[2] *= scale;
    D_80154618[player] = 1;
    D_80154630[player] = 1;
    particle_velocity_set();
}

/* STAND-IN callers (not retail): the real caller is func_800D91A0. */
extern s8 D_zz;
