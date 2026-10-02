/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef unsigned short u16;typedef signed short s16;typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player76 {u8 index,selector;u8 opaque[70];Handle *handle;} Player76;
typedef struct Session76 {u8 prefix[5],option,enabled,level,mode,length;u8 gap[2];u32 seed;float score;u8 tail[56];} Session76;
extern Player76 input_rec0[];
extern u8 D_801543D4,D_80155140[];
extern s16 D_80155148[][6];
extern u32 D_80111784[],D_8011176C[6];
extern int championship_standings(u8 *),func_800DC628(u32,u32),func_800DC57C(u32 *,u32);
extern void func_800FDF88(u32),net_session_update(void);
int stat_race_start(u8 *input) {
 u32 mode,option,length,seed,level;
 u32 values[6],ranks[6];
 Session76 *session;
 u32 i,j,total_weights,total_values;
 session=(Session76 *)(input_rec0[D_801543D4].handle->object->resource->data+1780);
 func_800DC628(86,14);
 if(!championship_standings(input))return 0;
 func_800DC57C(&mode,3);
 func_800DC57C(&option,1);
 func_800DC57C(&length,5);
 func_800DC57C(&seed,20);
 func_800DC57C(&level,3);
 for(i=0;i<6;i++)func_800DC57C(&values[i],9);
 if(length==0 || length>=D_80111784[mode]*4)return 0;
 for(i=0;i<6;i++)ranks[i]=i;
 for(i=0;i<5;i++) {
  for(j=i+1;j<6;j++) {
   if(values[ranks[i]]<values[ranks[j]]) {
    ranks[i]^=ranks[j];ranks[j]^=ranks[i];ranks[i]^=ranks[j];
   }
  }
 }
 if(D_8011176C[0]*length<values[ranks[0]])return 0;
 total_weights=0;
 for(i=0;i<6;i++)total_weights+=D_8011176C[i];
 total_values=0;
 for(i=0;i<6;i++)total_values+=values[ranks[i]];
 if(total_values!=total_weights*length)return 0;
 func_800FDF88(mode);
 session->enabled=1;
 session->option=option;
 session->length=length;
 D_80155140[input_rec0[D_801543D4].selector]=length;
 session->seed=seed;
 session->level=level;
 for(i=0;i<6;i++)D_80155148[input_rec0[D_801543D4].selector][i]=values[i];
 net_session_update();
 return 1;
}
