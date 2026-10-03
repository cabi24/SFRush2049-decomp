/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
extern unsigned char D_8004BE7B;
extern void func_80020610(unsigned int, unsigned char, unsigned char, unsigned char);
void func_80017824(unsigned char channel, unsigned char value)
{ func_80020610(7, value, D_8004BE7B, channel); }
