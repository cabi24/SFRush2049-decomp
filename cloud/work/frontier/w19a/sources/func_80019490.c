/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80019490 - lib_17dc0 (IDO 7.1). Dispatch a 0x28-byte sequence REQUEST for the sequence
 * named by its identifier (func_80017644 -> context index, -1 = none):
 *  - flag 4 (deferred): copy the request into the context's pending record at +0xFC8, clear
 *    flag 4 in the copy (+0xFEE), mark it pending (+0xFF4 = 1), remember RESULT (+0xFF0) and
 *    return identifier | 0x80000000 through RESULT;
 *  - otherwise stop/fade the current sequence (func_80019194 when UNLOCKED, else the locking
 *    wrapper func_80019370; mode 2 for flag 1, 3 for flag 0x40, else 1); then, when RESULT is
 *    given, either chain the next identifier (flag 2: start it, optional first/second (0x10)
 *    and value (0x20) settings) or construct a new sequence from group/program/stream with
 *    options built from the flags (func_8001558C / func_800156E8), optionally with defaults
 *    (0x80). RESULT receives the started identifier or -1.
 * Caller: func_80019850 (lib_17dc0, locked) via the TU's `void *` prototype.
 * Types: the TU's sequence_context.h (SequenceContext, D_80043EB8, func_80017644,
 * func_80019194); the pending record is addressed as a SequenceRequest view of the header's
 * unknownFC8 bytes (type view, no code: the header names +0xFEE flagsFEE, +0xFF0 pendingFF0
 * (u32, stores the RESULT pointer), +0xFF4 unknownFF4). Callee prototypes as the locked
 * definitions (func_800156E8 in lib_15a10 and its extern for func_8001558C take the stream and
 * options as `unsigned int`: casts at the call, no code). The parameter is `void *` as the
 * TU's later prototype (func_80019850's block) declares it; `state` is the typed view of it
 * (a real used local; declared after `index` it takes the slot D10's `context` pointer had).
 * w19a: from D10 BT03-sequence-dispatch (5.3, 109 words; 7.1 true=30): the pending mark is
 * stored before the flag is cleared (retail's temporary numbering). No shaping devices,
 * no unused locals. */
#include "sequence_context.h"
typedef struct SequenceRequest {
    unsigned int identifier;
    unsigned short duration;
    unsigned char unknown006[2];
    unsigned int nextIdentifier;
    unsigned short nextDuration;
    unsigned char unknown00E[2];
    void *stream;
    unsigned short group, program;
    unsigned char channel;
    unsigned char unknown019[3];
    unsigned int first, second;
    unsigned short value;
    unsigned char flags;
    unsigned char unknown027;
} SequenceRequest;
typedef struct SequenceOptions {
    unsigned int flags, first, second;
    unsigned short value, duration;
    unsigned char channel;
    unsigned char unknown011;
    unsigned char mapCount;
    unsigned char unknown013;
    void *map;
    unsigned char channelCount;
    unsigned char unknown019[3];
    void *channels;
} SequenceOptions;
extern void func_80018FEC(unsigned int);
extern void func_8001906C(unsigned int);
extern void func_800190AC(unsigned int, unsigned int, unsigned int);
extern void func_80019144(unsigned int, unsigned int, unsigned int);
extern void func_80018F20(unsigned int, unsigned short);
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
