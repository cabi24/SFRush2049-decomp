/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Update the four-byte cached state only when it changes, then invalidate it. */
typedef struct NativeTextColor { union { unsigned char rgba[8]; unsigned int rgbaU32; } fore; } NativeTextColor;
extern NativeTextColor D_80149B48;
extern unsigned char *D_8012E6D0;
void func_800B7360(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
    if (a != D_80149B48.fore.rgba[0] || b != D_80149B48.fore.rgba[1] || c != D_80149B48.fore.rgba[2] || d != D_80149B48.fore.rgba[3]) {
        D_80149B48.fore.rgba[0] = a;
        D_80149B48.fore.rgba[1] = b;
        D_80149B48.fore.rgba[2] = c;
        D_80149B48.fore.rgba[3] = d;
        D_8012E6D0 = 0;
    }
}
