/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80017D38 - lib_17dc0 (IDO 7.1). Per-frame NOTE/EVENT stream service of the current sequence
 * context D_8004BE80: for each of the 64 tracks with an event cursor (+0x0C), consume every event
 * (u16 delta time, key byte, velocity byte[, u16 duration]) whose due time has been reached by the
 * track clock plus the look-ahead (+0x120). 0xFF,0xFF ends the stream; key bit 7 with velocity 0 =
 * program change (func_80017720), velocity 1 = func_800177EC, velocity bit 7 = controller
 * (velocity & 127; 104 fires the context's deferred request through func_80019490); otherwise a
 * note-on (6 bytes): if the track's group bit is enabled (+0x110 bitmask) and the channel has a
 * program (+0xF80), transpose/offset and clamp key and velocity to 0..127, allocate a note node
 * (func_800173B4), start the voice (func_8001A270) and record identifier, release time and the
 * track clock in the node (else free it, func_80017410). Returns 1 when any track was active.
 * Siblings: func_8001824C / func_80018448 (the two controller streams at +0x10/+0x14).
 * Types: the TU's sequence_context.h (SequenceContext, SequenceNode, D_8004BE80, D_8004BE78,
 * D_8004BE7C, func_800173B4/func_80017410 as defined in the TU). The header names the 0x28-byte
 * track and the +0xF68..+0xFF7 range only partly, so the context is read through a private view
 * (SequenceContextEvents, cast once in CTX: type view, no code; TU check identical); the
 * request at +0xFC8 is passed as `void *` like the TU's later func_80019490 prototype. The
 * node's clock pair (+0x10/+0x14) is copied as one 8-byte struct (SequenceTime_17D38 view):
 * two field stores give true=405. D_8004BE7B is the low byte of D_8004BE78 under its own
 * relocation symbol (the target relocates to D_8004BE7B). func_8001A270's prototype is
 * lib_1a660's.
 * Shaping devices (disclosed): `number & 255` for func_8001A270's u16 group argument (retail
 * `andi t2,s3,0xff`, 7.1 copy-then-narrow, as w19a func_80024988); the plain int gives true=310.
 * Unused locals = exact frame residual: retail frame 120 (`active` at sp+102, `advance` at
 * sp+88); without them the frame is 104 with `advance` at sp+92. Four ints declared first
 * (unused0..3, above `active`) and one between `active` and `advance` (unused4): 5 slots,
 * 20 bytes. Measured alternatives: 1-3 leading ints or no unused4 leave the frame or the offsets
 * off; two leading doubles + unused4 are also exact (3 locals, 20 bytes) - the int form was kept
 * as the less specific guess. Under 7.1 an unused or dead-store local takes a slot (measured).
 * w20a, from D10 BT03-high-sequence-events (7.1 true=715): due = event delta + event time
 * (operand order), locals `channel, active` before the ints, the two levers above. */
#include "sequence_context.h"
typedef struct SequenceTrackEvents {
    unsigned int fraction;
    int whole;
    unsigned int event_time;
    unsigned char *events, *pitch, *modulation;
    unsigned short pitch_value, modulation_value;
    unsigned int pitch_time, modulation_time;
    unsigned char channel;
    signed char velocity_offset, transpose;
    unsigned char number;
} SequenceTrackEvents;
typedef struct SequenceContextEvents {
    unsigned char unknown000[0x110];
    unsigned int enabled[2];
    unsigned char unknown118[8];
    unsigned int lookahead;
    unsigned char unknown124[0x404];
    unsigned char groups[64];
    SequenceTrackEvents tracks[64];
    unsigned char unknownF68[0x18];
    unsigned int programs[16];
    unsigned char unknownFC0[8];
    unsigned char request[0x28];
    unsigned int *result;
    unsigned char pending;
    unsigned char unknownFF5[3];
} SequenceContextEvents;
typedef struct SequenceTime_17D38 { unsigned int fraction, whole; } SequenceTime_17D38;
#define CTX ((SequenceContextEvents *)D_8004BE80)
extern unsigned char D_8004BE7B;
extern SequenceNode *func_800173B4(void);
extern void func_80017410(SequenceNode *);
extern void func_80017720(SequenceContext *, unsigned char, unsigned char);
extern void func_800177EC(unsigned char, unsigned char);
extern void func_80019490(void *, unsigned int *, unsigned char);
extern void func_80020610(unsigned char, unsigned char, unsigned char, unsigned char);
extern unsigned int func_8001A270(unsigned int, unsigned char, unsigned char, unsigned char, unsigned char,
                                  unsigned char, unsigned short, unsigned short, unsigned char, short);
unsigned char func_80017D38(void)
{
    int unused0, unused1, unused2, unused3; /* frame residual, see header */
    unsigned char channel,active;
    int unused4;                            /* frame residual, see header */
    int i,advance,key,velocity,number;
    unsigned int due,program,identifier;
    SequenceNode *note;
    active=0;
    for (i=0;i<64;i++) {
        if (CTX->tracks[i].events!=0) active=1;
        while (CTX->tracks[i].events!=0) {
            due=*(unsigned short *)CTX->tracks[i].events+CTX->tracks[i].event_time;
            if (CTX->tracks[i].whole+CTX->lookahead<due) break;
            CTX->tracks[i].event_time=due;
            channel=CTX->tracks[i].channel;
            key=CTX->tracks[i].events[2];velocity=CTX->tracks[i].events[3];
            if (key==255 && velocity==255) {
                CTX->tracks[i].events=0;
            } else {
                advance=4;
                if ((key&128)==128 && velocity==0) {
                    func_80017720(D_8004BE80,key&127,channel);
                } else if ((key&128)==128 && velocity==1) {
                    func_800177EC(key&127,channel);
                } else if ((key&128)==128 && (velocity&128)==128) {
                    if ((velocity&127)==104) {
                        if (CTX->pending) {
                            func_80019490(CTX->request,CTX->result,1);
                            CTX->pending=0;
                        }
                    } else {
                        func_80020610(velocity&127,channel,D_8004BE7B,key&127);
                    }
                } else if (key!=0 || velocity!=0) {
                    advance=6;
                    number=CTX->tracks[i].number;
                    if (CTX->enabled[number/32] & (1U<<(number&31))) {
                        program=CTX->programs[channel];
                        if (program!=0xFFFFFFFFU) {
                            key+=CTX->tracks[i].transpose;
                            key=key>127 ? 127 : key<0 ? 0 : key;
                            velocity+=CTX->tracks[i].velocity_offset;
                            velocity=velocity>127 ? 127 : velocity<0 ? 0 : velocity;
                            note=func_800173B4();
                            if (note!=0) {
                                identifier=func_8001A270(program,key,velocity,64,channel,
                                    D_8004BE78,0,number & 255,CTX->groups[number],
                                    D_8004BE7C ? -1 : 0);
                                if (identifier!=0xFFFFFFFFU) {
                                    note->identifier=identifier;
                                    note->deadline0C=*(unsigned short *)(CTX->tracks[i].events+4)+due;
                                    *(SequenceTime_17D38 *)&note->fraction10=*(SequenceTime_17D38 *)&CTX->tracks[i].fraction;
                                } else func_80017410(note);
                            }
                        }
                    }
                }
                CTX->tracks[i].events+=advance;
            }
        }
    }
    return active;
}
