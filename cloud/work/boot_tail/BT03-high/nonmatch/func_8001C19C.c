/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
extern unsigned char D_8002C630;
extern unsigned char D_8004F314[][40];
void func_8001C19C(unsigned char channel, unsigned char value)
{ if (D_8002C630) D_8004F314[channel][0] = value; }
