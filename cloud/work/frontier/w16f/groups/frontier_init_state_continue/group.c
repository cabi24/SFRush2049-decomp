/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w16f group frontier_init_state_continue -- PROVISIONAL (stand-in caller), NOT installable as a match.
 *
 * init_state_continue (0x800FAF6C, 178 w, frameless, writes s0-s8 unsaved: IPA-internal): fill the six
 * 8-byte race slot configs at D_80153E88. The extra-player count is min(D_80142724, 6 - D_8014A108), zero in
 * modes 4/5/6; D_801543CA = D_8014A108 + extra. Mode 3 with D_80154628 > 0 takes player ids from D_80154450[]
 * (76-byte records, slots 0/1 swapped by D_801543D4); human slots get kind 6, others kind 0 with either a
 * sequential id or (mode 3) a random id not already used (retry until unique); empty slots kind 7.
 * (Semantics: w2e.)
 *
 * Code identical with the stand-in caller (score.py group: MATCH). Its real callers display_list_flush
 * (0x800FB2C8) and countdown (0x800FBF88) are unmatched, so the private convention cannot be proven in the
 * unit: kept it gets a standard frame (131/178 differ), internal with no C caller it is deleted.
 *
 * What closed it (w16f, from w2e's 50 rows):
 *  - the random pick is the locked Random() func_8008B2E4 (func_8008B2E4.c in this group, byte-identical
 *    copy of src/blob/func_8008B2E4.c), umerge-inlined with its static rand(): its redundant `& 0x7FFF` and
 *    named `rannum` give retail's temp ring in the LCG block (w2e's own frand had the rand value in v0);
 *  - the retry loop is `while (1) { ...; for (prior = 0; ...) ...; if (slot == prior) break; }`: retail's
 *    `beql a1,v1,exit; b top; move v1,zero` latch; `slot == prior` gives the `a1,v1` operand order;
 *  - `extern volatile s16 D_801543CA` (as in several locked files): retail reloads it at the retry-loop top;
 *    non-volatile, PRE moves the load to the latch and the address web drops to s1 (90 rows);
 *  - the mode 4/5/6 test as an if-chain (`bnel a0,v0` order; with the other changes no `ra` frame any more);
 *  - `config = D_80153E88` in the for-init after `slot = 0` (as1 order of the `addiu a2`).
 * Disclosed shaping: `volatile` on D_801543CA (declaration used by locked code); no unused locals, no
 * compiled-out reads.
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
extern volatile s16 D_801543CA;
extern s32 D_8014A110;
extern s8 D_80154628;
extern s8 D_8014978C;
extern u8 D_801543D4;
extern Player D_80154450[];
extern Config D_80153E88[];

f32 func_8008B2E4(f32 max);
void init_state_continue(void)
{
    s32 extra;
    s32 slot;
    s32 prior;
    u8 next = 0;
    Config *config;

    extra = D_80142724;
    if (6 - D_8014A108 < extra) {
        extra = 6 - D_8014A108;
    }
    if (D_8014A110 == 4 || D_8014A110 == 5 || D_8014A110 == 6) {
        extra = 0;
    }
    D_801543CA = D_8014A108 + extra;
    for (slot = 0, config = D_80153E88; slot < 6; slot++, config++) {
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
                while (1) {
                    config->selection = func_8008B2E4(D_801543CA);
                    for (prior = 0; prior < slot; prior++) {
                        if (config->selection == D_80153E88[prior].selection) {
                            break;
                        }
                    }
                    if (slot == prior) {
                        break;
                    }
                }
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

/* STAND-IN caller (not retail): the real callers display_list_flush and countdown are unmatched. */
void standin_caller(s32 a)
{
    if (a) {
        init_state_continue();
    }
    init_state_continue();
}
