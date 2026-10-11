/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct Entity {
    u8 pad0[12]; s32 id; s32 state; u8 pad14[6];
    u8 msgCount; u8 pad1B[9]; f32 x,y,z,w; u8 pad34[16];
} Entity;
typedef struct Msg {
    s16 id; s8 type,used;
    union { struct { f32 x,y,z,w; } value; Entity *target; } payload;
    Entity *ent;
} Msg;
extern Entity *D_80110244;
extern s32 D_80146104;
extern Msg D_80142DD8[128];
extern s32 D_80142728, D_801427A8;
s32 osRecvMesg(void *,void *,s32);
s32 osJamMesg(void *,void *,s32);
Entity *func_80091BA8(s32 h) {
    if(h==-1)return 0;
    if(D_80110244[h&D_80146104].id!=h)return 0;
    return &D_80110244[h&D_80146104];
}
Msg *func_80091B00(void) {
    s32 i;
    for(i=0;i<128;i++){
        if(D_80142DD8[i].used==0){
            D_80142DD8[i].used=1;
            D_80142DD8[i].id=-1;
            return &D_80142DD8[i];
        }
    }
    return 0;
}
void entity_transform_calc(s32 h,f32 x,f32 y,f32 z,f32 w) {
    Msg *m=0;
    Entity *e=func_80091BA8(h);
    if(e!=0){
        if((x!=-2.0f && x!=e->x) || (y!=-2.0f && y!=e->y) ||
           (z!=-2.0f && z!=e->z) || (w!=-2.0f && w!=e->w)) {
            m=func_80091B00();
            m->type=4;
            m->ent=e;
            e->msgCount++;
            if(x!=-2.0f && x!=e->x)m->payload.value.x=x; else m->payload.value.x=-2.0f;
            if(y!=-2.0f && y!=e->y)m->payload.value.y=y; else m->payload.value.y=-2.0f;
            if(z!=-2.0f && z!=e->z)m->payload.value.z=z; else m->payload.value.z=-2.0f;
            if(w!=-2.0f && w!=e->w)m->payload.value.w=w; else m->payload.value.w=-2.0f;
        }
    }
    if(m!=0)osJamMesg(&D_801427A8,m,0);
}
void client_sync(s32 h,f32 opacity) {
    osRecvMesg(&D_80142728,0,1);
    entity_transform_calc(h,opacity<0.0f ? 0.0f : opacity>1.0f ? 1.0f : opacity,
                          -2.0f,-2.0f,-2.0f);
    osJamMesg(&D_80142728,0,0);
}
void entity_hierarchy_update(s32 h,f32 value);
void scheduler_recv(s32 h) {
    Msg *m=0;
    Entity *e;
    osRecvMesg(&D_80142728,0,1);
    e=func_80091BA8(h);
    if(e!=0) {
        m=func_80091B00();
        m->type=6;
        m->payload.target=e;
        e->msgCount++;
    }
    osJamMesg(&D_80142728,0,0);
    if(m!=0)osJamMesg(&D_801427A8,m,0);
}

void stat_lap_complete(s32 h,f32 value) {
    osRecvMesg(&D_80142728,0,1);
    entity_transform_calc(h,-2.0f,value<-1.0f ? -1.0f : value>1.0f ? 1.0f : value,-2.0f,-2.0f);
    osJamMesg(&D_80142728,0,0);
}
void game_timer_resume(s32 h,f32 value) {
    osRecvMesg(&D_80142728,0,1);
    entity_transform_calc(h,-2.0f,-2.0f,value<-1.0f ? -1.0f : value>1.0f ? 1.0f : value,-2.0f);
    osJamMesg(&D_80142728,0,0);
}
