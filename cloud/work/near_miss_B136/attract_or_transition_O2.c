/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;
typedef short s16;
typedef unsigned char u8;
typedef struct Gfx {u32 w0,w1;} Gfx;
typedef struct Frame128 {u8 prefix[88];u8 *commands;u8 gap[32];void *colorBuffer;} Frame128;
extern Frame128 D_80156BE0[];
extern s8 D_8015F72D;
extern u8 *D_8015B250,*D_8015B260;
extern Gfx *D_801497C8;
extern Gfx D_8002E528[],D_8002E4A0[];
extern void *D_8002EBB0;
extern s32 D_8002AFC0,D_8002AFC4;
extern u32 D_80124FC8;
extern s16 D_80151AD0;
extern s32 D_8015F738,D_80161380,D_80161398,D_801613A4;
extern u32 osVirtualToPhysical(void *);
extern void osViSetSpecialFeatures(u32);
extern s32 wheel_render_full(s32);
extern void sound_init(void),world_trigger_check(void);
#define COMMAND(first,second) do {Gfx *command=D_801497C8++;command->w0=(first);command->w1=(second);} while(0)
#define IMAGE(first,address) do {Gfx *command=D_801497C8++;command->w0=(first);command->w1=osVirtualToPhysical(address);} while(0)
#define RECTANGLE(x0,y0,x1,y1) COMMAND(0xF6000000|(((x1)&0x3FF)<<14)|(((y1)&0x3FF)<<2),(((x0)&0x3FF)<<14)|(((y0)&0x3FF)<<2))
void attract_or_transition(void) {
    Frame128 *frame;
    u32 color;
    s32 layout;
    frame=&D_80156BE0[D_8015F72D];
    D_8015B250=frame->commands;
    D_8015B260=D_8015B250+1728;
    D_801497C8=(Gfx *)(D_8015B250+0x9CC0);
    COMMAND(0xDB060000,0);
    COMMAND(0xDE000000,(u32)D_8002E528);
    COMMAND(0xDE000000,(u32)D_8002E4A0);
    IMAGE(0xFE000000,D_8002EBB0);
    COMMAND(0xE7000000,0);
    COMMAND(0xE3000A01,0x00300000);
    IMAGE(0xFF100000|((D_8002AFC0-1)&0xFFF),D_8002EBB0);
    COMMAND(0xF7000000,0xFFFCFFFC);
    RECTANGLE(0,0,D_8002AFC0-1,D_8002AFC4-1);
    COMMAND(0xD9FFFFFF,1);
    color=D_80124FC8;
    COMMAND(0xE7000000,0);
    IMAGE(0xFF100000|((D_8002AFC0-1)&0xFFF),frame->colorBuffer);
    COMMAND(0xF7000000,color);
    RECTANGLE(0,0,D_8002AFC0-1,D_8002AFC4-1);
    layout=wheel_render_full(0);
    if(layout==1) {
        COMMAND(0xE7000000,0);
        COMMAND(0xF7000000,0x00010001);
        RECTANGLE((D_8002AFC0*3)/4,0,D_8002AFC0-1,D_8002AFC4-1);
        COMMAND(0xE7000000,0);
        RECTANGLE(0,D_8002AFC4/2-1,D_8002AFC0-1,D_8002AFC4/2);
        COMMAND(0xE7000000,0);
        RECTANGLE(0,0,1,D_8002AFC4-1);
    } else if(layout==2) {
        if(D_80151AD0==3) {
            COMMAND(0xE7000000,0);
            COMMAND(0xF7000000,0x00010001);
            RECTANGLE(D_8002AFC0/2,D_8002AFC4/2,D_8002AFC0-1,D_8002AFC4-1);
        }
        COMMAND(0xE7000000,0);
        COMMAND(0xF7000000,0x00010001);
        RECTANGLE(D_8002AFC0/2-1,0,D_8002AFC0/2,D_8002AFC4-1);
        COMMAND(0xE7000000,0);
        RECTANGLE(0,D_8002AFC4/2-1,D_8002AFC0-1,D_8002AFC4/2);
    }
    COMMAND(0xE7000000,0);
    switch(D_8015F738) {
    case 0:COMMAND(0xE3001201,0x2000);break;
    case 1:COMMAND(0xE3001201,0x3000);break;
    case 2:COMMAND(0xE3001201,0);break;
    }
    switch(D_80161380) {
    case 0:COMMAND(0xE3001801,0xC0);break;
    case 1:COMMAND(0xE3001801,0x40);break;
    case 2:COMMAND(0xE3001801,0);break;
    case 3:COMMAND(0xE3001801,0x80);break;
    }
    switch(D_80161398) {
    case 0:osViSetSpecialFeatures(128);break;
    case 1:osViSetSpecialFeatures(64);break;
    }
    if(D_801613A4)osViSetSpecialFeatures(16);
    else osViSetSpecialFeatures(32);
    sound_init();
    world_trigger_check();
}
