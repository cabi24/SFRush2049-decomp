/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Initialize queue/audio state, then submit the requested format. Native ABI reconstruction. */
extern unsigned char D_80038290;
extern unsigned char D_80038291;
extern void func_80014550(void);
extern void func_80010C68(unsigned int *);
extern unsigned char func_80014374(unsigned int);
extern void func_80014198(unsigned int *, unsigned short, unsigned short, unsigned char);
int func_800143C0(unsigned int *frequency, unsigned short count, unsigned short size, unsigned int flags)
{
    D_80038291 = 0;
    func_80014550();
    D_80038290 = (flags & 0x100000) != 0;
    func_80010C68(frequency);
    func_80014198(frequency, count, size, func_80014374(flags));
    return 0;
}
