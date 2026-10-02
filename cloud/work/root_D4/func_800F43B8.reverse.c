/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef unsigned int u32;typedef int s32;typedef float f32;
typedef struct Stats28 {u32 checksum;u16 total,kind0,kind1,kind2,flagged,qualified,maximum,pad18;f32 best;u32 sum;} Stats28;
typedef struct Data {u8 *base;} Data;
typedef struct Car {u8 pad0[44];Data *data;} Car;
typedef struct Ref {Car *car;} Ref;
typedef struct Player76 {u8 pad0,index,pad2[70];Ref *ref;} Player76;
typedef struct Result4 {u8 pad0,kind;u16 value;} Result4;
extern u8 D_801543D4;extern Player76 D_8014A118[];extern Car *D_80146150[];
extern Stats28 D_80151618[];extern Result4 D_80154450;extern s8 D_80142760;
extern void menu_dialog_close(Ref *,u8);
#define CURRENT_DATA (D_8014A118[D_801543D4].ref->car->data->base)
void func_800F43B8(void) {
 Player76 *player;Ref *ref;Stats28 *record;s32 kind,i;
 player=&D_8014A118[D_801543D4];ref=player->ref;
 if(!ref){ref=(Ref *)&D_80146150[player->index];player->ref=ref;}
 kind=*(s8 *)(ref->car->data->base+1788);
 for(i=0;i<2;i++) {
  if(i!=0)record=&D_80151618[kind];
  else record=(Stats28 *)(CURRENT_DATA+1668)+kind;
  record->total++;
  if(D_80154450.kind==0)record->kind0++;
  else if(D_80154450.kind==1)record->kind1++;
  else if(D_80154450.kind==2)record->kind2++;
  if(D_80142760){record->flagged++;if(D_80154450.kind<3)record->qualified++;}
  record->sum+=D_80154450.value;
  if(record->maximum<D_80154450.value)record->maximum=D_80154450.value;
  if(record->best==0.0f||*(f32 *)(CURRENT_DATA+1796)<record->best)record->best=*(f32 *)(CURRENT_DATA+1796);
 }
 menu_dialog_close(D_8014A118[D_801543D4].ref,*(u8 *)(CURRENT_DATA+1788));
}
