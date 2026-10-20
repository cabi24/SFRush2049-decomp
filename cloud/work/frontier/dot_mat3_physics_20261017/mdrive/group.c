/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 Init_MDrive, func_800E543C (0x800E543C, 700 bytes).
 * Stock IDO 5.3 strict group MATCH: 175/175 words, native 72-byte frame.
 * Base 58425072b49e95f0fbe5c4c239988fb4885f7c5c; no image/ROM claim.
 *
 * MAT3 donor: mdrive.c Init_MDrive and its genuine static multiinit boundary.
 * Keep the final mode/drift logic in the root: index remains live across
 * SetResurrectionData/Resurrect, matching the native argument home and t0.
 * The forward clear loop uses the donor's wheel/sideforce/traction order.
 * Source history is inferred; this is not a claim of recovered exact source.
 *
 * Disclosures: the live Config8 pointer is a natural N64 address carrier,
 * not a donor-DWARF local; it supplies the final 8-byte frame difference.
 * Statement-line boundaries at the active/state stores and final gear arms
 * resolve two as1 scheduling ties. Keep their line separation when editing.
 * D_801525F0 retains the accepted volatile-s16 store-side contract from
 * src/blob/func_800D60AC.c and players_frame_update.c.
 * All gaps describe real structure offsets. No unused local, padding buffer,
 * false prototype, synthetic caller, new volatile access or assembly.
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
static void multiinit_n64(s16,s16);

void func_800E543C(s16 action,s16 index)
{
 Model952 *model=&player_array[index];
 Vehicle2056 *vehicle=&D_8014A250[index];
 Config8 *config;
 if(D_80142760 && !model->active && action==2) {
  model->active=1;
  vehicle->state=0;
  car_setup_confirm(index,model->setup);vehicle->selected=1;
 } else {
  if(model->active<0)model->active=0;
  vehicle->selected=0;
  multiinit_n64(action,index);
  config=&D_80153E88[index];
  if(config->mode==6) {
   if(action!=0) {if(vehicle->speed>=44.0f)
    vehicle->mode=2;
   else
    vehicle->mode=1;}
   model->drift=0.0f;
  }
  D_801525F0=1;
 }
}

static void multiinit_n64(s16 action,s16 index)
{
 Vehicle2056 *vehicle=&D_8014A250[index];
 int i;
 if(action==0) {
  if(gameplay_mode==2) vehicle->camera=0;
  else vehicle->camera=D_80153E88[index].camera;
  func_800C4F68(0,vehicle,0);
 } else {
  if(gameplay_mode==2) vehicle->camera=6;
  else vehicle->camera=index+6;
 }
 stunt_combo_display(vehicle);
 player_array[index].clear=0;
 for(i=0;i<4;i++) {
  vehicle->wheels[i]=0.0f;
  vehicle->tires[i].a=0.0f;
  vehicle->tires[i].b=0.0f;
 }
 if(gameplay_mode!=2 || vehicle->index==0)func_800D5E64(vehicle);
}
