/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research: FindSurfaceUvsFromResPoint is strict exact only in this genuine
 * caller closure. SetResurrectionData uses no unused locals and remains
 * NONMATCH (17/354; frame80 vs104). All native labels/ABIs are retained.
 * The six-local exact diagnostic is reproduced separately by verify.py and
 * is not promoted as recovered original source. See the packet README.
 */
typedef float f32;
typedef int s32;
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef s16 Position[3];
typedef s16 SearchPoint[4];
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
typedef struct Checkpoint80 {f32 pos[3],normal[3];u8 pad18[10];s16 pathPts[20];u8 tail[6];} Checkpoint80;
typedef struct CheckpointHeader {s16 reserved,loopPt;u8 pad4[4];s16 numPoints;u8 padA[2];Checkpoint80 points[1];} CheckpointHeader;
#define CHECKPOINTS (*(CheckpointHeader *)D_80151CE8)

extern u32 D_801174B4;
extern s32 D_8014A110;
extern s16 D_80154348;
extern f32 D_8011418C[9];
extern void func_8008E0B8(f32 *);
extern void func_800C4078(s16);
typedef struct Surface Surface;
extern Surface *camera_trigger_check(f32 *,f32 *,f32 *);
f32 camera_update_c(f32);
void camera_follow_path(s32,s32,s32,f32 *);
void func_800C4F68(s16,Vehicle2056 *,u32);
void func_800C4CF8(State *state, s16 selector, s16 point);
void math_utility(void *arg0, void *arg1);
f32 func_80098A54(f32 *arg0);
void func_800C40E8(f32 arg0, f32 arg1, f32 *arg2);
void func_800C4180(f32 *m, f32 *out);
f32 func_8009C3F8(f32 x, s32 flag);
void camera_follow_path(s32 pathMode, s32 route, s32 point, f32 *matrix)
{
    float sine, cosine, yaw, angle;
    float snapPos[3], pt[3], pos[3];
    Surface *surf;
    if (route >= 0) {
        if (pathMode) {
            pos[0] = D_801407F0.descriptors[route].points[point][0];
            pos[1] = D_801407F0.descriptors[route].points[point][1];
            pos[2] = D_801407F0.descriptors[route].points[point][2];
            if (point == D_801407F0.descriptors[route].count - 1) {
                if (D_801407F0.descriptors[route].parent >= 0) {
                    pt[0] = D_801407F0.descriptors[D_801407F0.descriptors[route].parent].points[D_801407F0.descriptors[route].end][0] - pos[0];
                    pt[1] = D_801407F0.descriptors[D_801407F0.descriptors[route].parent].points[D_801407F0.descriptors[route].end][1] - pos[1];
                    pt[2] = D_801407F0.descriptors[D_801407F0.descriptors[route].parent].points[D_801407F0.descriptors[route].end][2] - pos[2];
                } else {
                    pt[0] = D_801407F0.points[D_801407F0.descriptors[route].end][0] - pos[0];
                    pt[1] = D_801407F0.points[D_801407F0.descriptors[route].end][1] - pos[1];
                    pt[2] = D_801407F0.points[D_801407F0.descriptors[route].end][2] - pos[2];
                }
            } else {
                pt[0] = D_801407F0.descriptors[route].points[point+1][0] - pos[0];
                pt[1] = D_801407F0.descriptors[route].points[point+1][1] - pos[1];
                pt[2] = D_801407F0.descriptors[route].points[point+1][2] - pos[2];
            }
        } else {
            pos[0] = ((Route8 *)D_8012E5E8)[route].points[point][0];
            pos[1] = ((Route8 *)D_8012E5E8)[route].points[point][1];
            pos[2] = ((Route8 *)D_8012E5E8)[route].points[point][2];
            if (point == ((Route8 *)D_8012E5E8)[route].count - 1) {
                pt[0] = ((Route8 *)D_8012E5E8)[route].points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[route+1]][0] - ((Route8 *)D_8012E5E8)[route].points[point][0];
                pt[1] = ((Route8 *)D_8012E5E8)[route].points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[route+1]][1] - ((Route8 *)D_8012E5E8)[route].points[point][1];
                pt[2] = ((Route8 *)D_8012E5E8)[route].points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[route+1]][2] - ((Route8 *)D_8012E5E8)[route].points[point][2];
            } else {
                pt[0] = ((Route8 *)D_8012E5E8)[route].points[point+1][0] - ((Route8 *)D_8012E5E8)[route].points[point][0];
                pt[1] = ((Route8 *)D_8012E5E8)[route].points[point+1][1] - ((Route8 *)D_8012E5E8)[route].points[point][1];
                pt[2] = ((Route8 *)D_8012E5E8)[route].points[point+1][2] - ((Route8 *)D_8012E5E8)[route].points[point][2];
            }
        }
    } else {
        pos[0] = D_801407F0.points[point][0];
        pos[1] = D_801407F0.points[point][1];
        pos[2] = D_801407F0.points[point][2];
        if (point == D_801407F0.count - 1) {
            pt[0] = D_801407F0.points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[0]][0] - D_801407F0.points[point][0];
            pt[1] = D_801407F0.points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[0]][1] - D_801407F0.points[point][1];
            pt[2] = D_801407F0.points[CHECKPOINTS.points[CHECKPOINTS.loopPt].pathPts[0]][2] - D_801407F0.points[point][2];
        } else {
            pt[0] = D_801407F0.points[point+1][0] - D_801407F0.points[point][0];
            pt[1] = D_801407F0.points[point+1][1] - D_801407F0.points[point][1];
            pt[2] = D_801407F0.points[point+1][2] - D_801407F0.points[point][2];
        }
    }
    func_80098A54(pt);
    surf = camera_trigger_check(pos,snapPos,matrix);
    if (surf) {
        angle = camera_update_c(pt[0]);
        if (pt[2] < 0.0f) angle = 3.1415927410125732f - angle;
        func_800C4180(matrix,&yaw);
        sine = sinf(angle+yaw);
        cosine = cosf(angle+yaw);
    } else {
        sine = pt[1];
        cosine = sqrtf(pt[0]*pt[0] + pt[2]*pt[2]);
        if (pt[2] < 0.0f) sine = -sine;
        matrix[0]=1.0f;
        matrix[5]=-sine;
        matrix[1]=0.0f;
        matrix[2]=0.0f;
        matrix[3]=0.0f;
        matrix[4]=cosine;
        matrix[6]=0.0f;
        matrix[7]=sine;
        matrix[8]=cosine;
        sine=pt[0];
        cosine=pt[2];
    }
    func_800C40E8(-sine,cosine,matrix);
}

void func_800C4F68(s16 mode,Vehicle2056 *state,u32 callbackFlag) {
    s32 i,index;
    Player *player;
    switch (mode) {
    case 0:
        if(D_801174B4&8) {
            camera_follow_path(1,-1,D_80154348,state->matrix);
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
            state->position[0]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)][0];
            state->position[1]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)][1];
            state->position[2]=(f32)((Route8 *)D_8012E5E8)[player->region.path].points[HALF(player,824)][2];
        } else if(D_8014A110==6 || D_8014A110==4) {
#define B135_SELECT(p) ((p)==0?0:((p)==1?2:((p)==2?1:3)))
            state->position[0]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],12);
            state->position[1]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],16);
            state->position[2]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],20);
            math_utility(D_8011418C,state->matrix);
            state->matrix[6]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],24);
            state->matrix[7]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],28);
            state->matrix[8]=FLOAT(&D_80151CE8[B135_SELECT(state->player)],32);
            state->matrix[7]=0.0f;
            state->matrix[6]*=-1.0f;
            state->matrix[8]*=-1.0f;
            func_8008E0B8(&state->matrix[6]);
            state->matrix[0]=state->matrix[4]*state->matrix[8]-state->matrix[5]*state->matrix[7];
            state->matrix[1]=state->matrix[5]*state->matrix[6]-state->matrix[3]*state->matrix[8];
            state->matrix[2]=state->matrix[3]*state->matrix[7]-state->matrix[4]*state->matrix[6];
            func_8008E0B8(state->matrix);
            func_800C4078(state->player);
        } else {
            state->position[0]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)][0];
            state->position[1]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)][1];
            state->position[2]=(f32)D_801407F0.points[HALF(&D_80151CE8[state->current],46)][2];
            camera_follow_path(1,-1,HALF(&D_80151CE8[state->current],46),state->matrix);
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
