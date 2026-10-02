/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned char u8;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Point {s16 x,y,z;} Point;
typedef struct Route {u16 count;u8 opaque2[2];Point *points;} Route;
typedef struct Player {u8 opaque0[8];Vec3 position;u8 opaque20[932];} Player;
extern Route D_801407F0;
extern Player D_80152818[];
extern s16 D_8014AA14;
extern float D_80124174;
void func_800D169C(void) {
    Vec3 direction,relative,copy;
    float x,y,z,dx,dy,dz,distance,best;
    s32 nearest,i;

    Point *point;
    Route *route=&D_801407F0;
    Vec3 *position=&D_80152818[0].position;
    x=position->x;
    y=position->y;
    z=position->z;
    best=D_80124174;
    point=route->points;
    for(i=0;i<route->count;i++,point++) {
        s16 px,py,pz;
        px=point->x;
        py=point->y;
        pz=point->z;
        dx=(float)px-x;
        dy=(float)py-y;
        dz=(float)pz-z;
        distance=(dx*dx+dy*dy)+dz*dz;
        if(distance<best) {
            best=distance;
            nearest=i;
        }
    }
    if(nearest&1) {
        nearest--;
        if(D_8014AA14<nearest) D_8014AA14=nearest;
    } else {
        point=&route->points[nearest];
        direction.x=(float)point[1].x-(float)point->x;
        direction.y=(float)point[1].y-(float)point->y;
        direction.z=(float)point[1].z-(float)point->z;
        copy.x=direction.x;
        copy.y=direction.y;
        copy.z=direction.z;
        relative.x=position->x-(float)point->x;
        relative.y=position->y-(float)point->y;
        relative.z=position->z-(float)point->z;

        distance=relative.z*copy.z+(copy.x*relative.x+copy.y*relative.y);
        if(distance<0.0f && nearest>0) nearest-=2;
        if(D_8014AA14<nearest) D_8014AA14=nearest;
    }
}
