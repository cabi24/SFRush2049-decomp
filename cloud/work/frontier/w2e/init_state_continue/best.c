/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH (provisional lane): 89/178 words strict, 50 aligned rows,
 * scored as a group with keep=standin_caller (gfull.sh / grp/). Real callers display_list_flush
 * (0x800FB2C8) and countdown (0x800FBF88) are unmatched, so it cannot be more than provisional.
 *
 * init_state_continue (0x800FAF6C, frameless, writes s0-s8 unsaved): fill the six 8-byte race slot
 * configs at D_80153E88: the extra-player count is min(D_80142724, 6 - D_8014A108), zero in modes
 * 4/5/6; D_801543CA = D_8014A108 + extra; mode 3 with D_80154628 > 0 takes player ids from
 * D_80154450[] (76-byte records, slot 0/1 swapped by D_801543D4), human slots get kind 6, others
 * kind 0 with either a sequential id or (mode 3) a random id not already used; empty slots kind 7.
 * frand/func_8008B2B4 (LCG 1103515245/12345) are inlined.
 *
 * Moved: the mode 4/5/6 test as a switch (the if-chain makes uopt keep 6 in ra and gives the
 * function a frame: 181 words); the player-index ternary written inline in the subscript.
 * Residual: `bnel v0,a0` operand order of the case-6 test (if-chain gives retail's a0,v0 but the ra
 * frame); the random-retry loop rotation (retail tests prior==slot at the bottom and branches back;
 * a while(1)/for(;;) form gets that shape but shifts t-registers: 90 rows); temp numbering in the
 * frand block follows from those.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Config {
    /* 0 */ s8 type;
    /* 1 */ u8 pad1[4];
    /* 5 */ u8 selection;
    /* 6 */ u8 flags;
    /* 7 */ u8 kind;
} Config;

typedef struct Player {
    /* 0 */ u8 pad0;
    /* 1 */ u8 id;
    /* 2 */ u8 pad2[74];
} Player; /* 0x4C */

extern s16 D_8014A108;
extern s16 D_80142724;
extern s16 D_801543CA;
extern s32 D_8014A110;
extern s8 D_80154628;
extern s8 D_8014978C;
extern u8 D_801543D4;
extern Player D_80154450[];
extern Config D_80153E88[];
extern int D_8011735C;

int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

f32 frand(f32 range) {
    return func_8008B2B4() * range / 32768.0f;
}
s32 pick(s32 slot) {
    if (slot == 0) {
        return D_801543D4;
    }
    return (slot == 1) ? D_801543D4 ^ 1 : slot;
}
void init_state_continue(void)
{
    s32 extra;
    s32 slot;
    s32 selected;
    s32 prior;
    u8 next = 0;
    Config *config;

    extra = D_80142724;
    if (6 - D_8014A108 < extra) {
        extra = 6 - D_8014A108;
    }
    switch (D_8014A110) {
    case 4:
    case 5:
    case 6:
        extra = 0;
        break;
    }
    D_801543CA = D_8014A108 + extra;
    config = D_80153E88;
    for (slot = 0; slot < 6; slot++, config++) {
        if (D_8014A110 == 3 && slot < D_801543CA && D_80154628 > 0) {
            config->selection = D_80154450[(slot == 0) ? D_801543D4 : ((slot == 1) ? D_801543D4 ^ 1 : slot)].id;
            config->flags = 176;
            if (slot < D_8014A108) {
                config->kind = 6;
            } else {
                config->kind = 0;
            }
        } else if (slot < D_8014A108) {
            config->selection = extra++;
            config->kind = 6;
            config->flags = 176;
        } else if (slot < D_801543CA) {
            if (D_8014A110 == 3) {
                do {
                    config->selection = frand(D_801543CA);
                    for (prior = 0; prior < slot; prior++) {
                        if (config->selection == D_80153E88[prior].selection) {
                            break;
                        }
                    }
                } while (prior != slot);
            } else {
                config->selection = next++;
            }
            config->kind = 0;
            config->flags = 176;
        } else {
            config->kind = 7;
            config->flags = 0;
        }
        config->type = D_8014978C;
    }
}

void standin_caller(s32 a)
{
    if (a) {
        init_state_continue();
    }
    init_state_continue();
}
