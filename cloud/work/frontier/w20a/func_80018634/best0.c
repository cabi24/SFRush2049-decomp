/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SequenceEvent {
    u32 time;
    u8 program;
    u8 controller;
    u16 unknown06;
    u16 code;
    union {
        u16 jump;
        struct { s8 first; s8 second; } bytes;
    } value;
} SequenceEvent;
typedef struct SequenceHeader {
    u32 unknown00;
    u32 patterns;
    u32 channels;
    u32 unknown0C;
    u32 unknown10;
    u32 loop_time;
} SequenceHeader;
typedef struct Pattern {
    u32 unknown00;
    u32 optional04;
    u32 optional08;
    u8 data[1];
} Pattern;
typedef struct Track {
    SequenceEvent *start;
    SequenceEvent *cursor;
    u32 fraction;
    u32 time;
} Track;
typedef struct Playback {
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
} Playback;
typedef struct Context {
    u8 unknown000[0x10C];
    SequenceHeader *sequence;
    u32 unknown110;
    u32 unknown114;
    u32 step_fraction;
    u32 step_time;
    u32 lookahead;
    u32 unknown124;
    Track tracks[64];
    u8 unknown528[0x40];
    Playback playback[64];
    u8 unknownF68[0x5D];
    u8 stop_loop;
    u16 loop_count;
} Context;
extern Context *D_8004BE80;
extern void func_80017720(Context *, u8, u8);
extern void func_80017824(u8, u8);
extern void func_80018B3C(u32);
u8 func_80018634(void)
{
    int i;
    u8 first_loop;
    u8 active;
    SequenceEvent *event;
    SequenceHeader *sequence;
    u32 *patterns;
    Pattern *pattern;
    u32 sum;
    first_loop = 1;
    active = 0;
    for (i = 0; i < 64; i++) {
        if (D_8004BE80->tracks[i].cursor != 0) {
            active = 1;
            while (D_8004BE80->tracks[i].time + D_8004BE80->lookahead >=
                   D_8004BE80->tracks[i].cursor->time) {
                event = D_8004BE80->tracks[i].cursor;
                switch (event->code) {
                case 0xFFFF:
                    D_8004BE80->tracks[i].cursor = 0;
                    goto next_track;
                case 0xFFFE:
                    if (D_8004BE80->stop_loop != 0) {
                        D_8004BE80->tracks[i].cursor = 0;
                        goto next_track;
                    }
                    D_8004BE80->tracks[i].cursor = D_8004BE80->tracks[i].start + event->value.jump;
                    D_8004BE80->tracks[i].time = D_8004BE80->sequence->loop_time;
                    D_8004BE80->tracks[i].fraction = 0;
                    if (first_loop) {
                        first_loop = 0;
                        func_80018B3C(D_8004BE80->sequence->loop_time);
                        D_8004BE80->loop_count++;
                    }
                    break;
                default:
                    sequence = D_8004BE80->sequence;
                    patterns = (u32 *)((u8 *)sequence + sequence->patterns);
                    pattern = (Pattern *)((u8 *)sequence + patterns[event->code]);
                    D_8004BE80->playback[i].counter04 = 0;
                    D_8004BE80->playback[i].counter00 = 0;
                    D_8004BE80->playback[i].cursor = pattern->data;
                    D_8004BE80->playback[i].counter08 = 0;
                    if (pattern->optional04) {
                        D_8004BE80->playback[i].optional10 = (u8 *)D_8004BE80->sequence + pattern->optional04;
                    } else {
                        D_8004BE80->playback[i].optional10 = 0;
                    }
                    D_8004BE80->playback[i].counter1C = 0;
                    D_8004BE80->playback[i].value18 = 0x2000;
                    if (pattern->optional08) {
                        D_8004BE80->playback[i].optional14 = (u8 *)D_8004BE80->sequence + pattern->optional08;
                    } else {
                        D_8004BE80->playback[i].optional14 = 0;
                    }
                    D_8004BE80->playback[i].counter20 = 0;
                    D_8004BE80->playback[i].value1A = 0;
                    sequence = D_8004BE80->sequence;
                    D_8004BE80->playback[i].channel = ((u8 *)sequence + sequence->channels)[i];
                    D_8004BE80->playback[i].second = event->value.bytes.second;
                    D_8004BE80->playback[i].first = event->value.bytes.first;
                    D_8004BE80->playback[i].index = i;
                    if (event->program != 0xFF) {
                        func_80017720(D_8004BE80, event->program, D_8004BE80->playback[i].channel);
                    }
                    if (event->controller != 0xFF) {
                        func_80017824(event->controller, D_8004BE80->playback[i].channel);
                    }
                    D_8004BE80->tracks[i].cursor++;
                    break;
                }
            }
            sum = D_8004BE80->tracks[i].fraction + D_8004BE80->step_fraction;
            D_8004BE80->tracks[i].fraction = sum & 0xFFFF;
            D_8004BE80->tracks[i].time += (sum >> 16) + D_8004BE80->step_time;
        }
next_track:
        ;
    }
    return active;
}
