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
__inline void entity_hierarchy_update(s32 h,f32 value) {
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
typedef unsigned short u16;
typedef struct LayerState {s32 handle;f32 level,style;u8 other12[8];} LayerState;
typedef struct LayerSet {LayerState layer[3];} LayerSet;
typedef struct ModelView {u8 other0[1564];u16 contact[4];u8 other1572[308];s16 steering;u8 other1882[108];s16 index;u8 other1992[48];f32 power[4];} ModelView;
extern LayerSet D_801406C0[];
extern f32 D_80124370,D_80124374;
s32 frame_sync(s32,s16,s32,s32);
void player_conditional_call(LayerState *);
void mode_select_input(LayerState *state,f32 level,f32 style)
{
    if (level!=state->level) {
        state->level=level;
        client_sync(state->handle,level);
    }
    if (style!=state->style) {
        state->style=style;
        entity_hierarchy_update(state->handle,style);
    }
}
void func_800DFBA0(ModelView *model)
{
    s32 i,count0=0,count1=0,count2=0;
    f32 sum0=0.0f,sum1=0.0f,sum2=0.0f;
    f32 fraction,scale;
    s16 slot=model->index;
    LayerSet *set;
    for (i=0;i<4;i++) {
        if (model->contact[i]==0) {
            fraction=(f32)(model->steering>>2)/160.0f;
            if(fraction<0.0f) fraction=-fraction;
            fraction=fraction<0.0f ? 0.0f : fraction>1.0f ? 1.0f : fraction;
            fraction=1.0f-(1.0f-fraction)*(1.0f-fraction);
            count0++;
            if(count0==1) fraction/=2.0f;
            else fraction/=count0==2 ? 4.0f : 8.0f;
            sum0+=fraction;
        } else if(model->contact[i]==1) {
            fraction=(f32)(model->steering>>2)/80.0f;
            count1++;
            if(fraction!=0.0f) {
                if(fraction<0.0f) fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1.0f ? 1.0f : fraction;
                fraction=1.0f-(1.0f-fraction)*(1.0f-fraction);
            }
            if(fraction<model->power[i])fraction=model->power[i];
            if(count1==1)fraction/=2.0f;
            else fraction/=count1==2 ? 4.0f : 8.0f;
            sum1+=fraction;
        } else if(model->contact[i]==2 || model->contact[i]==3) {
            fraction=(f32)(model->steering>>2)/80.0f;
            count2++;
            if(fraction!=0.0f) {
                if(fraction<0.0f)fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1.0f ? 1.0f : fraction;
                fraction=1.0f-(1.0f-fraction)*(1.0f-fraction);
            }
            scale=count2==1 ? 2.0f : count2==2 ? 4.0f : 8.0f;
            fraction/=scale;
            sum2+=fraction;
        }
    }
    if(sum0<sum1 && sum2<sum1) {
        set=&D_801406C0[slot];
        if(set->layer[0].handle==-1) {
            scheduler_recv(set->layer[1].handle);
            player_conditional_call(&set->layer[1]);
            scheduler_recv(set->layer[2].handle);
            player_conditional_call(&set->layer[2]);
            set->layer[0].handle=frame_sync(11,model->index,1,2);
        }
        mode_select_input(&set->layer[0],sum1*D_80124370,(sum1>0.5f ? sum1 : 0.5f)-0.25f);
    } else if(sum0<sum2 && sum1<sum2) {
        set=&D_801406C0[slot];
        if(set->layer[1].handle==-1) {
            scheduler_recv(set->layer[0].handle);
            player_conditional_call(&set->layer[0]);
            scheduler_recv(set->layer[2].handle);
            player_conditional_call(&set->layer[2]);
            if(sum2>D_80124374)frame_sync(21,model->index,2,1);
            set->layer[1].handle=frame_sync(20,model->index,1,2);
        }
        mode_select_input(&set->layer[1],sum2,1.0f);
    } else {
        set=&D_801406C0[slot];
        if(set->layer[2].handle==-1) {
            scheduler_recv(set->layer[0].handle);
            player_conditional_call(&set->layer[0]);
            scheduler_recv(set->layer[1].handle);
            player_conditional_call(&set->layer[1]);
        }
    }
}
