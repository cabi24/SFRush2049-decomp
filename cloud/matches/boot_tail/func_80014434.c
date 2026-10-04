/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Initialize queue and update frequency before submitting the requested format. */
extern void func_80014550(void);
extern void func_80010D3C(unsigned int *);
extern unsigned char func_80014374(unsigned int);
extern void func_80014198(unsigned int *, unsigned short, unsigned short, unsigned char);
int func_80014434(unsigned int *frequency, unsigned short count, unsigned short size, unsigned int flags)
{
    func_80014550();
    func_80010D3C(frequency);
    func_80014198(frequency, count, size, func_80014374(flags));
    return 0;
}
