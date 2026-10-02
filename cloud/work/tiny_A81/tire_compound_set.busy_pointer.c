/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct Slot20 {u8 other0[2];s8 flag;u8 other3[9];void *resource;u8 other16[4];} Slot20;
extern u8 D_80035440[];
extern volatile s16 D_8002EB70;
extern int D_80156990,D_80156CE0,D_801613AC,D_80156BAC,D_801613B4;
extern s16 D_8015B254,D_80140618;
extern float D_801613B8;
extern void *D_801406B8;
extern u8 D_8011EA18[];
extern Slot20 D_80156D38[];
extern u8 D_80140BDC;
extern int osRecvMesg(void *,void *,int);
extern int osJamMesg(void *,void *,int);
extern void func_800A5A40(void),func_800A5488(void),func_800A5158(void);
extern void func_80096130(int);
extern void car_damage_visual(int);
void tire_compound_set(void)
{
    void *message;
    int i;
    volatile s16 *busy;
    if(osRecvMesg(D_80035440,&message,0)==-1) osRecvMesg(D_80035440,&message,1);
    osJamMesg(D_80035440,0,1);
    busy=&D_8002EB70;
    while(*busy!=0) {}
    D_80156990=0;
    D_80156CE0=0;
    D_801613AC=0;
    D_80156BAC=0;
    D_801613B4=0;
    D_8015B254=-1;
    func_800A5A40();
    D_801613B8=1.0f;
    D_80140618=0;
    D_801406B8=D_8011EA18;
    func_800A5488();
    for(i=0;i<64;i++) if(D_80156D38[i].resource!=0 && D_80156D38[i].flag==0)func_80096130(i);
    D_80140BDC=0;
    for(i=0;i<64;i++) if(D_80156D38[i].resource!=0)D_80140BDC=i+1;
    car_damage_visual(0);
    func_800A5158();
}
