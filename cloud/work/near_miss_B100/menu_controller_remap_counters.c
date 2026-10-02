/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Model952 {
 u8 gap0[8];
 Vec3 position;
 u8 gap20[24];
 float basis[9];
 u8 gap80[159];
 s8 marker;
 u8 gap240[10];
 s16 segment;
 s16 point;
 u8 gap254[2];
 float progress;
 float previous;
 u8 gap264[568];
 float start;
 u8 gap836[20];
 s8 clear;
 s8 active;
 u8 gap858[10];
 float angle;
 u8 tail[80];
} Model952;
typedef struct Vehicle2056 {
 u8 gap0[1008];
 float speed;
 u8 gap1012[720];
 s16 phase;
 u8 gap1734[6];
 s8 state;
 u8 gap1741[71];
 float clock;
 u8 gap1816[174];
 s16 owner;
 u8 gap1992[2];
 s16 special;
 u8 gap1996[12];
 float timer;
 u8 gap2012[4];
 s16 force;
 u8 gap2018[10];
 float update;
 u8 tail[24];
} Vehicle2056;
typedef struct Input76 {u8 player;u8 gap1[7];u32 pressed;u8 gap12[32];u32 mask;u8 tail[28];} Input76;
typedef struct Point6 {s16 x,y,z;} Point6;
typedef struct Route16 {u16 count,unknown;Point6 *points;u8 extra[4];struct Route16 *children;} Route16;
typedef struct Config8 {u8 prefix[7],mode;} Config8;
extern Model952 player_array[];
extern Vehicle2056 D_8014A250[];
extern Input76 input_rec0[];
extern Route16 D_801407F0;
extern Config8 D_80153E88[];
extern float D_801543CC,D_80124124;
extern s8 D_80152544,D_801403D0,D_80152718,D_8013FECB;
extern s16 active_player_count;
extern u32 state_word_a;
extern int gameplay_mode;
extern void func_800C54F0(s16,int);
extern int func_800CF604(s16);
extern void func_800C4F68(int,Vehicle2056 *,int);
extern void func_800B9F60(int,int,int *,int *);
extern void func_800A61B0(Vec3 *,Vec3 *,float *);
extern float func_8008C768(float,float);
void menu_controller_remap(void)
{
 Vehicle2056 *vehicle;
 Model952 *model;
 Input76 *input;
 Vec3 delta,view;
 s16 reset_index,car_index,player_index,owner,force,j;
 int factor,segment,index;
 if(D_801543CC<5.0f) {
  for(reset_index=0;reset_index<6;reset_index++) {
   D_8014A250[reset_index].timer=0.0f;player_array[reset_index].start=0.0f;player_array[reset_index].angle=0.0f;
  }
  return;
 }
 for(car_index=0;car_index<D_80152544;car_index++) {
  owner=D_8014A250[car_index].owner;vehicle=&D_8014A250[owner];model=&player_array[owner];
  if(!vehicle->special && D_80153E88[owner].mode!=6)continue;
  force=vehicle->force;
  if(model->position.y<-190.0f && !model->active && !model->clear && vehicle->phase==-1) {
   vehicle->state=1;func_800C54F0(vehicle->owner,0);return;
  }
  if(gameplay_mode==4 || gameplay_mode==5 || gameplay_mode==6)continue;
  if((state_word_a&0x400008) && gameplay_mode!=1 && !D_801403D0 && !D_80152718 &&
     !D_8013FECB && model->marker!=1 && func_800CF604(vehicle->owner) &&
     vehicle->update>D_80124124 && (vehicle->speed<25.0f || force)) {
   if(!force) {
    if(vehicle->timer==0.0f)vehicle->timer=vehicle->clock;
    else {
     input=&input_rec0[owner];factor=1;if(input->mask&input->pressed)factor=2;
     if(factor==4)vehicle->timer=vehicle->clock;
     else if(6.0f*factor<vehicle->clock-vehicle->timer)force=1;
    }
   }
   if(force) {
    func_800C4F68(2,vehicle,1);model->clear=2;model->active=-1;vehicle->timer=0.0f;vehicle->state=1;
   }
  } else vehicle->timer=0.0f;
 }
 for(player_index=0;player_index<active_player_count;player_index++) {
  owner=input_rec0[player_index].player;model=&player_array[owner];vehicle=&D_8014A250[owner];
  if(model->progress>model->previous) {
   model->start=0.0f;model->previous=model->progress;model->angle=0.0f;continue;
  }
  if(!func_800CF604(vehicle->owner)) {model->start=0.0f;model->angle=0.0f;continue;}
  if(model->start==0.0f) {model->start=vehicle->clock;continue;}
  if(!(2.0f<vehicle->clock-model->start))continue;
  segment=model->segment;index=model->point;
  for(j=0;j<5;j++)func_800B9F60(segment,index,&segment,&index);
  if(segment>=0) {
   delta.x=D_801407F0.children[segment].points[index].x-model->position.x;
   delta.y=D_801407F0.children[segment].points[index].y-model->position.y;
   delta.z=D_801407F0.children[segment].points[index].z-model->position.z;
  } else {
   delta.x=D_801407F0.points[index].x-model->position.x;
   delta.y=D_801407F0.points[index].y-model->position.y;
   delta.z=D_801407F0.points[index].z-model->position.z;
  }
  func_800A61B0(&delta,&view,model->basis);model->angle=func_8008C768(view.x,view.z);
 }
}
