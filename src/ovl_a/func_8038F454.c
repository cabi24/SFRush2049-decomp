/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Runtime A func_8038F454, [0x8038F454,0x8038F648), 500 bytes.
 * Strict standalone MATCH, 125/125 words, normal O3 plus r4300_mul.
 * Timed three-descriptor prompt, input/expiry branches and player-state reset.
 * Historical helper names identify their native addresses; original function
 * naming and exact arcade ancestry are not established.
 *
 * The native repeated handle=-1 writes and following handle!=-1 guard are
 * retained. IDO leaves a compare against identical registers for that redundant
 * guard, as in the target. This mirrors native behavior; it does not prove an
 * original source idiom or the identity of any formerly inlined helper.
 * The PlayerStatus view records only the observed state byte and 76-byte stride.
 * entity_flags_apply's real unsigned return/parameter contract is required for
 * the final loop's register allocation. No false return prototype, stand-in
 * callers, unused locals, assembly, volatility or private compiler flags.
 */
typedef signed char s8;
typedef unsigned int u32;
typedef int s32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    short dulx, duly, width, height, top, bot, left, right;
    u32 zdepth, alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;

typedef struct PlayerStatus {
    unsigned char header, state, rest[74];
} PlayerStatus;
extern u32 D_801174BC, D_801174B4;
extern s32 D_803B6AEC, D_803B6AE4;
extern s8 D_80149774, D_801461F8, D_80157244;
extern Blit *D_803B6AE8;
extern MultiBlit D_803B6A78[];
extern u32 D_8015694C;
extern PlayerStatus D_8014A118[4];
extern void func_800B5570(s32);
extern Blit *sound_control(short, short, const MultiBlit *, short);
extern void sound_stop(Blit *);
extern void viScheduleTick(float);
extern s32 viDeadlinePassed(void);
extern void entity_audio_update(void);
extern u32 entity_flags_apply(u32, u32, u32, unsigned char);

void func_8038F454(void)
{
    s32 i;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) {
            func_800B5570(0x10);
            return;
        }
        D_801174BC = 1;
    }
    if (D_803B6AEC != -1)
        D_803B6AEC = -1;
    if (D_803B6AEC == -1 && D_80149774 < 2) {
        D_803B6AEC = -1;
        D_80149774++;
        if (D_803B6AEC != -1)
            return;
    }
    if (!D_803B6AE4) {
        D_803B6AE8 = sound_control(0, 0, D_803B6A78, 3);
        viScheduleTick(5.0f);
        D_803B6AE4 = 1;
        if (D_801461F8 && D_80157244) {
            D_801461F8 = 0;
            D_80157244 = 0;
        }
        entity_audio_update();
    }
    if (D_8015694C & 1) {
        if (D_803B6AEC != -1)
            D_803B6AEC = -1;
        if (D_803B6AE8) {
            sound_stop(D_803B6AE8);
            D_803B6AE8 = 0;
        }
        D_803B6AE4 = 0;
        entity_flags_apply(0x4F, 0, 1, 0);
        D_8014A118[0].state = 0;
        for (i = 1; i < 4; i++)
            D_8014A118[i].state = 5;
        func_800B5570(0x10);
    } else if (viDeadlinePassed()) {
        if (D_803B6AE8) {
            sound_stop(D_803B6AE8);
            D_803B6AE8 = 0;
        }
        D_803B6AE4 = 0;
        func_800B5570(8);
    }
}
