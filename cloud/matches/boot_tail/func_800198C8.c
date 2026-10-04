/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SequenceContext {
    u8 unknown000[4032];
    u8 activeFC0;
    u8 inactiveFC1;
    u8 unknownFC2[2];
    u8 channelFC4;
    u8 unknownFC5[51];
} SequenceContext;
extern SequenceContext D_80043EB8[8];
extern SequenceContext *D_8004BE80;
extern u32 D_8004BE78;
extern u8 D_8004BE7C;
extern u32 D_8004F808;
extern int func_8001BDB8(u8);
extern void func_80018A30(void);
extern void func_8001897C(void);
extern void func_80018AEC(void);
extern u8 func_80018634(void);
extern u8 func_80017D38(void);
extern void func_8001824C(void);
extern void func_80018448(void);
extern void func_800180A0(void);
extern int func_80018184(void);
extern void func_80017540(void);
void func_800198C8(void)
{
    int i;
    u8 first, second, third;
    if (D_8004F808) {
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].activeFC0) {
                D_8004BE80 = &D_80043EB8[i];
                D_8004BE78 = i;
                D_8004BE7C = func_8001BDB8(D_80043EB8[i].channelFC4);
                func_80018A30();
                func_8001897C();
                func_80018AEC();
                first = func_80018634();
                second = func_80017D38();
                func_8001824C();
                func_80018448();
                func_800180A0();
                third = func_80018184();
                func_80017540();
                if (!first && !second && !third) {
                    D_80043EB8[i].activeFC0 = 0;
                    D_80043EB8[i].inactiveFC1 = 1;
                }
            }
        }
    }
}
