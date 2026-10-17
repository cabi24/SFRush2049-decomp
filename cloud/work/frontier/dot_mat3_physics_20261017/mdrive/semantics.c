/* Host-side layout and branch/order regression test; never linked into N64 code. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "group.c"
#define CHECK(c) do { if (!(c)) { fprintf(stderr,"line %d: %s\n",__LINE__,#c); return 1; } } while (0)
#define OFF(type,field,n) _Static_assert(offsetof(type,field)==n,#field)
_Static_assert(sizeof(Tire92)==92,"tire stride");
_Static_assert(sizeof(Vehicle2056)==2056,"vehicle stride");
_Static_assert(sizeof(Model952)==952,"model stride");
_Static_assert(sizeof(Config8)==8,"configuration stride");
OFF(Vehicle2056,tires,1152); OFF(Vehicle2056,state,1600);
OFF(Vehicle2056,speed,1724); OFF(Vehicle2056,camera,1744);
OFF(Vehicle2056,selected,1820); OFF(Vehicle2056,mode,1840);
OFF(Vehicle2056,index,1990); OFF(Vehicle2056,wheels,2040);
OFF(Model952,setup,4); OFF(Model952,clear,856); OFF(Model952,active,857);
OFF(Model952,drift,868); OFF(Config8,camera,5); OFF(Config8,mode,7);
s8 D_80142760;
Model952 player_array[2];
Vehicle2056 D_8014A250[2];
Config8 D_80153E88[2];
int gameplay_mode;
volatile s16 D_801525F0;
static int event, bad, tested_index;
void car_setup_confirm(int index,float setup) {
 if(index!=tested_index || setup!=17.0f || event!=0)bad=1;
 event=1;
}
void func_800C4F68(int action,Vehicle2056 *vehicle,int flags) {
 if(action!=0 || flags!=0 || vehicle!=&D_8014A250[tested_index] || event!=0)bad=1;
 event=2;
}
void stunt_combo_display(Vehicle2056 *vehicle) {
 if(vehicle!=&D_8014A250[tested_index] || (event!=0 && event!=2))bad=1;
 event=event==2?3:4;
}
void func_800D5E64(Vehicle2056 *vehicle) {
 int i;
 if(vehicle!=&D_8014A250[tested_index] || (event!=3 && event!=4))bad=1;
 for(i=0;i<4;i++)if(vehicle->wheels[i] || vehicle->tires[i].a || vehicle->tires[i].b)bad=1;
 event+=4;
}
int main(void) {
 int ghost,active,action,mode,config,speed,index,i,cases=0;
 for(ghost=0;ghost<2;ghost++)for(active=-1;active<2;active++)
 for(action=0;action<3;action++)for(mode=1;mode<3;mode++)
 for(config=0;config<=6;config+=6)for(speed=43;speed<=44;speed++)
 for(index=0;index<2;index++) {
  Model952 *m=&player_array[index];Vehicle2056 *v=&D_8014A250[index];
  memset(player_array,0,sizeof player_array);memset(D_8014A250,0,sizeof D_8014A250);
  memset(D_80153E88,0,sizeof D_80153E88);
  D_80142760=ghost;gameplay_mode=mode;D_801525F0=0;tested_index=index;event=bad=0;
  m->active=active;m->clear=9;m->setup=17.0f;m->drift=19.0f;
  v->state=5;v->speed=speed;v->selected=9;v->camera=9;v->mode=9;v->index=index;
  D_80153E88[index].camera=3;D_80153E88[index].mode=config;
  for(i=0;i<4;i++) {v->wheels[i]=21.0f;v->tires[i].a=22.0f;v->tires[i].b=23.0f;}
  func_800E543C(action,index);CHECK(!bad);
  if(ghost && active==0 && action==2) {
   CHECK(event==1 && m->active==1 && v->state==0 && v->selected==1);
   CHECK(m->clear==9 && m->drift==19.0f && v->camera==9 && v->mode==9 && D_801525F0==0);
   for(i=0;i<4;i++)CHECK(v->wheels[i]==21.0f && v->tires[i].a==22.0f && v->tires[i].b==23.0f);
  } else {
   CHECK(event==(action==0?3:4)+((mode!=2 || index==0)?4:0));
   CHECK(m->active==(active<0?0:active) && v->state==5 && v->selected==0 && m->clear==0);
   CHECK(v->camera==(action==0?(mode==2?0:3):(mode==2?6:index+6)));
   CHECK(v->mode==((config==6 && action!=0)?(speed>=44?2:1):9));
   CHECK(m->drift==(config==6?0.0f:19.0f) && D_801525F0==1);
   for(i=0;i<4;i++)CHECK(v->wheels[i]==0.0f && v->tires[i].a==0.0f && v->tires[i].b==0.0f);
  }
  cases++;
 }
 printf("%d bounded host cases passed\n",cases);return 0;
}
