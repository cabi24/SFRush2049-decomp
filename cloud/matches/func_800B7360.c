/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Update the four-byte cached state only when it changes, then invalidate it. */
extern unsigned char D_80149B48[4];
extern int D_8012E6D0;
void func_800B7360(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
    if (a != D_80149B48[0] || b != D_80149B48[1] || c != D_80149B48[2] || d != D_80149B48[3]) {
        D_80149B48[0] = a;
        D_80149B48[1] = b;
        D_80149B48[2] = c;
        D_80149B48[3] = d;
        D_8012E6D0 = 0;
    }
}
