/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80017720 - lib_17dc0 (IDO 7.1). MusyX seq.c DoPrgChange (older revision than the GameCube decomp
 * doldecomp/ttyd libs/musyx: no seqMIDIPriority reset, prgState packed in one word): map PROGRAM through the
 * sequence's normal (channel != 9) or drum translation table (0xFF = none) and store the program entry's
 * macro << 16 | priority << 8 | max voices as channel CHANNEL's program state (context + 0xF80).
 * Caller: func_800178B0 (seqStartPlay) per MIDI channel; TU prototype `(SequenceContext *, unsigned char,
 * unsigned char)`.
 * w23c (D10 BT03-high-transitions true=925): the packed word is written in field order,
 * `(macro << 16) | (priority << 8) | maxVoices` (D10's high | low | middle order reassociates differently:
 * 925 -> 0). No devices, no unused locals. Types: sequence_context.h's SequenceContext (the TU's), read through
 * a private view Seq_80017720 (normtab 0x04, normTrans 0x08, drumtab 0x88, drumTrans 0x8C, prgState 0xF80)
 * by the cast macro SEQ_17720 (type view, no code); ProgramEntry_80017720 = MusyX's packed page entry. */
#include "sequence_context.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct ProgramEntry_80017720 { u16 macro; u8 priority; u8 maxVoices; u32 unknown04; } ProgramEntry_80017720;
#pragma pack(0)
typedef struct Seq_80017720 {
    u32 identifier;
    ProgramEntry_80017720 *normtab;
    u8 normTrans[128];
    ProgramEntry_80017720 *drumtab;
    u8 drumTrans[128];
    u8 unknown10C[0xE74];
    u32 prgState[16];
} Seq_80017720;
#define SEQ_17720 ((Seq_80017720 *)context)

void func_80017720(SequenceContext *context, unsigned char program, unsigned char channel)
{
    if (channel != 9) {
        program = SEQ_17720->normTrans[program];
        if (program != 255) {
            SEQ_17720->prgState[channel] = ((u32)SEQ_17720->normtab[program].macro << 16) |
                ((u32)SEQ_17720->normtab[program].priority << 8) | SEQ_17720->normtab[program].maxVoices;
        }
    } else {
        program = SEQ_17720->drumTrans[program];
        if (program != 255) {
            SEQ_17720->prgState[channel] = ((u32)SEQ_17720->drumtab[program].macro << 16) |
                ((u32)SEQ_17720->drumtab[program].priority << 8) | SEQ_17720->drumtab[program].maxVoices;
        }
    }
}
