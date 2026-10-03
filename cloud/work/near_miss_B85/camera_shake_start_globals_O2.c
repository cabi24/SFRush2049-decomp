/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Resource32 {u8 opaque[20];void *bank,*data;u32 flags;} Resource32;
typedef struct Row20 {char *name;u8 opaque[5];s8 kind;u8 gap[2];f32 time;void *data;} Row20;
typedef struct Item12 {char *name;Resource32 *resource;void *output;} Item12;
typedef struct Sequence20 {s16 count,maximum,unused,current;f32 time,default_time;Item12 *items;} Sequence20;
extern Row20 *D_8011A840[];
extern Sequence20 *D_8011A31C[];
extern Item12 D_8011905C[];
extern u32 state_word_a;
extern s8 D_80156994,D_80114650,D_8014978C;
extern u8 D_80140BDC;
extern f32 D_80123E58;
extern Resource32 *func_800B24EC(char *,u16 *,s8,s8,int),*sound_bank_load(char *,u16 *,s8,s8,int);
extern void *func_800BDA24(void *);
void camera_shake_start(void)
{
    Row20 *row;
    Sequence20 *sequence;
    Resource32 *resource;
    int i;
    u16 id;
    if((state_word_a&8) && !D_80156994)return;
    row=D_8011A840[D_8014978C];
    if(row) {
        while(row->name) {
            row->time=D_80123E58;
            if(row->kind>=9)row->data=func_800B24EC(row->name,&id,0,D_80140BDC-1,1)->data;
            else {
                resource=sound_bank_load(row->name,&id,0,D_80140BDC-1,1);
                if(resource)row->data=resource->bank;
            }
            row++;
        }
    }
    if(D_80114650)return;
    sequence=D_8011A31C[D_8014978C];
    if(!sequence)return;
    while(sequence->count) {
        for(i=0;i<sequence->count;i++) {
            if(D_80156994 || D_8014978C>=6) {
                sequence->items[i].resource=func_800B24EC(sequence->items[i].name,&id,0,D_80140BDC-1,1);
            } else if(!D_80156994 && D_8014978C>=0 && D_8014978C<6 && sequence->items!=D_8011905C) {
                sequence->items[i].resource=func_800B24EC(sequence->items[i].name,&id,0,D_80140BDC-1,1);
            } else continue;
            resource=sequence->items[i].resource;
            if(resource->flags&0x08000000)sequence->items[i].output=resource->data;
            else sequence->items[i].output=func_800BDA24(resource->data);
            sequence->time=sequence->default_time;
            sequence->current=sequence->maximum;
        }
        sequence++;
    }
}
