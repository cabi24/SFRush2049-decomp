/* IDO flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;
typedef s16 PointList;
typedef struct { u8 other[16]; u16 width,height; } Dimensions;
extern PointList *D_80114628[4];
extern Dimensions *D_80114638;
extern s32 D_8011463C[];
extern s16 D_80151AD0;
extern f32 D_8002EB94;
extern u8 D_80114264[];
#define MOTION_AT(offset) (*(f32 *)(D_80114264 + D_80151AD0 * 224 + slot * 56 + (offset)))
void func_800FC9F8(void)
{
    s32 slot,i;
    PointList *list;
    f32 shift[2];
    for(slot=0;slot<4;slot++) {
        list=D_80114628[slot];
        if(list!=0) {
            shift[0]=0.0f; shift[1]=0.0f;
            for(i=0;i<list[0];i++) {
                list[10+i*10]=(s32)((f32)list[10+i*10] + (MOTION_AT(-208)*D_8002EB94)*(f32)(D_80114638->width<<5));
                list[11+i*10]=(s32)((f32)list[11+i*10] + (MOTION_AT(-204)*D_8002EB94)*(f32)(D_80114638->height<<5));
            }
            for(i=0;i<list[0];i++) {
                if(list[10+i*10] >= (D_80114638->width<<5)) shift[0]-=1.0f;
                if(list[10+i*10] <= -(D_80114638->width<<5)) shift[0]+=1.0f;
                if(list[11+i*10] >= (D_80114638->height<<5)) shift[1]-=1.0f;
                if(list[11+i*10] <= (D_80114638->height<<5)) shift[1]+=1.0f;
            }
            if(shift[0]==-4.0f) shift[0]=(f32)-(D_80114638->width<<5);
            else if(shift[0]==4.0f) shift[0]=(f32)(D_80114638->width<<5);
            else shift[0]=0.0f;
            if(shift[1]==-4.0f) shift[1]=(f32)-(D_80114638->height<<5);
            else if(shift[1]==4.0f) shift[1]=(f32)(D_80114638->height<<5);
            else shift[1]=0.0f;
            for(i=0;i<list[0];i++) {
                list[10+i*10]=(s32)((f32)list[10+i*10]+shift[0]);
                list[11+i*10]=(s32)((f32)list[11+i*10]+shift[1]);
                MOTION_AT(-200+i*4)=(f32)list[10+i*10];
                MOTION_AT(-184+i*4)=(f32)list[11+i*10];
            }
            D_8011463C[D_80151AD0-1]=1;
        }
    }
}
