/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
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
Entity *func_80091BA8(s32 h);
Msg *func_80091B00(void);
void entity_transform_calc(s32 h,f32 x,f32 y,f32 z,f32 w);
void client_sync(s32 h,f32 opacity);
__inline void entity_hierarchy_update(s32 h,f32 value) {
    osRecvMesg(&D_80142728,0,1);
    entity_transform_calc(h,-2.0f,-2.0f,-2.0f,value);
    osJamMesg(&D_80142728,0,0);
}
void scheduler_recv(s32 h);
typedef struct LayerState {s32 handle;f32 level,style;u8 other12[8];} LayerState;
typedef struct LayerSet {LayerState layer[3];} LayerSet;
typedef struct ModelView {u8 other0[1564];u16 contact[4];u8 other1572[308];s16 steering;u8 other1882[108];s16 index;u8 other1992[48];f32 power[4];} ModelView;
extern LayerSet D_801406C0[];

s32 frame_sync(s32,s32,s32,s32);
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
    s32 i,count0,count1,count2;
    f32 sum0,sum1,sum2;
    f32 fraction,scale;
    s16 slot;
    LayerSet *set;
    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    for (i=0;i<4;i++) {
        if (count0|count1|count2|i) {}
        if (model->contact[i]==0) {
            fraction=(f32)(model->steering>>2)/160.0f;
            if(fraction<0.0f) fraction=-fraction;
            fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
            fraction=1-(1-fraction)*(1-fraction);
            count0++;
            fraction/=count0==1 ? 2.0f : count0==2 ? 4.0f : 8.0f;
            sum0+=fraction;
        } else if(model->contact[i]==1) {
            fraction=(f32)(model->steering>>2)/80.0f;
            if(fraction!=0.0f) {
                if(fraction<0.0f) fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
                fraction=1-(1-fraction)*(1-fraction);
            }
            if(fraction<model->power[i])fraction=model->power[i];
            count1++;
            fraction/=count1==1 ? 2.0f : count1==2 ? 4.0f : 8.0f;
            sum1+=fraction;
        } else if(model->contact[i]==2 || model->contact[i]==3) {
            fraction=(f32)(model->steering>>2)/80.0f;
            count2++;
            if(fraction!=0.0f) {
                if(fraction<0.0f)fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
                fraction=1-(1-fraction)*(1-fraction);
            }
            fraction/=count2==1 ? 2.0f : count2==2 ? 4.0f : 8.0f;
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
        mode_select_input(&set->layer[0],sum1*0.7f,(sum1>0.5f ? sum1 : 0.5f)-0.25f);
    } else if(sum0<sum2 && sum1<sum2) {
        set=&D_801406C0[slot];
        if(set->layer[1].handle==-1) {
            scheduler_recv(set->layer[0].handle);
            player_conditional_call(&set->layer[0]);
            scheduler_recv(set->layer[2].handle);
            player_conditional_call(&set->layer[2]);
            if(sum2>0.33f)frame_sync(21,model->index,2,1);
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
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern s8 D_8010FFC0,D_8010FFCC[],D_8010FFC4[];
extern s8 D_801115CD[][13];
extern volatile f32 D_8002EB90;
extern f32 D_8011F060[],D_80110020[],D_8012438C,D_80124390;
extern LayerState D_80140640[];
extern s32 D_801407E0[],D_801407C0[];
extern f32 D_801141B0[3];
extern u8 input_rec0[];
void mode_select_handler(ModelView *);
s32 camera_target_track(f32 *,s32,f32,f32,f32,f32,s32,s32,s32,u8);
void results_screen_update(s32);
void camera_clip_planes(s32,f32 *,f32 *,f32,f32);

typedef struct {
    u8 pad0[4];
    u16 r0;          /* 0x04 */
    u16 r1;          /* 0x06 */
    u16 b0;          /* 0x08 */
    u16 b1;          /* 0x0A */
    u16 b2;          /* 0x0C */
    u8 padE[2];
    f32 p0;          /* 0x10 */
    f32 p1;          /* 0x14 */
    f32 p2;          /* 0x18 */
    u8 pad1C[4];
} EngTbl; /* 0x20 */
typedef struct {
    EngTbl *tbl;
    LayerState rec[4];
} SndSet; /* 0x54 */
typedef struct {
    u8 pad0[0x22C];
    f32 pos[3];      /* 0x22C */
    u8 pad238[0x404 - 0x238];
    f32 f404;        /* 0x404 */
    u8 pad408[0x7C6 - 0x408];
    s16 slot;        /* 0x7C6 */
    u8 pad7C8[4];
    s8 unk7CC;       /* 0x7CC */
    u8 pad7CD[3];
    s16 rpm;         /* 0x7D0 */
} MODELDAT;
extern u32 state_word_a;
extern SndSet D_80140420[];
extern s32 gameplay_mode;
extern s16 active_player_count;
extern s16 D_80152032;
s32 leaderboard_update(s32 handle);
f32 func_8008B2E4(f32 max);
void player_conditional_check(LayerState *rec, s32 arg1);
/*
 * func_800E0050 (w12c): EQUAL (360 words) in the whole-program unit with its REAL caller func_800E05F0
 * (jal at 0x800E0688; the parameter is passed in the caller's out-arg slot 0(sp), s0-s8/f20-f30 unsaved).
 * Provisional until func_800E05F0 itself matches.  Body from w10d (per-car engine-loop sound update) with:
 *  - player_conditional_check is NOT redefined: the locked kept body is inlined cross-file by umerge.
 *  - colouring tie (w10d's 8-word residual, b0/b1 float conversions, save 15 each): broken by the
 *    compiled-out `if (rpm < t->b1) {}` in the final else arm (adds one use to the b1 conversion web).
 *    Shaping quirk, disclosed.
 *  - `f32 unused[2]` is still the exact frame residual (retail frame 72; without it 19 words differ).
 */
void func_800E0050(MODELDAT *m) {
    f32 rpm;
    f32 load;
    f32 unused[2];
    s32 slot;
    s32 i;
    SndSet *set;
    EngTbl *t;
    f32 vol;
    f32 pitch;
    f32 *pos;
    f32 *ref;

    slot = m->slot;
    if (D_8010FFC0 == 0) {
        return;
    }
    if (m->rpm < 0) {
        rpm = -m->rpm;
    } else {
        rpm = m->rpm;
    }
    load = m->f404;
    if (!(state_word_a & 0x400000) && m->unk7CC != 2 && !(rpm < 890.0f)) {
        rpm = 890.0f;
    }
    set = &D_80140420[slot];
    ref = D_801141B0;
    for (i = 0; i < 2; i++) {
        if (set->rec[i].handle == -1) {
            return;
        }
        t = &set->tbl[i];
        vol = ((f32) t->r0 / (f32) t->r1) * (rpm / t->r0 - 1.0f) + 1.0f;
        vol = vol < 0.0f ? 0.0f : vol > 2.0f ? 2.0f : vol;
        if (rpm < t->b0) {
            pitch = t->p0;
        } else if (rpm < t->b1) {
            pitch = (rpm - t->b0) / (t->b1 - t->b0);
            pitch = (t->p1 - t->p0) * pitch + t->p0;
        } else if (rpm < t->b2) {
            pitch = (rpm - t->b1) / (t->b2 - t->b1);
            pitch = (t->p2 - t->p1) * pitch + t->p1;
        } else {
            if (rpm < t->b1) {}
            pitch = t->p2;
        }
        pitch *= 0.85f + ((load + 200.0f) / 900.0f) * 0.15f;
        if (m->unk7CC == 2) {
            pitch *= 0.8f - 0.05f * (gameplay_mode == 2 ? 1 : active_player_count);
            if (vol != set->rec[i].style) {
                set->rec[i].style = vol;
                entity_hierarchy_update(set->rec[i].handle, vol);
            }
            if (pitch != set->rec[i].level) {
                set->rec[i].level = pitch;
                client_sync(set->rec[i].handle, pitch);
            }
        } else {
            pitch *= 0.75f;
            pos = m->pos;
            if (state_word_a & 0x400000) {
                if (set->rec[3].handle != -1) {
                    player_conditional_check(&set->rec[3], 1);
                }
            } else if (0.75f < D_80152032) {
                if (set->rec[3].handle == -1 || leaderboard_update(set->rec[3].handle) == 0) {
                    if ((s32) func_8008B2E4(5.0f) == 0) {
                        set->rec[3].handle = camera_target_track(ref, (s32) ref, 400.0f, 0.0f, 1.0f, 0.0f,
                            (s32) (func_8008B2E4(3.0f) + 98.0f), slot, 0, 128);
                        camera_clip_planes(set->rec[3].handle, pos, ref, 0.8f, -2.0f);
                    }
                }
            }
            if (vol != set->rec[i].style) {
                set->rec[i].style = vol;
            } else {
                vol = -2.0f;
            }
            camera_clip_planes(set->rec[i].handle, pos, ref, pitch, vol);
        }
    }
}
static void layer_set(LayerState *state, f32 level, f32 style)
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
static void layer_stop(LayerState *e, s32 mode)
{
    if (mode==2) scheduler_recv(e->handle);
    else results_screen_update(e->handle);
    player_conditional_call(e);
}
void func_800E0048(LayerState *e, f32 *pos, f32 level, f32 style)
{
    camera_clip_planes(e->handle,pos,D_801141B0,level,style);
}
/*
 * func_800E05F0 (w12c): NEAR-MISS, 12 ops rows / 43 word rows in the unit (w11f: 60 ops rows).
 *  - natural literals 0.6f / 0.1f (retail .rodata 0x8012438C / 0x80124390 are this function's own
 *    literals, not globals; verify at splice); D_8002EB90 volatile; frame_sync's 2nd parameter s32 here;
 *  - `style = style<value ? value : style` ternary max updates; `D_8011F060[i]/2.0f + 1` (int 1);
 *  - `weighted = model->power[i]*D_8011F060[i]` (uopt swaps operands: this gives retail's D*power);
 *  - `level = value = 0.0f` (dead init of value: makes value's web precede weighted's -> f2/f12);
 *  - entry->handle read directly (no handle local); `& 0x10` flag test with the w4/w56 operand order.
 *  - layer_set / layer_stop static inlines + block-local pos only reproduce the 224-byte frame: a
 *    hypothesis (the image has no stubs for two deleted statics near here).
 * Open residual (RESULTS.md): a1/a2 tie (&D_8011F060 vs const 4), `.alias $8,$sp` after
 * player_conditional_call (proven by listing edit to reorder mfc1/swc1 at client_sync), as1 hoisting of
 * the camera_clip_planes a2/a3 setup into the compare block, and two stack-home offsets.
 */
#define MODE(m) FIELD(m,s8,1996)
void func_800E05F0(ModelView *model)
{
    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 style;
    f32 weighted,value;
    s32 i;
    s32 n;
    s32 d2,d3;
    f32 level;
    if (!D_8010FFC0) return;
    if (D_8010FFCC[original_slot]) {
        D_8010FFCC[original_slot]=0;
        return;
    }
    D_8010FFCC[original_slot]=1;
    if (!D_8010FFC4[original_slot]) return;
    func_800E0050((MODELDAT *)model);
    slot=model->index;
    if (MODE(model)==2) {
        style=0.5f; level=value=0.0f;
        n=4;
        for(i=0;i<n;i++) {
            if(model->contact[i]==0) {
                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;
            }
        }
        entry=&D_80140640[slot];
        if(entry->handle==-1 && level>0.0f) {
            if(MODE(model)==2) {
                entry->handle=frame_sync(36,model->index,2,2);
            } else {
                entry->handle=camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
            }
        }
        if(entry->handle!=-1) {
            if(level<=0.0f) {
                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);
            } else if(MODE(model)==2) {
                if (level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if (style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }
            } else {
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                func_800E0048(entry,(f32 *)((u8 *)model+556),level,style);
            }
        }
    }
    if(MODE(model)!=2)return;
    func_800DFBA0(model);
    mode_select_handler(model);
    if(D_8002EB90<D_80110020[original_slot])D_80110020[original_slot]=0.0f;
    if(!(FIELD(model,s32,2004)&0x10) &&
       (FIELD(input_rec0,s32,original_slot*76+4)&FIELD(input_rec0,s32,original_slot*76+56)) &&
       D_8002EB90-D_80110020[original_slot]>0.1f) {
        D_80110020[original_slot]=D_8002EB90;
        D_801407E0[original_slot]=frame_sync(D_801115CD[original_slot][FIELD(model,u8,8)]+26,original_slot,1,4);
    }
    if(FIELD(model,s16,1628)==1) {
        D_801407C0[original_slot]=frame_sync(25,original_slot,2,1);
        FIELD(model,s16,1628)=2;
    } else if(FIELD(model,s16,1628)==3) {
        scheduler_recv(D_801407C0[original_slot]);
        D_801407C0[original_slot]=-1;
        FIELD(model,s16,1628)=0;
    }
}
