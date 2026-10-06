/* Host-only semantic checks; O32 layout is separately proved by verify.py. */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "func_800178B0.c"

SequenceContext D_80043EB8[8];
u8 D_8004F2B8[64];
static unsigned int maps, fades, groups, controllers, resets, presets, allocated;
static int selected;

void func_8001785C(u8 *map, BankRecord *bank)
{
    unsigned int i;
    maps++;
    memset(map, 255, 128);
    for (i = 0; bank[i].key != 255; i++) {
        assert(i < 128 && bank[i].key < 128);
        map[bank[i].key] = (u8)i;
    }
}
void func_8001C19C(u8 group, u8 value)
{
    assert(group >= 4 && group <= 6 && value == 0);
    groups++;
}
void func_8001B9F8(u8 volume, u16 time, u8 group, u8 mode, u32 identifier)
{
    assert(volume == 255 && time == 65535 && mode == 0 && identifier == 0);
    assert(group == selected + 23 || group == 10 || group == 11);
    fades++;
}
void func_80019A60(u32 tempo, u8 slot)
{
    assert(slot == selected && tempo == 0xFFFFFFFEU);
    assert(D_80043EB8[slot].tempo == tempo);
}
void func_80020820(u8 channel, u8 slot)
{
    assert(channel == resets && slot == selected);
    resets++;
}
void func_80017720(SequenceContext *context, u8 program, u8 channel)
{
    assert(context == D_80043EB8 + selected && channel == presets && program == channel + 9);
    presets++;
}
void func_80020610(u8 command, u8 channel, u8 slot, u8 value)
{
    static const u8 commands[4] = {7,10,91,93};
    assert(command == commands[controllers % 4] && channel == controllers / 4 && slot == selected);
    assert(value == channel + 10 + controllers % 4);
    controllers++;
}
u32 func_800175B4(u32 slot)
{
    assert(slot == (u32)selected && D_80043EB8[slot].available != 0);
    D_80043EB8[slot].identifier = 0xFEDCBA00U + slot;
    allocated++;
    return 0xFEDCBA00U + slot;
}

int main(void)
{
    BankRecord a[3], b[3];
    Program program;
    SequenceOptions options;
    GroupMap mapping[3];
    u8 fading[2];
    SequenceData *data;
    u32 *offsets;
    SequenceContext before[8], *context;
    unsigned int mask, flag, scenario, i, j, result, enabled;
    int actual_slot;
    data = (SequenceData *)malloc(4096);
    assert(data != NULL);
    memset(data, 0, 4096);
    data->track_offsets = 32;
    data->tempo = 0xFFFFFFFEU;
    offsets = (u32 *)((u8 *)data + 32);
    for (i = 0; i < 64; i++) offsets[i] = i % 3 ? 512 + i*4 : 0;
    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));
    for (i = 0; i < 2; i++) {
        a[i].identifier[0] = 0xFE;
        a[i].identifier[1] = 0xDC;
        a[i].identifier[2] = (u8)i;
        a[i].identifier[3] = 0x98;
        b[i] = a[i];
        a[i].channel = (u8)i;
        b[i].channel = i == 0 ? 0 : 255;
        a[i].key = b[i].key = (u8)(i*127);
        b[i].identifier[3] = 0x76;
    }
    a[2].key = b[2].key = 255;
    memset(&program, 0, sizeof(program));
    for (i = 0; i < 16; i++) {
        program.channels[i].program = (u8)(i+9);
        program.channels[i].volume = (u8)(i+10);
        program.channels[i].pan = (u8)(i+11);
        program.channels[i].reverb = (u8)(i+12);
        program.channels[i].chorus = (u8)(i+13);
    }
    memset(&options, 0, sizeof(options));
    options.enabled[0] = 0x80000001U;
    options.enabled[1] = 0xAAAAAAAAU;
    options.speed = 65535;
    options.fade_time = 65535;
    options.volume = 255;
    options.map_count = 3;
    options.map = mapping;
    options.fade_count = 2;
    options.fade_groups = fading;
    for (i = 0; i < 3; i++) {
        mapping[i].track = (u8)(i == 0 ? 0 : 63);
        mapping[i].group = (u8)(4+i);
    }
    fading[0] = 10;
    fading[1] = 11;
    for (mask = 0; mask < 256; mask++) for (flag = 0; flag < 33; flag++) for (scenario = 0; scenario < 2; scenario++) {
        memset(D_80043EB8, 0xA5, sizeof(D_80043EB8));
        memset(D_8004F2B8, 0xA5, sizeof(D_8004F2B8));
        actual_slot = 8;
        for (i = 0; i < 8; i++) {
            D_80043EB8[i].available = (u8)((mask>>i)&1);
            if (actual_slot == 8 && D_80043EB8[i].available) actual_slot = (int)i;
        }
        memcpy(before, D_80043EB8, sizeof(before));
        selected = actual_slot;
        options.flags = flag;
        data->master_offset = scenario ? 1024 : 0;
        maps = fades = groups = controllers = resets = presets = allocated = 0;
        result = func_800178B0(a, b, scenario ? &program : NULL, data, flag == 32 ? NULL : &options);
        if (actual_slot == 8) {
            assert(result == 0xFFFFFFFFU && maps+fades+groups+controllers+resets+presets+allocated == 0);
            assert(memcmp(before, D_80043EB8, sizeof(before)) == 0);
            continue;
        }
        assert(result == 0xFEDCBA00U+(u32)selected && allocated == 1);
        assert(maps == 2 && resets == 16 && presets == (scenario ? 16U : 0U) && controllers == (scenario ? 64U : 0U));
        assert(groups == (flag != 32 && (flag&8) ? 3U : 0U));
        assert(fades == (flag != 32 && (flag&4) ? 3U : 0U));
        for (i = 0; i < 8; i++) if (i != (unsigned int)selected) assert(memcmp(before+i, D_80043EB8+i, sizeof(*before)) == 0);
        context = D_80043EB8 + selected;
        assert(context->bank_a == a && context->bank_b == b && context->data == data);
        assert(context->available == 0 && context->pending == 0 && context->counter == 0 && context->state == 0);
        assert(context->active == (flag != 32 && (flag&16) ? 0xA5 : 1));
        assert(context->speed == (flag != 32 && (flag&2) ? 65535 : 256));
        for (i = 0; i < 2; i++) {
            enabled = flag != 32 && (flag&1) ? options.enabled[i] : 0xFFFFFFFFU;
            assert(context->enabled[i] == enabled);
        }
        assert(context->master.current == (scenario ? (u8 *)data+1024 : NULL));
        if (scenario) assert(context->master.start == (u8 *)data+1024 && context->master.time == 0 && context->master.offset == 0);
        else assert(memcmp(&context->master.start, &before[selected].master.start, sizeof(context->master)-sizeof(context->master.current)) == 0);
        for (i = 0; i < 64; i++) {
            assert(D_8004F2B8[i] == 127);
            assert(context->streams[i].current == (offsets[i] ? (u8 *)data+offsets[i] : NULL));
            assert(context->streams[i].start == context->streams[i].current && context->streams[i].time == 0 && context->streams[i].offset == 0);
            assert(context->tracks[i].events == NULL && context->tracks[i].pitch == NULL && context->tracks[i].modulation == NULL);
            j = flag != 32 && (flag&8) && (i==0 || i==63) ? (i==0 ? 4U : 6U) : (u32)selected+23;
            assert(context->groups[i] == j);
        }
        assert(context->programs[0] == 0xFEDC0076U && context->programs[1] == 0xFEDC0198U);
        for (i = 2; i < 16; i++) assert(context->programs[i] == 0xFFFFFFFFU);
        assert(memcmp(context->unknown118, before[selected].unknown118, sizeof(context->unknown118)) == 0);
        assert(memcmp(context->request_and_result, before[selected].request_and_result, sizeof(context->request_and_result)) == 0);
    }
    free(data);
    return 0;
}
