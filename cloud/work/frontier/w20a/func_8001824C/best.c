/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001824C - lib_17dc0 (IDO 7.1). Per-frame service of every sequence track's FIRST
 * continuous-controller stream (track +0x10 cursor, +0x18 accumulated 14-bit value, +0x1C due
 * time): for each of the 64 tracks of the current context D_8004BE80, consume every event whose
 * due time (delta 7-bit, or 15-bit when the first byte has bit 7 set) has been reached by the
 * track clock plus the context look-ahead (+0x120 half120): read the signed delta that follows
 * (15-bit when bit 7 is set, else 7-bit), add it to the 16-bit value and send it as controllers
 * 128 (value >> 7) and 129 (value & 127) on the track channel (+0x24) via func_80020610 with
 * the low byte of D_8004BE78. The pair 0x80,0x00 ends the stream (cursor cleared).
 * Sibling: func_80018448 (SECOND stream, +0x14/+0x1A/+0x20).
 * Types: the TU's sequence_context.h (SequenceContext, D_8004BE80, D_8004BE78). The header's
 * SequenceTrack names +0x10..+0x27 only as active10/active14/unknown18[16]; this file reads
 * them through a private 0x28-byte view (SequenceTrackNotes: first10/second14 cursors,
 * first18/second1A values, firstTime1C/secondTime20 due times, channel24) by casting the
 * header's tracks568 array (type view, no code: TU check identical). A header patch naming
 * those fields would let the view go; not needed for the match.
 * func_80020610's prototype is lib_207b0's/lib_1a660's (four unsigned char).
 * Shaping device (disclosed): `(first & 255) << 8` in the 15-bit delta. `first` is already an
 * unsigned char, so the mask is a no-op; under 7.1 it makes ugen copy `first` into a temp
 * before the shift (no instruction of it survives in the object), which is exactly the one-register advance of the
 * temp ring retail shows from that point on (w19a residual true=80: +1/+2 cursor steps and
 * the reload temps one register late). Same 7.1 copy-then-narrow idiom as w19a func_80024988.
 * s16 truncation in steps (`change <<= 1; change >>= 1`, w19a) and `now` after `deadline`
 * (w19a) kept. w19a best.c (from D10 BT03-high-runtime-followon) -> w20a. No unused locals,
 * no pad arrays, no false prototypes. */
#include "sequence_context.h"
typedef struct SequenceTrackNotes {
    unsigned int fraction;
    int whole;
    unsigned int unknown08;
    unsigned char *notes0C, *first10, *second14;
    unsigned short first18, second1A;
    unsigned int firstTime1C, secondTime20;
    unsigned char channel24, unknown25[3];
} SequenceTrackNotes;
#define TRACK(i) (((SequenceTrackNotes *)D_8004BE80->tracks568)[i])
extern void func_80020610(unsigned char, unsigned char, unsigned char, unsigned char);
void func_8001824C(void)
{
    int i;
    unsigned char done;
    unsigned char first,second;
    unsigned short increment;
    short change;
    unsigned int now,deadline;
    int width;
    for (i=0;i<64;i++) {
        done=0;
        if (TRACK(i).first10 != 0) {
            do {
                first=TRACK(i).first10[0];
                second=TRACK(i).first10[1];
                if (first==128 && second==0) {
                    TRACK(i).first10=0;
                    done=1;
                } else {
                    if (first & 128) {
                        increment=((first & 127) << 8) | second;
                        width=2;
                    } else {
                        increment=first;
                        width=1;
                    }
                    deadline=TRACK(i).firstTime1C+increment;
                    now=TRACK(i).whole+D_8004BE80->half120;
                    if (now >= deadline) {
                        TRACK(i).first10 += width;
                        TRACK(i).firstTime1C=deadline;
                        first=TRACK(i).first10[0];
                        second=TRACK(i).first10[1];
                        if (first & 128) {
                            change=((first & 255) << 8) | second;
                            change <<= 1;
                            change >>= 1;
                            TRACK(i).first10 += 2;
                        } else {
                            change=first;
                            change <<= 9;
                            change >>= 9;
                            ++TRACK(i).first10;
                        }
                        TRACK(i).first18 += change;
                        func_80020610(128,TRACK(i).channel24,(unsigned char)D_8004BE78,TRACK(i).first18 >> 7);
                        func_80020610(129,TRACK(i).channel24,(unsigned char)D_8004BE78,TRACK(i).first18 & 127);
                    } else {
                        done=1;
                    }
                }
            } while (!done);
        }
    }
}
