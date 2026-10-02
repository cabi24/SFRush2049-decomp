/* IDO flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct { u8 other0[14]; s16 owner,table; u8 other18[62]; s16 current; u8 other82[6]; s16 start,end; } Model;
typedef struct AnimationNode {
    struct AnimationNode *next; s16 counter,handle; u8 other8[4]; Model *model; f32 timer; s32 state;
} AnimationNode;
typedef struct { u8 other0[18]; u16 flags; u8 other20[28]; } Table48;
typedef struct { u8 other0[20]; u16 value; u8 other22[46]; } Node68;
extern Table48 D_80117530[];
extern Node68 D_8012E700[];
extern u16 D_801427C0[];
extern s32 D_801170FC;
extern f32 D_8002EB94;
extern void entity_transform_apply(AnimationNode *,s32);
extern void entity_spawn_callback(s16,s32,s32);
void audio_channel_setup(AnimationNode *node,s16 enable)
{
    Model *model;
    Table48 *table;
    s16 index;
    if(enable==0) {
remove:
        entity_transform_apply(node,1);
        return;
    }
    if(D_801170FC!=0) return;
    node->timer-=D_8002EB94;
    model=node->model;
    if(node->timer>0.0f) return;
    node->counter++;
    node->timer=0.0625f;
    if(node->counter>=model->end) {
        table=&D_80117530[model->table];
        if(table->flags & 0x1000) {
            goto remove;
        }
        if(table->flags & 0x2000) {
            entity_spawn_callback(model->owner,0,0);
            goto remove;
        }
        node->counter=0;
    }
    index=model->start+node->counter;
    if(index!=model->current) {
        D_8012E700[model->owner].value=D_801427C0[index];
        model->current=index;
    }
}
