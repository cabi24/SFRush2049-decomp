/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001A658 - lib_1a660 (IDO 7.1) - MATCH (w26b). MusyX synth.c per-voice update of the pre-job-queue revision
 * (synthHandle's voice loop with LowPrecision/ZeroOffset/EventHandler inline, then the 32 master faders and
 * hwFrameDone = func_80014D14). Builds on w23b/w24c/w25b (pan clamp form, sv++ latch, vol/pan loads, cpitch add order).
 * w26b levers (each traced: ucode oracle via uedit/urun, then the C):
 *  - `u8 midi`, read once in the volume block (`midi = sv->midi4A;`): retail's midi compares put the 0xFF register
 *    first, which uopt does only for a non-plain-variable operand (the u8 cvt); the u8 variable is split into the
 *    a0 piece (volume block) and the a1 piece saved at sp+0x48 around the inpGet calls, as retail.
 *  - the fader loop indexes the fader array directly (FADER_8001A658[j], a cast view of D_8004F300): stores through
 *    a known global base do not kill the D_8004BE90 load, so retail's a0 load webs appear and &D_8004BE90 stays out
 *    of the fader loop (the end pointer takes s3); `(time >> 8) * delta` is retail's multiply operand order.
 *  - no `smf` pointer local; `aux` is declared second (no unused locals): with this declaration list the frame (136)
 *    and every home offset are retail's (midi home 0x48, vrange 0x58, span 0x68).
 * Shaping devices (disclosed): span is reused as a scratch twice, both semantically neutral -
 *  (1) `span = cpitch; func_80014A0C(i, span); span = 0;` (span carries the pitch argument; this stops uopt's
 *      store PRE hoisting `span = 0` to the loop top, so the store stays after hwSetPitch as in retail);
 *  (2) `if ((span = midi) != 0xFF)` in the D_8004F2F8 == 0 arm (span is overwritten on both sub-paths; the store
 *      keeps the `span = 0` of the midi == 0xFF path from being deleted as redundant, as in retail).
 * Relocation: retail's fader end pointer &D_8004F300[32] is split as D_8004F800; needs the two reloc_addrs entries in
 * cloud/work/frontier/w26b/reloc_addrs_w26b.txt (as func_8001C1D8's), then a target refresh. */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
struct RampState;
struct VoicePrefix;
struct VoiceState;
struct VoiceState_80019C8C;
typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern ChannelState D_8004F300[32];
#pragma pack(1)
typedef struct SynthLfo_8001A658 { u32 time; u32 period; s16 value; s16 lastValue; } SynthLfo_8001A658;
typedef struct SynthVoice_8001A658 { void *addr00; u8 unknown04[0x20]; u32 cFlags24; u32 age28; u16 ageSpeed2C; u8 unknown2E; u8 track2F; s32 volume30; u8 unknown34[4]; s32 panning38; s32 panDelta3C; s32 sweepOff40[2]; u8 sweepNum48[2]; u8 midi4A; u8 midiSet4B; u8 fxFlag4C; u8 unknown4D[3]; u16 curNote50; u8 unknown52[0x18]; u8 pbLowerKeyRange6A; u8 pbUpperKeyRange6B; u32 vibCurTime6C; u32 vibPeriod70; s32 vibCurOffset74; u8 vibKeyRange78; u8 vibCentRange79; u8 unknown7A[2]; s32 sweepAdd7C[2]; s32 envDelta84; s32 envTarget88; u8 unknown8C[0xD]; u8 revVolScale99; u8 revVolOffset9A; u8 unknown9B[0xD]; u32 panTimeA8; s32 panTargetAC; u8 sweepCntB0[2]; u8 unknownB2[2]; u32 envTimeB4; u16 pbLastB8; u8 unknownBA[3]; u8 blockedBD; u8 vGroupBE; u8 unknownBF; s8 curDetuneC0; u8 unknownC1; u16 curOutputVolumeC2; u8 unknownC4[0xA4]; SynthLfo_8001A658 lfo168[2]; s16 localVars180[16]; } SynthVoice_8001A658;
#pragma pack(0)
typedef struct SynthFader_8001A658 { s32 volume; s32 target; s32 delta; s32 time; u32 seqId; u8 type; u8 seqMode; u8 unknown16[2]; s32 pauseVolume; s32 pauseTarget; s32 pauseDelta; s32 pauseTime; } SynthFader_8001A658;
extern struct VoiceState_80019C8C D_8004BEB8[];
extern u8 D_8004FA18;
extern u32 D_8004BE90;
extern u8 D_8004F2B8[];
extern u8 D_8004F2F8;
extern u8 func_8001467C(u32);
extern u32 func_80019AD4(struct RampState *, u32);
extern void func_80019BE4(struct VoiceState *);
extern u32 func_8001A5D8(struct VoicePrefix *, u32);
extern void func_80020FDC(u8, u8, u8);
extern void func_80023E9C(void *);
extern short func_8001E7BC(u16);
extern unsigned short func_80021428(void *);
extern unsigned short func_80021448(void *);
extern unsigned short func_80021468(void *);
extern unsigned short func_80021488(void *);
extern unsigned short func_800214A8(void *);
extern unsigned short func_800214C8(void *);
extern unsigned short func_800214E8(void *);
extern unsigned short func_80021508(void *);
extern unsigned short func_80021528(void *);
extern void func_800149BC(unsigned int);
extern void func_800149DC(int);
extern void func_80014A0C(int, unsigned short);
extern void func_80014A74(int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void func_80014D14(void);
extern void func_80018C2C(unsigned int);
extern void func_80018D40(unsigned int);
extern void func_800190AC(u32, u32, u32);

#define FADER_8001A658 ((SynthFader_8001A658 *)D_8004F300)

void func_8001A658(void)
{
    SynthVoice_8001A658 *sv;
    s32 aux;
    s32 i;
    s32 j;
    u32 ccents;
    s32 pbend;
    s32 voff;
    s32 span;
    u32 cpitch;
    s32 vol;
    s32 pan;
    s32 vrange;
    u8 midi;

    for (i = 0, sv = (SynthVoice_8001A658 *)D_8004BEB8; i < D_8004FA18; i++, sv++) {
        if (sv->addr00 != 0) {
            func_80023E9C(sv);
        } else if (!func_8001467C(i)) {
            continue;
        }
        if (sv->blockedBD != 0) {
            continue;
        }
        for (j = 0; j < 2; j++) {
            if (sv->lfo168[j].period != 0) {
                sv->lfo168[j].time += D_8004BE90;
                sv->lfo168[j].value = func_8001E7BC(((sv->lfo168[j].time % sv->lfo168[j].period) * 16) / (sv->lfo168[j].period >> 8));
            }
        }
        if (sv->cFlags24 & 0x4000) {
            sv->vibCurTime6C += D_8004BE90;
            sv->vibCurOffset74 = func_8001E7BC(((sv->vibCurTime6C % sv->vibPeriod70) * 16) / (sv->vibPeriod70 >> 8));
        }
        for (j = 0; j < 2; j++) {
            if (sv->sweepNum48[j] != 0) {
                if (--sv->sweepCntB0[j] == 0) {
                    sv->sweepCntB0[j] = sv->sweepNum48[j];
                    sv->sweepOff40[j] = 0;
                } else {
                    sv->sweepOff40[j] += sv->sweepAdd7C[j];
                }
            }
        }
        if (sv->midi4A != 0xFF) {
            if (func_800214E8(sv) <= 0x1F80) {
                sv->cFlags24 &= ~0x40000000;
            } else {
                sv->cFlags24 |= 0x40000000;
            }
        } else {
            sv->cFlags24 &= ~0x40000000;
        }
        if (!(sv->cFlags24 & 0x1000)) {
            if (sv->midi4A != 0xFF) {
                if (func_80021508(sv) <= 0x1F80) {
                    sv->cFlags24 &= ~0x800;
                    func_80020FDC(sv->midi4A, sv->midiSet4B, 0);
                } else {
                    if (!(sv->cFlags24 & 0x800)) {
                        func_80019BE4((struct VoiceState *)sv);
                    }
                    sv->cFlags24 |= 0x800;
                }
            }
            sv->cFlags24 |= 0x2000;
        }
        if (sv->cFlags24 & 0x20) {
            sv->cFlags24 &= ~0x20;
            sv->cFlags24 |= 0x10;
            func_800149BC(i);
        }
        if ((sv->cFlags24 & 0x80) && !(sv->cFlags24 & 0x40000000)) {
            sv->cFlags24 &= ~0x90;
            func_800149DC(i);
        }
        if (sv->age28 != 0) {
            sv->age28 -= sv->ageSpeed2C * D_8004BE90;
            if ((s32)sv->age28 < 0) {
                sv->age28 = 0;
            }
        }
        if (sv->cFlags24 & 0x10000) {
            sv->panning38 = sv->panTargetAC - (sv->panTimeA8 >> 8) * sv->panDelta3C;
            sv->panning38 = sv->panning38 < 0 ? 0 : (u32)sv->panning38 > 0x7F0000U ? 0x7F0000U : sv->panning38;
            sv->panTimeA8 -= D_8004BE90;
            if (sv->panTimeA8 == 0) {
                sv->cFlags24 &= ~0x10000;
                sv->panning38 = sv->panTargetAC;
            }
        }
        ccents = (sv->curNote50 << 16) + (sv->curDetuneC0 << 16) / 100;
        if (sv->cFlags24 & 0x40010) {
            if (sv->midi4A != 0xFF) {
                pbend = func_80021488(sv);
                sv->pbLastB8 = pbend;
                goto pbend_adjust;
            }
        } else {
            pbend = sv->pbLastB8;
        pbend_adjust:
            if (pbend != 0x2000) {
                pbend -= 0x2000;
                if (pbend < 0) {
                    ccents += sv->pbLowerKeyRange6A * pbend * 8;
                } else {
                    ccents += sv->pbUpperKeyRange6B * pbend * 8;
                }
            }
        }
        if (sv->cFlags24 & 0x4000) {
            vrange = (sv->vibKeyRange78 << 8) + (sv->vibCentRange79 << 8) / 100;
            if ((sv->cFlags24 & 0x8000) && sv->midi4A != 0xFF) {
                voff = ((func_800214C8(sv) >> 7) * sv->vibCurOffset74) >> 7;
            } else {
                voff = sv->vibCurOffset74;
            }
            ccents += (vrange * voff) >> 4;
        }
        cpitch = sv->sweepOff40[1] + (sv->sweepOff40[0] + func_8001A5D8((struct VoicePrefix *)sv, func_80019AD4((struct RampState *)sv, ccents)));
        if (sv->midi4A != 0xFF) {
            cpitch = (func_800214A8(sv) * (cpitch >> 16)) >> 13;
        } else {
            cpitch >>= 16;
        }
        span = cpitch;
        func_80014A0C(i, span);
        span = 0;
        if (sv->cFlags24 & 0x20000) {
            sv->volume30 = sv->envTarget88 - (sv->envTimeB4 >> 8) * sv->envDelta84;
            sv->volume30 = sv->volume30 < 0 ? 0 : (u32)sv->volume30 > 0x7F0000U ? 0x7F0000U : sv->volume30;
            sv->envTimeB4 -= D_8004BE90;
            if ((s32)sv->envTimeB4 <= 0) {
                sv->cFlags24 &= ~0x20000;
                sv->volume30 = sv->envTarget88;
            }
        }
        vol = sv->volume30;
        pan = sv->panning38;
        midi = sv->midi4A;
        vol = (FADER_8001A658[sv->vGroupBE].pauseVolume >> 16) * (vol >> 7);
        vol = (FADER_8001A658[sv->vGroupBE].volume >> 16) * (vol >> 7);
        vol = (FADER_8001A658[sv->fxFlag4C ? 22 : 21].volume >> 16) * (vol >> 7);
        if (sv->track2F != 0xFF) {
            vol = D_8004F2B8[sv->track2F] * (vol >> 7);
        }
        if (D_8004F2F8 == 0) {
            if ((span = midi) != 0xFF) {
                vol = (func_80021428(sv) >> 7) * (vol >> 7);
                if (pan != 0x800000) {
                    pan += ((func_80021448(sv) >> 7) - 0x40) << 16;
                    pan = pan < 0 ? 0 : pan > 0x7F0000 ? 0x7F0000 : pan;
                }
                span = func_80021468(sv) << 9;
                if (span > 0x7F0000) {
                    span = 0x7F0000;
                }
            } else {
                span = 0;
            }
        } else {
            if (midi != 0xFF) {
                vol = (func_80021428(sv) >> 7) * (vol >> 7);
            }
            pan = 0x400000;
        }
        if (midi != 0xFF) {
            aux = ((func_80021528(sv) >> 7) * vol) >> 7;
            if (aux > 0x7F0000) {
                aux = 0x7F0000;
            }
            aux = (aux >> 7) * sv->revVolScale99;
            if (aux > 0x7F0000) {
                aux = 0x7F0000;
            }
            aux = (sv->revVolOffset9A << 16) + aux;
            if (aux > 0x7F0000) {
                aux = 0x7F0000;
            }
        }
        else {
            aux = 0;
        }
        sv->curOutputVolumeC2 = vol >> 8;
        func_80014A74(i, vol, pan, span, aux);
    }
    for (j = 0; j < 32; j++) {
        if (FADER_8001A658[j].time != 0) {
            FADER_8001A658[j].volume = FADER_8001A658[j].target - (FADER_8001A658[j].time >> 8) * FADER_8001A658[j].delta;
            if (FADER_8001A658[j].volume < 0) {
                FADER_8001A658[j].volume = 0;
            }
            FADER_8001A658[j].time -= D_8004BE90;
            if (FADER_8001A658[j].time <= 0) {
                FADER_8001A658[j].time = 0;
                FADER_8001A658[j].volume = FADER_8001A658[j].target;
                switch (FADER_8001A658[j].seqMode) {
                case 1:
                    func_80018D40(FADER_8001A658[j].seqId);
                    break;
                case 2:
                    func_80018C2C(FADER_8001A658[j].seqId);
                    break;
                case 3:
                    func_800190AC(FADER_8001A658[j].seqId, 0, 0);
                    break;
                }
            }
        }
        if (FADER_8001A658[j].pauseTime != 0) {
            FADER_8001A658[j].pauseVolume = FADER_8001A658[j].pauseTarget - (FADER_8001A658[j].pauseTime >> 8) * FADER_8001A658[j].pauseDelta;
            if (FADER_8001A658[j].pauseVolume < 0) {
                FADER_8001A658[j].pauseVolume = 0;
            }
            FADER_8001A658[j].pauseTime -= D_8004BE90;
            if (FADER_8001A658[j].pauseTime <= 0) {
                FADER_8001A658[j].pauseTime = 0;
                FADER_8001A658[j].pauseVolume = FADER_8001A658[j].pauseTarget;
            }
        }
    }
    func_80014D14();
}
