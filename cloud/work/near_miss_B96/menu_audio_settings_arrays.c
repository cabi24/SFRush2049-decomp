/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Vehicle2056 {
 u8 prefix[244];Vec3 corners[4];u8 gap292[1328];float radius;
 u8 gap1624[316];Vec3 position;float basis[9];u8 gap1988[4];s16 active;
 u8 gap1994[32];s8 enabled;u8 tail[29];
} Vehicle2056;
extern s8 D_8017A4D8;
extern int gameplay_mode;
extern Vehicle2056 D_8014D280[];
extern void func_8009E820(Vec3 *,Vec3 *,float *);
extern void func_800A61B0(Vec3 *,Vec3 *,float *);
extern int func_800CEC8C(Vehicle2056 *,Vec3 *,float);
extern void menu_options_screen(Vehicle2056 *,Vehicle2056 *,Vehicle2056 *,Vec3 *,Vec3 *,float);
void menu_audio_settings(Vehicle2056 *vehicle)
{
 Vehicle2056 *other;
 float delta[3],world[3],local[3];
 Vec3 *point;
 float *value;
 float length;
 int offset;
 if(D_8017A4D8 && gameplay_mode!=1)return;
 if(!vehicle->enabled)return;
 if(gameplay_mode==2)return;
 for(other=vehicle+1;other<D_8014D280;other++) {
  if(!other->active || !other->enabled)continue;
  delta[0]=vehicle->position.x-other->position.x;
  delta[1]=vehicle->position.y-other->position.y;
  delta[2]=vehicle->position.z-other->position.z;
  length=0.0f;
  for(value=delta;value<delta+3;value++)length+=*value**value;
  if(length>(vehicle->radius+other->radius)*(vehicle->radius+other->radius))continue;
  for(offset=0,point=vehicle->corners;offset<(int)sizeof(vehicle->corners);offset+=(int)sizeof(Vec3),point++) {
   func_8009E820(point,(Vec3 *)world,vehicle->basis);
   world[0]=vehicle->position.x+world[0];world[1]=vehicle->position.y+world[1];world[2]=vehicle->position.z+world[2];
   world[0]-=other->position.x;world[1]-=other->position.y;world[2]-=other->position.z;
   func_800A61B0((Vec3 *)world,(Vec3 *)local,other->basis);
   if(func_800CEC8C(other,(Vec3 *)local,5.0f)) {
    menu_options_screen(vehicle,vehicle,other,(Vec3 *)delta,(Vec3 *)local,5.0f);return;
   }
  }
  for(offset=0,point=other->corners;offset!=(int)sizeof(other->corners);offset+=(int)sizeof(Vec3),point++) {
   func_8009E820(point,(Vec3 *)world,other->basis);
   world[0]=other->position.x+world[0];world[1]=other->position.y+world[1];world[2]=other->position.z+world[2];
   world[0]-=vehicle->position.x;world[1]-=vehicle->position.y;world[2]-=vehicle->position.z;
   func_800A61B0((Vec3 *)world,(Vec3 *)local,vehicle->basis);
   if(func_800CEC8C(vehicle,(Vec3 *)local,5.0f)) {
    menu_options_screen(vehicle,other,vehicle,(Vec3 *)delta,(Vec3 *)local,5.0f);return;
   }
  }
 }
}
