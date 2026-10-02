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

    float x,y,z,dx,dy,dz,distance,best;
    s32 nearest,i,count;
    s16 px,py,pz;
    Point *point;
    Route *route=&D_801407F0;
    Vec3 *position=&D_80152818[0].position;
    x=position->x;
    y=position->y;
    z=position->z;
    best=D_80124174;
    count=route->count;
    if(count>0) {
    point=route->points;
    for(i=0;i<count;i++,point++) {
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
    }
    if(nearest&1) {
        nearest--;
        if(D_8014AA14<nearest) D_8014AA14=nearest;
    } else {
        float direction[3],relative[3],copy[3];
        point=&route->points[nearest];
        direction[0]=(float)point[1].x-(float)point->x;
        direction[1]=(float)point[1].y-(float)point->y;
        direction[2]=(float)point[1].z-(float)point->z;
        copy[0]=direction[0];
        copy[1]=direction[1];
        copy[2]=direction[2];
        relative[0]=position->x-(float)point->x;
        relative[1]=position->y-(float)point->y;
        relative[2]=position->z-(float)point->z;

        distance=relative[2]*copy[2]+(copy[0]*relative[0]+copy[1]*relative[1]);
        if(distance<0.0f && nearest>0) nearest-=2;
        if(D_8014AA14<nearest) D_8014AA14=nearest;
    }
}
