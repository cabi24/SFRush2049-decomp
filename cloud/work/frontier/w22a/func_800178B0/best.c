/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#include "sequence_context.h"
typedef struct MapRecord { u8 unknown00[5]; u8 key; u8 unknown06[2]; } MapRecord;
#pragma pack(1)
typedef struct Page_800178B0 { u16 macro; u8 prio, maxVoices, slot, index, reserved[2]; } Page_800178B0;
#pragma pack()
typedef struct MidiChannel_800178B0 { u8 program, volume, panning, reverb, chorus, reserved[3]; } MidiChannel_800178B0;
typedef struct MidiSetup_800178B0 { u16 songId, reserved; MidiChannel_800178B0 channel[16]; } MidiSetup_800178B0;
typedef struct VolDef_800178B0 { u8 track, volGroup; } VolDef_800178B0;
typedef struct PlayPara_800178B0 {
    u32 flags;
    u32 trackMute[2];
    u16 speed;
    u16 time;
    u8 target;
    u8 pad11;
    u8 numSeqVolDef;
    VolDef_800178B0 *seqVolDef;
    u8 numFaded;
    u8 *faded;
} PlayPara_800178B0;
typedef struct Arr_800178B0 { u32 tTab, unknown04, unknown08, mTrack, info; } Arr_800178B0;
typedef struct Track_800178B0 { u8 *addr, *base; u32 pattern08, pattern0C; } Track_800178B0;
typedef struct TrackState_800178B0 { u32 unknown00[3]; u32 active0C, active10, active14; u8 unknown18[16]; } TrackState_800178B0;
typedef struct Seq_800178B0 {
    u32 identifier;
    Page_800178B0 *normtab;
    u8 normTrans[128];
    Page_800178B0 *drumtab;
    u8 drumTrans[128];
    Arr_800178B0 *arrbase;
    u32 trackMute[2];
    u32 fraction118, whole11C, half120;
    u32 bpm;
    Track_800178B0 track[64];
    u8 trackVolGroup[64];
    TrackState_800178B0 trackState[64];
    u32 *mTrackBase, *mTrackAddr;
    u32 lowF70, highF74;
    SequenceNode *noteUsed, *noteKeyOff;
    u32 prgState[16];
    u8 activeFC0, freeFC1;
    u16 speed;
    u8 defVGroup, disabledFC5;
    u16 valueFC6;
    u8 unknownFC8[0x2C];
    u8 syncFF4;
    u8 unknownFF5[3];
} Seq_800178B0;
extern u8 D_8004F2B8[64];
extern void func_8001785C(u8 *map, MapRecord *record);
extern void func_80017720(SequenceContext *, unsigned char, unsigned char);
extern void func_8001C19C(unsigned char, unsigned char);
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);
extern void func_80020820(u8, u8);
extern unsigned int func_800175B4(unsigned int slot);
unsigned int func_800178B0(Page_800178B0 *norm, Page_800178B0 *drum, MidiSetup_800178B0 *midiSetup, Arr_800178B0 *song,
                           PlayPara_800178B0 *para)
{
    Seq_800178B0 *nseq;
    u32 *tracktab;
    u8 *trans;
    Page_800178B0 *page;
    Page_800178B0 *entry;
    int i, j, t;
    u8 seqId;
    u32 id;

    for (i = 0; i < 8; i++) {
        if (D_80043EB8[i].inactiveFC1 != 0) break;
    }
    if (i != 8) {
    nseq = (Seq_800178B0 *)&D_80043EB8[i];
    nseq->syncFF4 = 0;
    nseq->normtab = norm;
    nseq->drumtab = drum;
    nseq->arrbase = song;
    func_8001785C(nseq->normTrans, (MapRecord *)nseq->normtab);
    func_8001785C(nseq->drumTrans, (MapRecord *)nseq->drumtab);
    nseq->defVGroup = i + 23;
    for (j = 0; j < 64; j++) {
        nseq->trackVolGroup[j] = nseq->defVGroup;
    }
    if (para == NULL) {
        nseq->trackMute[0] = 0xFFFFFFFF;
        nseq->trackMute[1] = 0xFFFFFFFF;
        nseq->speed = 256;
    } else {
        if (para->flags & 1) {
            nseq->trackMute[0] = para->trackMute[0];
            nseq->trackMute[1] = para->trackMute[1];
        } else {
            nseq->trackMute[0] = 0xFFFFFFFF;
            nseq->trackMute[1] = 0xFFFFFFFF;
        }
        if (para->flags & 2) {
            nseq->speed = para->speed;
        } else {
            nseq->speed = 256;
        }
        if (para->flags & 8) {
            for (j = 0; j < para->numSeqVolDef; j++) {
                nseq->trackVolGroup[para->seqVolDef[j].track] = para->seqVolDef[j].volGroup;
                func_8001C19C(para->seqVolDef[j].volGroup, 0);
            }
        }
        if (para->flags & 4) {
            func_8001B9F8(para->target, para->time, nseq->defVGroup, 0, 0);
            for (j = 0; j < para->numFaded; j++) {
                func_8001B9F8(para->target, para->time, para->faded[j], 0, 0);
            }
        }
    }
    seqId = i;
    nseq->bpm = song->info;
    func_80019A60(nseq->bpm, seqId);
    if (song->mTrack != 0) {
        nseq->mTrackBase = (u32 *)(song->mTrack + (u32)song);
        nseq->mTrackAddr = nseq->mTrackBase;
        nseq->lowF70 = 0;
        nseq->highF74 = 0;
    } else {
        nseq->mTrackBase = NULL;
    }
    nseq->disabledFC5 = 0;
    tracktab = (u32 *)(song->tTab + (u32)song);
    for (j = 0; j < 64; j++) {
        D_8004F2B8[j] = 127;
        nseq->track[j].pattern08 = 0;
        nseq->track[j].pattern0C = 0;
        nseq->trackState[j].active0C = 0;
        nseq->trackState[j].active10 = 0;
        nseq->trackState[j].active14 = 0;
        if (tracktab[j] != 0) {
            nseq->track[j].base = nseq->track[j].addr = (u8 *)(tracktab[j] + (u32)song);
        } else {
            nseq->track[j].base = nseq->track[j].addr = NULL;
        }
    }
    nseq->noteUsed = NULL;
    nseq->noteKeyOff = NULL;
    for (j = 0; j < 16; j++) {
        func_80020820(j, seqId);
    }
    for (j = 0; j < 16; j++) {
        nseq->prgState[j] = 0xFFFFFFFF;
    }
    page = nseq->normtab;
    trans = nseq->normTrans;
    for (t = 0; t < 2; t++) {
        for (j = 0; j < 128; j++) {
            if (trans[j] != 0xFF) {
                entry = page;
                entry += trans[j];
                if (entry->slot != 0xFF) {
                    nseq->prgState[entry->slot] = entry->maxVoices | (entry->macro << 16) | (entry->prio << 8);
                }
            }
        }
        page = nseq->drumtab;
        trans = nseq->drumTrans;
    }
    if (midiSetup != NULL) {
        for (j = 0; j < 16; j++) {
            func_80017720((SequenceContext *)nseq, midiSetup->channel[j].program, j);
            func_80020610(7, j, seqId, midiSetup->channel[j].volume);
            func_80020610(10, j, seqId, midiSetup->channel[j].panning);
            func_80020610(91, j, seqId, midiSetup->channel[j].reverb);
            func_80020610(93, j, seqId, midiSetup->channel[j].chorus);
        }
    }
    nseq->valueFC6 = 0;
    if (para != NULL) {
        if (!(para->flags & 0x10)) {
            nseq->activeFC0 = 1;
        }
    } else {
        nseq->activeFC0 = 1;
    }
    id = func_800175B4(i);
    nseq->freeFC1 = 0;
    return id;
    }
    return 0xFFFFFFFF;
}
