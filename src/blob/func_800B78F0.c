/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Resource { u8 *data; } Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 first;u8 selector;u8 opaque[70];Handle *handle;} Player;
typedef struct Record96 {u8 opaque[92];u16 bits;u8 tail[2];} Record96;
typedef struct Record64 {u8 opaque[60];u16 bits;u8 tail[2];} Record64;
extern s8 D_8014978C;
extern Player input_rec0[];
extern Object *D_80146150[];
extern u32 func_800B78A4(u32,u8);
u8 func_800B78F0(int index,int lane)
{
    int mode=D_8014978C;
    u32 bits;
    if(mode>=0 && mode<6) {
        Player *player=&input_rec0[index];
        Resource *resource;
        Record96 *record;
        if(player->handle==0)player->handle=(Handle *)&D_80146150[player->selector];
        resource=player->handle->object->resource;
        if(resource==0)return 16;
        record=(Record96 *)(resource->data+140)+mode;
        bits=record->bits;
    } else if(mode>=14 && mode<18) {
        Player *player=&input_rec0[index];
        Record64 *record;
        if(player->handle==0)player->handle=(Handle *)&D_80146150[player->selector];
        record=(Record64 *)(player->handle->object->resource->data+396)+mode;
        bits=record->bits;
    } else bits=0;
    if(lane!=0) {
        u32 mask;
        if(lane==1)mask=0xff00;
        else mask=0xff;
        bits&=mask;
    }
    return (u8)func_800B78A4(bits,16);
}
