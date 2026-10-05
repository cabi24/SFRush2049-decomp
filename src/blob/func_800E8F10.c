/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800E8F10: path-following camera target for one car slot.
 * update == 0 initialises the slot: current/previous path point index from
 * (s32)position[0], the path (track) number from object half 828, start time
 * from the race clock D_801543CC, distance travelled 0.
 * Otherwise: wraps the start time if the clock went backwards, computes the
 * distance to cover as elapsed * (100 + (speed >> 2) * 1.4666667 [mph->ft/s])
 * minus the distance already travelled, then walks the path's s16 XYZ points
 * (D_8012E5E8[track].points, next index from func_800B9338) consuming whole
 * segments, interpolates inside the current segment, lifts the point by 10,
 * makes it relative to position, zeroes the slot's elastic factor and hands
 * it to the follow-camera update func_800E8D50 (arcade UpdateCarObj family).
 * No arcade ancestor found (N64-only attract/replay path camera).
 * Shaping (all natural forms, found by trace/search):
 *  - chained D_80138668[slot] = D_801391E8[slot] = (s32)position[0];
 *  - the segment points are written as array expressions (no start/end
 *    pointer locals) so the start-point address is an expression web (a1);
 *  - the interpolation lives in the loop's else-arm before `break`, which
 *    ends a uopt block (result[0]/[2] are reloaded after +10);
 *  - `ratio*delta + start` operand order, left-assoc x*x+y*y+z*z;
 *  - `previous` is loaded in the for-initialiser (as1 line tie-break);
 *  - track is read before the amount statement;
 *  - result declared before delta (frame offsets 124 / 112).
 * Own .rodata: 1.4666667f (0x3FBBBBBC at 0x801244C4).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef struct { u8 other0[248]; s16 speed; u8 other250[578]; s16 track; u8 other830[30]; s8 slot; } Object;
typedef struct { s16 x,y,z,pad; } PathPoint;
typedef struct { u16 count,other2; PathPoint *points; } Path;
extern s16 D_801391E8[], D_80138668[], D_80138878[];
extern f32 D_801392B8[], D_80139308[], D_80152720[];
extern f32 D_801543CC;
extern Path D_8012E5E8[];
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern s16 func_800B9338(s16,s16);
extern void func_800E8D50(Object *,f32 *,Mat3 *,f32 *);
void func_800E8F10(s16 update,Object *object,f32 *position,Mat3 *matrix)
{
    s8 slot=object->slot;
    s32 next;
    Vec3 result,delta;
    s16 previous,track;
    f32 amount,length;
    if(!update) {
        D_80138668[slot]=D_801391E8[slot]=(s32)position[0];
        D_80138878[slot]=object->track;
        D_801392B8[slot]=D_801543CC;
        D_80139308[slot]=0.0f;
    } else {
        if(D_801392B8[slot]>D_801543CC) D_801392B8[slot]=0.0f;
        track=D_80138878[slot];
        amount=(D_801543CC-D_801392B8[slot])*(100.0f+(object->speed>>2)*1.4666667f)-D_80139308[slot];
        for(previous=D_801391E8[slot];;) {
            next=func_800B9338(previous,track);
            delta[0]=D_8012E5E8[track].points[next].x-D_8012E5E8[track].points[previous].x;
            delta[1]=D_8012E5E8[track].points[next].y-D_8012E5E8[track].points[previous].y;
            delta[2]=D_8012E5E8[track].points[next].z-D_8012E5E8[track].points[previous].z;
            length=sqrtf(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
            if(length<=amount) {
                D_80139308[slot]+=length;
                previous=next;
                D_801391E8[slot]=next;
                amount-=length;
            } else {
                result[0]=(amount/length)*delta[0]+D_8012E5E8[track].points[previous].x;
                result[1]=(amount/length)*delta[1]+D_8012E5E8[track].points[previous].y;
                result[2]=(amount/length)*delta[2]+D_8012E5E8[track].points[previous].z;
                break;
            }
        }
        result[1]+=10.0f;
        result[0]-=position[0];
        result[1]-=position[1];
        result[2]-=position[2];
        D_80152720[slot]=0.0f;
        func_800E8D50(object,position,matrix,result);
    }
}
