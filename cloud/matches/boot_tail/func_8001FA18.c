/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u32 command00;
    u8 unknown04[12];
    u32 next_identifier;
    u8 unknown14[16];
    u32 flags24;
    u8 unknown28[36];
    u8 external4C;
    u8 unknown4D[19];
    u32 identifier60;
    u8 unknown64[89];
    u8 activeBD;
    u8 unknownBE[226];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 D_8002C630;
extern int func_8001EDF4(u32);
extern void func_8001F9D0(VoiceState *);
extern void func_80014B3C(int);
int func_8001FA18(u32 key)
{
    int result;
    u32 index;
    u32 next;
    result = -1;
    if (D_8002C630) {
        key = func_8001EDF4(key);
        while (key != 0xFFFFFFFFU) {
            index = key & 255;
            next = D_8004BEB8[index].next_identifier;
            if (key == D_8004BEB8[index].identifier60) {
                result = 0;
                if (D_8004BEB8[index].command00 != 0) {
                    func_8001F9D0(&D_8004BEB8[index]);
                }
                func_80014B3C(index);
            }
            key = next;
        }
    }
    return result;
}
