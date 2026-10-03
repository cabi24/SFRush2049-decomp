/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native 4088-byte voice-record reconstruction; original field names unknown. */
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
extern void func_800171C0(void);
extern void func_800175A8(void);
void func_800199F4(void)
{
    u32 index;
    for (index = 0; index < 8; index++) {
        D_80043EB8[index].byteFC0 = 0;
        D_80043EB8[index].byteFC1 = 1;
    }
    func_800171C0();
    func_800175A8();
}
