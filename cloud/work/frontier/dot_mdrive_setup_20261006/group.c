/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH research: N64 Init_MDrive/multiinit-related setup, func_800E543C.
 * Canonical 132/175 differing words; exact 700-byte extent and native 72-byte
 * frame. No unresolved/unverified sites, extra words, or errors.
 * Base f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2; prior complete corrected
 * source: cloud/work/near_miss_B93/func_800E543C_branches.c (171/175).
 *
 * Donor ancestry: historicalsource/rushtherock
 * 845329d7b36f5a384c5625ed9a0aef584ab46139 game/mdrive.c:Init_MDrive/multiinit.
 * The two signed-halfword parameters, field offsets, N64-only branches and
 * final drift clear come from the corrected native-led B93 source.
 * The repeated 92-byte tire records and four trailing wheel values represent
 * observed real fields; unknown record gaps are layout, not stack padding.
 * The genuine multiinit boundary is an N64 source-history hypothesis and is
 * not assigned to a retail stub. It naturally inlines in the reported group.
 * D_801525F0 uses its accepted store-side volatile-s16 contract from
 * src/blob/func_800D60AC.c and players_frame_update.c, not a new carrier.
 * No unused locals, fake callers, added parameters, or assembly were added.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct Tire92 { float a, b; u8 rest[84]; } Tire92;
typedef struct Vehicle2056 {
 u8 gap0[1152];
 Tire92 tires[4];
 u8 gap1520[80];
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
 float wheels[4];
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
void multiinit_n64(s16 action,s16 index) {
 int i;
 Model952 *model;
 Vehicle2056 *vehicle;
 Config8 *config;
 model=&player_array[index];
 vehicle=&D_8014A250[index];
 if(action==0) {
  if(gameplay_mode==2) {config=&D_80153E88[index];vehicle->camera=0;}
  else {config=&D_80153E88[index];vehicle->camera=config->camera;}
  func_800C4F68(0,vehicle,0);
 } else {
  if(gameplay_mode==2) {config=&D_80153E88[index];vehicle->camera=6;}
  else {config=&D_80153E88[index];vehicle->camera=index+6;}
 }
 stunt_combo_display(vehicle);
 model->clear=0;
 for(i=3;i>=0;i--) {
  vehicle->tires[i].b=0.0f;
  vehicle->tires[i].a=0.0f;
  vehicle->wheels[i]=0.0f;
 }
 if(gameplay_mode!=2 || vehicle->index==0)func_800D5E64(vehicle);
 if(config->mode==6) {
  if(action!=0) {if(vehicle->speed>=44.0f)vehicle->mode=2;else vehicle->mode=1;}
  model->drift=0.0f;
 }
}

void func_800E543C(s16 action,s16 index)
{
 Model952 *model;Vehicle2056 *vehicle;
 if(D_80142760) {
  model=&player_array[index];
  if(!model->active && action==2) {
   model->active=1;vehicle=&D_8014A250[index];vehicle->state=0;
   car_setup_confirm(index,model->setup);vehicle->selected=1;return;
  }
 }
 model=&player_array[index];
 if(model->active<0)model->active=0;
 vehicle=&D_8014A250[index];vehicle->selected=0;
 multiinit_n64(action,index);
 D_801525F0=1;
}
