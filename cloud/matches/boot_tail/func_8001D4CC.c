/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001D084(void *);
int func_8001D4CC(void *state)
{
    if (D_8002C630) {
        func_80014594();
        func_8001D084(state);
        func_800145DC();
        return 1;
    }
    return 0;
}
