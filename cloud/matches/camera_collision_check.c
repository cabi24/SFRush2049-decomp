/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef unsigned char u8;
extern signed char D_8010FFC0;
extern int camera_target_track(const f32 *,int,f32,f32,f32,f32,int,int,int,u8);
int camera_collision_check(const f32 *first,int second,f32 x,f32 y,f32 z,f32 scalar,int index,int mode,int flags,u8 final) {
    if (!D_8010FFC0) return -1;
    if (index==-1) return -1;
    return camera_target_track(first,second,x,y,z,scalar,index,mode,flags,final);
}
