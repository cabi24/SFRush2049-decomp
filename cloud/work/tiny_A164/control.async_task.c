/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef short s16;typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *msgQ;} OSScClient;
typedef struct SchedulerView {u8 other0[628];void * volatile curRDPTask;} SchedulerView;
typedef struct ScMessage {s16 type;} ScMessage;
extern OSMesgQueue D_80152788;
extern void *D_801527A8[];
extern SchedulerView D_8002E8E8;
extern f32 D_80152748,D_8002AFB8;
extern void *D_8002AFA0;
extern void (*D_8011EAA8)(void);
void osCreateMesgQueue(OSMesgQueue *,void **,int);
void osScAddClient(SchedulerView *,OSScClient *,OSMesgQueue *);
int osRecvMesg(OSMesgQueue *,void **,int);
void func_800205E4(void);
void dma_wait_complete(void *userdata)
{
    OSScClient client;
    ScMessage *message=0;
    osCreateMesgQueue(&D_80152788,D_801527A8,8);
    osScAddClient(&D_8002E8E8,&client,&D_80152788);
    D_80152748=0.0f;
    for(;;) {
        osRecvMesg(&D_80152788,(void **)&message,1);
        if(message->type==1) {
            void (*callback)(void)=D_8011EAA8;
            if(callback) {
                void *task=D_8002AFA0;
                if(task && D_8002E8E8.curRDPTask==task) {
                    while(task && D_8002E8E8.curRDPTask==task) {}
                }
                callback();
            }
            D_80152748+=D_8002AFB8;
            if(D_80152748>14400.0f)D_80152748-=14400.0f;
        } else if(message->type==4) {
            func_800205E4();
        }
    }
}
