/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef struct { u8 other0[248]; s16 speed; u8 other250[578]; s16 other828; u8 other830[30]; s8 slot; } Object;
typedef struct { s16 x,y,z; u16 other6; } Point;
typedef struct { u16 count,other2; Point *points; } Path8;
extern s16 D_801391E8[], D_80138668[], D_80138878[];
extern f32 D_801392B8[],D_80139308[],D_80152720[];
extern f32 D_801543CC,D_801244C4;
extern Path8 D_8012E5E8[];
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern s16 func_800B9338(s16,s16);
extern void func_800E8D50(Object *,f32 *,Mat3 *,f32 *);
void func_800E8F10(s16 update,Object *object,f32 *position,Mat3 *matrix)
{
    s8 slot=object->slot;
    s16 previous,track;
    Vec3 result,delta;
    s32 next;
    Point *start;
    f32 amount,length;
    if(!update) {
        D_80138668[slot]=D_801391E8[slot]=(s32)position[0];
        D_80138878[slot]=object->other828;
        D_801392B8[slot]=D_801543CC;
        D_80139308[slot]=0.0f;
    } else {
        if(D_801392B8[slot]>D_801543CC) D_801392B8[slot]=0.0f;
        amount=(D_801543CC-D_801392B8[slot])*(100.0f+(object->speed>>2)*D_801244C4)-D_80139308[slot];
        previous=D_801391E8[slot];
        track=D_80138878[slot];
        for(;;) {
            next=func_800B9338(previous,track);
            start=&D_8012E5E8[track].points[previous];
            delta[0]=(&D_8012E5E8[track].points[next])->x-start->x;
            delta[1]=(&D_8012E5E8[track].points[next])->y-start->y;
            delta[2]=(&D_8012E5E8[track].points[next])->z-start->z;
            length=sqrtf(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
            if(length<=amount) {
                D_80139308[slot]+=length;
                previous=next;
                D_801391E8[slot]=next;
                amount-=length;
            } else break;
        }
        result[0]=start->x+(amount/length)*delta[0];
        result[1]=start->y+(amount/length)*delta[1];
        result[2]=start->z+(amount/length)*delta[2];
        result[1]+=10.0f;
        result[0]-=position[0];
        result[1]-=position[1];
        result[2]-=position[2];
        D_80152720[slot]=0.0f;
        func_800E8D50(object,position,matrix,result);
    }
}
