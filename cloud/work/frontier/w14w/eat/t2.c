/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct ModelObj {u8 opaque0[6];s16 model,player;u16 opaque10;u32 flags,opaque16,active;} ModelObj;
typedef struct Pose {f32 values[9];} Pose;
typedef struct Model68 {u32 flags,opaque4;Pose *pose;u8 opaque12[56];} Model68;
typedef struct Side24 {u32 flags;u8 opaque4[20];} Side24;
typedef struct Player952 {u8 opaque0[232];u32 flags;u8 opaque236[432];Side24 side[2];u8 opaque716[144];s8 shift,quality,opaque862,disabled;u8 opaque864[32];Pose *pose;u8 opaque900[52];} Player952;
extern Player952 player_array[];
extern Model68 D_8012E700[];
extern Pose D_8011418C;
extern s8 D_80140418;
void model_data_load(s16,s32,s32);
void model_transform_setup(s16,s32,s32);
void math_utility(void *,void *);
s32 model_bounds_calc(s32,ModelObj *);
s32 matrix_scale_apply(ModelObj *,s32,s32);
void entity_anim_texture(ModelObj *record,s16 mode) {
    /*@{d1*/Player952 *player;
    Pose *pose;
    s32 side;
    s32 view;
    s32 enabled;
    f32 scale;/*@| s32 side; Player952 *player; Pose *pose; s32 view; s32 enabled; f32 scale; @|f32 scale; s32 enabled; s32 view; Pose *pose; s32 side; Player952 *player; @}*/
    /*@{sp*//*@|s32 spare0; @|s32 spare0; s32 spare1; @}*/
    /*@{pre*/ /*@|player=&player_array[record->player]; @}*/ /*@{m1*/if(mode==0) {/*@| if(0==mode) { @|if(!mode) { @}*/
        if(record->model>=0) model_data_load(record->model,1,15);
        record->active=0;
        record->model=-1;
        return;
    }
    /*@{p1*/player=&player_array[record->player];/*@| player=player_array+record->player; @}*/
    /*@{s1*/side=(record->flags&1)?1:0;/*@| side=(record->flags&1)!=0; @|side=record->flags&1; @}*/
    if(player->side[side].flags&0x10) {
        model_transform_setup(record->model,0,15);
        pose=D_8012E700[record->model].pose;
        math_utility(&D_8011418C,pose);
        /*@{sc*/if(side) scale=player->pose->values[4]*8.0f;
        else scale=player->pose->values[4]*-8.0f;/*@| if(side==0) scale=player->pose->values[4]*-8.0f; else scale=player->pose->values[4]*8.0f; @|if(side) scale=8.0f*player->pose->values[4]; else scale=-8.0f*player->pose->values[4]; @}*/
        /*@{lt*/if(scale<2.0f) scale=2.0f;/*@| if(!(scale>=2.0f)) scale=2.0f; @|if(scale<2.0f) scale=2.0f; @}*/
        pose->values[4]=scale;
    } else model_data_load(record->model,1,15);
    /*@{v1*/view=(D_80140418!=0)||((player->flags&8)!=0);/*@| view=(D_80140418!=0)||(player->flags&8); @|view=(player->flags&8)!=0; view=view||(D_80140418!=0); @}*/
    /*@{e1*/enabled=((player->flags&0x10)==0)&&(player->disabled==0);/*@| enabled=(player->disabled==0)&&((player->flags&0x10)==0); @|enabled=!(player->flags&0x10)&&!player->disabled; @}*/
    if(model_bounds_calc(enabled,record)) {
        matrix_scale_apply(record,player->quality>=2,player->shift);
        if(view) D_8012E700[record->model].flags^=0x80000000U;
        else D_8012E700[record->model].flags&=0x7FFFFFFFU;
    }
}
