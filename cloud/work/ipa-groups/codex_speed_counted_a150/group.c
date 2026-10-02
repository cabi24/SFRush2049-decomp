/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed int s32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((char *)(p)+(o)))
extern s32 D_80142728,D_801427A8;
extern void *func_80091B00(void);
extern s32 osRecvMesg(OSMesgQueue *,void *,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
extern f32 D_80124280,D_801247EC;
extern s8 D_80146115,D_8010FFC0;
void speed_set(f32 blend,f32 amount,s8 first,s8 second) {
    f32 var_f0;
    void *temp_v0;

    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    temp_v0 = func_80091B00();
    M2C_FIELD(temp_v0, s8 *, 2) = 0xA;
    if (blend < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 4) = 0.0f;
    } else {
        if (blend > 1.0f) {
            var_f0 = 1.0f;
        } else {
            var_f0 = blend;
        }
        M2C_FIELD(temp_v0, f32 *, 4) = var_f0;
    }
    if (amount < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 8) = 0.0f;
    } else {
        M2C_FIELD(temp_v0, f32 *, 8) = (f32) amount;
    }
    M2C_FIELD(temp_v0, s8 *, 0xC) = (s8) first;
    M2C_FIELD(temp_v0, s8 *, 0xD) = (s8) second;
    osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    osJamMesg((OSMesgQueue *) &D_801427A8, temp_v0, 0);
}

void speed_mode0_wrapper(f32 blend,f32 amount) { speed_set(blend,amount,1,0); }
void speed_mode1_wrapper(f32 blend,f32 amount) { speed_set(blend,amount,0,1); }
void continue_prompt(void) { speed_set(0.0f,D_80124280,1,0); }
void vsync_wait(s32 flag) {
    f32 blend;
    D_8010FFC0=flag;
    if(flag) blend=0.0f;
    else blend=(f32)D_80146115/10.0f;
    speed_set(blend,D_801247EC,0,1);
}

typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 fields[68];s32 message;u8 tail[4]; } Record76;
extern Record76 D_8014A118[];
extern s32 D_801174B4,D_8014A110;
extern s16 D_8014A108;
extern f32 D_801141B0[3],D_801141A4[3],D_80114198[3],D_801241AC;
extern s8 D_80146115;
extern s32 func_800D63EC(f32 *,f32 *,f32 *,f32 *);
void func_800D6530(void)
{
    Record76 *record;
    if(!(D_801174B4&8)) {
        record=D_8014A118;
        if(D_8014A108>0) do {
            if(D_8014A110!=2 || record->fields[0]==0)
                record->message=func_800D63EC(D_801141B0,D_801141B0,D_801141A4,D_80114198);
            else record->message=-1;
        } while(++record<D_8014A118+D_8014A108);
        speed_set((f32)D_80146115/10.0f,D_801241AC,0,1);
    }
}
