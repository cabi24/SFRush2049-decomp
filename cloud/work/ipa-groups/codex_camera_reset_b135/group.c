/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct Position { s16 x,y,z; } Position;
typedef struct SearchPoint { s16 x,y,z,flag; } SearchPoint;
typedef struct PositionEntry { u8 pad[12]; Position *points; } PositionEntry;
typedef struct CountEntry { u16 count; u8 pad[6]; } CountEntry;
typedef struct PointEntry { SearchPoint *points; u8 pad[4]; } PointEntry;
typedef struct Waypoint { s16 index[40]; } Waypoint;
typedef struct Region { u8 pad[40]; s16 path; } Region;
typedef struct Player { u8 pad[788]; Region region; u8 rest[122]; } Player;
typedef struct State { u8 pad[1990]; s16 player; u8 gap[26]; s16 next, previous; } State;
extern PositionEntry *D_801407FC;
extern Position *D_801407F4;
extern CountEntry D_8012E5E8[];
extern PointEntry D_8012E5EC[];
extern Waypoint D_80151D18[];
extern Player player_array[];
extern f32 D_80123F88;
extern s16 func_800B9338(s16,s16);
extern void func_800C4C9C(State *,s16);
void func_800C4CF8(State *state, s16 selector, s16 point) {
    f32 x,y,z;
    f32 best;
    f32 dx,dy,dz,distance;
    Position *position;
    SearchPoint *scan;
    Region *region;
    s32 stop;
    s16 nextPoint;
    s32 i;
    best=D_80123F88;
    if(selector>=0) {
        position=&D_801407FC[selector].points[point];
        x=(f32)position->x;
        y=(f32)position->y;
        z=(f32)position->z;
    } else {
        x=(f32)D_801407F4[point].x;
        y=(f32)D_801407F4[point].y;
        z=(f32)D_801407F4[point].z;
    }
    region=&player_array[state->player].region;
    if(state->previous<state->next) stop=D_8012E5E8[region->path].count;
    else stop=D_80151D18[state->previous].index[region->path];
    i=D_80151D18[state->next].index[region->path];
    point=0;
    if(i<stop) {
        scan=&D_8012E5EC[region->path].points[i];
        do {
            dx=(f32)scan->x-x;
            dy=(f32)scan->y-y;
            dz=(f32)scan->z-z;
            distance=dx*dx+dy*dy+dz*dz;
            if(distance<best) {
                best=distance;
                point=i;
            }
            i++;
            scan++;
        } while(i<stop);
    }
    i=0;
    while(i<state->player) {
        nextPoint=func_800B9338(point,region->path);
        i++;
        point=nextPoint;
    }
    func_800C4C9C(state,point);
}

typedef signed char s8;
typedef unsigned int u32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
#define HALF(p,n) (*(s16 *)((u8 *)(p)+(n)))
#define FLOAT(p,n) (*(f32 *)((u8 *)(p)+(n)))
extern f32 D_80123ABC,D_80123AC0,D_80123AC4,D_80123AC8,D_80123ACC,D_80123AD0,D_80123AD4,D_80123AD8,D_80123ADC,D_80123AE0,D_80123AE4,D_80123AE8;
extern f32 D_8011F010[4];
extern f32 D_80123F80,D_80123F84;
extern f32 func_8008C768(f32,f32);
extern f32 sqrtf(f32),fabsf(f32),sinf(f32),cosf(f32);
#pragma intrinsic(sqrtf,fabsf)
#define C24_ABS(x) fabsf(x)
typedef struct Descriptor16 {u8 prefix[4];s8 parent;u8 gap;u16 end;u16 gap8;u16 count;Position *points;} Descriptor16;
typedef struct Header16 {u16 count;u16 gap2;Position *points;u8 paths;u8 gap9[3];Descriptor16 *descriptors;} Header16;
typedef struct Route8 {u16 count,gap;SearchPoint *points;} Route8;
typedef struct Record80 {u8 bytes[80];} Record80;
typedef struct Vehicle2056 {u8 prefix[1644];f32 position[3],matrix[9];u8 gap1692[36];f32 speed;u8 gap1732[80];f32 savedSpeed;u8 gap1816[124];f32 savedPosition[3];u8 gap1952[38];s16 player;u8 gap1992[26];s16 current,next;u8 tail[34];} Vehicle2056;
extern Header16 D_801407F0;
extern Record80 D_80151CE8[];
extern u32 D_801174B4;
extern s32 D_8014A110;
extern s16 D_80154348;
extern f32 D_8011418C[9];
extern void func_8008E0B8(f32 *);
extern void func_800C4078(s16);
extern s32 camera_trigger_check(f32 *,f32 *,f32 *);
void camera_follow_path(s32,s32,s32,f32 *);
void func_800C4F68(s16,Vehicle2056 *,u32);
void math_utility(void *arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0) = (f32) M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 4);
    M2C_FIELD(arg1, f32 *, 8) = (f32) M2C_FIELD(arg0, f32 *, 8);
    M2C_FIELD(arg1, f32 *, 0xC) = (f32) M2C_FIELD(arg0, f32 *, 0xC);
    M2C_FIELD(arg1, f32 *, 0x10) = (f32) M2C_FIELD(arg0, f32 *, 0x10);
    M2C_FIELD(arg1, f32 *, 0x14) = (f32) M2C_FIELD(arg0, f32 *, 0x14);
    M2C_FIELD(arg1, f32 *, 0x18) = (f32) M2C_FIELD(arg0, f32 *, 0x18);
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) M2C_FIELD(arg0, f32 *, 0x1C);
    M2C_FIELD(arg1, f32 *, 0x20) = (f32) M2C_FIELD(arg0, f32 *, 0x20);
}
f32 func_80098A54(f32 *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f18;
    f32 temp_f2;

    temp_f2 = arg0[2];
    temp_f12 = arg0[0];
    temp_f14 = arg0[1];
    temp_f0 = sqrtf((temp_f2 * temp_f2) + ((temp_f12 * temp_f12) + (temp_f14 * temp_f14)));
    if (temp_f0 != 0.0f) {
        temp_f18 = 1.0f / temp_f0;
        arg0[0] = (f32) (temp_f12 * temp_f18);
        arg0[1] = (f32) (temp_f14 * temp_f18);
        arg0[2] = (f32) (temp_f2 * temp_f18);
    } else {
    arg0[1] = 0.0f;
    arg0[2] = 0.0f;
    arg0[0] = 1.0f;
    }
    return temp_f0;
}
void func_800C40E8(f32 arg0, f32 arg1, f32 *arg2) {
    f32 sp4;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f2_2;

    temp_f2 = arg2[6];
    temp_f16 = arg2[0];
    temp_f18 = arg2[7];
    arg2[0] = (f32) ((temp_f2 * arg0) + (temp_f16 * arg1));
    arg2[6] = (f32) ((temp_f2 * arg1) - (temp_f16 * arg0));
    sp4 = arg2[1];
    temp_f2_2 = arg2[8];
    temp_f16_2 = arg2[2];
    arg2[1] = (f32) ((temp_f18 * arg0) + (sp4 * arg1));
    arg2[7] = (f32) ((temp_f18 * arg1) - (sp4 * arg0));
    arg2[2] = (f32) ((temp_f2_2 * arg0) + (temp_f16_2 * arg1));
    arg2[8] = (f32) ((temp_f2_2 * arg1) - (temp_f16_2 * arg0));
}
void func_800C4180(f32 *m, f32 *out) {
 f32 x=m[6];
 if (C24_ABS(x)<D_80123F80 && C24_ABS(m[8])<D_80123F80) {
  *out=func_8008C768(m[2],m[0]);
 } else {
  *out=func_8008C768(-x,m[8]);
 }
}
f32 sqrtf(f32);
f32 fabsf(f32);
#pragma intrinsic (sqrtf,fabsf)

f32 func_8009C3F8(s32 arg0,f32 input) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 var_f12;
    f32 var_f12_2;
    f32 var_f14;
    f32 var_f2;
    s32 var_v0;

    temp_f0_3 = fabsf(input);
    var_v0 = arg0;
    var_f14 = temp_f0_3;
    if (temp_f0_3 < D_80123ABC) {
        var_f12_2 = temp_f0_3;
    } else if (temp_f0_3 >= 1.0f) {
        var_f12_2 = D_80123AC0;
    } else {
        if (temp_f0_3 > 0.5f) {
            var_v0 = 1 - arg0;
            var_f2 = ((0.5f - var_f14) + 0.5f) * 0.5f;
            var_f14 = sqrtf(var_f2);
            var_f14 = -(var_f14 + var_f14);
        } else {
            var_f2 = temp_f0_3 * temp_f0_3;
        }
        var_f12_2 = (((((((((((D_80123AC4 * var_f2) + D_80123AC8) * var_f2) + D_80123ACC) * var_f2) + D_80123AD0) * var_f2) + D_80123AD4) * var_f2) / (((((((((var_f2 + D_80123AD8) * var_f2) + D_80123ADC) * var_f2) + D_80123AE0) * var_f2) + D_80123AE4) * var_f2) + D_80123AE8)) * var_f14) + var_f14;
    }
    if (arg0 != 0) {
        if (input < 0.0f) {
            temp_f0_2 = D_8011F010[var_v0+2];
            return temp_f0_2 + (temp_f0_2 + var_f12_2);
        }
        temp_f0 = D_8011F010[var_v0];
        return temp_f0 + (temp_f0 - var_f12_2);
    }
    temp_f0_4 = D_8011F010[var_v0];
    var_f12 = temp_f0_4 + (temp_f0_4 + var_f12_2);
    if (input < 0.0f) {
        var_f12 = -var_f12;
    }
    return var_f12;
}

f32 camera_update_c(f32 input) {return func_8009C3F8(0,input);}
f32 select_screen_update(f32 input) {return func_8009C3F8(1,input);}

void camera_follow_path(s32 pathMode,s32 point,s32 route,f32 *matrix) {
    f32 origin[3],delta[3],surface[3],angle;
    f32 heading,sine,cosine,vertical,horizontal;
    Descriptor16 *descriptor;
    Position *current,*target;
    SearchPoint *searchCurrent,*searchTarget;
    if(route>=0) {
        if(pathMode) {
            descriptor=&D_801407F0.descriptors[route];
            origin[0]=(f32)descriptor->points[point].x;
            origin[1]=(f32)descriptor->points[point].y;
            origin[2]=(f32)descriptor->points[point].z;
            if(point+1==descriptor->count) {
                if(descriptor->parent>=0) {
                    delta[0]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].x-origin[0];
                    delta[1]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].y-origin[1];
                    delta[2]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].z-origin[2];
                } else {
                    delta[0]=(f32)D_801407F0.points[descriptor->end].x-origin[0];
                    delta[1]=(f32)D_801407F0.points[descriptor->end].y-origin[1];
                    delta[2]=(f32)D_801407F0.points[descriptor->end].z-origin[2];
                }
            } else {
                delta[0]=(f32)descriptor->points[point+1].x-origin[0];
                delta[1]=(f32)descriptor->points[point+1].y-origin[1];
                delta[2]=(f32)descriptor->points[point+1].z-origin[2];
            }
        } else {
            searchCurrent=&((Route8 *)D_8012E5E8)[route].points[point];
            origin[0]=(f32)searchCurrent->x;
            origin[1]=(f32)searchCurrent->y;
            origin[2]=(f32)searchCurrent->z;
            if(point+1==D_8012E5E8[route].count)
                searchTarget=&((Route8 *)D_8012E5E8)[route].points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],48+route*2)];
            else searchTarget=searchCurrent+1;
            delta[0]=(f32)(searchTarget->x-searchCurrent->x);
            delta[1]=(f32)(searchTarget->y-searchCurrent->y);
            delta[2]=(f32)(searchTarget->z-searchCurrent->z);
        }
    } else {
        current=&D_801407F0.points[point];
        origin[0]=(f32)current->x;
        origin[1]=(f32)current->y;
        origin[2]=(f32)current->z;
        if(point+1==D_801407F0.count)
            target=&D_801407F0.points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],46)];
        else target=current+1;
        delta[0]=(f32)(target->x-current->x);
        delta[1]=(f32)(target->y-current->y);
        delta[2]=(f32)(target->z-current->z);
    }
    func_80098A54(delta);
    if(camera_trigger_check(origin,surface,matrix)) {
        heading=func_8009C3F8(0,delta[0]);
        if(delta[2]<0.0f) heading=D_80123F84-heading;
        func_800C4180(matrix,&angle);
        sine=sinf(heading+angle);
        cosine=cosf(heading+angle);
    } else {
        horizontal=sqrtf(delta[2]*delta[2]+delta[0]*delta[0]);
        vertical=delta[1];
        if(delta[2]<0.0f) vertical=-vertical;
        matrix[0]=1.0f;
        matrix[5]=-vertical;
        matrix[1]=0.0f;
        matrix[2]=0.0f;
        matrix[3]=0.0f;
        matrix[4]=horizontal;
        matrix[6]=0.0f;
        matrix[7]=vertical;
        matrix[8]=horizontal;
        sine=delta[0];
        cosine=delta[2];
    }
    func_800C40E8(-sine,cosine,matrix);
}

void func_800C4F68(s16 mode,Vehicle2056 *state,u32 callbackFlag) {
    s32 i,index;
    Player *player;
    if(mode==0) {
        if(D_801174B4&8) {
            camera_follow_path(1,D_80154348,-1,state->matrix);
            for(i=HALF(D_80151CE8,8)-1;i>=0;i--) {
                if(D_80154348>=HALF(&D_80151CE8[i],46)) {
                    state->current=i;
                    if(state->current+1==HALF(D_80151CE8,8)) index=HALF(D_80151CE8,2);
                    else index=state->current+1;
                    state->next=index;
                    break;
                }
            }
            func_800C4CF8((State *)state,-1,D_80154348);
            player=&player_array[state->player];
            state->position[0]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)].x;
            state->position[1]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)].y;
            state->position[2]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)].z;
        } else if(D_8014A110==6 || D_8014A110==4) {
#define B135_SELECT(p) ((p)==0?0:((p)==1?2:((p)==2?1:3)))
            state->position[0]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],12);
            state->position[1]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],16);
            state->position[2]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],20);
            math_utility(D_8011418C,state->matrix);
            state->matrix[6]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],24);
            state->matrix[7]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],28);
            state->matrix[8]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],32);
            state->matrix[6]*=-1.0f;
            state->matrix[7]=0.0f;
            state->matrix[8]*=-1.0f;
            func_8008E0B8(&state->matrix[6]);
            state->matrix[0]=state->matrix[4]*state->matrix[8]-state->matrix[7]*state->matrix[5];
            state->matrix[1]=state->matrix[5]*state->matrix[6]-state->matrix[8]*state->matrix[3];
            state->matrix[2]=state->matrix[3]*state->matrix[7]-state->matrix[6]*state->matrix[4];
            func_8008E0B8(state->matrix);
            func_800C4078(state->player);
        } else {
            state->position[0]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)].x;
            state->position[1]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)].y;
            state->position[2]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)].z;
            camera_follow_path(1,HALF(&D_80151CE8[state->current],46),-1,state->matrix);
            func_800C4078(state->player);
        }
        state->speed=0.0f;
    } else if(mode==2) {
        state->position[0]=state->savedPosition[0];
        state->position[1]=state->savedPosition[1];
        state->position[2]=state->savedPosition[2];
        state->speed=state->savedSpeed;
    }
}
