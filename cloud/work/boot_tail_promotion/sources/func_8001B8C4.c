/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001B8C4.c: file-local type names VoiceState suffixed _8001B8C4 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState_8001B8C4 {
    u8 unknown00[16];
    u32 next_identifier;
    u8 unknown14[16];
    u32 flags;
    u8 unknown28[56];
    u32 identifier;
    u8 unknown64[316];
} VoiceState_8001B8C4;
#pragma pack(0)
extern VoiceState_8001B8C4 D_8004BEB8[];
extern u8 D_8002C630;
extern int func_8001EDF4(u32);
int func_8001B8C4(u32 key)
{
    u32 index;
    int result;
    result = -1;
    if (D_8002C630) {
        key = func_8001EDF4(key);
        while (key != 0xFFFFFFFFU) {
            index = key & 255;
            if (D_8004BEB8[index].identifier == key) {
                D_8004BEB8[index].flags |= 8;
                result = 0;
            }
            key = D_8004BEB8[index].next_identifier;
        }
    }
    return result;
}
