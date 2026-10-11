/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001B4A4 - lib_1a660 (IDO 7.1). Send a 14-bit value to every voice of the voice chain
 * starting at IDENTIFIER (func_80020610 commands 132/133): func_8001EDF4 maps the identifier to its first voice; for
 * each voice whose identifier still matches, two func_80020610 commands (132, 133) carry
 * value >> 7 and value & 127 to the voice's channel (channel55 when flags bit 1 is set,
 * else set); the chain continues at .child. Returns 0 if any voice matched, else -1.
 * w18c: `result = 0` is written after the two calls (it fills retail's delay slot after
 * the flags test; written first, the low-byte narrowing takes the slot: true=120). The
 * index is passed directly (the TU's locked func_8001B29C does the same). No shaping devices.
 * Types: the TU's VoiceState_80019C8C (lib_1a660 line 90, verbatim) and prototypes.
 * Prior: D10 BT03-high-chains (5.3, 61 words; 7.1 true=120 reloc_blind=2). */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState_80019C8C { u32 command00;u8 unknown04[12];u32 child,parent;u8 unknown18[12]; u32 flags;u32 age28;u8 unknown2C[2];u8 channel2E,unknown2F;u32 volume30;u8 unknown34[4];u32 panning38;u8 unknown3C[4];u32 words40[2];u8 status48,status49;u8 channel,set;u8 unknown4C[2]; u16 base_key,key;u8 unknown52[3];u8 channel55;u8 unknown56[10];u32 identifier; u8 unknown64[4];u16 loop68;u8 unknown6A[34];u32 glide;u8 unknown90[4];u32 pitch; u8 state98,depth99,flag9A;u8 unknown9B[34];u8 activeBD,groupBE,unknownBF;s8 cents;u8 original_key;u16 valueC2;u8 unknownC4[168];u32 word16C;u16 half170;u8 unknown172[6];u32 word178;u16 half17C;u8 unknown17E[34]; } VoiceState_80019C8C;
#pragma pack(0)
extern VoiceState_80019C8C D_8004BEB8[];
extern int func_8001EDF4(u32);
extern void func_80020610(u8, u8, u8, u8);

int func_8001B4A4(u32 identifier, u16 value)
{
    int result;
    u32 index;
    u8 low;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            low = value & 127;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(132, index, D_8004BEB8[index].channel55, value >> 7);
                func_80020610(133, index, D_8004BEB8[index].channel55, low);
            } else {
                func_80020610(132, index, D_8004BEB8[index].set, value >> 7);
                func_80020610(133, index, D_8004BEB8[index].set, low);
            }
            result = 0;
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}
