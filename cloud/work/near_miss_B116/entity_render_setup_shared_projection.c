/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef union Color4 {u32 word;u8 bytes[4];} Color4;
typedef struct Effect10 {u8 prefix[4];s16 view,mesh,player;} Effect10;
typedef struct Record952 {u8 prefix[8];Vec3 position;u8 gap20[836];s8 hidden;u8 gap857[51];u32 flags;u8 gap912[16];s8 color_index;u8 alpha;s8 alpha_mode;u8 tail[21];} Record952;
typedef struct Vehicle2056 {u8 prefix[1732];s16 state;u8 gap1734[256];s16 player;u8 gap1992[4];s8 mode;u8 tail[59];} Vehicle2056;
typedef struct Camera152 {float projection[9];Vec3 position;float basis[9];u8 tail[68];} Camera152;
typedef struct View72 {u8 prefix[12];float radius,scale;u8 tail[52];} View72;
typedef struct Record88 {u8 prefix[2];u16 flags;u8 rest[84];} Record88;
extern Record952 D_80152818[];extern Vehicle2056 D_8014A250[];
extern Camera152 D_80150B70[];extern View72 D_8017A510[];
extern Record88 D_8015B268[];
extern s8 D_801613A8,D_8017A63C,D_801613C0[],D_8012E67C[];
extern int D_801174B4,D_8014A110;
extern Color4 D_8011B558[];extern u32 D_8011B578;
extern u8 D_8011B56C[];extern Vec3 D_8011AD90[4];
extern float D_80123904,D_80123908,D_8012390C,D_80123910,D_80123914,D_80123918,D_8012391C,D_80123920,D_80123924,D_80123928,D_8012392C,D_80123930,D_80123934;
extern void func_8008C074(Record88 *,int,float *,u16,u8 *,u16,int);
extern void func_8008C544(Vec3 *,Vec3 *,float *);
extern float func_8008C768(float,float),func_8008B3C8(Vec3 *);
void entity_render_setup(Effect10 *effect)
{
 Record952 *record;
 Vehicle2056 *vehicle;
 Camera152 *camera;
 View72 *view;
 Color4 color;
 Vec3 delta,quad[4];
 float angle,yaw,depth,slope,scale,threshold,gain,x,y;
 int type,side,i;
 record=&D_80152818[effect->player];
 vehicle=&D_8014A250[effect->player];
 type=effect->view;
 color.word=D_8011B578;
 if(!D_801613A8 || !(D_801174B4&0x400000) || (!(!record->hidden && vehicle->state<0) && D_8014A110==6)) {
  D_8015B268[effect->mesh].flags|=0x8000;return;
 }
 if(D_8014A110==6 && (record->flags&1)) {
  if(record->alpha_mode==1 || record->alpha_mode==2)color.bytes[3]=record->alpha;
  else color.bytes[3]=D_8011B56C[record->color_index];
  if(color.bytes[3]<32)color.bytes[3]=32;
 }
 if(vehicle->mode==2) {
  if(D_8014A110==6) {
   color.bytes[0]=D_8011B558[D_8012E67C[vehicle->player]].bytes[0];
   color.bytes[1]=D_8011B558[D_8012E67C[vehicle->player]].bytes[1];
   color.bytes[2]=D_8011B558[D_8012E67C[vehicle->player]].bytes[2];
  } else {
   color.bytes[0]=D_8011B558[vehicle->player].bytes[0];
   color.bytes[1]=D_8011B558[vehicle->player].bytes[1];
   color.bytes[2]=D_8011B558[vehicle->player].bytes[2];
  }
  if(D_801613C0[vehicle->player*4+type] && color.bytes[3]>112)color.bytes[3]=112;
  if(D_8014A110==2 && effect->player>0)color.bytes[3]=128;
 } else {
  color.bytes[0]=D_8011B558[4].bytes[0];
  color.bytes[1]=D_8011B558[4].bytes[1];
  color.bytes[2]=D_8011B558[4].bytes[2];
 }
 camera=&D_80150B70[type];
 delta.x=record->position.x-camera->position.x;
 delta.y=record->position.y-camera->position.y;
 delta.z=record->position.z-camera->position.z;
 if(D_8014A110==6) {
  func_8008C544(&delta,&quad[0],camera->projection);
  angle=func_8008C768(-quad[0].x,quad[0].z);
  view=&D_8017A510[type];
  depth=D_80123904/view->radius;
  yaw=angle;
  if(D_8017A63C>=2) {slope=16.5f;scale=view->scale;threshold=scale*0.75f;}
  else {slope=14.5f;scale=view->scale;threshold=scale*D_80123908;}
  if(angle < -view->radius*D_8012390C && angle>D_80123910) {
   gain=D_80123914;x=view->radius*gain*19.0f;yaw=-angle;side=3;
   goto vertical_projection;
  } else if(angle<D_80123918 || D_80123920<angle) {
   if(angle<D_80123918)yaw=angle+D_8012391C;
   else yaw=angle-D_80123924;
   gain=D_80123928;
   x=view->radius*gain*19.0f*yaw*gain;
   y=scale*gain*slope;
   side=0;goto projected_quad;
  } else if(view->radius*D_8012390C<angle && angle<D_8012392C) {
   gain=D_80123930;x=view->radius*gain*(-19.0f);side=1;
   goto vertical_projection;
  } else goto world_quad;
vertical_projection:
  y=scale*gain*slope*(yaw-threshold)*(1.0f/(D_80123934-threshold));
projected_quad:
  for(i=0;i<4;i++) {
   quad[side].x=D_8011AD90[i].x+x;
   quad[side].z=depth;
   quad[side].y=D_8011AD90[i].y-y;
   side=(side+1)&3;
  }
  func_8008C074(&D_8015B268[effect->mesh],4,(float *)quad,0,color.bytes,(1<<type)|0x3610,0);
  return;
 }
world_quad:
 scale=func_8008B3C8(&delta)/25.0f;
 for(i=0;i<4;i++) {
  func_8008C544(&D_8011AD90[i],&quad[i],camera->basis);
  quad[i].x=record->position.x+quad[i].x*scale;
  quad[i].y=record->position.y+quad[i].y*scale+scale+5.0f;
  quad[i].z=record->position.z+quad[i].z*scale;
 }
 if(D_8014A110==6)func_8008C074(&D_8015B268[effect->mesh],4,(float *)quad,0,color.bytes,(1<<type)|0x3600,0);
 else func_8008C074(&D_8015B268[effect->mesh],4,(float *)quad,0,color.bytes,(1<<type)|0x1600,0);
}
