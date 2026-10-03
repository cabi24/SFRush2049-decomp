/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern void func_80014140(void);
extern void func_800118C0(void);
extern void func_800110C4(void);
extern void func_80011848(void);
extern void func_800121BC(void);
extern void func_8001261C(void);
extern void func_8001144C(void);
extern void func_80010D74(void);
void func_80014488(void)
{
    if (D_8002C630 != 0) {
        func_80014140();
        func_800118C0();
        func_800110C4();
        func_80011848();
        func_800121BC();
        func_8001261C();
        func_8001144C();
        func_80010D74();
    }
}
