/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct StatePrefix {
    unsigned char unknown00[8];
    unsigned int flags08;
    unsigned char unknown0C[40];
    unsigned int identifier34;
} StatePrefix;
unsigned int func_8001D518(StatePrefix *state)
{
    unsigned int result;
    result = 0xFFFFFFFF;
    if (D_8002C630) {
        func_80014594();
        if (state->flags08 & 0x10000) result = state->identifier34;
        func_800145DC();
    }
    return result;
}
