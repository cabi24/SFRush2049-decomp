/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern unsigned char *func_80017040(unsigned short);
extern void func_8001C19C(unsigned char, unsigned char);
void func_80020370(unsigned short identifier, unsigned char channel)
{
    unsigned char *state;
    if (D_8002C630) {
        func_80014594();
        state = func_80017040(identifier);
        if (state != 0) {
            if (channel != 254) {
                state[9] = channel;
                func_8001C19C(channel, 3);
            } else state[9] = 31;
        }
        func_800145DC();
    }
}
