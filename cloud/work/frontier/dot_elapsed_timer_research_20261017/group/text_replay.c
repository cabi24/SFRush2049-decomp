/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * world_velocity_integrate (0x800ED804, 712 B) -- historical label. Replays a recorded byte stream of
 * slot/menu-state changes (D_80149DD8[0..D_80149DD0)) when D_8011474C is set: resets the slot outputs
 * (func_800B6748), reselects the slot, keeps only bit 15 of D_80149B88, then per opcode 0..6:
 * slot_state_setup(b), mode_byte2_set(b), mode_byte_set(b), x/y s16 pair (0x8000 = keep), f32 for
 * func_800ED66C, four bytes for func_800B7360, a length-prefixed block for audio_doppler_calc.
 * STATE: draft, unit 141/178 words (rows: words 78 ops 44 norm 44), frame 96/96.
 * Residual class: callee convention. audio_doppler_calc (unmatched, blocked by Astra's func_80087110) is
 * absent from the unit, but retail passes it s2 = &D_80149DD8[pos] and s0 = len as IPA register args and
 * it clobbers s1-s8 (so retail spills pos/len to 92/88(sp) and rematerialises D_80149DD0 and &D_80149DD8
 * in a1/t0 after every call instead of keeping them in s-registers). Not finishable until it matches.
 * Started from the codex_defaults_a16 context draft (unit 161/178, words 139 ops 90).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void func_800B6748(u8 *rgba, f32 *amount, s16 *x, s16 *y, s8 *bank, s8 *mode, s8 *variant);
extern void mode_byte2_set(s16);
extern void mode_byte_set(s16);
extern void func_800ED66C(f32);
extern s32 func_800B7360();
extern void audio_doppler_calc(u8 *, u16);
extern u8 D_80149B48[8];
extern f32 D_80114748;
extern s16 D_80149D92, D_80149D9E;
extern s8 D_80149DA0, D_80149B60, D_80149B70;
extern s32 D_80149B88;
extern s8 D_8011474C, D_8011473C;
extern s32 D_80149DD0;
extern u8 D_80149DD8[800];
#define NULL 0
void world_velocity_integrate(s8 recording)
{
    s32 pos;
    s16 x, y;
    union { u32 w; f32 f; } cvt;
    u8 *p;
    u8 len;

    if (D_8011474C) {
        osRecvMesg(&D_801461D0, NULL, 1);
        D_8011473C = recording;
        func_800B6748((u8 *)&D_80149B48, &D_80114748, &D_80149D92, &D_80149D9E, &D_80149DA0, &D_80149B60, &D_80149B70);
        slot_state_setup(D_80149DA0);
        D_80149B88 &= 0x8000;
        pos = 0;
        while (pos < D_80149DD0) {
            p = &D_80149DD8[pos];
            pos++;
            switch (*p++) {
            case 0:
                pos++;
                slot_state_setup(*p++);
                break;
            case 1:
                pos++;
                mode_byte2_set(*p++);
                break;
            case 2:
                pos++;
                mode_byte_set(*p++);
                break;
            case 3:
                x = (p[0] << 8) | p[1];
                y = (p[2] << 8) | p[3];
                if (x != -32768) D_80149D92 = x;
                if (y != -32768) D_80149D9E = y;
                pos += 4;
                break;
            case 4:
                cvt.w = ((u32)p[0] << 24) | p[3] | ((u32)p[1] << 16) | ((u32)p[2] << 8);
                pos += 4;
                func_800ED66C(cvt.f);
                break;
            case 5:
                func_800B7360(p[0], p[1], p[2], p[3]);
                pos += 4;
                break;
            case 6:
                len = *p;
                pos++;
                audio_doppler_calc(&D_80149DD8[pos], len);
                pos += len;
                break;
            }
        }
        osSendMesg(&D_801461D0, NULL, 0);
    }
}
