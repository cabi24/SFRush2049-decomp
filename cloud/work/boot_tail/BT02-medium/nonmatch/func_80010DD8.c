/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void *(*D_80038018)(unsigned int, unsigned int);
extern unsigned int D_800382F4;
extern void *D_800382E8;
extern void *D_800382F0;
void func_80010DD8(unsigned int rate, unsigned short duration)
{
    unsigned int count;
    if (duration != 0) {
        count = duration * rate / 1000;
        count += 192 - count % 192;
        D_800382F4 = count;
        D_800382E8 = 0;
        D_800382F0 = D_80038018(count * 2, 0);
    } else {
        D_800382F0 = 0;
    }
}
