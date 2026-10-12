/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023E9C - lib_22300 (IDO 7.1). MusyX synthmacros.c macHandleActive, the pre-1.5 revision that also carries
 * the per-voice wait/time handling later moved into macHandle. On a (re)start (cFlags & 3): hwBreak (func_80014AF0)
 * when flag 1 is set; flag 0x40 frees the voice (func_8001F6EC) and returns; otherwise inpInit (func_8002106C), the
 * setup copy (pan/vol/midi/midiSet/section), last-note bookkeeping (func_80020F98 inpGetMidiLastNote,
 * func_80020F4C inpSetMidiLastNote), voiceSetLastStarted (func_800217E4) and the voice defaults. A pending key-off
 * trap (0x20) jumps the macro to it when the voice is keyed off (flag 8) and not sustained (0x40000000). With no
 * pending wait (wait9C == 0) the interpreter runs at most 32 steps: the 8-byte command is copied into the static
 * cstep and dispatched over (word[0] & 0x7F) to the mcmd* handlers of this TU (114-entry jump table, own .rodata at
 * 0x8002D964); a nonzero handler result stops it. Otherwise the wait is resolved by key-off (flag 4), sample end
 * (flag 0x100000, func_8001467C) or time (timeA4 >= wait9C). timeA4 advances by D_8004BE90 (macro tick) each call.
 * w22b, from m2c (true 855 -> 0). Levers: the 0x40 test as `if (!(cFlags & 0x40)) { init } else { free; return; }`
 * (retail lays the free path after the init block); `default: ex = 0;` instead of MusyX's `ex = 0;` before the
 * switch (retail's default block is last, no early `move v1,zero`); as1 store order: `field68 = 0;` after the
 * revVol stores, `waitTimeA0 = timeA4;` before `wait9C = 0;` in the key-off block, and the trap clear chained as
 * `trapAddr1C = trapCurAddr20 = 0;` (swr order 0x23 before 0x1F).
 * Statics (owned .bss, lib_22300's whole .bss 0x80056200..0x8005622F): debugMacroSteps_80056200 is synthmacros.c's
 * file-scope `static u8 DebugMacroSteps`; it is declared in func_80022874's source (earlier in the TU) so IDO 7.1
 * allocates it first (0x80056200), then func_80022874's sample record (0x80056208) and cstep_80056228 (0x80056228);
 * the repeated declaration here is a tentative definition deduplicated by the splice. Statics carry their address
 * in the name (asm-processor needs unique static names in a ROM TU).
 * Type views, no devices: private packed SynthVoice_80023E9C (MusyX field names where the role is clear; the TU's
 * VoiceState and the handlers' private views have other names); each handler is declared by its lib_22300 struct
 * tag with the TU's parameter types and called through a cast. No unused locals, no pad arrays, no implicit int. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
struct VoiceState;
struct VoiceControls;
#pragma pack(1)
typedef struct SynthVoice_80023E9C { MacroCommand *addr00; MacroCommand *curAddr04; u8 unknown08[0x14]; MacroCommand *trapAddr1C; MacroCommand *trapCurAddr20; u32 cFlags24; u8 unknown28[7]; u8 section2F; u32 volume30; u32 orgVolume34; u32 panning38; u8 unknown3C[4]; u32 sweepOff40[2]; u8 sweepNum48[2]; u8 midi4A; u8 midiSet4B; u8 unknown4C[2]; u16 orgNote4E; u8 unknown50[2]; u8 setupVol52; u8 setupPan53; u8 setupMidi54; u8 setupMidiSet55; u8 setupSection56; u8 unknown57[5]; u32 field5C; u32 id60; u8 unknown64[4]; u16 field68; u8 pbLowerKeyRange6A; u8 pbUpperKeyRange6B; u8 unknown6C[0x20]; u32 portTime8C; u32 portDuration90; u8 unknown94[4]; u8 portType98; u8 revVolScale99; u8 revVolOffset9A; u8 unknown9B; u32 wait9C; u32 waitTimeA0; u32 timeA4; u8 unknownA8[0x10]; u16 pbLastB8; u8 unknownBA[4]; u8 vGroupBE; u8 setupVGroupBF; u8 unknownC0; u8 lastNoteC1; u16 curOutputVolumeC2; u8 unknownC4[0xA8]; u32 lfoPeriod16C; s16 lfoValue170; u8 unknown172[6]; u32 lfoPeriod178; s16 lfoValue17C; u8 unknown17E[2]; s16 localVars180[16]; } SynthVoice_80023E9C;
#pragma pack(0)
struct MacroState_80021BF0;
struct MacroState_80021C58;
struct MacroState_80021CDC;
struct MacroState_80021F84;
struct MacroState_80021E74;
struct MacroState_80022160;
struct MacroState_80022324;
struct MacroState_80021D5C;
struct MacroState_800223D0;
struct MacroState_80022F24;
struct MacroState_800232A4;
struct VoiceState_800230B0;
struct MacroState_80023520;
struct MacroState_80022874;
struct MacroState_80022A40;
struct MacroState_80022A78;
struct MacroCommand_80022A78;
struct MacroState_80021DF8;
struct MacroState_800225FC;
struct MacroState_80022678;
struct MacroState_8002279C;
struct MacroState_800239A4;
struct MacroState_80022A98;
struct MacroState_8002300C;
struct PitchState_80022CD4;
struct PitchCommand_80022CD4;
struct MacroState_80021F08;
struct MacroState_80021F68;
struct MacroCommand_80021F68;
struct MacroCommand_800223D0;
struct MacroState_8002243C;
struct MacroCommand_8002243C;
struct MacroState_800225AC;
struct MacroCommand_800225AC;
struct MacroState_800225DC;
struct MacroCommand_800225DC;
struct MacroState_80023734;
struct MacroCommand_80023734;
struct MacroState_80023710;
struct MacroCommand_80023710;
struct MacroState_80022514;
struct MacroState_8002245C;
struct MacroCommand_8002245C;
struct MacroState_800224B0;
struct MacroCommand_800224B0;
struct MacroState_80023818;
struct MacroCommand_80023818;
struct MacroState_80022C58;
struct MacroCommand_80022C58;
struct VoiceVariables_80023AD4;
struct MacroVoice_80023DB8;
extern u8 func_80021BC0(MacroState *, MacroCommand *);
extern u8 func_80021BF0(struct MacroState_80021BF0 *, MacroCommand *);
extern u8 func_80021C58(struct MacroState_80021C58 *, MacroCommand *);
extern u8 func_80021CDC(struct MacroState_80021CDC *, MacroCommand *);
extern u8 func_8002193C(MacroState *, MacroCommand *);
extern u8 func_80021F84(struct MacroState_80021F84 *, MacroCommand *);
extern u8 func_80021E74(struct MacroState_80021E74 *, MacroCommand *);
extern u8 func_80021B9C(MacroState *, MacroCommand *);
extern u8 func_80022160(struct MacroState_80022160 *, MacroCommand *);
extern u8 func_80022324(struct MacroState_80022324 *, MacroCommand *);
extern u8 func_80021D5C(struct MacroState_80021D5C *, MacroCommand *);
extern u8 func_80023190(struct MacroState_800223D0 *, MacroCommand *);
extern u8 func_80022F24(struct MacroState_80022F24 *, MacroCommand *);
extern u8 func_800232A4(struct MacroState_800232A4 *, MacroCommand *);
extern u8 func_800230B0(struct VoiceState_800230B0 *, MacroCommand *);
extern u8 func_80023520(struct MacroState_80023520 *, MacroCommand *);
extern u8 func_80022874(struct MacroState_80022874 *, MacroCommand *);
extern u8 func_80022A40(struct MacroState_80022A40 *, MacroCommand *);
extern u8 func_80022A78(struct MacroState_80022A78 *, struct MacroCommand_80022A78 *);
extern u8 func_80021DF8(struct MacroState_80021DF8 *, MacroCommand *);
extern u8 func_80023544(MacroState *, MacroCommand *);
extern u8 func_80023564(struct MacroState_800225FC *, MacroCommand *);
extern u8 func_80022678(struct MacroState_80022678 *, MacroCommand *);
extern u8 func_800225FC(struct MacroState_800225FC *, MacroCommand *);
extern u8 func_8002279C(struct MacroState_8002279C *, MacroCommand *);
extern u8 func_800239A4(struct MacroState_800239A4 *, MacroCommand *);
extern u8 func_80022A98(struct MacroState_80022A98 *, MacroCommand *);
extern u8 func_8002300C(struct MacroState_8002300C *, MacroCommand *, int);
extern u8 func_80022CD4(struct PitchState_80022CD4 *, struct PitchCommand_80022CD4 *);
extern u8 func_80021F08(struct MacroState_80021F08 *, MacroCommand *);
extern u8 func_80021F68(struct MacroState_80021F68 *, struct MacroCommand_80021F68 *);
extern u8 func_800223D0(struct MacroState_800223D0 *, struct MacroCommand_800223D0 *);
extern u8 func_8002243C(struct MacroState_8002243C *, struct MacroCommand_8002243C *);
extern u8 func_800225AC(struct MacroState_800225AC *, struct MacroCommand_800225AC *);
extern u8 func_800225DC(struct MacroState_800225DC *, struct MacroCommand_800225DC *);
extern u8 func_80023734(struct MacroState_80023734 *, struct MacroCommand_80023734 *);
extern u8 func_80023710(struct MacroState_80023710 *, struct MacroCommand_80023710 *);
extern u8 func_80022580(MacroState *, MacroCommand *);
extern u8 func_80022514(struct MacroState_80022514 *, MacroCommand *);
extern u8 func_8002245C(struct MacroState_8002245C *, struct MacroCommand_8002245C *);
extern u8 func_800224B0(struct MacroState_800224B0 *, struct MacroCommand_800224B0 *);
extern u8 func_80023818(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_80023844(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_80023870(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_8002389C(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_800238C8(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_800238F4(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_80023920(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_8002394C(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_80023978(struct MacroState_80023818 *, struct MacroCommand_80023818 *);
extern u8 func_80022C58(struct MacroState_80022C58 *, struct MacroCommand_80022C58 *);
extern u8 func_80023BDC(struct VoiceVariables_80023AD4 *, MacroCommand *, u8);
extern u8 func_80023D70(MacroState *, MacroCommand *);
extern u8 func_80023DB8(struct MacroVoice_80023DB8 *, MacroCommand *, u8);
extern void func_80014AF0(int);
extern void func_8001F6EC(MacroState *);
extern void func_8002106C(struct VoiceControls *);
extern unsigned char func_80020F98(unsigned char channel, unsigned char set);
extern void func_80020F4C(u8, u8, u8);
extern void func_800217E4(struct VoiceState *);
extern u8 func_8001467C(int);
extern u32 D_8004BE90;
static u8 debugMacroSteps_80056200;

void func_80023E9C(SynthVoice_80023E9C *voice)
{
    static MacroCommand cstep_80056228;
    u8 i;
    u8 lastNote;
    u8 ex;

    if (voice->cFlags24 & 3) {
        if (voice->cFlags24 & 1) {
            voice->cFlags24 &= ~1;
            func_80014AF0(voice->id60 & 0xFF);
        }
        if (!(voice->cFlags24 & 0x40)) {
            func_8002106C((struct VoiceControls *)voice);
            voice->panning38 = voice->setupPan53 << 16;
            voice->volume30 = voice->setupVol52 << 16;
            voice->orgVolume34 = voice->volume30;
            voice->midi4A = voice->setupMidi54;
            voice->midiSet4B = voice->setupMidiSet55;
            voice->section2F = voice->setupSection56;
            if ((lastNote = func_80020F98(voice->midi4A, voice->midiSet4B)) != 0xFF) {
                voice->lastNoteC1 = lastNote;
            } else {
                voice->lastNoteC1 = voice->orgNote4E;
            }
            func_80020F4C(voice->midi4A, voice->midiSet4B, voice->orgNote4E);
            func_800217E4((struct VoiceState *)voice);
            voice->vGroupBE = voice->setupVGroupBF;
            voice->portTime8C = 0;
            voice->portDuration90 = 100;
            voice->portType98 = 0;
            voice->pbLowerKeyRange6A = 2;
            voice->pbUpperKeyRange6B = 2;
            voice->revVolScale99 = 128;
            voice->revVolOffset9A = 0;
            voice->field68 = 0;
            voice->sweepNum48[0] = 0;
            voice->sweepNum48[1] = 0;
            voice->sweepOff40[0] = 0;
            voice->sweepOff40[1] = 0;
            voice->lfoPeriod16C = 0;
            voice->lfoValue170 = 0;
            voice->lfoPeriod178 = 0;
            voice->lfoValue17C = 0;
            voice->trapAddr1C = 0;
            voice->trapCurAddr20 = 0;
            voice->field5C = 0x4000AC44;
            voice->pbLastB8 = 0x2000;
            voice->curOutputVolumeC2 = 0;
            voice->cFlags24 &= 8;
            for (i = 0; i < 16; i++) {
                voice->localVars180[i] = 0;
            }
            voice->waitTimeA0 = 0;
            voice->timeA4 = 0;
        } else {
            voice->cFlags24 = 0;
            func_8001F6EC((MacroState *)voice);
            return;
        }
    }
    if (voice->trapCurAddr20 != 0 && !(voice->cFlags24 & 0x40000000) && (voice->cFlags24 & 8)) {
        voice->addr00 = voice->trapAddr1C;
        voice->curAddr04 = voice->trapCurAddr20;
        voice->wait9C = 0;
        voice->trapAddr1C = voice->trapCurAddr20 = 0;
    }
    if (voice->wait9C == 0) {
        debugMacroSteps_80056200 = 0;
        do {
            if (++debugMacroSteps_80056200 > 32) {
                break;
            }
            cstep_80056228.word[0] = voice->curAddr04->word[0];
            cstep_80056228.word[1] = voice->curAddr04->word[1];
            ++voice->curAddr04;
            switch (cstep_80056228.word[0] & 0x7F) {
            case 0x0:
                ex = func_80021BC0((MacroState *)voice, &cstep_80056228);
                break;
            case 0x1:
                ex = func_80021BF0((struct MacroState_80021BF0 *)voice, &cstep_80056228);
                break;
            case 0x2:
                ex = func_80021C58((struct MacroState_80021C58 *)voice, &cstep_80056228);
                break;
            case 0x3:
                ex = func_80021CDC((struct MacroState_80021CDC *)voice, &cstep_80056228);
                break;
            case 0x4:
                ex = func_8002193C((MacroState *)voice, &cstep_80056228);
                break;
            case 0x5:
                ex = func_80021F84((struct MacroState_80021F84 *)voice, &cstep_80056228);
                break;
            case 0x6:
                ex = func_80021E74((struct MacroState_80021E74 *)voice, &cstep_80056228);
                break;
            case 0x7:
                ex = func_80021B9C((MacroState *)voice, &cstep_80056228);
                break;
            case 0x8:
                ex = func_80022160((struct MacroState_80022160 *)voice, &cstep_80056228);
                break;
            case 0x9:
                ex = func_80022324((struct MacroState_80022324 *)voice, &cstep_80056228);
                break;
            case 0xA:
                ex = func_80021D5C((struct MacroState_80021D5C *)voice, &cstep_80056228);
                break;
            case 0xB:
                ex = func_80023190((struct MacroState_800223D0 *)voice, &cstep_80056228);
                break;
            case 0xC:
                ex = func_80022F24((struct MacroState_80022F24 *)voice, &cstep_80056228);
                break;
            case 0xD:
                ex = func_800232A4((struct MacroState_800232A4 *)voice, &cstep_80056228);
                break;
            case 0xE:
                ex = func_800230B0((struct VoiceState_800230B0 *)voice, &cstep_80056228);
                break;
            case 0xF:
                ex = func_80023520((struct MacroState_80023520 *)voice, &cstep_80056228);
                break;
            case 0x10:
                ex = func_80022874((struct MacroState_80022874 *)voice, &cstep_80056228);
                break;
            case 0x11:
                ex = func_80022A40((struct MacroState_80022A40 *)voice, &cstep_80056228);
                break;
            case 0x12:
                ex = func_80022A78((struct MacroState_80022A78 *)voice, (struct MacroCommand_80022A78 *)&cstep_80056228);
                break;
            case 0x13:
                ex = func_80021DF8((struct MacroState_80021DF8 *)voice, &cstep_80056228);
                break;
            case 0x14:
                ex = func_80023544((MacroState *)voice, &cstep_80056228);
                break;
            case 0x17:
                ex = func_80023564((struct MacroState_800225FC *)voice, &cstep_80056228);
                break;
            case 0x18:
                ex = func_80022678((struct MacroState_80022678 *)voice, &cstep_80056228);
                break;
            case 0x19:
                ex = func_800225FC((struct MacroState_800225FC *)voice, &cstep_80056228);
                break;
            case 0x1A:
                ex = func_8002279C((struct MacroState_8002279C *)voice, &cstep_80056228);
                break;
            case 0x1B:
                ex = func_800239A4((struct MacroState_800239A4 *)voice, &cstep_80056228);
                break;
            case 0x1C:
                ex = func_80022A98((struct MacroState_80022A98 *)voice, &cstep_80056228);
                break;
            case 0x1D:
                ex = func_8002300C((struct MacroState_8002300C *)voice, &cstep_80056228, 0);
                break;
            case 0x1E:
                ex = func_8002300C((struct MacroState_8002300C *)voice, &cstep_80056228, 1);
                break;
            case 0x1F:
                ex = func_80022CD4((struct PitchState_80022CD4 *)voice, (struct PitchCommand_80022CD4 *)&cstep_80056228);
                break;
            case 0x28:
                ex = func_80021F08((struct MacroState_80021F08 *)voice, &cstep_80056228);
                break;
            case 0x29:
                ex = func_80021F68((struct MacroState_80021F68 *)voice, (struct MacroCommand_80021F68 *)&cstep_80056228);
                break;
            case 0x30:
                ex = func_800223D0((struct MacroState_800223D0 *)voice, (struct MacroCommand_800223D0 *)&cstep_80056228);
                break;
            case 0x31:
                ex = func_8002243C((struct MacroState_8002243C *)voice, (struct MacroCommand_8002243C *)&cstep_80056228);
                break;
            case 0x32:
                ex = func_800225AC((struct MacroState_800225AC *)voice, (struct MacroCommand_800225AC *)&cstep_80056228);
                break;
            case 0x33:
                ex = func_800225DC((struct MacroState_800225DC *)voice, (struct MacroCommand_800225DC *)&cstep_80056228);
                break;
            case 0x34:
                ex = func_80023734((struct MacroState_80023734 *)voice, (struct MacroCommand_80023734 *)&cstep_80056228);
                break;
            case 0x35:
                ex = func_80023710((struct MacroState_80023710 *)voice, (struct MacroCommand_80023710 *)&cstep_80056228);
                break;
            case 0x36:
                ex = func_80022580((MacroState *)voice, &cstep_80056228);
                break;
            case 0x37:
                ex = func_80022514((struct MacroState_80022514 *)voice, &cstep_80056228);
                break;
            case 0x38:
                ex = func_8002245C((struct MacroState_8002245C *)voice, (struct MacroCommand_8002245C *)&cstep_80056228);
                break;
            case 0x39:
                ex = func_800224B0((struct MacroState_800224B0 *)voice, (struct MacroCommand_800224B0 *)&cstep_80056228);
                break;
            case 0x40:
                ex = func_80023818((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x41:
                ex = func_80023844((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x42:
                ex = func_80023870((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x43:
                ex = func_8002389C((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x44:
                ex = func_800238C8((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x45:
                ex = func_800238F4((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x46:
                ex = func_80023920((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x47:
                ex = func_8002394C((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x48:
                ex = func_80023978((struct MacroState_80023818 *)voice, (struct MacroCommand_80023818 *)&cstep_80056228);
                break;
            case 0x50:
                ex = func_80022C58((struct MacroState_80022C58 *)voice, (struct MacroCommand_80022C58 *)&cstep_80056228);
                break;
            case 0x60:
                ex = func_80023BDC((struct VoiceVariables_80023AD4 *)voice, &cstep_80056228, 0);
                break;
            case 0x61:
                ex = func_80023BDC((struct VoiceVariables_80023AD4 *)voice, &cstep_80056228, 1);
                break;
            case 0x62:
                ex = func_80023BDC((struct VoiceVariables_80023AD4 *)voice, &cstep_80056228, 2);
                break;
            case 0x63:
                ex = func_80023BDC((struct VoiceVariables_80023AD4 *)voice, &cstep_80056228, 3);
                break;
            case 0x64:
                ex = func_80023BDC((struct VoiceVariables_80023AD4 *)voice, &cstep_80056228, 4);
                break;
            case 0x65:
                ex = func_80023D70((MacroState *)voice, &cstep_80056228);
                break;
            case 0x70:
                ex = func_80023DB8((struct MacroVoice_80023DB8 *)voice, &cstep_80056228, 0);
                break;
            case 0x71:
                ex = func_80023DB8((struct MacroVoice_80023DB8 *)voice, &cstep_80056228, 1);
                break;
            default:
                ex = 0;
            }
        } while (!ex);
        voice->timeA4 += D_8004BE90;
    } else {
        voice->timeA4 += D_8004BE90;
        if ((voice->cFlags24 & 4) && !(voice->cFlags24 & 0x40000000) && (voice->cFlags24 & 8)) {
            voice->cFlags24 &= ~4;
            voice->waitTimeA0 = voice->timeA4;
            voice->wait9C = 0;
        }
        if ((voice->cFlags24 & 0x100000) && !func_8001467C(voice->id60 & 0xFF)) {
            voice->cFlags24 &= ~0x100000;
            voice->wait9C = 0;
            voice->waitTimeA0 = voice->timeA4;
        }
        if (voice->wait9C != 0xFFFFFFFF && voice->timeA4 >= voice->wait9C) {
            voice->waitTimeA0 = voice->wait9C;
            voice->wait9C = 0;
        }
    }
}
