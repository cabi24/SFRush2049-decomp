/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern unsigned char D_8004FA18;
extern unsigned char D_8004FA19;
extern unsigned char D_8004FA1A;
extern int func_80014434(unsigned int *, unsigned short, unsigned short, unsigned int);
extern int func_800107E0(unsigned int);

int func_800108E0(unsigned int frequency, unsigned char voices,
                  unsigned char setting1, unsigned char setting2,
                  unsigned short count, unsigned int flags)
{
    int result;
    D_8002C630 = 0;
    if (voices <= 32) {
        D_8004FA18 = voices;
    } else {
        D_8004FA18 = 32;
    }
    D_8004FA19 = setting1;
    D_8004FA1A = setting2;
    result = func_80014434(&frequency, D_8004FA18, count, flags);
    if (result == 0) {
        result = func_800107E0(frequency);
    }
    return result;
}
