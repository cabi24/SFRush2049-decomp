/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800E8D50: follow-camera update for one car slot (N64 UpdateCarObj).
 * Arcade ancestor: game/camera.c UpdateCarObj(). An optional matrix is first
 * forwarded to the car-object setter (update_car_object). The slot's
 * elasticity (D_80152708[slot]) is 0 while the car's resurrect state is >= 0,
 * otherwise .6 * the slot's elastic factor (D_80152720[slot]), further scaled
 * by .75/.6/.5 for player-count modes 3/4/5. The camera position follows
 * carpos + res with that elasticity, and the look vector carpos - campos
 * builds the camera basis (LookInDir -> vector_normalize_length).
 * Shaping: the arcade expression verbatim -- the elasticity is re-read from
 * the global in both factors (no weight/inverse locals) and the sum is
 * written (carpos[i]+res[i]). A named `inverse` local is what flipped the
 * mul.s operand order in every earlier attempt. Own literal 0.6f (0x3F19999A)
 * is own .rodata, verified by the scorer.
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
void func_800E8D50(Object *object,f32 *carpos,Mat3 *matrix,f32 *res)
{
    s32 i;
    f32 rpos[3];
    s32 slot=object->slot;
    if(matrix) func_800E8CB8(object,carpos,matrix);
    if(D_8014A914[object->car].status>=0) {
        D_80152708[slot]=0.0f;
    } else {
        D_80152708[slot]=D_80152720[slot]*0.6f;
        if(D_8002EB98==3) D_80152708[slot]*=0.75f;
        else if(D_8002EB98==4) D_80152708[slot]*=0.6f;
        else if(D_8002EB98==5) D_80152708[slot]*=0.5f;
    }
    for(i=0;i<3;i++) {
        D_80150B70[slot].position[i]=(D_80150B70[slot].position[i]*D_80152708[slot]+(carpos[i]+res[i])*(1.0f-D_80152708[slot]));
        rpos[i]=carpos[i]-D_80150B70[slot].position[i];
    }
    vector_normalize_length(rpos,D_80150B70[slot].matrix);
}
