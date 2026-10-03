/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern int func_800164D0(unsigned short, void *, unsigned short);

void func_80015318(unsigned short id, unsigned short *data)
{
    unsigned short count;

    count = data[0];
    data += 2;
    func_800164D0(id, data, count);
}
