/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
extern s16 (*D_801407F4)[3];
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
        position=(Position *)&D_801407F4[point];
        x=(f32)position->x;
        y=(f32)position->y;
        z=(f32)position->z;
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
            if(distance<best) { point=i; best=distance; }
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
