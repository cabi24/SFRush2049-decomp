/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct { u8 fields[68];s32 message;u8 tail[4]; } Record76;
extern Record76 D_8014A118[];
extern s32 D_801174B4,D_8014A110;
extern s16 D_8014A108;
extern f32 D_801141B0[3],D_801141A4[3],D_80114198[3],D_801241AC;
extern s8 D_80146115;
extern s32 func_800D63EC(f32 *,f32 *,f32 *,f32 *);
extern void speed_set(f32,f32,s8,s8);
void func_800D6530(void)
{
    Record76 *record;
    if(!(D_801174B4&8)) {
        for(record=D_8014A118;record<D_8014A118+D_8014A108;record++) {
            if(D_8014A110!=2 || record->fields[0]==0)
                record->message=func_800D63EC(D_801141B0,D_801141B0,D_801141A4,D_80114198);
            else record->message=-1;
        }
        speed_set((f32)D_80146115/10.0f,D_801241AC,0,1);
    }
}
