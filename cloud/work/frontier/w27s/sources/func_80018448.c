/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018448 - lib_17dc0 (IDO 7.1). Per-frame service of every sequence track's SECOND
 * continuous-controller stream (track +0x14 cursor, +0x1A accumulated 14-bit value, +0x20 due
 * time): for each of the 64 tracks of the current context D_8004BE80, consume every event whose
 * due time (delta 7-bit, or 15-bit when the first byte has bit 7 set) has been reached by the
 * track clock plus the context look-ahead (+0x120 half120): read the signed delta that follows
 * (15-bit when bit 7 is set, else 7-bit), add it to the 16-bit value and send it as controllers
 * 1 (value >> 7) and 33 (value & 127) - MIDI modulation MSB/LSB - on the track channel (+0x24)
 * via func_80020610 with the low byte of D_8004BE78. The pair 0x80,0x00 ends the stream.
 * Sibling: func_8001824C (FIRST stream, +0x10/+0x18/+0x1C, controllers 128/129); same private
 * SequenceTrackNotes view of the header's tracks568 array and the same TRACK(i) macro.
 * w27s: the two delta reads build the 15-bit values from short operands,
 * `((short)(first & 127) << 8) | (short)second` and `((short)(first & 255) << 8) | (short)second`.
 * The (short) casts make cfe emit 16-bit narrowings of int-typed operands; 7.1 ugen evaluates
 * each into temps (sll/sra, and) that as1 deletes because the operands are already known
 * zero-extended bytes, so the temp ring advances exactly as retail's (w25f/w26b residual: the
 * hidden pops at lines 54, 59 and 62, proven at ucode level with uedit first). Semantically
 * neutral: `first & 255` on an unsigned char is the same no-op mask the sibling's
 * `(first & 255) << 8` uses (load-bearing, see func_8001824C). No unused locals, pad arrays,
 * implicit int or false prototypes; replaces w25f's `(u8)(first & 255) & 127` device. */
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
void func_80018448(void)
{
    int i;
    unsigned char done;
    unsigned char first,second;
    short change;
    unsigned int now,deadline;
    int width;
    for (i=0;i<64;i++) {
        done=0;
        if (TRACK(i).second14 != 0) {
            do {
                first=TRACK(i).second14[0];
                second=TRACK(i).second14[1];
                if (first==128 && second==0) {
                    TRACK(i).second14=0;
                    done=1;
                } else {
                    if (first & 128) {
                        change=((short)(first & 127) << 8) | (short)second;
                        width=2;
                    } else {
                        change=first;
                        width=1;
                    }
                    deadline=TRACK(i).secondTime20+change;
                    now=TRACK(i).whole+D_8004BE80->half120;
                    if (now >= deadline) {
                        TRACK(i).second14 += width;
                        TRACK(i).secondTime20=deadline;
                        first=TRACK(i).second14[0];
                        second=TRACK(i).second14[1];
                        if (first & 128) {
                            change=((short)(first & 255) << 8) | (short)second;
                            change <<= 1;
                            change >>= 1;
                            TRACK(i).second14 += 2;
                        } else {
                            change=(signed char)first;
                            ++TRACK(i).second14;
                        }
                        TRACK(i).second1A += change;
                        func_80020610(1,TRACK(i).channel24,(unsigned char)D_8004BE78,TRACK(i).second1A >> 7);
                        func_80020610(33,TRACK(i).channel24,(unsigned char)D_8004BE78,TRACK(i).second1A & 127);
                    } else {
                        done=1;
                    }
                }
            } while (!done);
        }
    }
}
