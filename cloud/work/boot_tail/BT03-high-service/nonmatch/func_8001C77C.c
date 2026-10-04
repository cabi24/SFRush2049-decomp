/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: genuine five-input scaling wrapper, see packet README. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80014A74(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void func_8001C77C(unsigned int channel, unsigned char first, unsigned char second, unsigned char third, unsigned char fourth)
{
    if (D_8002C630 && channel != 0xFFFFFFFF) {
        func_80014594();
        func_80014A74(channel, first << 16, second << 16, third << 16, fourth << 16);
        func_800145DC();
    }
}
