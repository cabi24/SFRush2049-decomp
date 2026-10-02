/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Model952 {u8 prefix[8];f32 position[3];u8 opaque[932];} Model952;
typedef struct Record2056 {u8 prefix[38];s8 active;u8 opaque[2017];} Record2056;
extern Model952 player_array[];
extern Record2056 D_8014AA14[];
int steering_apply(s16 *index,f32 *position,f32 *radius,f32 *output) {
 Vec3 point,difference;
 f32 r,distance;
 int i=*index;
 r=*radius;
 if(D_8014AA14[i].active==0)return 0;
 point.x=position[0];point.y=position[1];point.z=position[2];
 difference.x=player_array[i].position[0]-point.x;
 difference.y=player_array[i].position[1]-point.y;
 difference.z=player_array[i].position[2]-point.z;
 r+=3.5f;
 distance=difference.x*difference.x+difference.y*difference.y+difference.z*difference.z;
 if(output)*output=distance-r*r;
 if(distance-r*r<=0.0f)return 1;
 return 0;
}
