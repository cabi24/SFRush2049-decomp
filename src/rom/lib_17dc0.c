/* GENERATED ROM-aligned TU — segment 0x17dc0 (rom/lib_17dc0)
 * layout map 8baa7d85ab5236a98d6d20c261b8e0ad26e5b810082eeb29bf486ec2aa49ae83; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"
#include "sequence_context.h"

/* PROMOTED 2026-10-05 — func_800171C0
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800171C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800171C0.c:func_800171C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceNode D_800426B0[256];
void func_800171C0(void)
{
    SequenceNode *previous;
    int i;
    previous = 0;
    D_80043EB0 = D_800426B0;
    for (i = 0; i < 256; i++) {
        D_800426B0[i].previous = previous;
        if (previous != 0) {
            previous->next = &D_800426B0[i];
        }
        previous = &D_800426B0[i];
    }
    previous->next = 0;
}

/* PROMOTED 2026-10-04 — func_8001729C
 * Source:   cloud/matches/boot_tail/func_8001729C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001729C.c:func_8001729C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceNode *D_80043EB0;
extern SequenceContext *D_8004BE80;
void func_8001729C(SequenceContext *state)
{
    SequenceNode *node;
    node = state->active;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->active;
        state->active = 0;
    }
    node = state->pending;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->pending;
        state->pending = 0;
    }
}

/* PROMOTED 2026-10-04 — func_8001734C
 * Source:   cloud/matches/boot_tail/func_8001734C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001734C.c:func_8001734C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001FA18(u32);
void func_8001734C(SequenceContext *context)
{
    SequenceNode *entry;
    entry = context->active;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
    entry = context->pending;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
}

/* PROMOTED 2026-10-04 — func_800173B4
 * Source:   cloud/matches/boot_tail/func_800173B4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800173B4.c:func_800173B4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
SequenceNode *func_800173B4(void)
{
    SequenceNode *node;
    SequenceNode *head;
    node = D_80043EB0;
    if (node != 0) {
        D_80043EB0 = node->next;
        if (D_80043EB0 != 0) {
            D_80043EB0->previous = 0;
        }
        node->previous = 0;
        head = D_8004BE80->active;
        node->next = head;
        if (head != 0) {
            D_8004BE80->active->previous = node;
        }
        D_8004BE80->active = node;
    }
    return node;
}

/* PROMOTED 2026-10-04 — func_80017410
 * Source:   cloud/matches/boot_tail/func_80017410.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017410.c:func_80017410 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017410(SequenceNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    node->next = D_80043EB0;
    if (node->next != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}

/* PROMOTED 2026-10-04 — func_80017470
 * Source:   cloud/matches/boot_tail/func_80017470.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017470.c:func_80017470 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017470(SequenceNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->pending = node->next;
    }
    node->next = D_80043EB0;
    if (node->next != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}

/* PROMOTED 2026-10-04 — func_800174D0
 * Source:   cloud/matches/boot_tail/func_800174D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800174D0.c:func_800174D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800174D0(SequenceNode *node)
{
    SequenceNode *head;
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    head = D_8004BE80->pending;
    node->next = head;
    if (head != 0) {
        D_8004BE80->pending->previous = node;
    }
    node->previous = 0;
    D_8004BE80->pending = node;
}

/* PROMOTED 2026-10-05 — func_80017540
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80017540.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80017540.c:func_80017540 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017540(void)
{
    SequenceNode *entry;
    SequenceNode *next;
    entry = D_8004BE80->pending;
    while (entry != 0) {
        next = entry->next;
        if (func_800201D0(entry->identifier) == 0xFFFFFFFFU) func_80017470(entry);
        entry = next;
    }
}

/* PROMOTED 2026-10-04 — func_800175A8
 * Source:   cloud/matches/boot_tail/func_800175A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800175A8.c:func_800175A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8004BE84;
void func_800175A8(void)
{
    D_8004BE84 = 0;
}

/* PROMOTED 2026-10-05 — func_800175B4
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800175B4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800175B4.c:func_800175B4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned int func_800175B4(unsigned int slot)
{
    unsigned int identifier;
    int i;
    do {
        identifier = D_8004BE84++;
        D_8004BE84 &= 0x7FFFFFFFU;
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].inactiveFC1 == 0 &&
                D_80043EB8[i].identifier == identifier) {
                identifier = 0xFFFFFFFFU;
                break;
            }
        }
    } while (identifier == 0xFFFFFFFFU);
    D_80043EB8[slot].identifier = identifier;
    return identifier;
}

/* PROMOTED 2026-10-04 — func_80017644
 * Source:   cloud/matches/boot_tail/func_80017644.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017644.c:func_80017644 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceContext D_80043EB8[8];
u32 func_80017644(u32 identifier)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (D_80043EB8[i].inactiveFC1 == 0 &&
            D_80043EB8[i].identifier == (identifier & 0x7FFFFFFFU)) {
            return (identifier & 0x80000000U) | i;
        }
    }
    return 0xFFFFFFFFU;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80017720.s")
/* PROMOTED 2026-10-11 — func_800177EC
 * Source:   cloud/work/frontier/w20b/sources/func_800177EC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_800177EC.c:func_800177EC (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_8004BE7B;
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);
void func_800177EC(u8 value, u8 channel)
{
    func_80020610(130, channel, D_8004BE7B, value);
}

/* PROMOTED 2026-10-11 — func_80017824
 * Source:   cloud/work/frontier/w20b/sources/func_80017824.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_80017824.c:func_80017824 (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017824(u8 value, u8 channel)
{
    func_80020610(7, channel, D_8004BE7B, value);
}

/* PROMOTED 2026-10-11 — func_8001785C
 * Source:   cloud/work/frontier/w17g/sources/func_8001785C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001785C.c:func_8001785C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MapRecord { u8 unknown00[5]; u8 key; u8 unknown06[2]; } MapRecord;
void func_8001785C(u8 *map, MapRecord *record)
{
    u8 index;
    for (index = 0; index < 128; index++) map[index] = 255;
    index = 0;
    while (record->key != 255) {
        map[record->key] = index;
        index++;
        record++;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_800178B0.s")
/* PROMOTED 2026-10-11 — func_80017D38
 * Source:   cloud/work/frontier/w20a/sources/func_80017D38.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20a/sources/func_80017D38.c:func_80017D38 (score0); rom_tu.h context: cloud/work/frontier/w20a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SequenceTrackEvents { unsigned int fraction; int whole; unsigned int event_time; unsigned char *events, *pitch, *modulation; unsigned short pitch_value, modulation_value; unsigned int pitch_time, modulation_time; unsigned char channel; signed char velocity_offset, transpose; unsigned char number; } SequenceTrackEvents;
typedef struct SequenceContextEvents { unsigned char unknown000[0x110]; unsigned int enabled[2]; unsigned char unknown118[8]; unsigned int lookahead; unsigned char unknown124[0x404]; unsigned char groups[64]; SequenceTrackEvents tracks[64]; unsigned char unknownF68[0x18]; unsigned int programs[16]; unsigned char unknownFC0[8]; unsigned char request[0x28]; unsigned int *result; unsigned char pending; unsigned char unknownFF5[3]; } SequenceContextEvents;
typedef struct SequenceTime_17D38 { unsigned int fraction, whole; } SequenceTime_17D38;
#define CTX ((SequenceContextEvents *)D_8004BE80)
extern unsigned char D_8004BE7B;
extern SequenceNode *func_800173B4(void);
extern void func_80017410(SequenceNode *);
extern void func_80017720(SequenceContext *, unsigned char, unsigned char);
extern void func_800177EC(unsigned char, unsigned char);
extern void func_80019490(void *, unsigned int *, unsigned char);
extern void func_80020610(unsigned char, unsigned char, unsigned char, unsigned char);
extern unsigned int func_8001A270(unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short, unsigned short, unsigned char, short);
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

/* PROMOTED 2026-10-11 — func_800180A0
 * Source:   cloud/work/frontier/w18c/sources/func_800180A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_800180A0.c:func_800180A0 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800180A0(void)
{
    int i;
    int sum;
    for (i = 0; i < 64; i++) {
        if (D_8004BE80->tracks568[i].active0C || D_8004BE80->tracks568[i].active10 ||
            D_8004BE80->tracks568[i].active14) {
            sum = (int)(D_8004BE80->tracks568[i].fraction + D_8004BE80->fraction118);
            D_8004BE80->tracks568[i].fraction = (unsigned int)sum & 65535;
            sum >>= 16;
            D_8004BE80->tracks568[i].whole = (int)((unsigned int)D_8004BE80->tracks568[i].whole +
                (unsigned int)sum + (unsigned int)D_8004BE80->whole11C);
        }
    }
}

/* PROMOTED 2026-10-11 — func_80018184
 * Source:   cloud/work/frontier/w18c/sources/func_80018184.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80018184.c:func_80018184 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B8C4(unsigned int);
int func_80018184(void)
{
    SequenceNode *node;
    SequenceNode *next;
    int sum;
    if (D_8004BE80->active == 0) return 0;
    node = D_8004BE80->active;
    while (node != 0) {
        next = node->next;
        sum = (int)((unsigned int)node->whole14 + (unsigned int)D_8004BE80->half120);
        if (sum >= node->deadline0C) {
            func_8001B8C4(node->identifier);
            func_800174D0(node);
        } else {
            sum = (int)(node->fraction10 + D_8004BE80->fraction118);
            node->fraction10 = (unsigned int)sum & 65535;
            sum >>= 16;
            node->whole14 = (int)((unsigned int)node->whole14 + (unsigned int)sum +
                (unsigned int)D_8004BE80->whole11C);
        }
        node = next;
    }
    return 1;
}

/* PROMOTED 2026-10-11 — func_8001824C
 * Source:   cloud/work/frontier/w20a/sources/func_8001824C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20a/sources/func_8001824C.c:func_8001824C (score0); rom_tu.h context: cloud/work/frontier/w20a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SequenceTrackNotes { unsigned int fraction; int whole; unsigned int unknown08; unsigned char *notes0C, *first10, *second14; unsigned short first18, second1A; unsigned int firstTime1C, secondTime20; unsigned char channel24, unknown25[3]; } SequenceTrackNotes;
#define TRACK(i) (((SequenceTrackNotes *)D_8004BE80->tracks568)[i])
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018448.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018634.s")
/* PROMOTED 2026-10-11 — func_8001897C
 * Source:   cloud/work/frontier/w18c/sources/func_8001897C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001897C.c:func_8001897C (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int D_8004F800;
void func_8001897C(void)
{
    unsigned int value;
    value = ((D_8004BE80->rate124 * 3072U / 60U) *
        (unsigned int)((int)((unsigned int)D_8004F808 << 16) / D_8004F800)) >> 3;
    value = (D_8004BE80->valueFC2 * value) >> 8;
    D_8004BE80->fraction118 = value & 65535;
    D_8004BE80->whole11C = value >> 16;
    D_8004BE80->half120 = D_8004BE80->whole11C >> 1;
}

/* PROMOTED 2026-10-05 — func_80018A30
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018A30.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018A30.c:func_80018A30 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018A30(void)
{
    if (D_8004BE80->timingStartF68) {
        while (D_8004BE80->currentF6C->time != 0xFFFFFFFFU) {
            if (D_8004BE80->highF74 + D_8004BE80->half120 <
                D_8004BE80->currentF6C->time) break;
            func_80019A60(D_8004BE80->rate124 =
                D_8004BE80->currentF6C->value, D_8004BE78);
            D_8004BE80->currentF6C++;
        }
    }
}

/* PROMOTED 2026-10-11 — func_80018AEC
 * Source:   cloud/work/frontier/w18c/sources/func_80018AEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80018AEC.c:func_80018AEC (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018AEC(void)
{
    unsigned int value;
    if (D_8004BE80->timingStartF68) {
        value = D_8004BE80->lowF70 + D_8004BE80->fraction118;
        D_8004BE80->lowF70 = value & 0xFFFF;
        value >>= 16;
        D_8004BE80->highF74 = D_8004BE80->highF74 + value + D_8004BE80->whole11C;
    }
}

/* PROMOTED 2026-10-05 — func_80018B3C
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018B3C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018B3C.c:func_80018B3C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018B3C(unsigned int value)
{
    if (D_8004BE80->timingStartF68) {
        D_8004BE80->currentF6C = D_8004BE80->timingStartF68;
        D_8004BE80->highF74 = value;
        D_8004BE80->lowF70 = 0;
        func_80018A30();
        func_8001897C();
    }
}

/* PROMOTED 2026-10-05 — func_80018B8C
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018B8C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018B8C.c:func_80018B8C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_hoisted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
unsigned short func_80018B8C(unsigned int identifier)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF && (index & 0x80000000) == 0) return D_80043EB8[index].valueFC6;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80018BEC
 * Source:   cloud/work/frontier/w17g/sources/func_80018BEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80018BEC.c:func_80018BEC (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_8002C630;
extern u32 func_80017644(u32);
u8 func_80018BEC(u32 identifier)
{
    if (D_8002C630) return func_80017644(identifier) != 0xFFFFFFFF;
    return 0;
}

/* PROMOTED 2026-10-05 — func_80018C2C
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018C2C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018C2C.c:func_80018C2C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018C2C(unsigned int identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].flagsFEE |= 8;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018D00
 * Source:   cloud/matches/boot_tail/func_80018D00.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018D00.c:func_80018D00 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80018C2C(unsigned int);
void func_80018D00(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018C2C(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018D40
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018D40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018D40.c:func_80018D40 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018D40(unsigned int identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
            D_80043EB8[identifier].inactiveFC1 = 1;
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].pendingFF0 = 0;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018E2C
 * Source:   cloud/matches/boot_tail/func_80018E2C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018E2C.c:func_80018E2C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018D40(unsigned int);
void func_80018E2C(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018D40(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018E6C
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018E6C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018E6C.c:func_80018E6C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018E6C(void)
{
    int i;
    for (i = 0; i < 8; i++) {
        func_80018D40(D_80043EB8[i].identifier);
    }
}

/* PROMOTED 2026-10-05 — func_80018EB4
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018EB4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018EB4.c:func_80018EB4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_hoisted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_80018EB4(unsigned int identifier, unsigned char enabled)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF && (index & 0x80000000) == 0) {
            D_80043EB8[index].disabledFC5 = !enabled;
            return 1;
        }
    }
    return 0;
}

/* PROMOTED 2026-10-05 — func_80018F20
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018F20.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018F20.c:func_80018F20 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018F20(unsigned int identifier, unsigned short value)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].valueFC2 = value;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].valueFEC = value;
            D_80043EB8[index].flagsFEE |= 0x20;
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018FA4
 * Source:   cloud/matches/boot_tail/func_80018FA4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018FA4.c:func_80018FA4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018F20(unsigned int, unsigned short);
void func_80018FA4(unsigned int identifier, unsigned short value)
{
    if (D_8002C630) {
        func_80014594();
        func_80018F20(identifier, value);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018FEC
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018FEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018FEC.c:func_80018FEC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018FEC(unsigned int identifier)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].activeFC0 = 1;
        } else {
            D_80043EB8[index & 0x7FFFFFFFU].flagsFEE &= ~8;
        }
    }
}

/* PROMOTED 2026-10-04 — func_8001906C
 * Source:   cloud/matches/boot_tail/func_8001906C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001906C.c:func_8001906C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018FEC(unsigned int);
void func_8001906C(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018FEC(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_800190AC
 * Source:   cloud/matches/boot_tail/func_800190AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800190AC.c:func_800190AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u32 func_80017644(u32);
void func_800190AC(u32 identifier, u32 first, u32 second)
{
    u32 index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].first110 = first;
            D_80043EB8[index].second114 = second;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].firstFE4 = first;
            D_80043EB8[index].secondFE8 = second;
            D_80043EB8[index].flagsFEE |= 0x10;
        }
    }
}

/* PROMOTED 2026-10-04 — func_80019144
 * Source:   cloud/matches/boot_tail/func_80019144.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019144.c:func_80019144 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800190AC(unsigned int, unsigned int, unsigned int);
void func_80019144(unsigned int identifier, unsigned int first, unsigned int second)
{
    if (D_8002C630) {
        func_80014594();
        func_800190AC(identifier, first, second);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80019194
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80019194.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80019194.c:func_80019194 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80019194(unsigned char channel,unsigned short duration,unsigned int identifier,unsigned char flags)
{
    unsigned int original;
    int i;
    original=identifier;
    identifier=func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if (!(identifier & 0x80000000U)) {
            func_8001B9F8(channel,duration,D_80043EB8[identifier].channelFC4,flags,original);
            for (i=0;i<64;i++) {
                if (D_80043EB8[identifier].channels528[i] != D_80043EB8[identifier].channelFC4) {
                    func_8001B9F8(channel,duration,D_80043EB8[identifier].channels528[i],0,0);
                }
            }
        } else {
            identifier &= 0x7FFFFFFFU;
            switch (flags & 15) {
            case 0: D_80043EB8[identifier].valueFE0=channel; break;
            case 1: D_80043EB8[identifier].pendingFF0=0; break;
            case 2:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 8;
                break;
            case 3:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 128;
                break;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80019370
 * Source:   cloud/matches/boot_tail/func_80019370.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019370.c:func_80019370 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80019194(unsigned char, unsigned short, unsigned int, unsigned char);
void func_80019370(unsigned char value, unsigned short duration, unsigned int identifier, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_80019194(value, duration, identifier, mode);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_800193C8
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800193C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800193C8.c:func_800193C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_800193C8(unsigned int identifier)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) return (*(SequenceContext *)((unsigned int)D_80043EB8 +
                index * sizeof(SequenceContext))).channelFC4;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80019420
 * Source:   cloud/work/frontier/w18c/sources/func_80019420.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80019420.c:func_80019420 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_800443E0[][sizeof(SequenceContext)];
extern void func_8001C19C(unsigned char, unsigned char);
void func_80019420(unsigned int identifier, unsigned char selector, unsigned char channel)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) {
            D_800443E0[index][selector] = channel;
            func_8001C19C(channel, 2);
        }
    }
}

/* PROMOTED 2026-10-11 — func_80019490
 * Source:   cloud/work/frontier/w19a/sources/func_80019490.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_80019490.c:func_80019490 (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SequenceRequest { unsigned int identifier; unsigned short duration; unsigned char unknown006[2]; unsigned int nextIdentifier; unsigned short nextDuration; unsigned char unknown00E[2]; void *stream; unsigned short group, program; unsigned char channel; unsigned char unknown019[3]; unsigned int first, second; unsigned short value; unsigned char flags; unsigned char unknown027; } SequenceRequest;
typedef struct SequenceOptions { unsigned int flags, first, second; unsigned short value, duration; unsigned char channel; unsigned char unknown011; unsigned char mapCount; unsigned char unknown013; void *map; unsigned char channelCount; unsigned char unknown019[3]; void *channels; } SequenceOptions;
extern void func_8001906C(unsigned int);
extern void func_80019144(unsigned int, unsigned int, unsigned int);
extern void func_80018FA4(unsigned int, unsigned short);
extern void func_80019370(unsigned char, unsigned short, unsigned int, unsigned char);
extern int func_8001558C(unsigned short, unsigned short, unsigned int, unsigned int, unsigned char);
extern int func_800156E8(unsigned short, unsigned short, unsigned int, unsigned int);
void func_80019490(void *request, unsigned int *result, unsigned char unlocked)
{
    SequenceOptions options;
    unsigned int index;
    SequenceRequest *state = request;
    index = func_80017644(state->identifier);
    if (index != 0xFFFFFFFFU) {
        if (state->flags & 4) {
            *(SequenceRequest *)D_80043EB8[index].unknownFC8 = *state;
            D_80043EB8[index].unknownFF4[0] = 1;
            D_80043EB8[index].flagsFEE &= ~4;
            D_80043EB8[index].pendingFF0 = (unsigned int)result;
            *result = state->identifier | 0x80000000U;
        } else {
            if (unlocked) {
                if (state->flags & 1) func_80019194(0,state->duration,state->identifier,2);
                else if (state->flags & 0x40) func_80019194(0,state->duration,state->identifier,3);
                else func_80019194(0,state->duration,state->identifier,1);
            } else {
                if (state->flags & 1) func_80019370(0,state->duration,state->identifier,2);
                else if (state->flags & 0x40) func_80019370(0,state->duration,state->identifier,3);
                else func_80019370(0,state->duration,state->identifier,1);
            }
            if (result != 0) {
                if (state->flags & 2) {
                    if (func_80017644(state->nextIdentifier) != 0xFFFFFFFFU) {
                        if (unlocked) {
                            func_80018FEC(state->nextIdentifier);
                            func_80019194(state->channel,state->nextDuration,state->nextIdentifier,0);
                            if (state->flags & 0x10) func_800190AC(state->nextIdentifier,state->first,state->second);
                            if (state->flags & 0x20) func_80018F20(state->nextIdentifier,state->value);
                        } else {
                            func_8001906C(state->nextIdentifier);
                            func_80019370(state->channel,state->nextDuration,state->nextIdentifier,0);
                            if (state->flags & 0x10) func_80019144(state->nextIdentifier,state->first,state->second);
                            if (state->flags & 0x20) func_80018FA4(state->nextIdentifier,state->value);
                        }
                        *result = state->nextIdentifier;
                    } else *result = 0xFFFFFFFFU;
                } else {
                    /* Only fields enabled by flags/counts are consumed by
                     * the constructor; unused option fields stay untouched. */
                    options.flags = 4;
                    if (state->flags & 8) options.flags = 0x14;
                    if (state->flags & 0x20) { options.flags |= 2; options.value = state->value; }
                    if (state->flags & 0x10) { options.flags |= 1; options.first = state->first; options.second = state->second; }
                    options.duration = state->nextDuration;
                    options.channel = state->channel;
                    options.channelCount = 0;
                    if (unlocked) {
                        *result = func_8001558C(state->group,state->program,(unsigned int)state->stream,(unsigned int)&options,1);
                        if (*result != 0xFFFFFFFFU && (state->flags & 0x80)) func_800190AC(*result,0,0);
                    } else {
                        *result = func_800156E8(state->group,state->program,(unsigned int)state->stream,(unsigned int)&options);
                        if (*result != 0xFFFFFFFFU && (state->flags & 0x80)) func_80019144(*result,0,0);
                    }
                }
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80019850
 * Source:   cloud/matches/boot_tail/func_80019850.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019850.c:func_80019850 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80019490(void *, unsigned int *, unsigned char);
void func_80019850(void *state, unsigned int *result)
{
    if (D_8002C630) {
        func_80014594();
        func_80019490(state, result, 0);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-11 — func_8001989C
 * Source:   cloud/work/frontier/w17g/sources/func_8001989C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001989C.c:func_8001989C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_8001989C(unsigned int *state) { if (*state != 0xFFFFFFFF) return (*state & 0x80000000) == 0; return 1; }

/* PROMOTED 2026-10-05 — func_800198C8
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_800198C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_800198C8.c:func_800198C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800198C8(void)
{
    int i;
    unsigned char first, second, third;
    if (D_8004F808) {
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].activeFC0) {
                D_8004BE80 = &D_80043EB8[i];
                D_8004BE78 = i;
                D_8004BE7C = func_8001BDB8(D_80043EB8[i].channelFC4);
                func_80018A30();
                func_8001897C();
                func_80018AEC();
                first = func_80018634();
                second = func_80017D38();
                func_8001824C();
                func_80018448();
                func_800180A0();
                third = func_80018184();
                func_80017540();
                if (!first && !second && !third) {
                    D_80043EB8[i].activeFC0 = 0;
                    D_80043EB8[i].inactiveFC1 = 1;
                }
            }
        }
    }
}

/* PROMOTED 2026-10-05 — func_800199F4
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800199F4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800199F4.c:func_800199F4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800171C0(void);
extern void func_800175A8(void);
void func_800199F4(void)
{
    unsigned int index;
    for (index = 0; index < 8; index++) {
        D_80043EB8[index].activeFC0 = 0;
        D_80043EB8[index].inactiveFC1 = 1;
    }
    func_800171C0();
    func_800175A8();
}

