/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void (*D_80038020)(void);
extern void (*D_80038024)(void);
extern unsigned char D_8002C630;
extern volatile unsigned char D_80038288;
extern unsigned short D_80038292;
extern unsigned char D_8003829E;
extern unsigned short D_800382CE;
extern unsigned short *D_800382D0;
extern unsigned short *D_800382D4;
extern unsigned short *D_800382D8[];
extern unsigned short D_800382E0[];
extern unsigned short D_80038360;
extern short D_80038362;
extern unsigned int __osDisableInt(void);
extern void __osRestoreInt(unsigned int);
extern void func_80014624(void);
extern void func_80014650(void);
extern void func_8001DDE0(void);
extern void func_80011CD8(void);
extern void func_80011894(void);
extern void func_80012200(void);
extern void func_80013964(void);
extern void func_80013C84(void);
extern int func_80013D70(unsigned short, unsigned short);
extern void func_80011D24(void);
extern void func_8001C508(void);
extern void func_800118C0(void);
extern void func_800119E0(void);
extern void func_8001C3CC(void);

void func_80013DEC(void)
{
    register unsigned short index;
    register unsigned short offset;
    register unsigned short count;
    int target;
    unsigned int mask;
    if (D_80038024 != 0) {
        D_80038024();
    }
    func_80014624();
    mask = __osDisableInt();
    if (D_80038288 == 0) {
        index = D_80038292 - 1;
    } else {
        index = D_80038288 - 1;
    }
    __osRestoreInt(mask);
    if (D_8002C630 != 0) {
        func_8001DDE0();
    }
    if (D_80038360 != 0xFFFF) {
        func_80011CD8();
        func_80011894();
        if (D_80038020 != 0) {
            D_80038020();
        }
        func_80012200();
        func_80013964();
        target = index;
        if (index != D_80038360) {
            func_80013C84();
            offset = 0;
            count = 0;
            if (D_80038360 < target) {
                for (index = D_80038360;
                     index < target && count < D_800382CE;
                     index++) {
                    func_80013D70(index, offset);
                    offset += 192;
                    count++;
                }
            } else {
                for (index = D_80038360;
                     index < D_80038292 && count < D_800382CE;
                     index++) {
                    func_80013D70(index, offset);
                    offset += 192;
                    count++;
                }
                for (index = 0;
                     index < target && count < D_800382CE;
                     index++) {
                    func_80013D70(index, offset);
                    offset += 192;
                    count++;
                }
            }
            func_80011D24();
            D_80038362 -= count;
            D_80038360 = index;
        }
        func_8001C508();
        index = D_8003829E ^ 1;
        D_800382D4 = &D_800382E0[index];
        D_800382D0 = D_800382D8[index];
        func_800118C0();
        func_800119E0();
        D_8003829E ^= 1;
        if (D_8002C630 != 0) {
            func_8001C3CC();
        }
    } else {
        D_80038360 = index;
    }
    func_80014650();
}
