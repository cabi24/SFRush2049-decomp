import sys
nargs=int(sys.argv[1]); callargs=sys.argv[2]; extra=sys.argv[3] if len(sys.argv)>3 else ''
ps=['int a','int b','int c','int d'][:nargs]
print('''typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
extern u8 *D_801497F0;
extern s8 D_80149B70;
extern s8 D_80149B60;
extern int D_80149B00;
extern int D_80149B10;
void func_80096288(%s)
{
    if (0) { switch (a) { case 1: D_80149B10 = 3; case 2: D_80149B10 = 4; case 3: D_80149B10 = 5; } }
    D_80149B10 = a + b%s;
}
void sound_update_channel(int force)
{
    int i = D_80149B00;
    func_80096288(%s);
    %s
    if (force || D_80149B00 != D_80149B70) {
        D_80149B70 = D_80149B00;
    }
}
void mode_byte_set(s16 a)
{
    if (a < 0) { sound_update_channel(0); D_80149B70 = D_801497F0[8]; }
    else D_80149B70 = a;
}
void mode_byte2_set(s16 a)
{
    if (a < 0) { sound_update_channel(0); D_80149B60 = D_801497F0[4]; }
    else D_80149B60 = a;
}
'''%(', '.join(ps), ''.join(' + '+x for x in 'cd'[:nargs-2]), callargs, extra))
