/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[70];Handle *handle;} Player;
extern u32 state_word_a;
extern s8 D_8014978C;
extern Player input_rec0[];
extern Object *D_80146150[];
extern void func_800CD798(Handle *,u8);
#define FLAGS(base,stride,state,offset) (*(u16 *)((base)+(stride)*(state)+(offset)))
void linear_interp(int player,int bit) {
 Player *record;
 u32 mask;
 if(state_word_a&8)return;
 if(D_8014978C>=0 && D_8014978C<6) {
  record=&input_rec0[player];
  if(record->handle==0) {
   mask=1u<<bit;
   FLAGS(D_80146150[record->selector]->resource->data,96,D_8014978C,232)|=mask;
   FLAGS(D_80146150[record->selector]->resource->data,96,D_8014978C,808)|=mask;
  } else if(record->handle->object->resource!=0) {
   mask=1u<<bit;
   FLAGS(record->handle->object->resource->data,96,D_8014978C,232)|=mask;
   FLAGS(record->handle->object->resource->data,96,D_8014978C,808)|=mask;
   func_800CD798(record->handle,(u8)D_8014978C);
  }
 } else if(D_8014978C>=14 && D_8014978C<18) {
  record=&input_rec0[player];
  if(record->handle==0) {
   FLAGS(D_80146150[record->selector]->resource->data,64,D_8014978C,456)|=1u<<bit;
  } else {
   FLAGS(record->handle->object->resource->data,64,D_8014978C,456)|=1u<<bit;
   func_800CD798(record->handle,(u8)D_8014978C);
  }
 }
}
