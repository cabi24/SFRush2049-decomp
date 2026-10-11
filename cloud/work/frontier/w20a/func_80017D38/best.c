/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
    int u1,u2,u3,u4;
    unsigned char channel,active;
    int x;
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
