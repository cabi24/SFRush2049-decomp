/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct TaskView128 {u8 other0[88];void *commands;u16 kind;u8 other94[30];void *buffer;} TaskView128;
extern int D_8015F738,D_80161380,D_80161398,D_801613A4;
extern float D_80154188;
extern int D_80000300,D_8002AFC0,D_8002AFC4;
extern void *D_8002EBB0,*D_80156CEC,*D_80157240;
extern TaskView128 D_80156BE0[];
extern u8 D_800586D0[],D_8006A090[];
extern void *D_8015B250,*D_8015B260,*D_801497F4;
extern void *msgq_ptr;
extern void *D_8011025C,*D_80110260;
extern int D_80152770;
extern s8 D_80156994,D_801147C0,D_80149DA0;
extern OSMesgQueue D_801461D0;
extern void *D_801461FC;
void func_800E7D0C(int,int);void menu_save_options(void);
int get_tv_offset(void);void viewport_setup(int,int,int,int);
void *audio_dma_sync(int,u32);void *sound_play_menu(int,u32);
void audio_loop_control(void *,int);
void func_800A5B3C(void);void func_800A5488(void);void particle_velocity_set(void);
void hud_setup(int,int,int,int,int,float,float,int);
int osRecvMesg(OSMesgQueue *,void **,int);int osJamMesg(OSMesgQueue *,void *,int);
void audio_reverb_update(void *,u32);
void effect_spawn(void);void player_state_set(int,int);void func_800DE45C(void);
void viTickStart(void);void osCreateMesgQueue(OSMesgQueue *,void **,int);
void game_late_init(void)
{
    void *release;
    func_800E7D0C(50,10);
    menu_save_options();
    D_8015F738=0;
    D_80161380=0;
    D_80161398=0;
    D_801613A4=0;
    D_80154188=90.0f;
    while(!D_80000300) {}
    viewport_setup(D_80000300,get_tv_offset(),D_8002AFC0,D_8002AFC4);
    D_8002EBB0=audio_dma_sync(0,0x22620);
    audio_loop_control(D_8002EBB0,0);
    D_80156CEC=sound_play_menu(0,0x22620);
    audio_loop_control(D_80156CEC,0);
    D_80157240=sound_play_menu(0,0x22620);
    audio_loop_control(D_80157240,0);
    D_8002EBB0=(void *)(((u32)D_8002EBB0+32)&~63u);
    D_80156CEC=(void *)(((u32)D_80156CEC+32)&~63u);
    D_80157240=(void *)(((u32)D_80157240+32)&~63u);
    D_80156BE0[1].buffer=D_80157240;
    D_80156BE0[0].commands=D_800586D0;
    D_80156BE0[1].commands=D_8006A090;
    D_80156BE0[0].kind=2;
    D_80156BE0[1].kind=2;
    D_80156BE0[0].buffer=D_80156CEC;
    D_8015B250=D_800586D0;
    D_8015B260=D_800586D0+1728;
    msgq_ptr=D_800586D0+0x9cc0;
    D_801497F4=msgq_ptr;
    func_800A5B3C();
    func_800A5488();
    particle_velocity_set();
    hud_setup(32,16,16,5,64,1.0f,200.0f,0);
    if(!D_80156994) {
        if((release=D_8011025C)!=0) {
            osRecvMesg((OSMesgQueue *)&D_80152770,0,1);
            audio_reverb_update(release,0);
            osJamMesg((OSMesgQueue *)&D_80152770,0,0);
            D_8011025C=0;
        }
        if((release=D_80110260)!=0) {
            osRecvMesg((OSMesgQueue *)&D_80152770,0,1);
            audio_reverb_update(release,0);
            osJamMesg((OSMesgQueue *)&D_80152770,0,0);
            D_80110260=0;
        }
    }
    effect_spawn();
    player_state_set(-1,0);
    func_800DE45C();
    viTickStart();
    if(!D_801147C0) {
        D_801147C0=1;
        osCreateMesgQueue(&D_801461D0,&D_801461FC,1);
        osJamMesg(&D_801461D0,0,0);
    }
    D_80149DA0=-1;
}
