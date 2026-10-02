/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
extern int D_80155238, D_80155240;
typedef struct Tail {void *queue;int word4;} Tail;
extern Tail D_80155288;
extern u8 D_80155248[64];
extern int D_80152750, D_8002E960, D_8002E928;
void *memcpy(void *,const void *,unsigned int);
int osJamMesg(void *,void *,int);
void func_8010FBE0(const void *source) {
    D_80155238=0;
    D_80155288.queue=&D_80152750;
    D_80155288.word4=0;
    D_80155240=2;
    memcpy(D_80155248,source,64);
    osJamMesg(&D_8002E960,&D_80155238,1);
    osJamMesg(&D_8002E928,(void *)670,1);
}
