/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80086A50(s32 mode)
{
    Gfx *g;
    switch (mode) {
    case 0:
        if (D_8014A248 != 0) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x200000; g->w0 = 0xE3000A01; g->w1 = x; }
        }
        g = D_80149438; D_80149438 = g + 1;
        { u32 x = 0x0F0A4000; g->w0 = 0xE200001C; g->w1 = x; }
        g = D_80149438; D_80149438 = g + 1;
        { u32 x = 0xFFFCF279; g->w0 = 0xFCFFFFFF; g->w1 = x; }
        break;
    case 1:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0; g->w0 = 0xE3000A01; g->w1 = x; }
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504A70; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x20) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504240; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x10) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504A70; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFCF279; g->w0 = 0xFCFFFFFF; g->w1 = x; }
        }
        else {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504240; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFCF279; g->w0 = 0xFCFFFFFF; g->w1 = x; }
        }
        break;
    case 2:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0; g->w0 = 0xE3000A01; g->w1 = x; }
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504A70; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x20) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504240; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x10) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504A70; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFFF3F9; g->w0 = 0xFC11FE23; g->w1 = x; }
        }
        else {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00504240; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFFF3F9; g->w0 = 0xFC11FE23; g->w1 = x; }
        }
        break;
    case 3:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0; g->w0 = 0xE3000A01; g->w1 = x; }
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00553078; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x20) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x0F0A7008; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFF2FFFFF; g->w0 = 0xFC119623; g->w1 = x; }
        }
        else if (D_8012E608 & 0x10) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x00553078; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFFF3F9; g->w0 = 0xFC11FE23; g->w1 = x; }
        }
        else {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x0F0A7008; g->w0 = 0xE200001C; g->w1 = x; }
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0xFFFFF3F9; g->w0 = 0xFC11FE23; g->w1 = x; }
        }
        break;
    case 4:
        if (D_8014A248 < 4) {
            g = D_80149438; D_80149438 = g + 1;
            { u32 x = 0x100000; g->w0 = 0xE3000A01; g->w1 = x; }
        }
        g = D_80149438; D_80149438 = g + 1;
        { u32 x = 0x00504240; g->w0 = 0xE200001C; g->w1 = x; }
        g = D_80149438; D_80149438 = g + 1;
        { u32 x = 0xFFFC9238; g->w0 = 0xFCFFABFF; g->w1 = x; }
        g = D_80149438; D_80149438 = g + 1;
        { u32 x = 0x55; g->w0 = 0xFB000000; g->w1 = x; }
        break;
    }
    D_8014A248 = mode;
}

typedef unsigned short u16;
typedef unsigned char u8;
extern Gfx *D_80149438;
extern Gfx D_80124FE8[2][2400];
extern s32 D_8012E60C,D_8012E668,D_8012E610,D_8012E674,D_8012E684;
extern s32 D_8012E6C0,D_8012E680,D_8012E6D0;
extern double D_8012E688;
extern u16 D_8012E67A;
extern u8 D_8011EACF;
extern s32 D_8002AFC0,D_8002AFC4;
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
