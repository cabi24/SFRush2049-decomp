/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NON-MATCHING RESEARCH: one of 112 fully relocated words still differs.
 * Blend a slot's position toward position + delta, then build its full 3x3
 * orientation basis from the remaining direction. A supplied matrix is first
 * forwarded to the genuine matrix setter. No arcade ancestor is established.
 * Object car/slot indexes are signed bytes; slots have a 152-byte stride.
 * The owned smoothing coefficient is written as 0.6f, with its bytes verified
 * separately; the archive incorrectly declared it as an external global.
 * The consumed assignment-expression for weight reproduces the native load
 * scheduling. The inverse-product's operand order remains different.
 */
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
extern f32 D_80152708[],D_80152720[];
extern volatile s32 D_8002EB98;
extern Slot152 D_80150B70[];
extern void func_800E8CB8(void *,void *,void *);
extern void vector_normalize_length(f32 *,f32 [3][3]);
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
        D_80152708[slot]=D_80152720[slot]*0.6f;
        if(D_8002EB98==3) D_80152708[slot]*=0.75f;
        else if(D_8002EB98==4) D_80152708[slot]*=0.6f;
        else if(D_8002EB98==5) D_80152708[slot]*=0.5f;
    }
    inverse=1.0f-(weight=D_80152708[slot]);
    for(i=0;i<3;i++) {
        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);
        out[i]=position[i]-D_80150B70[slot].position[i];
    }
    vector_normalize_length(out,D_80150B70[slot].matrix);
}
