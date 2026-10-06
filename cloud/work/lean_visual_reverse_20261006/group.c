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
void entity_hierarchy_update(s32 h,f32 value) {
    osRecvMesg(&D_80142728,0,1);
    entity_transform_calc(h,-2.0f,-2.0f,-2.0f,value);
    osJamMesg(&D_80142728,0,0);
}
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

/* Complete native Visual callback @800924F4. Visual type/operation ancestry
 * follows visuals.c; its timed motion/state behavior is specific to N64. */
typedef unsigned int u32;
typedef struct B137Visual24 {u8 prefix[4];s16 index,object,slot,reserved;u32 flags;f32 time;void(*callback)(void);} B137Visual24;
typedef struct B137Model2056 {u8 prefix[8];u8 type;u8 gap9[1007];s16 state;u8 gap1018[2];s32 sound;u8 gap1024[966];s16 player;u8 tail[64];} B137Model2056;
typedef struct B137Pose {u8 prefix[20];f32 angle;} B137Pose;
typedef struct B137Car952 {u8 prefix[232];u32 flags;u8 gap236[624];s8 part,quality,reserved,hidden;u8 gap864[32];B137Pose *pose;u8 tail[52];} B137Car952;
typedef struct B137Object68 {u32 flags;u8 rest[64];} B137Object68;
extern B137Model2056 D_8014A250[];
extern B137Car952 player_array[];
extern B137Object68 D_8012E700[];
extern f32 D_8011B4B4[][3],D_8011418C[9];
extern f32 D_801543CC,D_801239F0,D_801239F4,D_801239F8,D_801239FC;
extern s8 D_8013FECB,D_80140418;
extern s16 D_8015A108;
extern void model_data_load(s16,s32,s32),model_transform_setup(s16,s32,s32);
extern s32 entity_flags_apply(s32,s16,s32,s32);
extern void func_80092484(s16,s16),math_utility(void *,void *);
extern void func_80090F44(f32,void *),func_8008D6FC(s16,void *,void *);
extern void entity_spawn_init(s16,s32,s32,void *);
extern s32 model_bounds_calc(s32,B137Visual24 *);
extern s32 matrix_scale_apply(B137Visual24 *,s32,s32);
void buffer_swap(B137Visual24 *v,s16 op) {
    s16 slot;
    s32 reverse,lucent,hulk;
    f32 sign;
    f32 position[3],basis[9];
    B137Model2056 *model;
    B137Car952 *car;
    slot=v->slot;
    if(op==0) {
        if(v->object>=0)model_data_load(v->object,0,15);
        v->callback=0;
        v->object=-1;
        return;
    }
    if(v->flags&1)reverse=1;else reverse=0;
    sign=reverse?1.0f:-1.0f;
    model=&D_8014A250[slot];
    position[0]=sign*D_8011B4B4[model->type][0];
    position[1]=D_8011B4B4[model->type][1];
    position[2]=0.0f;
    if(model->state==0 || D_8013FECB!=0) {
        if(v->flags&0x10) {
            v->flags &= ~0x100;
            v->flags |= 0x1200;
            if(model->sound==-1) {
                s32 handle=entity_flags_apply(61,model->player,2,2);
                model->sound=handle;
                osRecvMesg(&D_80142728,0,1);
                entity_transform_calc(handle,-2.0f,-2.0f,-2.0f,0.75f);
                osJamMesg(&D_80142728,0,0);
                client_sync(model->sound,0.5f);
            }
        } else if(!(v->flags&0x1000)) {
            func_80092484(slot,(s16)reverse);
            model_data_load(v->object,0,15);
            return;
        }
    }
    if(model->state!=0 && !(v->flags&0x10)) {
        model_transform_setup(v->object,0,15);
        v->flags|=0x1110;
        v->index=0;
        v->time=D_801543CC+D_801239F0;
        if(model->sound==-1) {
            s32 handle=entity_flags_apply(61,model->player,2,2);
            model->sound=handle;
            osRecvMesg(&D_80142728,0,1);
            entity_transform_calc(handle,-2.0f,-2.0f,-2.0f,0.75f);
            osJamMesg(&D_80142728,0,0);
            client_sync(model->sound,0.5f);
        }
    }
    if(v->flags&0x1000) {
        if(v->flags&0x100) {
grow:
            if(v->time<D_801543CC) {
                position[0]*=(f32)v->index/6.0f;
                func_8008D6FC(v->object,position,0);
                v->index++;
                v->time=D_801543CC+D_801239F4;
                if(v->index==6) {
                    v->flags &= ~0x1100;
                    if(model->sound!=-1) {
                        scheduler_recv(model->sound);
                        model->sound=-1;
                    }
                }
            }
        } else if(v->flags&0x200) {
            if(model->state!=0 && D_8013FECB==0) {
                v->flags &= ~0x200;
                v->flags |= 0x100;
                goto grow;
            }
            if(v->time<D_801543CC) {
                position[0]*=(f32)v->index/6.0f;
                func_8008D6FC(v->object,position,0);
                v->index--;
                v->time=D_801543CC+D_801239F8;
                if(v->index==-1) {
                    v->flags &= ~0x1210;
                    if(model->sound!=-1) {
                        scheduler_recv(model->sound);
                        model->sound=-1;
                    }
                }
            }
        }
    }
    if(v->flags&0x10) {
        math_utility(D_8011418C,basis);
        func_80090F44(player_array[slot].pose->angle*D_801239FC,basis);
        func_8008D6FC(v->object,0,basis);
        if(D_8015A108==1)entity_spawn_init(slot,reverse?7:6,0,position);
    }
    car=&player_array[slot];
    hulk=(car->flags&0x10)!=0;
    lucent=(D_80140418!=0)||(car->flags&8)!=0;
    if(model_bounds_calc(!hulk&&!car->hidden,v)) {
        matrix_scale_apply(v,car->quality>=2,car->part);
        if(lucent)D_8012E700[v->object].flags^=0x80000000;
        else D_8012E700[v->object].flags&=0x7FFFFFFF;
    }
}
