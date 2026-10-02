/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct Point { s16 x,y,z,flag; } Point;
typedef struct Path { u16 count; u16 field2; Point *points; } Path;
typedef struct Waypoint { u8 pad[12]; f32 x,y,z,axisX,axisY,axisZ; s32 threshold; u8 rest[40]; } Waypoint;
extern Waypoint D_80151CE8[];
extern Path D_8012E5E8[];
extern f32 D_80123DFC;
s16 func_800BA2B8(s16 waypoint, s16 path) {
    s16 closest;
    s16 previousSign;
    f32 previousDistance;
    f32 delta[2];
    f32 direction[2];
    Waypoint *row;
    Path *route;
    Point *point;
    Point *points;
    f32 originX,originZ;
    f32 best;
    f32 dot;
    f32 distance;
    s32 count;
    s32 iterations;
    s16 index;
    s16 sign;
    row=&D_80151CE8[waypoint];
    route=&D_8012E5E8[path];
    count=route->count;
    direction[0]=row->axisX;
    direction[1]=row->axisZ;
    best=D_80123DFC;
    iterations=-1;
    index=0;
    if(count>=0) {
        points=route->points;
        originX=row->x;
        originZ=row->z;
        do {
            if(index==count) index=0;
            point=&points[index];
            delta[0]=(f32)point->x-originX;
            delta[1]=(f32)point->z-originZ;
            dot=direction[1]*delta[1]+delta[0]*direction[0];
            distance=delta[1]*delta[1]+delta[0]*delta[0];
            sign=1;
            if(dot<0.0f) sign=-1;
            if(distance<best) { best=distance; closest=index; }
            if(iterations>=0 && distance<=(f32)row->threshold && previousDistance<=(f32)row->threshold && sign!=previousSign) break;
            iterations++;
            index++;
            previousDistance=distance;
            previousSign=sign;
        } while(iterations<count);
    }
    if(iterations==count) return closest;
    return index;
}
