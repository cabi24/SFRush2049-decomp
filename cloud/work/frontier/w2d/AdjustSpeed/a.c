/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
typedef unsigned short u16;
typedef void (*NodeDone)(void *);
#define F(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct { s32 w[6]; } OSMesgQueue;
typedef struct PakState {u8 bytes[772];} PakState;
typedef struct PakList {u8 bytes[16];} PakList;
extern s8 D_8011194C,D_8011EAE8;
extern OSMesgQueue D_801497D0;
extern void *D_801527E4;
extern PakState D_80144030[];
extern PakList D_80144D60[];
extern u8 D_801460E0[];
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 osPfsRename(void *,u16,u32,u8 *,u8 *);
s32 osPfsFreeBlocks(void *,s32 *);
void func_8008A6A4(void);
typedef void (*PakErrorCallback)(s32,s32,s32,s32,s8 *,s8 *,void (*)(void));
extern PakErrorCallback D_80144008;
void func_8009211C(void *,void *);
void func_80091FBC(void *,void *,void *);
void func_8008A704(void);
s32 func_800A1E94(s32);
void audio_effect_process(u32);
void AdjustSpeed(void *node)
{
    void *request,*current;
    u8 *controller,*file;
    PakList *list;
    s32 port,index,result;
    s8 retry,status;
    func_8008A704();
    request=F(node,void *,0);
    port=F(request,u8,16);
    index=F(request,u8,17);
    controller=D_80144030[port].bytes;
    file=controller+140+index*40;
    for(;;) {
        result=osPfsRename(controller+12,F(file,u16,8),F(file,u32,4),file+14,file+10);
        if(result==0) break;
        list=&D_80144D60[port];
        F(controller,s32,116)=func_800A1E94(result);
        osJamMesg(&D_801497D0,0,0);
        D_8011EAE8=port;
        D_80144008(port,F(controller,s32,116),0,0,&retry,&status,func_8008A6A4);
        current=F(list,void *,8);
        D_8011EAE8=-1;
        while(current && current!=node) current=F(F(current,void *,0),void *,0);
        if(!current) return;
        func_8008A704();
        if(retry==0) {
            osJamMesg(&D_801497D0,0,0);
            return;
        }
    }
    osPfsFreeBlocks(controller+12,(s32 *)(controller+120));
    if(F(request,NodeDone,8)) F(request,NodeDone,8)(node);
    F(file,s32,0)=0;
    if(F(request,u32,72)) {
        audio_effect_process(F(request,u32,72));
        F(request,u32,72)=0;
    }
    func_8009211C(&D_80144D60[F(F(node,void *,0),u8,16)],node);
    func_80091FBC(D_801460E0,node,F(D_801460E0,void *,8));
    osJamMesg(&D_801497D0,0,0);
}
