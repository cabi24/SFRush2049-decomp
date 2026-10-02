/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Record24 {u8 prefix[6];s16 handle,vehicle;u8 gap10[10];void *state;} Record24;
typedef struct Model952 {
 u8 prefix[116];Vec3 wheels[4];u8 gap164[68];u32 flags;
 u8 gap236[624];s8 selected,selection_mode,unknown,inactive;u8 tail[88];
} Model952;
typedef struct Vehicle2056 {
 u8 prefix[8],type;u8 gap9[1723];s16 state;u8 gap1734[166];float heights[4];
 u8 gap1916[99];s8 inactive;u8 tail[40];
} Vehicle2056;
typedef struct Vertex20 {Vec3 point;u16 s,t;u32 color;} Vertex20;
typedef struct Record88 {u16 kind,flags;u8 header[4];Vertex20 vertex[4];} Record88;
typedef struct Width16 {float first,second,third,fourth;} Width16;
extern Model952 player_array[];
extern Vehicle2056 D_8014A250[];
extern Record88 D_8015B268[];
extern s8 D_80140418;
extern Width16 D_8011F914[];
extern u8 D_8011AD8C[4];
extern float D_80123938;
extern float fabsf(float);
#pragma intrinsic (fabsf)
extern void func_8008D0C0(Record88 *);
extern void func_8008C074(Record88 *,int,Vec3 *,u16,u8 *,u16,int);
void entity_process_main(Record24 *record,s16 enabled)
{
 Model952 *model=&player_array[record->vehicle];
 Vehicle2056 *vehicle=&D_8014A250[record->vehicle];
 Vec3 points[4];float heights[4];
 float difference_a,difference_b,difference_c,difference_d,average;
 float height,offset,first,second,delta_a,delta_b;
 Width16 *width;
 s16 i,j;
 Record88 *destination;
 if(!enabled) {
  if(record->handle>=0)func_8008D0C0(&D_8015B268[record->handle]);
  record->state=0;record->handle=-1;return;
 }
 difference_a=vehicle->heights[3]-vehicle->heights[1];
 difference_b=vehicle->heights[2]-vehicle->heights[0];
 difference_c=vehicle->heights[1]-vehicle->heights[0];
 difference_d=vehicle->heights[3]-vehicle->heights[2];
 if(fabsf(difference_a)>20.0f || fabsf(difference_b)>20.0f ||
    fabsf(difference_c)>20.0f || fabsf(difference_d)>20.0f ||
    model->inactive || vehicle->state>=0 || (model->flags&8) ||
    D_80140418 || vehicle->inactive) {
  D_8015B268[record->handle].flags|=0x8000;return;
 }
 average=(float)0;
 for(i=0;i<4;i++) {
  height=vehicle->heights[i];heights[i]=height;
  if(height<0.0f)heights[i]=0.0f;
  average+=height;
 }
 average*=0.25f;
 for(i=0;i<4;i++) {
  if(i<2)j=i;else j=5-i;
  height=heights[j];
  if(height>10.0f)offset=2.0f;else offset=height*D_80123938+1.0f;
  points[i].x=model->wheels[j].x;
  points[i].y=model->wheels[j].y-height+offset;
  points[i].z=model->wheels[j].z;
 }
 if(model->flags&0x10)width=&D_8011F914[13];else width=&D_8011F914[vehicle->type];
 first=width->first;second=width->second;
 for(i=0;i<3;i++) {
  delta_a=(((float *)points)[i]-((float *)points)[i+3])*first;
  delta_b=(((float *)points)[i+9]-((float *)points)[i+6])*second;
  ((float *)points)[i]+=delta_a;
  ((float *)points)[i+3]-=delta_a;
  ((float *)points)[i+9]+=delta_b;
  ((float *)points)[i+6]-=delta_b;
 }
 first=width->third;second=width->fourth;
 for(i=0;i<3;i++) {
  difference_a=((float *)points)[i]-((float *)points)[i+9];
  difference_b=((float *)points)[i+3]-((float *)points)[i+6];
  ((float *)points)[i]+=difference_a*first;
  ((float *)points)[i+9]-=difference_a*second;
  ((float *)points)[i+3]+=difference_b*first;
  ((float *)points)[i+6]-=difference_b*second;
 }
 destination=&D_8015B268[record->handle];destination->flags&=0x7FFF;
 height=average*8.0f;
 if(height<20.0f)height=20.0f;else if(height>255.0f)height=255.0f;
 D_8011AD8C[3]=(u8)(255.0f-height);
 if(D_8011AD8C[3]>192)D_8011AD8C[3]=192;
 if(model->selected>=0 && (model->selection_mode==0 || model->selection_mode==1))
  destination->flags&=~(1<<model->selected);
 else destination->flags|=0xF;
 func_8008C074(destination,4,points,0,D_8011AD8C,0,0);
}
