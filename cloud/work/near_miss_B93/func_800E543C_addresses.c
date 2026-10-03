/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct Vehicle2056 {
 u8 gap0[1152];
 float velocity_a;
 float velocity_b;
 u8 gap1160[84];
 float velocity_c;
 float velocity_d;
 u8 gap1252[84];
 float velocity_e;
 float velocity_f;
 u8 gap1344[84];
 float velocity_g;
 float velocity_h;
 u8 gap1436[164];
 s8 state;
 u8 gap1601[123];
 int speed;
 u8 gap1728[16];
 s16 camera;
 u8 gap1746[74];
 s16 selected;
 u8 gap1822[18];
 u8 mode;
 u8 gap1841[149];
 s16 index;
 u8 gap1992[48];
 float wheel_a;
 float wheel_b;
 float wheel_c;
 float wheel_d;
} Vehicle2056;
typedef struct Model952 {u8 prefix[4];float setup;u8 gap8[848];s8 clear,active;u8 gap858[10];float drift;u8 tail[80];} Model952;
typedef struct Config8 {u8 prefix[5],camera,unknown,mode;} Config8;
extern s8 D_80142760;
extern Model952 player_array[];
extern Vehicle2056 D_8014A250[];
extern Config8 D_80153E88[];
extern int gameplay_mode;
extern s16 D_801525F0;
extern void car_setup_confirm(int,float);
extern void func_800C4F68(int,Vehicle2056 *,int);
extern void stunt_combo_display(Vehicle2056 *);
extern void func_800D5E64(Vehicle2056 *);
void func_800E543C(s16 action,s16 index)
{
 Model952 *model;Config8 *config;
 if(D_80142760) {
  model=&player_array[index];
  if(!model->active && action==2) {
   model->active=1;D_8014A250[index].state=0;
   car_setup_confirm(index,model->setup);D_8014A250[index].selected=1;return;
  }
 }
 model=&player_array[index];
 if(model->active<0)model->active=0;
 D_8014A250[index].selected=0;
 if(action==0) {
  if(gameplay_mode==2) {config=&D_80153E88[index];D_8014A250[index].camera=0;}
  else {config=&D_80153E88[index];D_8014A250[index].camera=config->camera;}
  func_800C4F68(0,&D_8014A250[index],0);
 } else {
  if(gameplay_mode==2) {config=&D_80153E88[index];D_8014A250[index].camera=6;}
  else {config=&D_80153E88[index];D_8014A250[index].camera=index+6;}
 }
 stunt_combo_display(&D_8014A250[index]);
 model->clear=0;
 D_8014A250[index].velocity_h=0.0f;D_8014A250[index].velocity_g=0.0f;D_8014A250[index].wheel_d=0.0f;
 D_8014A250[index].velocity_f=0.0f;D_8014A250[index].velocity_e=0.0f;D_8014A250[index].wheel_c=0.0f;
 D_8014A250[index].velocity_d=0.0f;D_8014A250[index].velocity_c=0.0f;D_8014A250[index].wheel_b=0.0f;
 D_8014A250[index].wheel_a=0.0f;D_8014A250[index].velocity_a=0.0f;D_8014A250[index].velocity_b=0.0f;
 if(gameplay_mode!=2 || D_8014A250[index].index==0)func_800D5E64(&D_8014A250[index]);
 if(config->mode==6) {
  if(action!=0) {if(D_8014A250[index].speed>=44.0f)D_8014A250[index].mode=2;else D_8014A250[index].mode=1;}
  model->drift=0.0f;
 }
 D_801525F0=1;
}
