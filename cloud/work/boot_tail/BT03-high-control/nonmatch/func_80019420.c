/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: bounded scalar-coalescing residual; see packet README. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct VoiceState {
    u8 unknown000[0x528];
    u8 selector528[256];
    u8 unknown628[0x998];
    u8 byteFC0;
    u8 byteFC1;
    u8 unknownFC2[2];
    u8 byteFC4;
    u8 byteFC5;
    u16 valueFC6;
    u8 unknownFC8[0x30];
} VoiceState;
extern VoiceState D_80043EB8[8];
extern u8 D_8002C630;
extern u32 func_80017644(u32);
extern void func_8001C19C(u8, u8);
/* On the N64 ABI, unsigned 32-bit byte-offset arithmetic removes the
 * translator's high tag bit before recovering a pointer to its validated row.
 * Do not use the tagged word directly as a C array subscript.
 */
void func_80019420(u32 identifier, u8 selector, u8 channel)
{
    u32 index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) {
            (*(VoiceState *)((u32)D_80043EB8 +
                index * sizeof(VoiceState))).selector528[selector] = channel;
            func_8001C19C(channel, 2);
        }
    }
}
