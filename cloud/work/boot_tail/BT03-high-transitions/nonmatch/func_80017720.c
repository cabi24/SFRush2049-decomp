/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct ProgramEntry { u16 high; u8 middle; u8 low; u32 unknown04; } ProgramEntry;
#pragma pack(0)
typedef struct Context {
    u32 unknown00;
    ProgramEntry *primary;
    u8 primary_map[128];
    ProgramEntry *secondary;
    u8 secondary_map[128];
    u8 unknown10C[0xE74];
    u32 selected[16];
} Context;
void func_80017720(Context *context, u8 program, u8 channel)
{
    if (channel != 9) {
        program = context->primary_map[program];
        if (program != 255) {
            context->selected[channel] = ((u32)context->primary[program].high << 16) |
                context->primary[program].low | ((u32)context->primary[program].middle << 8);
        }
    } else {
        program = context->secondary_map[program];
        if (program != 255) {
            context->selected[channel] = ((u32)context->secondary[program].high << 16) |
                context->secondary[program].low | ((u32)context->secondary[program].middle << 8);
        }
    }
}
