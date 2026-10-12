/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018634 (lib_17dc0, IDO 7.1): per-frame sequence track event handler (MusyX seq.c, the N64 ancestor of
 * GenerateNextTrackEvent/HandleTrackEvents): for each of the 64 event tracks of D_8004BE80 consume every track
 * event due by the track clock plus the look-ahead (+0x120): 0xFFFF ends the track; 0xFFFE loops (cursor cleared
 * if loops are disabled, +0xFC5; else jump to start[value], clock to the header's loop time and, on the first
 * loop of this call, func_80018B3C(loop time) and ++loop count +0xFC6); any other code starts pattern `code`
 * on the matching playback record (+0x568, 0x28 each), with the event's program/controller changes; then
 * advance the 16.16 track clock by the tempo increment (+0x118/+0x11C). Returns whether any track was active.
 * w21a lead_jump_v1.c -> w22c (force71: exactly code a3/patterns a1/pattern a2/optional04 t0) -> w23a.
 * Lever (w23a): ONE variable `index` holds both the 0xFFFE jump index and the default arm's pattern number
 * (`index = event->code; patterns[index]`). IDO 7.1 builds one web per C variable, so the jump web (v1) now
 * spans the default arm and v1 is forbidden to patterns/pattern; uopt still uses the CSE'd `event->code`
 * register (a3) for the index, so no instruction of the default-arm copy survives. `index` must be u32: a u16
 * makes 7.1 narrow the copy (`andi v1,a3,0xffff`, 18 rows).
 * Types: the TU's sequence_context.h (SequenceContext, D_8004BE80, disabledFC5, valueFC6, half120,
 * fraction118, whole11C); private suffixed views for what the header leaves unnamed: the sequence header
 * pointer at +0x10C (SequenceContextHead_80018634: a pointer-field view; a cast of unknown004 + 0x108 swaps the
 * operands of the two `optional + header` adds), the 64 event tracks at +0x128 (unknown128) and the playback
 * records at +0x568 (tracks568). No code from the views (0 differing rows).
 * Unused locals (disclosed, exact frame residual): `u1..u4`, four ints. Retail frame 88; without them 72
 * (spill71: cfe local area 36 -> 52 with them; `active`'s home stays sp+66 either way); 4 slots of 4 bytes.
 * No other devices, no pad arrays, no false prototypes (func_80018B3C is void(unsigned int) as defined later
 * in the TU; func_80017720/func_80017824 as declared/defined in the TU). */
#include "sequence_context.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SequenceEvent_80018634 {
    u32 time;
    u8 program;
    u8 controller;
    u16 unknown06;
    u16 code;
    union {
        u16 jump;
        struct { s8 first; s8 second; } bytes;
    } value;
} SequenceEvent_80018634;
typedef struct SequenceHeader_80018634 {
    u32 unknown00;
    u32 patterns;
    u32 channels;
    u32 unknown0C;
    u32 unknown10;
    u32 loop_time;
} SequenceHeader_80018634;
typedef struct SequencePattern_80018634 {
    u32 unknown00;
    u32 optional04;
    u32 optional08;
    u8 data[1];
} SequencePattern_80018634;
typedef struct SequenceEventTrack_80018634 {
    SequenceEvent_80018634 *start;
    SequenceEvent_80018634 *cursor;
    u32 fraction;
    u32 time;
} SequenceEventTrack_80018634;
typedef struct SequencePlayback_80018634 {
    u32 counter00;
    u32 counter04;
    u32 counter08;
    u8 *cursor;
    u8 *optional10;
    u8 *optional14;
    u16 value18;
    u16 value1A;
    u32 counter1C;
    u32 counter20;
    u8 channel;
    s8 second;
    s8 first;
    u8 index;
} SequencePlayback_80018634;
typedef struct SequenceContextHead_80018634 {
    u8 unknown000[0x10C];
    SequenceHeader_80018634 *sequence;
} SequenceContextHead_80018634;
#define HEADER (((SequenceContextHead_80018634 *)D_8004BE80)->sequence)
#define TRACK(i) (((SequenceEventTrack_80018634 *)D_8004BE80->unknown128)[i])
#define PLAY(i) (((SequencePlayback_80018634 *)D_8004BE80->tracks568)[i])
extern void func_80017720(SequenceContext *, unsigned char, unsigned char);
extern void func_80017824(u8, u8);
extern void func_80018B3C(unsigned int);
u8 func_80018634(void)
{
    int u1, u2, u3, u4; /* unused: frame residual, see header */
    int i;
    u8 first_loop;
    u8 active;
    SequenceEvent_80018634 *event;
    SequenceHeader_80018634 *sequence;
    u32 *patterns;
    SequencePattern_80018634 *pattern;
    u32 sum;
    u32 index;
    first_loop = 1;
    active = 0;
    for (i = 0; i < 64; i++) {
        if (TRACK(i).cursor != 0) {
            active = 1;
            while (TRACK(i).time + D_8004BE80->half120 >= TRACK(i).cursor->time) {
                event = TRACK(i).cursor;
                switch (event->code) {
                case 0xFFFF:
                    TRACK(i).cursor = 0;
                    goto next_track;
                case 0xFFFE:
                    if (D_8004BE80->disabledFC5 != 0) {
                        TRACK(i).cursor = 0;
                        goto next_track;
                    }
                    index = event->value.jump;
                    TRACK(i).cursor = TRACK(i).start + index;
                    TRACK(i).time = HEADER->loop_time;
                    TRACK(i).fraction = 0;
                    if (first_loop) {
                        first_loop = 0;
                        func_80018B3C(HEADER->loop_time);
                        D_8004BE80->valueFC6++;
                    }
                    break;
                default:
                    sequence = HEADER;
                    patterns = (u32 *)(sequence->patterns + (u32)sequence);
                    index = event->code;
                    pattern = (SequencePattern_80018634 *)(patterns[index] + (u32)sequence);
                    PLAY(i).counter04 = 0;
                    PLAY(i).counter00 = 0;
                    PLAY(i).cursor = pattern->data;
                    PLAY(i).counter08 = 0;
                    if (pattern->optional04) {
                        PLAY(i).optional10 = (u8 *)(pattern->optional04 + (u32)HEADER);
                    } else {
                        PLAY(i).optional10 = 0;
                    }
                    PLAY(i).counter1C = 0;
                    PLAY(i).value18 = 0x2000;
                    if (pattern->optional08) {
                        PLAY(i).optional14 = (u8 *)(pattern->optional08 + (u32)HEADER);
                    } else {
                        PLAY(i).optional14 = 0;
                    }
                    PLAY(i).counter20 = 0;
                    PLAY(i).value1A = 0;
                    sequence = HEADER;
                    PLAY(i).channel = ((u8 *)(sequence->channels + (u32)sequence))[i];
                    PLAY(i).second = event->value.bytes.second;
                    PLAY(i).first = event->value.bytes.first;
                    PLAY(i).index = i;
                    if (event->program != 0xFF) {
                        func_80017720(D_8004BE80, event->program, PLAY(i).channel);
                    }
                    if (event->controller != 0xFF) {
                        func_80017824(event->controller, PLAY(i).channel);
                    }
                    TRACK(i).cursor++;
                    break;
                }
            }
            sum = TRACK(i).fraction + D_8004BE80->fraction118;
            TRACK(i).fraction = sum & 0xFFFF;
            sum >>= 16;
            TRACK(i).time = TRACK(i).time + sum + D_8004BE80->whole11C;
        }
next_track:
        ;
    }
    return active;
}
