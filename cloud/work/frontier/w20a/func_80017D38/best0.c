/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef struct SequenceTime {u32 fraction,whole;} SequenceTime;
typedef struct SequenceNote {
    struct SequenceNote *next,*previous;
    u32 identifier,due;SequenceTime time;
} SequenceNote;
typedef struct SequenceRequest {
    u32 identifier;u16 duration;u8 unknown006[2];
    u32 nextIdentifier;u16 nextDuration;u8 unknown00E[2];
    void *stream;u16 group,program;u8 channel;u8 unknown019[3];
    u32 first,second;u16 value;u8 flags,unknown027;
} SequenceRequest;
typedef struct SequenceTrack {
    SequenceTime time;u32 event_time;
    u8 *events,*pitch,*modulation;
    u16 pitch_value,modulation_value;
    u32 pitch_time,modulation_time;
    u8 channel;s8 velocity_offset,transpose;u8 number;
} SequenceTrack;
typedef struct SequenceContext {
    u8 unknown000[272];u32 enabled[2];u8 unknown118[8];
    u32 lookahead;u8 unknown124[1028];u8 groups[64];
    SequenceTrack tracks[64];u8 unknownF68[24];u32 programs[16];
    u8 unknownFC0[8];SequenceRequest request;u32 *result;u8 pending;u8 unknownFF5[3];
} SequenceContext;
extern SequenceContext *D_8004BE80;
extern u32 D_8004BE78;
extern u8 D_8004BE7B,D_8004BE7C;
extern SequenceNote *func_800173B4(void);
extern void func_80017410(SequenceNote *);
extern void func_80017720(SequenceContext *,u8,u8);
extern void func_800177EC(u8,u8);
extern void func_80019490(SequenceRequest *,u32 *,u8);
extern void func_80020610(u8,u8,u8,u8);
extern u32 func_8001A270(u32,u8,u8,u8,u8,u8,u16,u16,u8,s16);
u8 func_80017D38(void)
{
    int i,advance,key,velocity,number;
    u8 channel,active;
    u32 due,program,identifier;
    SequenceNote *note;
    active=0;
    for (i=0;i<64;i++) {
        if (D_8004BE80->tracks[i].events!=0) active=1;
        while (D_8004BE80->tracks[i].events!=0) {
            due=D_8004BE80->tracks[i].event_time+*(u16 *)D_8004BE80->tracks[i].events;
            if (D_8004BE80->tracks[i].time.whole+D_8004BE80->lookahead<due) break;
            D_8004BE80->tracks[i].event_time=due;
                channel=D_8004BE80->tracks[i].channel;
            key=D_8004BE80->tracks[i].events[2];velocity=D_8004BE80->tracks[i].events[3];
            if (key==255 && velocity==255) {
                D_8004BE80->tracks[i].events=0;
            } else {
                advance=4;
                if ((key&128)==128 && velocity==0) {
                    func_80017720(D_8004BE80,key&127,channel);
                } else if ((key&128)==128 && velocity==1) {
                    func_800177EC(key&127,channel);
                } else if ((key&128)==128 && (velocity&128)==128) {
                    if ((velocity&127)==104) {
                        if (D_8004BE80->pending) {
                            func_80019490(&D_8004BE80->request,D_8004BE80->result,1);
                            D_8004BE80->pending=0;
                        }
                    } else {
                        func_80020610(velocity&127,channel,D_8004BE7B,key&127);
                    }
                } else if (key!=0 || velocity!=0) {
                    advance=6;
                    number=D_8004BE80->tracks[i].number;
                    if (D_8004BE80->enabled[number/32] & (1U<<(number&31))) {
                        program=D_8004BE80->programs[channel];
                        if (program!=0xFFFFFFFFU) {
                            key+=D_8004BE80->tracks[i].transpose;
                            key=key>127 ? 127 : key<0 ? 0 : key;
                            velocity+=D_8004BE80->tracks[i].velocity_offset;
                            velocity=velocity>127 ? 127 : velocity<0 ? 0 : velocity;
                            note=func_800173B4();
                            if (note!=0) {
                                identifier=func_8001A270(program,key,velocity,64,channel,
                                    D_8004BE78,0,number,D_8004BE80->groups[number],
                                    D_8004BE7C ? -1 : 0);
                                if (identifier!=0xFFFFFFFFU) {
                                    note->identifier=identifier;
                                    note->due=*(u16 *)(D_8004BE80->tracks[i].events+4)+due;
                                    note->time=D_8004BE80->tracks[i].time;
                                } else func_80017410(note);
                            }
                        }
                    }
                }
                D_8004BE80->tracks[i].events+=advance;
            }
            }
    }
    return active;
}
