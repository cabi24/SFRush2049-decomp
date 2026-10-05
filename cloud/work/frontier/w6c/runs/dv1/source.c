typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
s32 osRecvMesg(void *,void *,s32);
s32 osJamMesg(void *,void *,s32);
extern s32 D_80142728;
void client_sync(s32 h, f32 opacity);
void entity_transform_calc(s32 h, f32 x, f32 y, f32 z, f32 w);
void scheduler_recv(s32 h);
typedef struct LayerState {s32 handle;f32 level,style;u8 other12[8];} LayerState;
typedef struct LayerSet {LayerState layer[3];} LayerSet;
typedef struct ModelView {u8 other0[1564];u16 contact[4];u8 other1572[308];s16 steering;u8 other1882[108];s16 index;u8 other1992[48];f32 power[4];} ModelView;
extern LayerSet D_801406C0[];
s32 entity_flags_apply(s32,s16,s32,s32);
void player_conditional_call(LayerState *);
__inline void entity_hierarchy_update(s32 h, f32 value) {
    osRecvMesg(&D_80142728, 0, 1);
    entity_transform_calc(h, -2.0f, -2.0f, -2.0f, value);
    osJamMesg(&D_80142728, 0, 0);
}
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
    s32 i,count0,count1,count2;
    f32 sum0=0.0f,sum1=0.0f,sum2=0.0f;
    f32 fraction,scale;
    s16 slot=model->index;
    LayerSet *set;
    count0=0;
    count1=0;
    count2=0;
    for (i=0;i<4;i++) {
        if (model->contact[i]==0) {
            fraction=(f32)(model->steering>>2)/160.0f;
            if(fraction<0.0f) fraction=-fraction;
            fraction=fraction<0.0f ? 0.0f : fraction>1.0f ? 1.0f : fraction;
            fraction=1.0f-(1.0f-fraction)*(1.0f-fraction);
            count0++;
            fraction/=count0==1 ? 2.0f : count0==2 ? 4.0f : 8.0f;
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
            fraction/=count1==1 ? 2.0f : count1==2 ? 4.0f : 8.0f;
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
            set->layer[0].handle=entity_flags_apply(11,model->index,1,2);
        }
        mode_select_input(&set->layer[0],sum1*0.7f,(sum1>0.5f ? sum1 : 0.5f)-0.25f);
    } else if(sum0<sum2 && sum1<sum2) {
        set=&D_801406C0[slot];
        if(set->layer[1].handle==-1) {
            scheduler_recv(set->layer[0].handle);
            player_conditional_call(&set->layer[0]);
            scheduler_recv(set->layer[2].handle);
            player_conditional_call(&set->layer[2]);
            if(sum2>0.33f)entity_flags_apply(21,model->index,2,1);
            set->layer[1].handle=entity_flags_apply(20,model->index,1,2);
        }
        mode_select_input(&set->layer[1],sum2,1);
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
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern s8 D_8010FFC0,D_8010FFCC[],D_8010FFC4[],D_801115CD[];
extern f32 D_8011F060[],D_80110020[],D_8002EB90;
extern LayerState D_80140640[];
extern s32 D_801407E0[],D_801407C0[];
extern f32 D_801141B0[3];
extern u8 input_rec0[];
void func_800E0050(ModelView *);
void mode_select_handler(ModelView *);
s32 camera_target_track(f32 *,s32,f32,f32,f32,f32,s32,s32,s32,u8);
void results_screen_update(s32);
void camera_clip_planes(s32,f32 *,s32,f32,f32);
void func_800E05F0(ModelView *model)
{
    s16 original_slot=model->index;
    s16 slot;
    LayerState *entry;
    f32 style=0.5f,level=0.0f;
    s8 mode;
    s32 i,handle;
    if (!D_8010FFC0) return;
    if (D_8010FFCC[original_slot]) {
        D_8010FFCC[original_slot]=0;
        return;
    }
    D_8010FFCC[original_slot]=1;
    if (!D_8010FFC4[original_slot]) return;
    func_800E0050(model);
    mode=FIELD(model,s8,1996);
    slot=model->index;
    if (mode==2) {
        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {
                f32 weighted=D_8011F060[i]*model->power[i];
                f32 value=weighted-D_8011F060[i]*0.5f+1.0f;
                if(style<value)style=value;
                value=weighted*0.6f;
                if(level<value)level=value;
            }
        }
        entry=&D_80140640[slot];
        handle=entry->handle;
        if(handle==-1 && level>0.0f) {
            if(mode==2) handle=entity_flags_apply(36,model->index,2,2);
            else handle=camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
            entry->handle=handle;
            mode=FIELD(model,s8,1996);
        }
        if(handle!=-1) {
            if(level<=0.0f) {
                if(mode==2)scheduler_recv(handle);
                else results_screen_update(handle);
                player_conditional_call(entry);
                mode=FIELD(model,s8,1996);
            } else if(mode==2) {
                if(level!=entry->level) {
                    entry->level=level;
                    client_sync(handle,level);
                }
                if(style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }
                mode=FIELD(model,s8,1996);
            } else {
                f32 next_style;
                if(style!=entry->style) {
                    entry->style=style;
                    next_style=style;
                } else next_style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,next_style);
                mode=FIELD(model,s8,1996);
            }
        }
    }
    if(mode!=2)return;
    func_800DFBA0(model);
    mode_select_handler(model);
    if(D_8002EB90<D_80110020[original_slot])D_80110020[original_slot]=0.0f;
    if(!(FIELD(model,s32,2004)&0x10) &&
       (FIELD(input_rec0,s32,original_slot*76+56)&FIELD(input_rec0,s32,original_slot*76+4)) &&
       D_8002EB90-D_80110020[original_slot]>0.1f) {
        D_80110020[original_slot]=D_8002EB90;
        D_801407E0[original_slot]=entity_flags_apply(D_801115CD[original_slot*13+FIELD(model,u8,8)]+26,original_slot,1,4);
    }
    if(FIELD(model,s16,1628)==1) {
        D_801407C0[original_slot]=entity_flags_apply(25,original_slot,2,1);
        FIELD(model,s16,1628)=2;
    } else if(FIELD(model,s16,1628)==3) {
        scheduler_recv(D_801407C0[original_slot]);
        D_801407C0[original_slot]=-1;
        FIELD(model,s16,1628)=0;
    }
}
