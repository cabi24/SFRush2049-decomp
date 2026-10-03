/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct {OSThread *mtqueue,*fullqueue;int validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *msgQ;} OSScClient;
typedef struct OSSched OSSched;
extern OSSched D_8002E8E8;
extern OSMesgQueue D_8017A498;
extern OSMesg D_8017A4B8[];
extern unsigned short D_801525F0;
extern unsigned int __setfpcsr(unsigned int);
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,int);
extern void osScAddClient(OSSched *,OSScClient *,OSMesgQueue *);
extern int osRecvMesg(OSMesgQueue *,OSMesg *,int);
extern void func_800E7710(void),func_800E762C(int),func_800E6AF8(void);
extern int func_800E73D8(void);
/* ABI is the project's SDK thread entry void (*)(void *). */
void render_thread_entry(void *arg)
{
    OSMesg message=0;
    OSScClient client;
    int status;
    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&D_8017A498,D_8017A4B8,8);
    func_800E7710();
    D_801525F0=0;
    func_800E762C(0);
    osScAddClient(&D_8002E8E8,&client,&D_8017A498);
    for(;;) {
        osRecvMesg(&D_8017A498,&message,1);
        do {status=osRecvMesg(&D_8017A498,&message,0);} while(status!=-1);
        status=func_800E73D8();
        if(!status)continue;
        if(D_801525F0)func_800E6AF8();
    }
}
