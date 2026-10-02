/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Sprite {
    u32 word0,word4,word8;
    u16 half12;
    s16 x,y;
    u16 half18;
    s16 width,height;
    u8 byte24,byte25;
    s8 hidden;
    u8 byte27;
    s16 left,right,top,bottom;
    u32 word36;
    s32 state;
    u32 selector;
    u32 word48;
    u16 half52;
} Sprite;
typedef struct Player {u8 opaque0[239];s8 hidden;u8 opaque240[24];float distance;u8 opaque268[684];} Player;
typedef struct Car {u8 opaque0[1990];s16 player;u8 opaque1992[64];} Car;
typedef struct Position {s32 x,y;} Position;
extern Player D_80152818[];
extern Car D_8014A250[];
extern Position D_80115AE8[][4];
extern s8 D_80146111,D_8015F734;
extern s16 D_80151AD0;
extern s32 D_8014A110;
extern float D_801248B4,D_801248B8,D_801248BC,D_801248C0;
extern void Input_ApplyPadConfig(Sprite *);
s32 func_80106D94(Sprite *sprite) {
    s32 player,digit,whole,ones,half_width;
    s32 radix=10,last=9;
    s32 hidden;
    float distance,fraction=0.0f;
    player=(sprite->selector&0xf0)>>4;
    digit=sprite->selector&0xf;
    distance=D_80152818[player].distance;
    if(D_80146111) distance/=D_801248B4;
    if(player>=D_80151AD0||D_8014A110==5) {
        sprite->state=0;
        if(sprite->hidden!=1) {
            sprite->hidden=1;
            Input_ApplyPadConfig(sprite);
        }
        return 1;
    }
    hidden=!D_8015F734;
    if(!hidden) hidden=D_80152818[D_8014A250[player].player].hidden==1;
    if(hidden!=sprite->hidden) {
        sprite->hidden=hidden;
        Input_ApplyPadConfig(sprite);
    }
    if(sprite->hidden) return 1;
    {
    float number;
    number=distance/528.0f;
    whole=(s32)number;
    ones=whole%radix;
    if(ones==last) fraction=number-(float)whole;
    switch(digit) {
    case 0:
        half_width=sprite->width/2;
        sprite->x=D_80115AE8[D_80151AD0-1][player].x-half_width*2;
        sprite->y=D_80115AE8[D_80151AD0-1][player].y;
        sprite->top=0;
        sprite->left=((s32)(distance/D_801248B8)%radix)*half_width;
        if((s32)(distance/D_801248BC)%radix==last&&(s32)(distance/5280.0f)%radix==last&&ones==last)
            sprite->left=(s16)((float)sprite->left+fraction*(float)half_width);
        break;
    case 1:
        half_width=sprite->width/2;
        sprite->x=D_80115AE8[D_80151AD0-1][player].x-half_width;
        sprite->y=D_80115AE8[D_80151AD0-1][player].y;
        sprite->top=0;
        sprite->left=((s32)(distance/D_801248C0)%radix)*half_width;
        if((s32)(distance/5280.0f)%radix==last&&ones==last)
            sprite->left=(s16)((float)sprite->left+fraction*(float)half_width);
        break;
    case 2:
        sprite->x=D_80115AE8[D_80151AD0-1][player].x;
        sprite->y=D_80115AE8[D_80151AD0-1][player].y;
        sprite->top=0;
        half_width=sprite->width/2;
        sprite->left=((s32)(distance/5280.0f)%radix)*half_width;
        if(ones==last) sprite->left=(s16)((float)sprite->left+fraction*(float)half_width);
        break;
    case 3:
        half_width=sprite->width/2;
        sprite->x=D_80115AE8[D_80151AD0-1][player].x+half_width;
        sprite->y=D_80115AE8[D_80151AD0-1][player].y;
        sprite->top=half_width;
        sprite->left=(s16)((float)half_width*((float)ones+number-(float)whole));
        break;
    default:
        half_width=sprite->width/2;
        break;
    }
    sprite->bottom=sprite->top+half_width-1;
    sprite->right=sprite->left+half_width-1;
    Input_ApplyPadConfig(sprite);
    return 1;
    }
}
