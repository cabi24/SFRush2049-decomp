/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800C4F68 (0x800C4F68, 1416 B) + camera_follow_path (0x800C46D0, 1484 B) -- DRAFT pair
 * (component 13; camera_follow_path is internal: matrix arrives in s0, func_800C4F68 saves s0/f20/f22
 * for it). Started from cloud/work/ipa-groups/codex_camera_reset_b135/group.c (members only).
 * Unit (blob_unit --internal camera_follow_path):
 *   func_800C4F68: 13 of 354 words, ops 2, frame 104/104 -- camera_follow_path argument registers
 *     (pathMode a0 / point a1 in retail) follow the callee's colouring; FP colouring of the
 *     cross-product block (m[0..2]) differs.
 *   camera_follow_path: 33 of 371 words, ops 0, frame 104/112 -- `addu v0,a0,t6` operand order
 *     (descriptor base first in retail), FP colouring (heading f22, sine/vertical f20), and the
 *     local area (retail: 2 slots above `angle`, one 4-byte slot between angle and surface).
 * Changes from the research draft: switch(mode) (case 0 / case 2), `current == count - 1` test,
 * delta computed inside both arms of each end-of-path test (retail duplicates them), arrays declared
 * surface/delta/origin after angle, func_8009C3F8(f32 x, s32 flag) (locked prototype; it was swapped).
 * HALF/FLOAT views and stand-in struct names are from the research draft.
 */
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
typedef signed char s8; typedef unsigned int u32;
#define HALF(p,n) (*(s16 *)((u8 *)(p)+(n)))
#define FLOAT(p,n) (*(f32 *)((u8 *)(p)+(n)))
extern f32 D_80123ABC,D_80123AC0,D_80123AC4,D_80123AC8,D_80123ACC,D_80123AD0,D_80123AD4,D_80123AD8,D_80123ADC,D_80123AE0,D_80123AE4,D_80123AE8;
extern f32 D_8011F010[4];
extern f32 D_80123F80,D_80123F84;
extern f32 func_8008C768(f32,f32);
extern f32 sqrtf(f32),fabsf(f32),sinf(f32),cosf(f32);
#pragma intrinsic(sqrtf,fabsf)
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
void func_800C4CF8(State *state, s16 selector, s16 point);
void math_utility(void *arg0, void *arg1);
f32 func_80098A54(f32 *arg0);
void func_800C40E8(f32 arg0, f32 arg1, f32 *arg2);
void func_800C4180(f32 *m, f32 *out);
f32 func_8009C3F8(f32 x, s32 flag);
void camera_follow_path(s32 pathMode,s32 point,s32 route,f32 *matrix) {
    f32 heading,sine,cosine,vertical,horizontal;
    f32 angle;
    f32 surface[3],delta[3],origin[3];
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
            if(point+1==D_8012E5E8[route].count) {
                searchTarget=&((Route8 *)D_8012E5E8)[route].points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],48+route*2)];
                delta[0]=(f32)(searchTarget->x-searchCurrent->x);
                delta[1]=(f32)(searchTarget->y-searchCurrent->y);
                delta[2]=(f32)(searchTarget->z-searchCurrent->z);
            } else {
                searchTarget=searchCurrent+1;
                delta[0]=(f32)(searchTarget->x-searchCurrent->x);
                delta[1]=(f32)(searchTarget->y-searchCurrent->y);
                delta[2]=(f32)(searchTarget->z-searchCurrent->z);
            }
        }
    } else {
        current=&D_801407F0.points[point];
        origin[0]=(f32)current->x;
        origin[1]=(f32)current->y;
        origin[2]=(f32)current->z;
        if(point+1==D_801407F0.count) {
            target=&D_801407F0.points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],46)];
            delta[0]=(f32)(target->x-current->x);
            delta[1]=(f32)(target->y-current->y);
            delta[2]=(f32)(target->z-current->z);
        } else {
            target=current+1;
            delta[0]=(f32)(target->x-current->x);
            delta[1]=(f32)(target->y-current->y);
            delta[2]=(f32)(target->z-current->z);
        }
    }
    func_80098A54(delta);
    if(camera_trigger_check(origin,surface,matrix)) {
        heading=func_8009C3F8(delta[0],0);
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
    s32 xv, yv, zv, last_cp_index;
    f32 *dp;
    void *chkpnt;
    switch (mode) {
    case 0:
        if(D_801174B4&8) {
            camera_follow_path(1,D_80154348,-1,state->matrix);
            for(i=HALF(D_80151CE8,8)-1;i>=0;i--) {
                if(D_80154348>=HALF(&D_80151CE8[i],46)) {
                    state->current=i;
                    if(state->current==HALF(D_80151CE8,8)-1) index=HALF(D_80151CE8,2);
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
        break;
    case 2:
        state->position[0]=state->savedPosition[0];
        state->position[1]=state->savedPosition[1];
        state->position[2]=state->savedPosition[2];
        state->speed=state->savedSpeed;
        break;
    }
}
