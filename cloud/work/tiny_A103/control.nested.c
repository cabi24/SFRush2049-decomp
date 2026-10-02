/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef struct Slot64 {u8 opaque[20];union {void *node;struct {u16 high;s16 id;} half;} object;u8 tail[40];} Slot64;
extern Slot64 D_80139320[][13];
extern char *D_8011B438[];
extern u8 D_80140BDC;
extern void model_data_load(void *,int,s16);
extern void *func_800B24EC(char *,u16 *,s8,s8,int);
extern void func_8008D870(s16,void *,int);
extern void model_transform_setup(void *,int,s16);
void sfx_stop(s16 index,s16 lane,s16 resource)
{
    s16 mask;
    u16 result_id;
    if(resource < -1 || resource>=11)return;
    switch(lane) {
    case 0:mask=1;break;
    case 1:mask=2;break;
    case 2:mask=4;break;
    case 3:mask=8;break;
    }
    if(resource==-1)model_data_load(D_80139320[lane][index].object.node,0,mask);
    else {
        func_8008D870(D_80139320[lane][index].object.half.id,
            func_800B24EC(D_8011B438[resource],&result_id,0,(s8)(D_80140BDC-1),1),-1);
        model_transform_setup(D_80139320[lane][index].object.node,0,mask);
    }
}
