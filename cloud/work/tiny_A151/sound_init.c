/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { u32 w0,w1; } Gfx;
extern Gfx *D_80149438;
extern Gfx D_80124FE8[2][2400];
extern u32 D_8012E608;
extern s32 D_8012E60C,D_8012E668,D_8012E610,D_8012E674,D_8012E684;
extern s32 D_8012E6C0,D_8014A248,D_8012E680,D_8012E6D0;
extern double D_8012E688;
extern u16 D_8012E67A;
extern u8 D_8011EACF;
extern s32 D_8002AFC0,D_8002AFC4;
extern void func_80086A50(s32);
extern void func_800878E0(u32);
void sound_init(void)
{
    Gfx *g;
    if(D_80149438==0) {
        D_8012E608=0;
        D_8012E60C=0;
        D_8012E668=0;
        D_8012E610=D_8002AFC0-1;
        D_8012E674=D_8002AFC4-1;
        D_8012E67A=0;
        D_8011EACF=255;
        D_8012E684=0;
        D_8012E688=0.0;
        D_8012E6C0++;
        if(D_8012E6C0>=2) D_8012E6C0=0;
        D_80149438=D_80124FE8[D_8012E6C0];
        D_8014A248=-1;
        D_8012E680=-1;
        D_8012E6D0=0;
        g=D_80149438;D_80149438=g+1;g->w1=16;g->w0=0xF9000000;
        g=D_80149438;D_80149438=g+1;g->w1=0;g->w0=0xE7000000;
        g=D_80149438;D_80149438=g+1;g->w1=0;g->w0=0xE3000C00;
        g=D_80149438;D_80149438=g+1;g->w1=0;g->w0=0xD9000000;
        func_80086A50(1);
        if(D_8002AFC4>=221) func_800878E0(0x8000);
    }
}
