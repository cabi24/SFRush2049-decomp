/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef union { s32 value; struct { s16 bank,id; } parts; } Handle;
typedef struct { Handle root; u32 gap4[3]; Handle secondary,body; u32 gap24; Handle details[9]; } Handles64;
typedef struct { u8 prefix[60]; void *texture0,*texture1; } Resource68;
typedef struct { u8 prefix[844]; s32 texture0,texture1; u8 gap852[12]; s32 color; u8 tail[952-868]; } Car952;
typedef struct { u8 prefix[1997]; u8 tune,variant,style; u8 tail[2056-2000]; } Vehicle2056;
typedef struct { u8 prefix,model; u8 gap2[5]; u8 type; } Selection8;
typedef struct { u8 *data; s32 other; } TextureGroup8;
typedef struct { void *first,*second; } TexturePair8;
extern s16 D_8014A108;
extern u16 D_801427C0[];
extern Handles64 D_80139320[];
extern u16 D_8016137A;
extern TextureGroup8 D_80151AE8[];
extern Resource68 D_8012E700[];
extern void *D_8011AE58;
extern s8 D_8014978C;
extern TexturePair8 D_8011ADC0[];
extern u16 D_801428F8,D_80142944;
extern s8 D_80156994;
extern Selection8 D_80153E88[];
extern s8 D_8011157C[];
extern void *D_80143F74[];
extern void *D_80143FA8;
extern Car952 D_80152818[];
extern s32 *D_8011AF90[];
extern Vehicle2056 D_8014A250[];
extern s32 save_slot_valid(u32,s16,s16,u32,s16,s16,s32);
extern void func_8008D870(s16,void *,s32);
extern void sound_bank_unload(s16,u32,u32,u32,u32);
extern void func_80092FE0(s16,void **);
void music_seq_load(s16 index,s16 palette,s32 mode)
{
    /* Arcade visuals.c CreateCar keeps these original unused declarations. */
    s16 j;
    float x,y,z;
    u32 refl_flag;
    Handles64 *handles;
    u16 *resources;
    s16 resource_index;
    u32 base,flags;
    s32 offset;
    u16 texture;
    Resource68 *model;
    TexturePair8 *pair;
    s32 variant,car;
    Car952 *state;
    Vehicle2056 *vehicle;
    s32 *values;
    offset=(D_8014A108==1 || mode==0)?0:5;
    base=(!mode?0x40004:0)|0x2000;
    resource_index=index*3;
    resources=&D_801427C0[resource_index];
    handles=&D_80139320[index];
    handles->root.value=save_slot_valid(resources[0],palette,-1,base|0x18,index,0,mode);
    handles->secondary.value=save_slot_valid(resources[1],palette,handles->root.value,base|0x80,index,-1,mode);
    handles->body.value=save_slot_valid(resources[2],palette,handles->root.value,base|0x284080,index,-1,mode);
    texture=D_8016137A;
    func_8008D870(handles->body.parts.id,D_80151AE8[texture>>10].data+(texture&0x3ff)*36,-1);
    model=&D_8012E700[handles->body.parts.id];
    if(!mode) {
        model->texture0=D_8011AE58;
        model->texture1=D_8011AE58;
    } else {
        pair=&D_8011ADC0[D_8014978C];
        model->texture0=pair->first;
        model->texture1=pair->second;
    }
    if(!mode) flags=(offset|0x40000|0x2004)|0x2000;
    else {
        variant=(D_8014A108==1)?0:5;
        flags=(variant|offset)|0x2000;
    }
    handles->details[0].value=save_slot_valid(D_801428F8,palette,handles->root.value,flags,index,3,mode);
    handles->details[1].value=save_slot_valid(D_801428F8,palette,handles->root.value,flags,index,4,mode);
    handles->details[2].value=save_slot_valid(D_801428F8,palette,handles->root.value,flags,index,5,mode);
    handles->details[3].value=save_slot_valid(D_801428F8,palette,handles->root.value,flags,index,6,mode);
    if(D_80156994 || D_8014978C>=6) {
        car=index%13;
        variant=(D_80153E88[car].type==6)?car+1:0;
        func_8008D870(handles->details[0].parts.id,D_80143F74[D_8011157C[variant*13+car]],-1);
    } else func_8008D870(handles->details[0].parts.id,D_80143FA8,-1);
    if(mode) {
        handles->details[4].value=save_slot_valid(D_80142944,palette,handles->root.value,3840,index,7,mode);
        handles->details[5].value=save_slot_valid(D_80142944,palette,handles->root.value,3840,index,8,mode);
        handles->details[6].value=save_slot_valid(D_80142944,palette,handles->root.value,3840,index,9,mode);
        handles->details[7].value=save_slot_valid(D_80142944,palette,handles->root.value,3840,index,10,mode);
        handles->details[8].value=save_slot_valid(D_80142944,palette,handles->root.value,3840,index,11,mode);
    } else {
        handles->details[4].value=-1;
        handles->details[5].value=-1;
        handles->details[6].value=-1;
        handles->details[7].value=-1;
        handles->details[8].value=-1;
    }
    if(mode) {
        state=&D_80152818[index];
        state->color=255;
        values=D_8011AF90[D_8014978C];
        state->texture0=*values;
        state->texture1=*values;
        vehicle=&D_8014A250[index];
        sound_bank_unload(index,D_80153E88[index].model,vehicle->tune,vehicle->variant,vehicle->style);
    }
    func_80092FE0(index,&D_8011AE58);
}
