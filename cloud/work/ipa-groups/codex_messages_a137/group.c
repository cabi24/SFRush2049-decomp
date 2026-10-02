/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef struct Msg24 { s16 id; s8 type,used; u8 payload[20]; } Msg24;
typedef struct OSMesgQueue OSMesgQueue;
extern s32 D_80142728,D_801427A8;
extern s32 osRecvMesg(OSMesgQueue *,void **,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
extern Msg24 *func_80091B00(void);
void object_type1_create(void) {
    Msg24 *m;
    osRecvMesg((OSMesgQueue *)&D_80142728,0,1);
    m=func_80091B00();
    m->type=1;
    osJamMesg((OSMesgQueue *)&D_80142728,0,0);
    osJamMesg((OSMesgQueue *)&D_801427A8,m,0);
}
void object_type7_create(void) {
    Msg24 *m;
    osRecvMesg((OSMesgQueue *)&D_80142728,0,1);
    m=func_80091B00();
    m->type=7;
    osJamMesg((OSMesgQueue *)&D_80142728,0,0);
    osJamMesg((OSMesgQueue *)&D_801427A8,m,0);
}
