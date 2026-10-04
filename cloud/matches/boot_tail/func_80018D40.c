/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct VoiceRecord {
    u32 identifier;
    u8 unknown004[0x10C];
    u32 first110;
    u32 second114;
    u8 unknown118[0xEA8];
    u8 activeFC0;
    u8 inactiveFC1;
    u16 valueFC2;
    u8 unknownFC4[0x20];
    u32 firstFE4;
    u32 secondFE8;
    u16 valueFEC;
    u8 flagsFEE;
    u8 unknownFEF;
    u32 pendingFF0;
    u8 unknownFF4[4];
} VoiceRecord;
extern VoiceRecord D_80043EB8[8];
extern u32 func_80017644(u32);
extern void func_8001734C(VoiceRecord *);
extern void func_8001729C(VoiceRecord *);
void func_80018D40(u32 identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
            D_80043EB8[identifier].inactiveFC1 = 1;
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].pendingFF0 = 0;
            }
        }
    }
}
