/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
extern int D_800385A0;
extern int D_80038608;
extern int D_8003C610;
extern int D_8003CE18;
extern int D_80042228;
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern void func_80014CF4(void);

void func_80017108(void)
{
    int i;
    D_800385A0 = 0;
    D_80038608 = 0;
    D_8003C610 = 0;
    D_8003CE18 = 0;
    D_80042228 = 0;
    D_8003DA20 = 0;
    for (i = 0; i < 512; ++i) {
        D_8003DA28[i].count = 0;
        D_8003DA28[i].first = 0;
    }
    func_80014CF4();
}
