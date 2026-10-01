/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define FIELD(e,t,o) (*(t *)((u8 *)(e)+(o)))
extern u8 D_8014A250[];
extern u8 *D_80150F38;
extern void func_8009E820(void *,void *,void *);
extern void func_800A61B0(void *,void *,void *);
extern void random_float(void *,void *,void *,void *,void *);
s32 func_8010C02C(void *arg0,s16 *arg1,void *arg2,s32 arg3) {
    f32 delta[3],world[3],local[3];
    f32 *p;
    f32 dist=0.0f,radius;
    s32 offset,hit;
    u8 *car,*shape,*point;
    f32 *bounds;
    car=D_8014A250+*arg1*0x808;
    shape=D_80150F38+(FIELD(arg0,s8,0x65)<<6);
    if(!FIELD(car,s8,0x7EA))return 0;
    delta[0]=FIELD(car,f32,0x794)-FIELD(shape,f32,0x28);
    delta[1]=FIELD(car,f32,0x798)-FIELD(shape,f32,0x2C);
    delta[2]=FIELD(car,f32,0x79C)-FIELD(shape,f32,0x30);
    for(p=delta;(u32)p<(u32)(delta+3);p++)dist+=*p**p;
    bounds=FIELD(shape,f32 *,0);
    radius=bounds[6]+FIELD(car,f32,0x654);
    if(radius*radius<dist)return 0;
    point=car+0xF4;
    offset=0;
    do {
        func_8009E820(point,world,car+0x7A0);
        world[0]=FIELD(car,f32,0x794)+world[0];
        world[1]=FIELD(car,f32,0x798)+world[1];
        world[2]=FIELD(car,f32,0x79C)+world[2];
        world[0]-=FIELD(shape,f32,0x28);
        world[1]-=FIELD(shape,f32,0x2C);
        world[2]-=FIELD(shape,f32,0x30);
        func_800A61B0(world,local,shape+4);
        bounds=FIELD(shape,f32 *,0);
        offset+=12;
        if(bounds[1]<local[0]||local[0]<bounds[0])hit=0;
        else if(bounds[4]<local[1]||local[1]<bounds[5])hit=0;
        else if(bounds[2]<local[2]||local[2]<bounds[3])hit=0;
        else hit=1;
        if(hit) { random_float(car,car,shape,delta,local);break; }
        point+=12;
    }while(offset!=48);
    return 0;
}
