/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Vehicle2056 {
 u8 prefix[196];float forces[4][3];u8 gap244[336];
 Vec3 tire_world[4];u8 gap628[156];float road_basis[4][9];u8 gap928[556];
 float compression[4],tire_compression[4],air_distance[4],air_velocity[4];
 int road_code[4];u16 visual_code[4];u8 gap1572[20];float inverse_dt;
 u8 gap1596[12];int state1608;u8 tail1612[444];
} Vehicle2056;
extern void camera_victory(Vehicle2056 *);
extern void entity_update(Vehicle2056 *,Vec3 *,Vec3 *,int *,float *,int);
extern void menu_audio_settings(Vehicle2056 *);
void func_800CF06C(Vehicle2056 *m)
{
 Vec3 ground;
 float last_air_distance;
 int i,j;
 for(i=0;i<3;i++) {
  for(j=1;j<4;j++)m->forces[j][i]=0.0f;
  m->forces[0][i]=0.0f;
 }
 camera_victory(m);
 m->state1608=0;
 for(i=0;i<4;i++) {
  entity_update(m,&m->tire_world[i],&ground,&m->road_code[i],m->road_basis[i],i);
  last_air_distance=m->air_distance[i];
  m->compression[i]=m->tire_compression[i]-ground.y;
  m->air_distance[i]=-m->compression[i];
  if(m->compression[i]>3)m->compression[i]=3.0f;
  else if(m->compression[i]<-3)m->compression[i]=-3.0f;
  m->tire_compression[i]=(m->compression[i]<0.0f)?0.0f:m->compression[i];
  if(m->compression[i]<0.0f)m->road_code[i]=m->visual_code[i]=8;
  m->air_velocity[i]=(m->air_distance[i]-last_air_distance)*m->inverse_dt;
  if(m->air_velocity[i]<-40)m->air_velocity[i]=-40.0f;
  else if(m->air_velocity[i]>40)m->air_velocity[i]=40.0f;
 }
 menu_audio_settings(m);
}
