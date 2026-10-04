/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Vector {float x,y,z;} Vector;
typedef struct Emitter {
    struct Emitter *next,*previous;
    u32 flags;
    Vector position,velocity;
    float range,gain,minimum,curve;
    u32 handle,context;
    u16 identifier,counter;
    float fade;
} Emitter;
extern Emitter *D_8004FD50;
extern Emitter D_8004FD58;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001C860(Emitter *,float *,float *,float *,float *,float *);
extern void func_8001CCDC(Emitter *,float,float,float,float,float);
extern u32 func_80020174(u16,u8,u8);
u32 func_8001D1F4(Emitter *state,const Vector *position,const Vector *velocity,
    float range,float curve,u32 flags,u16 identifier,u32 context,u8 gain,u8 minimum)
{
    Emitter *target;
    float volume,pitch,pan,span,send;
    func_80014594();
    if (state==0) target=&D_8004FD58;
    else target=state;
    target->flags=flags;
    target->position=*position;
    target->velocity=*velocity;
    target->range=range;
    target->identifier=identifier;
    target->gain=(float)gain/127.0f;
    target->minimum=(float)minimum/127.0f;
    target->curve=curve;
    target->context=context;
    if (state==0) {
        func_8001C860(target,&volume,&pitch,&pan,&span,&send);
        if (volume==0.0f) {func_800145DC();return 0xFFFFFFFFU;}
        target->handle=func_80020174(target->identifier,127,64);
        if (target->handle==0xFFFFFFFFU) {func_800145DC();return 0xFFFFFFFFU;}
        func_8001CCDC(target,volume,pan,span,send,pitch);
        func_800145DC();
        return target->handle;
    }
    if ((target->next=D_8004FD50)!=0) D_8004FD50->previous=target;
    target->previous=0;
    D_8004FD50=target;
    target->handle=0xFFFFFFFFU;
    target->counter=0;
    target->flags|=0x30000;
    func_800145DC();
    return 0xFFFFFFFFU;
}
