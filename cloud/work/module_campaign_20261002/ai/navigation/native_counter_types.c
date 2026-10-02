/* Native road-point/branch search, historical name world_gravity_apply.
 * Research only. Flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 * This is a full native reconstruction, not a claimed verbatim arcade port.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef s16 RoadPoint[3];
typedef struct RoadBranch {
    u8 kind, unknown1[3]; s8 next_branch; u8 unknown5;
    u16 next_point, unknown8, count; RoadPoint *points;
} RoadBranch;
typedef struct RoadGraph {
    u16 count, unknown2; RoadPoint *points;
    u8 branch_count, unknown9[3]; RoadBranch *branches;
} RoadGraph;
typedef struct RoadSection {
    u8 unknown0[8]; f32 length; u8 unknownC[34]; s16 first_point;
    u8 unknown30[32];
} RoadSection;
typedef struct RoadCar {
    u8 unknown0[0xEF]; s8 status; u8 unknownF0[10];
    s16 branch, closest_point, progress_point; f32 distance;
    u8 unknown104[0x3B8-0x104];
} RoadCar;
typedef struct RoadVehicle {
    u8 unknown0[0x22C]; f32 position[3];
    u8 unknown238[0x7C6-0x238]; s16 slot;
    u8 unknown7C8[0x7E2-0x7C8]; s16 checkpoint, next_checkpoint;
    u8 unknown7E6[2]; s8 laps; u8 unknown7E9[0x808-0x7E9];
} RoadVehicle;
extern RoadCar player_array[];
extern RoadVehicle D_8014A250[];
extern RoadGraph D_801407F0;
extern RoadSection D_80151CE8[];
extern s8 D_80152744;
extern f32 D_80124574, D_80124578, D_8012457C, D_80124580, D_80124584;
extern f32 D_80152800, D_801543AC;
extern s32 func_800CF604(s16);
extern void func_800B9F60(s32, s32, s32 *, s32 *);
extern s32 func_800D3430(s32, s32, s32 *, s32 *, s32);
extern f32 func_80098A54(f32 *);
extern f32 audio_channel_alloc(s16, s32);
#define ROAD_HEADER(offset) (*(s16 *)((u8 *)D_80151CE8+(offset)))
#define POINT_DISTANCE(point) do { \
 dx = (f32)((point)[0]-position[0]); \
 dy = (f32)((point)[1]-position[1]); \
 dz = (f32)((point)[2]-position[2]); \
 distance = dx*dx + dy*dy + dz*dz; \
} while (0)
void world_gravity_apply(void) {
    s16 slot, node, position[3];
    s32 point_index, stop;
    s32 next_index, best_index, best_branch, next_branch;
    s32 j, branch_index;
    RoadVehicle *vehicle;
    RoadCar *car;
    RoadBranch *branch;
    RoadPoint *point, *next_point;
    f32 best_distance, distance, dx, dy, dz, along;
    f32 direction[3], displacement[3];
    f32 near_distance, branch_distance;
    if (D_80152744<=0) return;
    near_distance = D_80124574;
    branch_distance = D_80124578;
    for (slot=0; slot<D_80152744; slot++) {
        node = D_8014A250[slot].slot;
        vehicle = &D_8014A250[node];
        if (!func_800CF604(node)) continue;
        car = &player_array[node];
        if (car->status == 1) continue;
        best_distance = D_8012457C;
        position[0] = (s16)(s32)vehicle->position[0];
        position[1] = (s16)(s32)vehicle->position[1];
        position[2] = (s16)(s32)vehicle->position[2];
        best_index=0; best_branch=-1;
        next_index=car->progress_point;
        for (j=0;j<5;j++) {
            point=&D_801407F0.points[next_index];
            POINT_DISTANCE(*point);
            if (distance<best_distance) {best_distance=distance;best_index=next_index;}
            func_800B9F60(-1,next_index,0,&next_index);
        }
        if (near_distance<best_distance) {
            if (car->branch>=0) {
                branch=&D_801407F0.branches[car->branch];
                point_index=car->closest_point;
                if (branch->count-point_index>=6) stop=(s16)(point_index+5);
                else stop=(s16)branch->count;
                for (;point_index<stop;point_index++) {
                    point=&D_801407F0.branches[car->branch].points[point_index];
                    POINT_DISTANCE(*point);
                    if (distance<best_distance) {
                        best_index=point_index;best_distance=distance;best_branch=car->branch;
                    }
                }
            } else if (car->closest_point!=car->progress_point) {
                next_index=car->closest_point;
                for (j=0;j<5;j++) {
                    point=&D_801407F0.points[next_index];
                    POINT_DISTANCE(*point);
                    if (distance<best_distance) {best_distance=distance;best_index=next_index;best_branch=-1;}
                    func_800B9F60(-1,next_index,0,&next_index);
                }
            }
        }
        if (D_80124580<best_distance) {
            if (vehicle->next_checkpoint<vehicle->checkpoint) stop=(s16)D_801407F0.count;
            else stop=D_80151CE8[vehicle->next_checkpoint].first_point;
            point_index=D_80151CE8[vehicle->checkpoint].first_point;
            for (;point_index<stop;point_index++) {
                point=&D_801407F0.points[point_index];
                POINT_DISTANCE(*point);
                if (distance<best_distance) {
                    best_distance=distance;best_index=point_index;best_branch=-1;
                    if (distance<=near_distance) break;
                }
            }
            if (point_index==stop) {
                for (branch_index=0;branch_index<D_801407F0.branch_count;branch_index++) {
                    branch=&D_801407F0.branches[branch_index];
                    if (branch->kind!=2) {
                        for (j=0;j<branch->count;j++) {
                            point=&branch->points[j];
                            POINT_DISTANCE(*point);
                            if (distance<best_distance) {
                                best_distance=distance;best_index=j;best_branch=branch_index;
                                if (distance<=branch_distance) break;
                            }
                        }
                        if (j<branch->count) break;
                    }
                }
                if (branch_index==D_801407F0.branch_count) {
                    point_index=car->progress_point;
                    do {
                        point=&D_801407F0.points[point_index];
                        POINT_DISTANCE(*point);
                        if (distance<best_distance) {
                            best_distance=distance;best_index=point_index;best_branch=-1;
                            if (distance<=near_distance) break;
                        }
                        point_index++;
                        if (point_index>=D_801407F0.count) point_index=0;
                    } while (point_index!=car->progress_point);
                }
            }
        }
        car->branch=best_branch;
        car->closest_point=best_index;
        if (D_80124584<best_distance) continue;
        if (best_branch>=0) func_800D3430(best_branch,best_index,&best_branch,&best_index,0);
        if (best_index<D_80151CE8[vehicle->checkpoint].first_point) continue;
        if (vehicle->next_checkpoint<vehicle->checkpoint) stop=D_801407F0.count;
        else stop=D_80151CE8[vehicle->next_checkpoint].first_point;
        if (best_index>=stop) continue;
        car->progress_point=best_index;
        if (car->branch>=0) along=0.0f;
        else {
            func_800B9F60(-1,best_index,0,&next_branch);
            next_point=&D_801407F0.points[next_branch];
            point=&D_801407F0.points[best_index];
            direction[0]=(f32)((*next_point)[0]-(*point)[0]);
            direction[1]=(f32)((*next_point)[1]-(*point)[1]);
            direction[1]=0.0f;
            direction[2]=(f32)((*next_point)[2]-(*point)[2]);
            func_80098A54(direction);
            point=&D_801407F0.points[best_index];
            displacement[0]=vehicle->position[0]-(f32)(*point)[0];
            displacement[1]=vehicle->position[1]-(f32)(*point)[1];
            displacement[2]=vehicle->position[2]-(f32)(*point)[2];
            along=direction[2]*displacement[2]+displacement[0]*direction[0];
        }
        car->distance=0.0f;
        if (vehicle->laps>0) {
            car->distance += D_80152800+(f32)(vehicle->laps-1)*D_801543AC;
            if (vehicle->checkpoint<ROAD_HEADER(4)) stop=ROAD_HEADER(8);
            else stop=vehicle->checkpoint;
            next_index=ROAD_HEADER(4);
            while (next_index<stop) {
                car->distance+=D_80151CE8[next_index+1].length;
                next_index++;
            }
            if (stop!=vehicle->checkpoint) {
                next_index=ROAD_HEADER(2);
                while (next_index<vehicle->checkpoint) {
                    car->distance+=D_80151CE8[next_index+1].length;
                    next_index++;
                }
            }
        } else {
            next_index=0;
            while (next_index<vehicle->checkpoint) {
                car->distance+=D_80151CE8[next_index+1].length;
                next_index++;
            }
        }
        car->distance+=audio_channel_alloc(D_80151CE8[vehicle->checkpoint].first_point,best_index);
        car->distance+=along;
    }
}
