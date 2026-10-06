/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: sequence constructor; see README.md and evidence.json. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct BankRecord {
    u8 identifier[4];
    u8 channel, key;
    u8 unknown06[2];
} BankRecord;

typedef struct ChannelSetup {
    u8 program, volume, pan, reverb, chorus;
    u8 unknown05[3];
} ChannelSetup;

typedef struct Program {
    u8 identifier[2];
    u8 unknown02[2];
    ChannelSetup channels[16];
} Program;

typedef struct SequenceData {
    u32 track_offsets;
    u32 unknown04[2];
    u32 master_offset;
    u32 tempo;
} SequenceData;

typedef struct GroupMap { u8 track, group; } GroupMap;
typedef struct SequenceOptions {
    u32 flags, enabled[2];
    u16 speed, fade_time;
    u8 volume, unknown11, map_count, unknown13;
    GroupMap *map;
    u8 fade_count, unknown19[3];
    u8 *fade_groups;
} SequenceOptions;

typedef struct SequenceStream {
    u8 *current, *start;
    u32 time, offset;
} SequenceStream;

typedef struct SequenceTrack {
    u32 fraction, whole, event_time;
    u8 *events, *pitch, *modulation;
    u16 pitch_value, modulation_value;
    u32 pitch_time, modulation_time;
    u8 channel;
    s8 velocity_offset, transpose;
    u8 number;
} SequenceTrack;

typedef struct SequenceContext {
    u32 identifier;
    BankRecord *bank_a;
    u8 map_a[128];
    BankRecord *bank_b;
    u8 map_b[128];
    SequenceData *data;
    u32 enabled[2];
    u32 unknown118[2];
    u32 lookahead, tempo;
    SequenceStream streams[64];
    u8 groups[64];
    SequenceTrack tracks[64];
    SequenceStream master;
    u32 note_head, note_tail;
    u32 programs[16];
    u8 active, available;
    u16 speed;
    u8 group, state;
    u16 counter;
    u8 request_and_result[44];
    u8 pending, unknownFF5[3];
} SequenceContext;

extern SequenceContext D_80043EB8[8];
extern u8 D_8004F2B8[64];
extern void func_8001785C(u8 *, BankRecord *);
extern void func_8001C19C(u8, u8);
extern void func_8001B9F8(u8, u16, u8, u8, u32);
extern void func_80019A60(u32, u8);
extern void func_80020820(u8, u8);
extern void func_80017720(SequenceContext *, u8, u8);
extern void func_80020610(u8, u8, u8, u8);
extern u32 func_800175B4(u32);

u32 func_800178B0(BankRecord *bank_a, BankRecord *bank_b,
                  Program *program, SequenceData *data, SequenceOptions *options)
{
    SequenceContext *context;
    BankRecord *bank;
    u8 *map;
    u32 *offsets;
    u32 offset;
    int slot, i, pass;
    u8 index;
    context = D_80043EB8;
    for (slot = 0; slot < 8; slot++, context++) {
        if (context->available) break;
    }
    if (slot != 8) {
        context->pending = 0;
        context->bank_a = bank_a;
        context->bank_b = bank_b;
        context->data = data;
        func_8001785C(context->map_a, bank_a);
        func_8001785C(context->map_b, context->bank_b);
        context->group = slot + 23;
        for (i = 0; i < 64; i++) context->groups[i] = context->group;
        if (!options) {
            context->enabled[0] = context->enabled[1] = 0xFFFFFFFFU;
            context->speed = 256;
        } else {
            if (options->flags & 1) {
                context->enabled[0] = options->enabled[0];
                context->enabled[1] = options->enabled[1];
            } else context->enabled[0] = context->enabled[1] = 0xFFFFFFFFU;
            if (options->flags & 2) context->speed = options->speed;
            else context->speed = 256;
            if (options->flags & 8) {
                for (i = 0; i < options->map_count; i++) {
                    context->groups[options->map[i].track] = options->map[i].group;
                    func_8001C19C(options->map[i].group, 0);
                }
            }
            if (options->flags & 4) {
                func_8001B9F8(options->volume, options->fade_time, context->group, 0, 0);
                for (i = 0; i < options->fade_count; i++) {
                    func_8001B9F8(options->volume, options->fade_time, options->fade_groups[i], 0, 0);
                }
            }
        }
        context->tempo = data->tempo;
        func_80019A60(data->tempo, slot);
        if (data->master_offset) {
            context->master.current = context->master.start = (u8 *)data + data->master_offset;
            context->master.time = context->master.offset = 0;
        } else context->master.current = 0;
        context->state = 0;
        offsets = (u32 *)((u8 *)data + data->track_offsets);
        for (i = 0; i < 64; i++) {
            D_8004F2B8[i] = 127;
            context->streams[i].time = context->streams[i].offset = 0;
            context->tracks[i].events = context->tracks[i].pitch = context->tracks[i].modulation = 0;
            offset = offsets[i];
            if (offset) context->streams[i].current = context->streams[i].start = (u8 *)data + offset;
            else context->streams[i].current = context->streams[i].start = 0;
        }
        context->note_head = context->note_tail = 0;
        for (i = 0; i < 16; i++) func_80020820(i, slot);
        for (i = 0; i < 16; i++) context->programs[i] = 0xFFFFFFFFU;
        bank = context->bank_a;
        map = context->map_a;
        for (pass = 0; pass < 2; pass++) {
            for (i = 0; i < 128; i++) {
                index = map[i];
                if (index != 255 && bank[index].channel != 255) {
                    context->programs[bank[index].channel] =
                        (((u32)bank[index].identifier[0] << 8 | bank[index].identifier[1]) << 16) |
                        bank[index].identifier[3] | bank[index].identifier[2] << 8;
                }
            }
            bank = context->bank_b;
            map = context->map_b;
        }
        if (program) {
            for (i = 0; i < 16; i++) {
                func_80017720(context, program->channels[i].program, i);
                func_80020610(7, i, slot, program->channels[i].volume);
                func_80020610(10, i, slot, program->channels[i].pan);
                func_80020610(91, i, slot, program->channels[i].reverb);
                func_80020610(93, i, slot, program->channels[i].chorus);
            }
        }
        context->counter = 0;
        if (options) {
            if (!(options->flags & 16)) context->active = 1;
        } else context->active = 1;
        offset = func_800175B4(slot);
        context->available = 0;
        return offset;
    }
    return 0xFFFFFFFFU;
}
