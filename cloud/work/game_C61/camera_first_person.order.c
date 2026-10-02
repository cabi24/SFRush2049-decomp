/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Matrix {float m[3][3];} Matrix;
typedef struct PackedMatrix {s16 m[3][3];} PackedMatrix;
typedef struct Item {
    u16 tag,matrix;
    PackedMatrix orientation;
    u16 point;
    s16 x,y,z;
    u16 fractions;
} Item;
typedef struct OutputMatrix {u32 opaque;PackedMatrix matrix;u16 end;} OutputMatrix;
typedef struct PackedPoint {s16 x,y,z;u16 fractions;} PackedPoint;
extern u16 D_8015267C;
extern Item *D_801525EC;
extern OutputMatrix *D_801497F8;
extern PackedPoint *D_8015201C;
extern void func_800AD650(Matrix *,PackedMatrix *);
extern void func_800BF780(Matrix *,Matrix *,Matrix *);
extern void func_8009E820(Vec3 *,Vec3 *,Matrix *);
void camera_first_person(s32 tag,Vec3 *origin,Matrix *first,Matrix *second) {
    Matrix intermediate,transformed;
    Vec3 point,relative;
    s32 x,y,z;
    Item *item;
    OutputMatrix *output;
    PackedPoint *packed;
    u32 i;
    item=D_801525EC;
    for(i=0;i<D_8015267C;i++,item++) {
        if(tag==item->tag) {
            func_800AD650(&transformed,&item->orientation);
            func_800BF780(first,&transformed,&intermediate);
            func_800BF780(second,&intermediate,&transformed);
            output=&D_801497F8[item->matrix];
            output->matrix.m[0][0]=(s32)(transformed.m[0][0]*16384.0f);
            output->matrix.m[0][1]=(s32)(transformed.m[0][1]*16384.0f);
            output->matrix.m[0][2]=(s32)(transformed.m[0][2]*16384.0f);
            output->matrix.m[1][0]=(s32)(transformed.m[1][0]*16384.0f);
            output->matrix.m[1][1]=(s32)(transformed.m[1][1]*16384.0f);
            output->matrix.m[1][2]=(s32)(transformed.m[1][2]*16384.0f);
            output->matrix.m[2][0]=(s32)(transformed.m[2][0]*16384.0f);
            output->matrix.m[2][1]=(s32)(transformed.m[2][1]*16384.0f);
            output->matrix.m[2][2]=(s32)(transformed.m[2][2]*16384.0f);
            point.x=(float)((item->x<<5)+((item->fractions&0x7C00)>>10))*.03125f;
            point.y=(float)((item->y<<5)+((item->fractions&0x3E0)>>5))*.03125f;
            point.z=(float)((item->z<<5)+(item->fractions&0x1F))*.03125f;
            relative.x=point.x-origin->x;
            relative.y=point.y-origin->y;
            relative.z=point.z-origin->z;
            func_8009E820(&relative,&point,first);
            func_8009E820(&point,&relative,second);
            point.x=origin->x+relative.x;
            point.y=origin->y+relative.y;
            point.z=origin->z+relative.z;
            x=(s32)(point.x*32.0f);
            y=(s32)(point.y*32.0f);
            z=(s32)(point.z*32.0f);
            packed=&D_8015201C[item->point];
            packed->fractions=(z&31)+((x&31)<<10)+((y&31)<<5);
            packed->x=x>>5;
            packed->y=y>>5;
            packed->z=z>>5;
        }
    }
}
