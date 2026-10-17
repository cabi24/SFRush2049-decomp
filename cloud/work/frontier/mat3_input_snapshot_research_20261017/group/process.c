/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Player76 {u8 index,selector;u8 gap[2];u32 previous,current,pressed;f32 horizontal,vertical;u8 opaque[52];} Player76;
typedef struct Axes8 {f32 horizontal,vertical;} Axes8;
extern Player76 input_rec0[];
extern s16 active_player_count;
extern u32 D_80156978[],D_80156998[],D_80143A00[];
extern Axes8 D_80156958[];
extern void controller_poll(void);
void process_inputs(void) {
 int player;
 Player76 *record;
 controller_poll();
 for(player=0;player<4;player++) {
  record=&input_rec0[player];
  if(record->selector==5 || player>=active_player_count) {
   record->current=0;
   record->previous=0;
   record->pressed=0;
   record->horizontal=0.0f;
   record->vertical=0.0f;
  } else {
   record->current=D_80156978[record->selector];
   record->previous=D_80156998[record->selector];
   record->pressed=D_80143A00[record->selector];
   record->horizontal=D_80156958[record->selector].horizontal;
   record->vertical=D_80156958[record->selector].vertical;
  }
 }
}
