typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float F32;
typedef struct Point { s16 pos[3]; s16 flags; } Point;
typedef struct PointSet { u16 count; u16 flags; Point *points; } PointSet;
extern PointSet D_8012E5E8[];
extern F32 D_8012417C;
#define mvecsub(a,b,r) {r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
F32 dotprod(F32 a[3], F32 b[3])
{
    return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}
s16 func_800D2C10(Point *position, s16 set)
{
    s16 nearest;
    F32 delta[3];
    F32 distance;
    F32 best;
    u32 i;
    u16 count;
    PointSet *list;
    Point *point;
    list = &D_8012E5E8[set];
    count = list->count;
    best = D_8012417C;
    i = 0;
    if (count > 0) {
        point = list->points;
        do {
            mvecsub(point->pos, position->pos, delta);
            distance = dotprod(delta, delta);
            if (distance < best) {
                best = distance;
                nearest = i;
            }
            i++;
            point++;
        } while (i < (s32)count);
    }
    return nearest;
}
