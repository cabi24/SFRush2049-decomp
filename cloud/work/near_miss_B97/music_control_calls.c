/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Record24 Record24;
struct Record24 {u8 prefix[4];s16 target,handle,owner;u8 gap10[6];float scale;void (*callback)(Record24 *,s16);};
typedef struct Model952 {u8 prefix[512];Record24 main,peers[2];u8 tail[368];} Model952;
typedef struct Vehicle2056 {u8 prefix[1992];s16 active;u8 tail[62];} Vehicle2056;
typedef struct Color4 {u8 r,g,b,a;} Color4;
typedef struct Vertex20 {Vec3 point;u16 s,t;u32 color;} Vertex20;
typedef struct Record88 {u16 type,flags;u8 gap4[2];u16 state;Vertex20 vertices[4];} Record88;
typedef struct Resource16 {u8 prefix[12];u16 single,peer;} Resource16;
typedef struct Config8 {u8 prefix[7];u8 mode;} Config8;
extern Model952 player_array[];
extern Vehicle2056 D_8014A250[];
extern Vec3 D_8011AD90[4];
extern Resource16 D_80161368;
extern Record88 D_8015B268[];
extern Color4 D_8011AD8C,D_8011B574,D_8011B558[];
extern float D_801543CC;
extern u32 state_word_a;
extern s16 active_player_count,D_80151AD0;
extern int gameplay_mode;
extern Config8 D_80153E88[];
extern s8 D_8012E67C[];
extern void music_seq_load(s16,int,int);
extern void audio_frame_update(s16);
extern void entity_process_main(Record24 *,s16);
extern void entity_render_setup(Record24 *,s16);
extern Record88 *func_800A78BC(int,Vec3 *,u16,u8 *,u16,int);
void music_control(void)
{
 s16 i,owner;
 int j,flags;
 Model952 *model;
 Record24 *peer;
 Record88 *created;
 Color4 color;
 for(i=0;i<6;i++) {
  if(D_8014A250[i].active) {music_seq_load(i,15,1);audio_frame_update(i);}
 }
 for(i=0;i<6;i++) {
  if(!D_8014A250[i].active)continue;
  model=&player_array[i];model->main.callback=entity_process_main;model->main.owner=i;
  model->main.scale=D_801543CC;D_8011AD8C.a=192;
  if(state_word_a&0x100)flags=1;else flags=15;
  created=func_800A78BC(4,D_8011AD90,D_80161368.single,(u8 *)&D_8011AD8C,flags|0x8200,1);
  model->main.handle=created-D_8015B268;
  if(active_player_count<2 || D_80153E88[i].mode!=6)continue;
  owner=i;color=D_8011B574;peer=model->peers;
  if(gameplay_mode==6) {
   color.r=D_8011B558[D_8012E67C[i]].r;color.g=D_8011B558[D_8012E67C[i]].g;
   color.b=D_8011B558[D_8012E67C[i]].b;color.a=D_8011B558[D_8012E67C[i]].a;
  } else {
   color.r=D_8011B558[i].r;color.g=D_8011B558[i].g;
   color.b=D_8011B558[i].b;color.a=D_8011B558[i].a;
  }
  for(j=0;j<D_80151AD0;j++) {
   if(i==j)continue;
   peer->callback=entity_render_setup;peer->owner=i;peer->target=j;
   if(gameplay_mode==6)created=func_800A78BC(4,D_8011AD90,D_80161368.peer,(u8 *)&color,(1<<j)|0x3200,1);
   else created=func_800A78BC(4,D_8011AD90,D_80161368.peer,(u8 *)&color,(1<<j)|0x1200,1);
   created->state=1;peer->handle=created-D_8015B268;peer++;
  }
  while(peer<player_array[owner].peers+2) {peer->callback=0;peer->handle=-1;peer++;}
 }
}
