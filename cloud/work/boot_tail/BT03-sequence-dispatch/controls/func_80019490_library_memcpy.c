/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SequenceRequest {
    u32 identifier;
    u16 duration;
    u8 unknown006[2];
    u32 nextIdentifier;
    u16 nextDuration;
    u8 unknown00E[2];
    void *stream;
    u16 group, program;
    u8 channel;
    u8 unknown019[3];
    u32 first, second;
    u16 value;
    u8 flags;
    u8 unknown027;
} SequenceRequest;
typedef struct SequenceOptions {
    u32 flags, first, second;
    u16 value, duration;
    u8 channel;
    u8 unknown011;
    u8 mapCount;
    u8 unknown013;
    void *map;
    u8 channelCount;
    u8 unknown019[3];
    void *channels;
} SequenceOptions;
typedef struct SequenceContext {
    u8 unknown000[0xFC8];
    SequenceRequest request;
    u32 *pendingResult;
    u8 pending;
    u8 unknownFF5[3];
} SequenceContext;
extern SequenceContext D_80043EB8[8];
extern void *memcpy(void *, const void *, u32);
extern u32 func_80017644(u32);
extern void func_80019194(u8,u16,u32,u8);
extern void func_80019370(u8,u16,u32,u8);
extern void func_80018FEC(u32);
extern void func_8001906C(u32);
extern void func_800190AC(u32,u32,u32);
extern void func_80019144(u32,u32,u32);
extern void func_80018F20(u32,u16);
extern void func_80018FA4(u32,u16);
extern u32 func_8001558C(u16,u16,void *,SequenceOptions *,u8);
extern u32 func_800156E8(u16,u16,void *,SequenceOptions *);
void func_80019490(SequenceRequest *state, u32 *result, u8 unlocked)
{
    SequenceOptions options;
    u32 index;
    SequenceContext *context;
    index = func_80017644(state->identifier);
    if (index != 0xFFFFFFFFU) {
        if (state->flags & 4) {
            context = (SequenceContext *)((u8 *)D_80043EB8 + (u32)(index * sizeof(SequenceContext)));
            memcpy(&context->request,state,sizeof(*state));
            context->request.flags &= ~4;
            context->pending = 1;
            context->pendingResult = result;
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
                    options.flags = 4;
                    if (state->flags & 8) options.flags = 0x14;
                    if (state->flags & 0x20) { options.flags |= 2; options.value = state->value; }
                    if (state->flags & 0x10) { options.flags |= 1; options.first = state->first; options.second = state->second; }
                    options.duration = state->nextDuration;
                    options.channel = state->channel;
                    options.channelCount = 0;
                    if (unlocked) {
                        *result = func_8001558C(state->group,state->program,state->stream,&options,1);
                        if (*result != 0xFFFFFFFFU && (state->flags & 0x80)) func_800190AC(*result,0,0);
                    } else {
                        *result = func_800156E8(state->group,state->program,state->stream,&options);
                        if (*result != 0xFFFFFFFFU && (state->flags & 0x80)) func_80019144(*result,0,0);
                    }
                }
            }
        }
    }
}
