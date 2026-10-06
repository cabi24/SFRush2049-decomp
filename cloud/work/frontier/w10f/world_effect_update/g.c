typedef unsigned int u32;
typedef short s16;
typedef unsigned char u8;
typedef struct Record128 {u8 opaque0[88];void *resource;s16 kind;u8 opaque94[30];void *buffer;} Record128;
extern u32 D_8015F738,D_80161380,D_80161398,D_801613A4;
extern float D_80154188;
extern int D_80000300;
extern void *D_8002AFC0,*D_8002AFC4;
extern void * volatile D_8002EBB0, * volatile D_80156CEC, * volatile D_80157240;
extern Record128 D_80156BE0[2];
extern u8 D_800586D0[],D_8006A090[];
extern void *D_8015B250,*D_8015B260,*D_801497F4; extern void * volatile D_801497C8;
extern int get_tv_offset(void);
extern void viewport_setup(int,int,void *,void *);
extern void *audio_dma_sync(void *,u32);
extern void *sound_play_menu(void *,u32);
extern void audio_loop_control(void *,int);
extern void func_800A5B3C(void),func_800A5488(void),particle_velocity_set(void);
void world_effect_update(void) {
    void *a, *b, *c;

    while (D_80000300 == 0) {}

    D_80156BE0[0].buffer = a;
    D_80156BE0[1].buffer = c;
    D_80156BE0[0].kind = 2;
    D_80156BE0[1].kind = 2;
    D_80156BE0[0].resource = D_800586D0;
    D_80156BE0[1].resource = D_8006A090;
    D_8015B250 = D_800586D0;
    D_8015B260 = (u8 *)D_8015B250 + 1728;
    D_801497C8 = (u8 *)D_8015B250 + 0x9cc0;
    D_801497F4 = D_801497C8;
    func_800A5B3C();
    func_800A5488();
    particle_velocity_set();
}
