/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * func_800E543C (0x800E543C, 700 B): per-car restart/reset (camera view, stunt display, velocity clear, drift
 * mode). Lane w28d draft from near_miss_B93 `branches`, NOT a match.
 * Unit (us.sh): 152/175 words, rows 180, ops 26, frame 72/72 (B93 start: rows 223, frame 56).
 * Levers: expression form (no model/vehicle/config locals: retail spills them to spill-temp slots 28/32/36,
 * so they are coloured expression webs, not named locals); the camera stores through a 4-site inlined static
 * `set_cam(v, cam)` (hypothesis for the caller-less stub func_800E5434; it alone sizes the frame to 72).
 * SHAPING DEVICES (disclosed): three compiled-out `do {} while (0);` add blocks so the callee-saved cost
 * (7.55 -> above the 8.3 caller cost) keeps &D_8014A250[index] out of s0, as in retail.
 * `extern volatile s16 D_801525F0` (render-thread flag; retail forms its address: `addiu; sh 0(reg)`) -> ops 24.
 * Residual: index in a2 (retail t0, spilled to its home 78(sp) across func_800C4F68/stunt_combo_display: retail
 * keeps index live past those calls, use not identified); &D_801525F0 formed in a register in retail.
 */
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
extern volatile s16 D_801525F0;
extern void car_setup_confirm(int,float);
extern void func_800C4F68(int,Vehicle2056 *,int);
extern void stunt_combo_display(Vehicle2056 *);
extern void func_800D5E64(Vehicle2056 *);
static void set_cam(Vehicle2056 *v, s16 cam)
{
 v->camera=cam;
}
void func_800E543C(s16 action,s16 index)
{
 if(D_80142760) {
  if(!player_array[index].active && action==2) {
   player_array[index].active=1;D_8014A250[index].state=0;
   car_setup_confirm(index,player_array[index].setup);D_8014A250[index].selected=1;return;
  }
 }
 if(player_array[index].active<0)player_array[index].active=0;
 D_8014A250[index].selected=0;
 do {} while (0);
 if(action==0) {
  if(gameplay_mode==2) {set_cam(&D_8014A250[index],0);}
  else {set_cam(&D_8014A250[index],D_80153E88[index].camera);}
  func_800C4F68(0,&D_8014A250[index],0);
 } else {
  if(gameplay_mode==2) {set_cam(&D_8014A250[index],6);}
  else {set_cam(&D_8014A250[index],index+6);}
 }
 stunt_combo_display(&D_8014A250[index]);
 do {} while (0);
 player_array[index].clear=0;
 D_8014A250[index].velocity_h=0.0f;D_8014A250[index].velocity_g=0.0f;D_8014A250[index].wheel_d=0.0f;
 D_8014A250[index].velocity_f=0.0f;D_8014A250[index].velocity_e=0.0f;D_8014A250[index].wheel_c=0.0f;
 D_8014A250[index].velocity_d=0.0f;D_8014A250[index].velocity_c=0.0f;D_8014A250[index].wheel_b=0.0f;
 D_8014A250[index].wheel_a=0.0f;D_8014A250[index].velocity_a=0.0f;D_8014A250[index].velocity_b=0.0f;
 if(gameplay_mode!=2 || D_8014A250[index].index==0)func_800D5E64(&D_8014A250[index]);
 if(D_80153E88[index].mode==6) {
  if(action!=0) {if(D_8014A250[index].speed>=44.0f)D_8014A250[index].mode=2;else D_8014A250[index].mode=1;}
  player_array[index].drift=0.0f;
 }
 do {} while (0);
 D_801525F0=1;
}
