/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef f32 Mat3[3][3];
typedef struct { u8 other[859]; s8 car,slot; } Object;
typedef struct { s16 status;u8 other[2054]; } CarField2056;
typedef struct { u8 other[96];Mat3 matrix;f32 position[3];u8 tail[8]; } Slot152;
extern CarField2056 D_8014A914[];
extern f32 D_80152708[],D_80152720[],D_801244C0;
extern volatile s32 D_8002EB98;
extern Slot152 D_80150B70[];
extern void func_800E8CB8(Object *,f32 *,Mat3 *);
extern void vector_normalize_length(f32 *,Mat3 *);
void func_800E8D50(Object *object,f32 *position,Mat3 *matrix,f32 *delta)
{
    s32 i;
    f32 out[3];
    s32 slot=object->slot;
    f32 weight,inverse;
    if(matrix) func_800E8CB8(object,position,matrix);
    if(D_8014A914[object->car].status>=0) {
        D_80152708[slot]=0.0f;
    } else {
        D_80152708[slot]=D_80152720[slot]*D_801244C0;
        if(D_8002EB98==3) D_80152708[slot]*=0.75f;
        else if(D_8002EB98==4) D_80152708[slot]*=D_801244C0;
        else if(D_8002EB98==5) D_80152708[slot]*=0.5f;
    }
    weight=D_80152708[slot];
    inverse=1.0f-weight;
    for(i=0;i<3;i++) {
        f32 sum=position[i]+delta[i];
        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*sum;
        out[i]=position[i]-D_80150B70[slot].position[i];
    }
    vector_normalize_length(out,&D_80150B70[slot].matrix);
}
