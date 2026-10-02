/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef struct Resource {unsigned char prefix[16];u16 width,height;} Resource;
typedef struct Object {
    const char *name;
    void *fallback;
    Resource *resource;
    u16 index;
    unsigned char opaque[4];
    u16 state,width,height;
    u8 color;
    unsigned char gap[3];
    s16 first,second,third,fourth,fifth;
} Object;
extern unsigned char D_80110664[];
extern u8 D_80140BDC;
extern Resource *func_800B24EC(const char *,u16 *,s8,s8,int);
void collision_sound_play(Object *object) {
    if (object->name==0) {
        object->index=0;
        object->resource=0;
        object->width=0;
        object->height=0;
        object->fallback=D_80110664;
    } else if (object->name==(const char *)-1) {
        object->index=0;
        object->resource=0;
        object->width=0;
        object->height=0;
        object->fallback=0;
    } else {
        object->resource=func_800B24EC(object->name,&object->index,0,(s8)(D_80140BDC-1),1);
        object->width=object->resource->width;
        object->height=object->resource->height;
        object->fallback=0;
    }
    object->state=0;
    object->color=255;
    object->first=object->second=object->third=object->fourth=object->fifth=-1;
}
