/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * music_tempo_set @ 0x800B200C, 980 bytes (w7d; from w6b's 31/245 best.c). Names are historical labels.
 * music_tempo_set(player, mode, apply): car model/resource refresh for one player. When apply is set and
 * mode != 13 and the car's model is visible (flags & 0x10), under the D_80034840 thread stop/start
 * (osPfsChecker_full / osStartThread labels) clear the hulk/visibility bits of the vehicle record
 * (D_8014A250, 0x808 bytes) and the model (player_array, 952 bytes) with a mask picked from the slot mode
 * (D_80153E88[player].mode) and D_80146180[player]. Then set the object (+20, u16) of the car's root, body
 * and four wheel resources (D_8012E700, 68 bytes) from func_80092B80(player, mode), D_801427C2[player*3]
 * and D_801428F8; load the wheel model set (vehicle type, or 13 in attract/when D_8014978C >= 6); show the
 * root, the body unless selection == 1, and (apply) the extras and wheels; mode 13 also restores the
 * visibility bits.
 * Shaping (measured in the whole-program unit with --block func_80092B80, see w7d/RESULTS.md):
 *  - resource_set_object is the inlined static setter w6b found (also in func_8010E0FC): each inlined
 *    call adds 8 bytes of named locals;
 *  - declaration order model, slot, mask, color, i: the two 2-byte locals (u16 color, s16 i) are adjacent,
 *    so the named area is 40, not 48 (the 8-byte "frame 96 vs 88" residual was alignment padding);
 *  - the first block uses player_array[player] directly (no `model` there): the spilled address is then a
 *    uopt temp whose base ugen knows, so as1 may schedule the model load above the vehicle store;
 *  - if (model) {} after the second model assignment (compiled-out check) ends the block (w6b);
 *  - the D_80156994 if/else on separate lines from its calls (as1 tie-break by .loc: lui of D_8014978C
 *    before the branch, the 2056 multiply's addu in the delay slot).
 */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;
typedef struct Model952 {u8 prefix[232];u32 flags;u8 gap236[8];int model;u8 gap248[613];s8 selection;u8 tail[90];} Model952;
typedef struct Vehicle2056 {u8 prefix[15];s8 type;u8 gap16[1988];u32 flags;u8 tail[48];} Vehicle2056;
typedef struct Slot64 {int root;u8 gap4[12];int body;u8 gap20[8];int wheels[4],extras[5];} Slot64;
typedef struct Resource68 {u8 prefix[20];u16 object;u8 tail[46];} Resource68;
typedef struct Config8 {u8 prefix[7];u8 mode;} Config8;
typedef struct OSThread OSThread;
extern Model952 player_array[];
extern Vehicle2056 D_8014A250[];
extern Slot64 D_80139320[];
extern Resource68 D_8012E700[];
extern Config8 D_80153E88[];
extern OSThread D_80034840;
extern s8 D_80146180[],D_80156994,D_8014978C;
extern u16 D_801427C2[],D_801428F8;
extern void *D_80143F74[];
extern void osPfsChecker_full(OSThread *);
extern void osStartThread(OSThread *);
extern u16 func_80092B80(s16,u8);
extern void func_8008D870(s16,void *,int);
extern void model_transform_setup(int,int,int);
extern void model_data_load(int,int,int);
static void resource_set_object(s16 index, u16 object)
{
    D_8012E700[index].object = object;
}

void music_tempo_set(s16 player,u8 mode,int apply)
{
 Model952 *model;
 Slot64 *slot;
 int mask;
 u16 color;
 s16 i;
 if(apply && mode!=13) {
  if(player_array[player].flags&0x10) {
   osPfsChecker_full(&D_80034840);
   if(D_80153E88[player].mode!=6 || D_80146180[player]==0)mask=~0x10;
   else if(D_80146180[player]==2)mask=~0x60;
   else mask=-1;
   D_8014A250[player].flags&=mask;
   player_array[player].flags&=mask;
   osStartThread(&D_80034840);
  }
 }
 model=&player_array[player];
 if(model){}
 color=func_80092B80(player,mode);
 slot=&D_80139320[player];
 resource_set_object(slot->root,color);
 model->model=slot->root;
 resource_set_object(slot->body,D_801427C2[(s16)(player*3)]);
 for(i=0;i<4;i++)resource_set_object(D_80139320[player].wheels[i],D_801428F8);
 if(D_80156994 || D_8014978C>=6) {
  func_8008D870((s16)slot->wheels[0],D_80143F74[D_8014A250[player].type],-1);
 } else {
  func_8008D870((s16)slot->wheels[0],D_80143F74[13],-1);
 }
 model_transform_setup(slot->root,1,15);
 if(model->selection!=1)model_data_load(slot->body,1,15);
 if(apply) {
  model_data_load(slot->extras[0],1,15);
  model_data_load(slot->extras[1],1,15);
  model_data_load(slot->extras[2],1,15);
  model_data_load(slot->extras[3],1,15);
  model_data_load(slot->extras[4],1,15);
  if(mode==13 || model->selection<2) {
   for(i=0;i<4;i++)model_data_load(D_80139320[player].wheels[i],1,15);
   if(mode==13) {
    model_data_load(slot->body,1,15);
    osPfsChecker_full(&D_80034840);
    D_8014A250[player].flags|=0x10;
    model->flags|=0x10;
    osStartThread(&D_80034840);
   }
  }
 }
}
