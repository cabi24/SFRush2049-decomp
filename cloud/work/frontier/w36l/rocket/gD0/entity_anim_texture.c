/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul */
/* N64 game: entity_anim_texture, 0x80091874, 636 bytes.
 * Arcade/MAT3 ancestor: visual.c AnimateWingFlame. Adaptation tier.
 * Shows a car wing flame, scales its matrix, and applies visibility/translucency.
 * The genuine ZOID_FindMatrix boundary is essential to IDO's inline allocation.
 * N64 uses a separate side index and appearance hulk, and a pre-mode slot read.
 * All seven locals are used. No padding, dead guards, or forced registers.
 * The complete genuine group is required for private visibility-call registers.
 */
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
s32 model_bounds_calc(ModelObj *,s32);
s32 matrix_scale_apply(ModelObj *,s32,s32);
Pose *func_8008B3A0(s16 objnum) { return D_8012E700[objnum].pose; }
/* MAT3 helper/body evidence with N64 separate side-index lifetime and
 * trace-supported native homes for the used inv and pose locals. */
void entity_anim_texture(ModelObj *record,s16 mode) {
    s32 hulk;
    s32 inv;
    Pose *pose;
    Player952 *player;
    f32 scale;
    s32 index;
    s16 slot;
    slot=record->player;
    if(mode==0) {
        if(record->model>=0) model_data_load(record->model,1,15);
        record->active=0;
        record->model=-1;
        return;
    }
    index=(record->flags&1)?1:0;
    player=&player_array[slot];
    if(player->side[index].flags&0x10) {
        model_transform_setup(record->model,0,15);
        pose=func_8008B3A0(record->model);
        math_utility(&D_8011418C,pose);
        scale=index ? 8.0f*player->pose->values[4] : -8.0f*player->pose->values[4];
        if(scale<2.0f) scale=2.0f;
        pose->values[4]=scale;
    } else model_data_load(record->model,1,15);
    hulk=(player->flags&0x10)!=0;
    {
        inv=D_80140418 || (player->flags&8);
        if(model_bounds_calc(record,!hulk&&!player->disabled)) {
            matrix_scale_apply(record,player->quality>=2,player->shift);
            if(inv) D_8012E700[record->model].flags^=0x80000000U;
            else D_8012E700[record->model].flags&=0x7FFFFFFFU;
        }
    }
}
