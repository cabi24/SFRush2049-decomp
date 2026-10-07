/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * NONMATCH research: runtime A 0x8038A400..0x8038A62C (556 bytes).
 * Input-driven storage/slot prompt inferred from four slot-status records,
 * the established slot-status/capacity helpers, and its BLIT/state lifecycle.
 * No original function name or exact arcade donor is established.
 *
 * Complete natural standalone O3 reconstruction, 116/139 words different
 * versus 121/139 plus 3 extra words in the initial goto-shaped draft.
 * Control-flow and allocation remain unresolved; not a matching candidate.
 * SlotStatus describes the observed 16-byte stride and first-byte access.
 * Both loop counters and all other locals are used. No invented context,
 * unused locals, dummy reads, volatile shaping, assembly or private flags.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    s16 dulx,duly,width,height,top,bot,left,right;
    u32 zdepth,alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;
typedef struct SlotStatus {s8 available;u8 other[15];} SlotStatus;
extern Blit *D_803B83C8;
extern s8 D_803B839C;
extern s8 D_80116DB4;
extern void *D_8012E6E0;
extern SlotStatus D_80156CF0[4];
extern u8 D_803B83A0;
extern MultiBlit D_803B83A4[];
extern u32 D_8015694C;
extern s8 D_803BA7EB;
extern s8 func_800A1A3C(s32);
extern s8 func_800A35F8(s32);
extern s8 func_80094FC4(s32);
extern u32 func_800A35BC(s32);
extern s32 func_800A3508(s32);
extern s32 func_800A3518(s32);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void player_state_set(s32,s32);
extern void player_mode_set(s32,s32);
extern void audio_distance_atten(s32);
extern void resource_type_select(s32);
extern void sound_stop(Blit *);
extern void func_800B5570(s32);

void func_8038A400(void)
{
    SlotStatus *slot;
    s32 i;
    s32 available;

    if (!D_803B83C8) {
        D_803B839C = 0;
        D_80116DB4 = 0;
    }
    if (!D_8012E6E0) {
        for (slot = D_80156CF0; slot < D_80156CF0 + 4; slot++) {
            if (slot->available)
                break;
        }
        if (slot < D_80156CF0 + 4) {
            available = 0;
            for (i = 0; i < 4; i++) {
                if (func_800A1A3C(i) && !func_800A35F8(i) && !func_80094FC4(i)) {
                    available++;
                    if (func_800A35BC(i) >= (u32)func_800A3508(0x810) && func_800A3518(i))
                        break;
                }
            }
            if (i == 4) {
                D_803B83A0 = available;
                if (D_803B83A0 > 0 || !D_80116DB4) {
                    if (!D_803B83C8) {
                        D_803B83C8 = sound_control(0, 0, D_803B83A4, 1);
                        player_state_set(-1, 1);
                        player_mode_set(-1, 1);
                    }
                    if (D_803B83A0 > 0) {
                        if (D_8015694C & 0xC00) {
                            audio_distance_atten(D_8015694C);
                            D_803B839C = !D_803B839C;
                        }
                    } else {
                        D_803B839C = 0;
                    }
                    if (!(D_8015694C & 3))
                        return;
                    resource_type_select(D_8015694C);
                }
            }
        }
    }
    if (D_803B83C8) {
        sound_stop(D_803B83C8);
        D_803B83C8 = 0;
    }
    D_803BA7EB = func_800A3508(0x810);
    func_800B5570(D_803B839C ? 0x04000000 : 4);
}
