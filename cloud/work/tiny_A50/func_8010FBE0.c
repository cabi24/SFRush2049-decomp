/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef int s32;typedef unsigned int u32;
void *memcpy(void *,const void *,u32);
typedef struct Queue24 {u32 opaque[6];} Queue24;
typedef struct Message88 {u32 flag,p4,command,p12;u8 data[64];void *buffer;u32 p84;} Message88;
extern Message88 D_80155238;extern u8 D_80152750[];extern Queue24 D_8002E960,D_8002E928;
s32 osJamMesg(Queue24 *,void *,s32);
void func_8010FBE0(void *data) {
 D_80155238.flag=0;D_80155238.buffer=D_80152750;D_80155238.p84=0;D_80155238.command=2;
 memcpy(D_80155238.data,data,64);
 osJamMesg(&D_8002E960,&D_80155238,1);
 osJamMesg(&D_8002E928,(void*)670,1);
}
