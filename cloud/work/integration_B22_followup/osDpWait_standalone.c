/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
extern void __osSpSetStatus(u32);
void osDpWait(void) { __osSpSetStatus(0x400); }
