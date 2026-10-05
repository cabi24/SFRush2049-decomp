/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSSched OSSched;
typedef struct OSScClient { struct OSScClient *next; OSMesgQueue *queue; } OSScClient;
typedef struct {
    u8 prefix[6]; s8 present; u8 to_active[117];
    s8 active,strength,current; u8 gap127;
    s16 cycle; u8 tail[772-130];
} Controller772;
extern Controller772 D_80144030[];
extern OSMesgQueue D_80154378,D_801497D0,D_80035458;
extern void *D_8015439C,*D_801527E4;
extern OSSched D_8002E8E8;
extern s8 D_8011194C,D_8012EAE4;
extern u16 D_8012ECEC[];
extern void osCreateMesgQueue(OSMesgQueue *,void **,s32);
extern void osScAddClient(OSSched *,OSScClient *,OSMesgQueue *);
extern s32 osRecvMesg(OSMesgQueue *,void **,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
/* Retail symbol labels are historical: these visible contracts are
   three-input pak initialization and two-input motor access. */
extern s32 osMotorStart(OSMesgQueue *,void *,s32);
extern s32 osMotorInit(void *,s32);
void audio_queue_process(void *userdata)
{
    OSScClient client;
    void *message;
    void *semaphore;
    Controller772 *controller;
    void *pak;
    s32 port,retried,power;
    s16 cycle;
    osCreateMesgQueue(&D_80154378,&D_8015439C,1);
    osScAddClient(&D_8002E8E8,&client,&D_80154378);
    for(;;) {
        osRecvMesg(&D_80154378,&message,1);
        if(!D_8011194C) {
            D_8011194C=1;
            osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
            osJamMesg(&D_801497D0,0,0);
        }
        osRecvMesg(&D_801497D0,&semaphore,1);
        for(port=0;port<4;port++) {
            controller=&D_80144030[port];
            if(!controller->present) continue;
            retried=0;
            cycle=controller->cycle+1;
            controller->cycle=cycle;
            if(cycle>=10) controller->cycle=0;
            power=controller->strength;
            if(power>=41) power=40;
            if(power<controller->current) power=controller->current-1;
            controller->current=power;
            if(D_8012EAE4 && power>0) {
                if(power>=10) goto start_motor;
                if(D_8012ECEC[power]&(1<<controller->cycle)) goto start_motor;
            }
            if(!controller->active) continue;
            if(!D_8012EAE4) osMotorStart(&D_80035458,(u8 *)controller+12,port);
            pak=(u8 *)controller+12;
stop_retry:
            if(!osMotorInit(pak,0)) { controller->active=0; continue; }
            if(retried) continue;
            if(osMotorStart(&D_80035458,pak,port)) continue;
            retried=1;
            goto stop_retry;
start_motor:
            pak=(u8 *)controller+12;
            if(controller->active) continue;
start_retry:
            if(!osMotorInit(pak,1)) { controller->active=1; continue; }
            if(retried) continue;
            if(osMotorStart(&D_80035458,pak,port)) continue;
            retried=1;
            goto start_retry;
        }
        osJamMesg(&D_801497D0,0,0);
    }
}
