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
Entity *func_80091BA8(s32 h);
Msg *func_80091B00(void);
void entity_transform_calc(s32 h,f32 x,f32 y,f32 z,f32 w);
void client_sync(s32 h,f32 opacity);
void entity_hierarchy_update(s32 h,f32 value);
void scheduler_recv(s32 h);

void stat_lap_complete(s32 h,f32 value);
void game_timer_resume(s32 h,f32 value);


typedef struct { float a; s32 pad[3]; float b; s32 c; } Ent;      /* 24 bytes */
typedef struct { Ent e[5]; } Slot;                                 /* 120 bytes */

extern Slot D_80140808[];
extern s16 D_80140A08[];
extern s32 D_80140AE0[];
extern float D_80140B10[];
extern float D_80140BE0[];
extern float D_80142518[];
extern void scheduler_recv(s32 h);

void best_times_display(s16 idx)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        D_80140808[idx].e[i].c = 0;
        D_80140808[idx].e[i].a = 0.0f;
        D_80140808[idx].e[i].b = 0.0f;
    }
    scheduler_recv(D_80140AE0[idx]);
    D_80140AE0[idx] = -1;
    D_80140A08[idx] = 0;
    D_80140B10[idx] = 0.0f;
    D_80140BE0[idx] = 0.0f;
    D_80142518[idx] = 0.0f;
}


typedef unsigned int u32;
typedef struct State20 {s32 handle;float a,b,c,d;} State20;
typedef struct Definition32 {s32 object;u8 rest[28];} Definition32;
typedef struct Definitions64 {Definition32 item[2];} Definitions64;
typedef struct Player84 {Definitions64 *definition;State20 pair[3];State20 extra;} Player84;
typedef struct States60 {State20 values[3];} States60;
typedef struct Vehicle2056 {u8 prefix[12];s8 category;u8 gap13[1587];s8 disabled;u8 gap1601[389];s16 player;u8 gap1992[4];s8 mode;u8 tail[59];} Vehicle2056;
extern s8 D_8010FFC0,D_8010FFC4[];
extern Definitions64 D_8010FD80[];
extern Player84 D_80140420[];
extern State20 D_80140640[];
extern States60 D_801406C0[];
extern s32 D_801407E0[],D_801407C0[];
extern char D_801141B0[];
extern u32 high_scores_display(u32,u32,u32,u8,float,float,float);
extern u32 camera_target_track(void *,void *,float,float,float,float,u32,u32,u32,u32);
void player_conditional_call(State20 *state);
void func_800D5E64(Vehicle2056 *vehicle)
{
 s16 player=vehicle->player;
 s8 *ready;
 Player84 *state;
 State20 *pair;
 int i;
 s32 object;
 if(!D_8010FFC0 || vehicle->disabled)return;
 ready=&D_8010FFC4[player];
 state=&D_80140420[player];
 state->definition=&D_8010FD80[vehicle->category];
 for(i=0,pair=state->pair;i<2;i++,pair++) {
  player_conditional_call(pair);
  object=state->definition->item[i].object;
  if(object!=-1) {
   if(vehicle->mode==2)pair->handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else pair->handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
  }
 }
 player_conditional_call(&state->extra);
 player_conditional_call(&D_80140640[player]);
 if(player<4) {
  player_conditional_call(&D_801406C0[player].values[0]);
  player_conditional_call(&D_801406C0[player].values[1]);
  player_conditional_call(&D_801406C0[player].values[2]);
  D_80140AE0[player]=-1;
  best_times_display(player);
  D_801407E0[player]=-1;
  D_801407C0[player]=-1;
 }
 *ready=1;
}

/* stand-in for the second real caller mode_select_handler (unmatched) */
void __standin_msh(Vehicle2056 *v)
{
    s16 p = v->player;
    if (v->disabled) {
        best_times_display(p);
        return;
    }
    scheduler_recv(p);
}
