/* Full genuine B115 player initializer/shared cleanup and unchanged B109 runtime context.
 * Research only; no synthetic callers. Flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
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


typedef struct Impact24 {f32 force;f32 vector[3];f32 timestamp;s32 flag;} Impact24;
typedef struct Impact120 {Impact24 point[5];} Impact120;
typedef Impact120 Slot;

extern Slot D_80140808[];
extern s16 D_80140A08[];
extern s32 D_80140AE0[];
extern float D_80140B10[];
extern float D_80140BE0[];
extern float D_80142518[];
extern void scheduler_recv(s32 h);

void best_times_display(s16 idx)
{
    Slot *s = &D_80140808[idx];
    s32 i;
    s->point[0].flag = 0;
    s->point[0].force = 0.0f;
    s->point[0].timestamp = 0.0f;
    i = 1;
    do {
        s->point[i].flag = 0;
        s->point[i].force = 0.0f;
        s->point[i].timestamp = 0.0f;
        i++;
    } while (i < 5);
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
void player_conditional_call(State20 *state) {state->handle=-1;state->a=-2.0f;state->b=-2.0f;state->c=-2.0f;state->d=-2.0f;}
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

#define NULL ((void *)0)
#define M2C_FIELD(base,type,offset) (*(type)((u8 *)(base)+(offset)))
extern float D_8002EB90;
extern s8 D_8010FFCC[], D_801115CD[];
extern f32 D_80110020[],D_80124320,D_80124370,D_80124374,D_80124378,D_8012437C,D_80124380,D_80124384,D_80124388,D_8012438C,D_80124390;
extern s16 D_80152032,active_player_count;
extern s32 gameplay_mode,state_word_a,D_8011735C;
s32 entity_flags_apply(s32,s16,s32,s32);
extern u8 input_rec0[];extern f32 D_8011F060[];
f32 sqrtf(f32);f32 fabsf(f32);
#pragma intrinsic(sqrtf)
#pragma intrinsic(fabsf)
s32 func_80007270(void*,void*,s32);s32 func_800075e0(void*,void*,s32);
void func_800E0050(void *);void func_800DFBA0(void *);void mode_select_handler(void *);
void mode_select_input(f32,void*,f32);void func_800DED78(s32,s16,void*,f32);
void camera_clip_planes(s32,void*,void*,f32,f32);
void results_screen_update(s32);s32 leaderboard_update(s32);

void func_800E0050(void *arg0) {
    f32 sp40;
    s32 sp34;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f28;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f12;
    f32 var_f20;
    f32 var_f28;
    f32 var_f2;
    f32 var_f30;
    f32 var_f6;
    f32 var_f8;
    s16 temp_v0;
    s16 var_v0;
    s32 temp_a0_3;
    s32 temp_t0;
    s32 temp_t6;
    s32 temp_v0_3;
    s32 temp_v1_2;
    s32 var_s5;
    u16 temp_a0;
    u16 temp_a0_2;
    u16 temp_t7;
    u16 temp_t9;
    u16 temp_v1;
    void *temp_s2;
    void *temp_s3;
    void *temp_v0_2;
    void *var_s1;

    sp34 = (s32) M2C_FIELD(arg0, s16 *, 0x7C6);
    if (D_8010FFC0 != 0) {
        temp_v0 = M2C_FIELD(arg0, s16 *, 0x7D0);
        var_s5 = 0;
        if (temp_v0 < 0) {
            var_f20 = (f32) -temp_v0;
        } else {
            var_f20 = (f32) temp_v0;
        }
        sp40 = M2C_FIELD(arg0, f32 *, 0x404);
        if (!(state_word_a & 0x400000) && (M2C_FIELD(arg0, s8 *, 0x7CC) != 2) && !(var_f20 < D_80124378)) {
            var_f20 = D_80124378;
        }
        temp_s3 = (sp34 * 0x54) + (u8 *)D_80140420;
        var_s1 = temp_s3;
loop_9:
        temp_t0 = M2C_FIELD(var_s1, s32 *, 4);
        if (temp_t0 != -1) {
            temp_v0_2 = (u8 *)M2C_FIELD(temp_s3, Definitions64 **, 0) + (var_s5 << 5);
            temp_t9 = M2C_FIELD(temp_v0_2, u16 *, 4);
            var_f0 = (f32) temp_t9;
            if ((s32) temp_t9 < 0) {
                var_f0 += 4294967296.0f;
            }
            temp_t7 = M2C_FIELD(temp_v0_2, u16 *, 6);
            var_f8 = (f32) temp_t7;
            if ((s32) temp_t7 < 0) {
                var_f8 += 4294967296.0f;
            }
            temp_f28 = ((var_f0 / var_f8) * ((var_f20 / var_f0) - 1.0f)) + 1.0f;
            if (temp_f28 < 0.0f) {
                var_f28 = 0.0f;
            } else {
                if (temp_f28 > 2.0f) {
                    var_f0_2 = 2.0f;
                } else {
                    var_f0_2 = temp_f28;
                }
                var_f28 = var_f0_2;
            }
            temp_a0 = M2C_FIELD(temp_v0_2, u16 *, 8);
            var_f12 = (f32) temp_a0;
            if ((s32) temp_a0 < 0) {
                var_f12 += 4294967296.0f;
            }
            if (var_f20 < var_f12) {
                var_f30 = M2C_FIELD(temp_v0_2, f32 *, 0x10);
            } else {
                temp_v1 = M2C_FIELD(temp_v0_2, u16 *, 0xA);
                var_f2 = (f32) temp_v1;
                if ((s32) temp_v1 < 0) {
                    var_f2 += 4294967296.0f;
                }
                if (var_f20 < var_f2) {
                    temp_f0 = M2C_FIELD(temp_v0_2, f32 *, 0x10);
                    var_f30 = ((M2C_FIELD(temp_v0_2, f32 *, 0x14) - temp_f0) * ((var_f20 - var_f12) / (f32) (temp_v1 - temp_a0))) + temp_f0;
                } else {
                    temp_a0_2 = M2C_FIELD(temp_v0_2, u16 *, 0xC);
                    var_f6 = (f32) temp_a0_2;
                    if ((s32) temp_a0_2 < 0) {
                        var_f6 += 4294967296.0f;
                    }
                    if (var_f20 < var_f6) {
                        temp_f0_2 = M2C_FIELD(temp_v0_2, f32 *, 0x14);
                        var_f30 = ((M2C_FIELD(temp_v0_2, f32 *, 0x18) - temp_f0_2) * ((var_f20 - var_f2) / (f32) (temp_a0_2 - temp_v1))) + temp_f0_2;
                    } else {
                        var_f30 = M2C_FIELD(temp_v0_2, f32 *, 0x18);
                    }
                }
            }
            temp_f30 = var_f30 * (D_80124380 + (((sp40 + 200.0f) / 900.0f) * D_8012437C));
            if (M2C_FIELD(arg0, s8 *, 0x7CC) == 2) {
                if (gameplay_mode == 2) {
                    var_v0 = 1;
                } else {
                    var_v0 = active_player_count;
                }
                temp_f30_2 = temp_f30 * (D_80124388 - (D_80124384 * (f32) var_v0));
                if (var_f28 != M2C_FIELD(var_s1, f32 *, 0xC)) {
                    M2C_FIELD(var_s1, f32 *, 0xC) = var_f28;
                    func_80007270(&D_80142728, NULL, 1);

                    entity_transform_calc(temp_t0, -2.0f, -2.0f, -2.0f, var_f28);
                    func_800075e0(&D_80142728, NULL, 0);
                }
                if (temp_f30_2 != M2C_FIELD(var_s1, f32 *, 8)) {
                    M2C_FIELD(var_s1, f32 *, 8) = temp_f30_2;
                    client_sync(M2C_FIELD(var_s1, s32 *, 4), temp_f30_2);
                }
            } else {
                temp_s2 = (u8 *) arg0 + 0x22C;
                if (state_word_a & 0x400000) {
                    if (M2C_FIELD(temp_s3, s32 *, 0x40) != -1) {
                        results_screen_update(M2C_FIELD(temp_s3, s32 *, 0x40));
                        player_conditional_call((State20 *)((u8 *)temp_s3 + 0x40));
                    }
                } else if (((f32) D_80152032 > 0.75f) && ((temp_a0_3 = M2C_FIELD(temp_s3, s32 *, 0x40), (temp_a0_3 == -1)) || (leaderboard_update(temp_a0_3) == 0))) {
                    temp_v1_2 = (D_8011735C * 0x41C64E6D) + 0x3039;
                    D_8011735C = temp_v1_2;
                    if ((s32) (((f32) ((temp_v1_2 >> 0x10) & 0x7FFF) * 5.0f) / 32768.0f) == 0) {
                        temp_t6 = (temp_v1_2 * 0x41C64E6D) + 0x3039;
                        D_8011735C = temp_t6;
                        temp_v0_3 = camera_target_track(D_801141B0, D_801141B0, 400.0f, 0.0f, 1.0f, 0.0f, (s32) ((((f32) ((temp_t6 >> 0x10) & 0x7FFF) * 3.0f) / 32768.0f) + 98.0f), sp34, 0, 0x80);
                        M2C_FIELD(temp_s3, s32 *, 0x40) = temp_v0_3;
                        camera_clip_planes(temp_v0_3, temp_s2, D_801141B0, 0.8f, -2.0f);
                    }
                }
                if (var_f28 != M2C_FIELD(var_s1, f32 *, 0xC)) {
                    M2C_FIELD(var_s1, f32 *, 0xC) = var_f28;
                } else {
                    var_f28 = -2.0f;
                }
                camera_clip_planes(M2C_FIELD(var_s1, s32 *, 4), temp_s2, D_801141B0, temp_f30 * 0.75f, var_f28);
            }
            var_s5 += 1;
            var_s1 = (u8 *) var_s1 + 0x14;
            if (var_s5 != 2) {
                goto loop_9;
            }
        }
    }
}

void mode_select_input(f32 arg0, void *ipa_s0, f32 ipa_f28) {
    s32 temp_s1;

    if (arg0 != M2C_FIELD(ipa_s0, f32 *, 4)) {
        M2C_FIELD(ipa_s0, f32 *, 4) = arg0;
        client_sync(M2C_FIELD(ipa_s0, s32 *, 0), arg0);
    }
    if (ipa_f28 != M2C_FIELD(ipa_s0, f32 *, 8)) {
        M2C_FIELD(ipa_s0, f32 *, 8) = ipa_f28;
        temp_s1 = M2C_FIELD(ipa_s0, s32 *, 0);
        func_80007270(&D_80142728, NULL, 1);
        entity_transform_calc(temp_s1, -2.0f, -2.0f, -2.0f, ipa_f28);
        func_800075e0(&D_80142728, NULL, 0);
    }
}

void func_800DFBA0(void *ipa_s0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f2;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f0_5;
    f32 var_f12;
    f32 var_f12_2;
    f32 var_f12_3;
    f32 var_f24;
    f32 var_f26;
    f32 var_f2;
    f32 var_f2_2;
    f32 var_f2_3;
    f32 var_f2_4;
    f32 var_f2_5;
    f32 var_f2_6;
    f32 var_f30;
    s16 temp_t4;
    s32 temp_t7;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;
    u16 temp_v0;
    void *temp_s1;
    void *temp_s1_2;
    void *temp_s1_3;
    void *var_t1;

    temp_t4 = M2C_FIELD(ipa_s0, s16 *, 0x7C6);
    var_v1 = 0;
    var_a0 = 0;
    var_a1 = 0;
    var_a2 = 0;
    var_t1 = ipa_s0;
    var_f26 = 0.0f;
    var_f24 = 0.0f;
    var_f30 = 0.0f;
    do {
        temp_v0 = M2C_FIELD(var_t1, u16 *, 0x61C);
        if (temp_v0 == 0) {
            var_f2 = (f32) ((s16) M2C_FIELD(ipa_s0, s16 *, 0x758) >> 2) / 160.0f;
            if (var_f2 < 0.0f) {
                var_f2 = -var_f2;
            }
            if (var_f2 < 0.0f) {
                var_f2_2 = 0.0f;
            } else {
                if (var_f2 > 1.0f) {
                    var_f0 = 1.0f;
                } else {
                    var_f0 = var_f2;
                }
                var_f2_2 = var_f0;
            }
            temp_f0 = 1.0f - var_f2_2;
            var_v1 += 1;
            temp_f2 = 1.0f - (temp_f0 * temp_f0);
            if (var_v1 == 1) {
                var_f24 += temp_f2 / 2.0f;
            } else {
                if (var_v1 == 2) {
                    var_f12 = 4.0f;
                } else {
                    var_f12 = 8.0f;
                }
                var_f24 += temp_f2 / var_f12;
            }
        } else if (temp_v0 == 1) {
            var_a0 += 1;
            var_f2_3 = (f32) ((s16) M2C_FIELD(ipa_s0, s16 *, 0x758) >> 2) / 80.0f;
            if (var_f2_3 != 0.0f) {
                if (var_f2_3 < 0.0f) {
                    var_f2_3 = -var_f2_3;
                }
                if (var_f2_3 < 0.0f) {
                    var_f2_4 = 0.0f;
                } else {
                    if (var_f2_3 > 1.0f) {
                        var_f0_2 = 1.0f;
                    } else {
                        var_f0_2 = var_f2_3;
                    }
                    var_f2_4 = var_f0_2;
                }
                temp_f0_2 = 1.0f - var_f2_4;
                var_f2_3 = 1.0f - (temp_f0_2 * temp_f0_2);
            }
            temp_f0_3 = M2C_FIELD(((u8 *) ipa_s0 + (var_a2 * 4)), f32 *, 0x7F8);
            if (var_f2_3 < temp_f0_3) {
                var_f2_3 = temp_f0_3;
            }
            if (var_a0 == 1) {
                var_f26 += var_f2_3 / 2.0f;
            } else {
                if (var_a0 == 2) {
                    var_f12_2 = 4.0f;
                } else {
                    var_f12_2 = 8.0f;
                }
                var_f26 += var_f2_3 / var_f12_2;
            }
        } else if ((temp_v0 == 2) || (temp_v0 == 3)) {
            var_a1 += 1;
            var_f2_5 = (f32) ((s16) M2C_FIELD(ipa_s0, s16 *, 0x758) >> 2) / 80.0f;
            if (var_f2_5 != 0.0f) {
                if (var_f2_5 < 0.0f) {
                    var_f2_5 = -var_f2_5;
                }
                if (var_f2_5 < 0.0f) {
                    var_f2_6 = 0.0f;
                } else {
                    if (var_f2_5 > 1.0f) {
                        var_f0_3 = 1.0f;
                    } else {
                        var_f0_3 = var_f2_5;
                    }
                    var_f2_6 = var_f0_3;
                }
                temp_f0_4 = 1.0f - var_f2_6;
                var_f2_5 = 1.0f - (temp_f0_4 * temp_f0_4);
            }
            if (var_a1 == 1) {
                var_f0_4 = 2.0f;
            } else {
                if (var_a1 == 2) {
                    var_f12_3 = 4.0f;
                } else {
                    var_f12_3 = 8.0f;
                }
                var_f0_4 = var_f12_3;
            }
            var_f30 += var_f2_5 / var_f0_4;
        }
        var_a2 += 1;
        var_t1 = (u8 *) var_t1 + 2;
    } while (var_a2 != 4);
    if ((var_f24 < var_f26) && (var_f30 < var_f26)) {
        temp_s1 = (temp_t4 * 0x3C) + (u8 *)D_801406C0;
        if (M2C_FIELD(temp_s1, s32 *, 0) == -1) {
            scheduler_recv(M2C_FIELD(temp_s1, s32 *, 0x14));
            player_conditional_call((State20 *)((u8 *)temp_s1 + 0x14));
            scheduler_recv(M2C_FIELD(temp_s1, s32 *, 0x28));
            player_conditional_call((State20 *)((u8 *)temp_s1 + 0x28));
            M2C_FIELD(temp_s1, s32 *, 0) = entity_flags_apply(0xB, M2C_FIELD(ipa_s0, s16 *, 0x7C6), 1, 2);
        }
        if (var_f26 > 0.5f) {
            var_f0_5 = var_f26;
        } else {
            var_f0_5 = 0.5f;
        }
        mode_select_input(var_f26 * D_80124370, temp_s1, var_f0_5 - 0.25f);
        return;
    }
    temp_t7 = temp_t4 * 0x3C;
    if ((var_f24 < var_f30) && (var_f26 < var_f30)) {
        temp_s1_2 = (temp_t4 * 0x3C) + (u8 *)D_801406C0;
        if (M2C_FIELD(temp_s1_2, s32 *, 0x14) == -1) {
            scheduler_recv(M2C_FIELD(temp_s1_2, s32 *, 0));
            player_conditional_call(temp_s1_2);
            scheduler_recv(M2C_FIELD(temp_s1_2, s32 *, 0x28));
            player_conditional_call((State20 *)((u8 *)temp_s1_2 + 0x28));
            if (D_80124374 < var_f30) {
                entity_flags_apply(0x15, M2C_FIELD(ipa_s0, s16 *, 0x7C6), 2, 1);
            }
            M2C_FIELD(temp_s1_2, s32 *, 0x14) = entity_flags_apply(0x14, M2C_FIELD(ipa_s0, s16 *, 0x7C6), 1, 2);
        }
        mode_select_input(var_f30, (u8 *) temp_s1_2 + 0x14, 1.0f);
        return;
    }
    temp_s1_3 = (u8 *)D_801406C0 + temp_t7;
    if (M2C_FIELD(temp_s1_3, s32 *, 0x28) == -1) {
        scheduler_recv(M2C_FIELD((u8 *)D_801406C0 + temp_t7, s32 *, 0));
        player_conditional_call(temp_s1_3);
        scheduler_recv(M2C_FIELD(temp_s1_3, s32 *, 0x14));
        player_conditional_call((State20 *)((u8 *)temp_s1_3 + 0x14));
    }
}

void func_800DED78(s32 arg0, s16 arg1, void *arg2, f32 ipa_f18) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f2;
    s32 temp_t8;
    u8 *temp_v0;

    temp_v0 = (u8 *)D_80140808 + arg0 * 0x78 + arg1 * 0x18;
    temp_t8 = M2C_FIELD(temp_v0, s32 *, 0x14);
    if (temp_t8 == 0) {
        temp_f2 = M2C_FIELD(temp_v0, f32 *, 0x10);
        if ((temp_f2 != 0.0f) && (M2C_FIELD(temp_v0, f32 *, 0) == 0.0f)) {
            if ((M2C_FIELD(arg2, f32 *, 0) == 0.0f) && (M2C_FIELD(arg2, f32 *, 4) == 0.0f) && (M2C_FIELD(arg2, f32 *, 8) == 0.0f)) {
                if ((D_8002EB90 - temp_f2) > 0.5f) {
                    M2C_FIELD(temp_v0, f32 *, 0x10) = 0.0f;
                }
            } else {
                M2C_FIELD(temp_v0, f32 *, 0x10) = (f32) D_8002EB90;
            }
        } else {
            temp_f12 = M2C_FIELD(arg2, f32 *, 0);
            if ((temp_f12 != 0.0f) || (M2C_FIELD(arg2, f32 *, 4) != 0.0f) || (M2C_FIELD(arg2, f32 *, 8) != 0.0f)) {
                temp_f14 = M2C_FIELD(arg2, f32 *, 4);
                temp_f16 = M2C_FIELD(arg2, f32 *, 8);
                temp_f0 = sqrtf((temp_f16 * temp_f16) + ((temp_f12 * temp_f12) + (temp_f14 * temp_f14)));
                if ((ipa_f18 < temp_f0) && (M2C_FIELD(temp_v0, f32 *, 0) < temp_f0)) {
                    M2C_FIELD(temp_v0, f32 *, 0) = temp_f0;
                    M2C_FIELD(temp_v0, f32 *, 4) = (f32) M2C_FIELD(arg2, f32 *, 0);
                    M2C_FIELD(temp_v0, f32 *, 8) = (f32) M2C_FIELD(arg2, f32 *, 4);
                    M2C_FIELD(temp_v0, f32 *, 0x10) = (f32) D_8002EB90;
                    M2C_FIELD(temp_v0, f32 *, 0xC) = (f32) M2C_FIELD(arg2, f32 *, 8);
                }
            }
            if ((M2C_FIELD(temp_v0, f32 *, 0x10) != 0.0f) && (D_80124320 < (D_8002EB90 - M2C_FIELD(temp_v0, f32 *, 0x10)))) {
                M2C_FIELD(temp_v0, s32 *, 0x14) = 1;
                M2C_FIELD(temp_v0, f32 *, 0x10) = (f32) D_8002EB90;
            }
        }
    }
}

void func_800E05F0(void *arg0) {
    s32 spDC;
    s32 spA8;
    s32 *sp90;
    s32 *sp8C;
    f32 *temp_v1_2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f2;
    f32 var_f14;
    f32 var_f28;
    s16 temp_t1;
    s16 temp_v0_6;
    s16 temp_v1;
    s32 *temp_v0_7;
    s32 *var_t0;
    s32 temp_t0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a0_2;
    s32 var_v1;
    s8 *temp_v0;
    s8 temp_a3;
    void *temp_v0_5;
    void *var_a0;

    temp_v1 = M2C_FIELD(arg0, s16 *, 0x7C6);
    if (D_8010FFC0 != 0) {
        temp_v0 = &D_8010FFCC[temp_v1];
        if (*temp_v0 != 0) {
            *temp_v0 = 0;
            return;
        }
        *temp_v0 = 1;
        if (D_8010FFC4[temp_v1] != 0) {
            spDC = (s32) temp_v1;
            func_800E0050(arg0);
            var_v1 = 0;
            temp_a3 = M2C_FIELD(arg0, s8 *, 0x7CC);
            temp_t1 = M2C_FIELD(arg0, s16 *, 0x7C6);
            var_a0 = arg0;
            if (temp_a3 == 2) {
                var_f28 = 0.5f;
                var_f14 = 0.0f;
                do {
                    temp_v0_2 = var_v1 * 4;
                    if (M2C_FIELD(var_a0, u16 *, 0x61C) == 0) {
                        temp_f0 = *(f32 *)((u8 *)D_8011F060 + temp_v0_2);
                        temp_f12 = temp_f0 * M2C_FIELD(((u8 *) arg0 + temp_v0_2), f32 *, 0x7F8);
                        temp_f2 = (temp_f12 - (temp_f0 * 0.5f)) + 1.0f;
                        if (var_f28 < temp_f2) {
                            var_f28 = temp_f2;
                        }
                        temp_f0_2 = temp_f12 * D_8012438C;
                        if (var_f14 < temp_f0_2) {
                            var_f14 = temp_f0_2;
                        }
                    }
                    var_v1 += 1;
                    var_a0 = (u8 *) var_a0 + 2;
                } while (var_v1 != 4);
                var_t0 = (s32 *)((temp_t1 * 0x14) + (u8 *)D_80140640);
                var_a0_2 = M2C_FIELD(var_t0, s32 *, 0);
                if ((var_a0_2 == -1) && (var_f14 > 0.0f)) {
                    if (temp_a3 == 2) {
                        sp8C = var_t0;

                        temp_v0_3 = entity_flags_apply(0x24, temp_t1, 2, 2);
                        var_a0_2 = temp_v0_3;
                        M2C_FIELD(var_t0, s32 *, 0) = temp_v0_3;
                    } else {
                        sp8C = var_t0;

                        temp_v0_4 = camera_target_track((u8 *) arg0 + 0x22C, D_801141B0, 400.0f, 0.0f, 1.0f, 0.0f, 0x24, (s32) temp_t1, 0, 0x82);
                        var_a0_2 = temp_v0_4;
                        M2C_FIELD(var_t0, s32 *, 0) = temp_v0_4;
                    }
                }
                if (var_a0_2 != -1) {
                    if (var_f14 <= 0.0f) {
                        if (M2C_FIELD(arg0, s8 *, 0x7CC) == 2) {
                            sp8C = var_t0;
                            scheduler_recv(var_a0_2);
                        } else {
                            sp8C = var_t0;
                            results_screen_update(var_a0_2);
                        }
                        player_conditional_call((State20 *)sp8C);
                    } else if (M2C_FIELD(arg0, s8 *, 0x7CC) == 2) {
                        if (var_f14 != M2C_FIELD(var_t0, f32 *, 4)) {
                            M2C_FIELD(var_t0, f32 *, 4) = var_f14;
                            sp8C = var_t0;
                            client_sync(var_a0_2, var_f14);
                        }
                        if (var_f28 != M2C_FIELD(var_t0, f32 *, 8)) {
                            M2C_FIELD(var_t0, f32 *, 8) = var_f28;
                            spA8 = M2C_FIELD(var_t0, s32 *, 0);
                            func_80007270(&D_80142728, NULL, 1);
                            entity_transform_calc(spA8, -2.0f, -2.0f, -2.0f, var_f28);
                            func_800075e0(&D_80142728, NULL, 0);
                        }
                    } else {
                        if (var_f28 != M2C_FIELD(var_t0, f32 *, 8)) {
                            M2C_FIELD(var_t0, f32 *, 8) = var_f28;
                            var_a0_2 = M2C_FIELD(var_t0, s32 *, 0);
                        } else {
                            var_f28 = -2.0f;
                        }
                        camera_clip_planes(var_a0_2, (u8 *) arg0 + 0x22C, D_801141B0, var_f14, var_f28);
                    }
                }
            }
            if (M2C_FIELD(arg0, s8 *, 0x7CC) == 2) {
                func_800DFBA0(arg0);
                mode_select_handler(arg0);
                temp_t0 = spDC * 4;
                temp_v1_2 = &D_80110020[spDC];
                if (D_8002EB90 < *temp_v1_2) {
                    *temp_v1_2 = 0.0f;
                }
                temp_v0_5 = (spDC * 0x4C) + input_rec0;
                if (!(M2C_FIELD(arg0, s32 *, 0x7D4) & 0x10) && (M2C_FIELD(temp_v0_5, s32 *, 0x38) & M2C_FIELD(temp_v0_5, s32 *, 4)) && (D_80124390 < (D_8002EB90 - *temp_v1_2))) {
                    *temp_v1_2 = D_8002EB90;

                    D_801407E0[spDC] = entity_flags_apply((s8) D_801115CD[(spDC * 0xD) + M2C_FIELD(arg0, u8 *, 8)] + 0x1A, (s16) spDC, 1, 4);
                }
                temp_v0_6 = M2C_FIELD(arg0, s16 *, 0x65C);
                if (temp_v0_6 == 1) {

                    D_801407C0[spDC] = entity_flags_apply(0x19, (s16) spDC, 2, 1);
                    M2C_FIELD(arg0, s16 *, 0x65C) = 2;
                    return;
                }
                if (temp_v0_6 == 3) {
                    temp_v0_7 = &D_801407C0[spDC];
                    sp90 = temp_v0_7;
                    scheduler_recv(*temp_v0_7);
                    *temp_v0_7 = -1;
                    M2C_FIELD(arg0, s16 *, 0x65C) = 0;
                }
            }
        }
    }
}
