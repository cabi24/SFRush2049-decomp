/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001FBB0 - lib_207b0 (IDO 7.1). Locked-audio twin of func_8001B4A4: when the audio
 * system is up (D_8002C630), inside func_80014594/func_800145DC (lock/unlock), send value >> 7
 * and value & 127 as func_80020610 commands 1 and 33 to every voice of the chain starting at
 * IDENTIFIER (channel55 when flags bit 1 is set, else channel4B; chain via next10).
 * Returns 0 if any voice matched, else -1. Same structure as the TU's locked func_8001FD2C.
 * w18c: `result = 0` after the two calls (see func_8001B4A4); index passed directly.
 * No shaping devices.
 * Types: the TU's VoiceState and externs from func_8001FD2C's block (lib_207b0 lines 14-22,
 * verbatim), which sits AFTER this slot: integration hoists those lines above this slot
 * (RESULTS.md "TU edits").
 * Prior: D10 BT03-high-runtime-lists (5.3, 66 words; 7.1 true=120 reloc_blind=2). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next10; u8 unknown14[16]; u32 flags24; u8 unknown28[3]; u8 channel2B; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[28]; u8 channel4B; u8 external4C; u8 unknown4D[8]; u8 channel55; u8 unknown56[10]; u32 identifier60; u8 unknown64[316]; } VoiceState;
#pragma pack(0)
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern VoiceState D_8004BEB8[];
extern int func_8001EDF4(u32);
extern void func_80020610(u8, u8, u8, u8);

int func_8001FBB0(u32 identifier, u16 value)
{
    int result;
    u32 index;
    u8 low;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        identifier = func_8001EDF4(identifier);
        while (identifier != 0xFFFFFFFFU) {
            index = identifier & 255;
            if (D_8004BEB8[index].identifier60 == identifier) {
                low = value & 127;
                if (D_8004BEB8[index].flags24 & 2) {
                    func_80020610(1, index, D_8004BEB8[index].channel55, value >> 7);
                    func_80020610(33, index, D_8004BEB8[index].channel55, low);
                } else {
                    func_80020610(1, index, D_8004BEB8[index].channel4B, value >> 7);
                    func_80020610(33, index, D_8004BEB8[index].channel4B, low);
                }
                result = 0;
                identifier = D_8004BEB8[index].next10;
            } else {
                func_800145DC();
                return result;
            }
        }
        func_800145DC();
    }
    return result;
}
