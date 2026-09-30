typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern s32 D_8012F001, D_8012F002, D_8012F003, D_8012F004, D_8012F005;

/* context stand-in for func_80086A50 (387 words retail); clobbers a0,v0,v1,t6-t9 only */
void func_80086A50(s32 a, s32 b, s32 c, s32 d)
{
    if (0) { switch (b) { case 0: D_8012F003 = 1; break; case 1: D_8012F003 = 5; break; case 2: D_8012F003 = 7; break; case 3: D_8012F003 = 9; break; } }
    if (a == 0) { D_8012F001 = 0x200000; D_8012F002 = 0xE3000A01; }
    else if (a == 1) { D_8012F001 = 0; D_8012F002 = 0xE3000A01; }
    D_8012E608 = a + b;
    D_8012F005 = c + d;
}
