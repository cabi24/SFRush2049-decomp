/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;
/* Inferred offset views, not established complete game class definitions.
 * Offsets below describe the 32-bit N64 ABI; host test pointers may be wider. */
typedef struct { f32 pos[3], vel[3]; } Model;
typedef struct { u8 unknown00[14]; s16 index, kind; u8 unknown12[2]; f32 position[3]; u8 unknown20[24]; f32 accumulated[3]; u8 unknown44[40]; Model *model; } Object;
typedef struct { u8 unknown00[12]; Object *obj; f32 timer; } State;
extern s32 D_801170FC;
extern f32 D_8002EB94, D_801249CC, D_80121DDC[3];
typedef struct { u8 unknown00[18]; u16 flags; u8 unknown14[28]; } Kind;
extern Kind D_80117530[];
extern u8 D_80143FC8[];
extern void entity_transform_apply(void *,s32);
extern void entity_spawn_callback(s32,s32,s32);
extern void func_800AFA84(void *,void *);
extern void sound_position_set(f32 *,void *);
void func_8010E4E4(State *arg0, s16 arg1) {
    Object *obj;
    Model *model;
    f32 *velocity;
    f32 pos[3];
    s16 i;
    f32 acceleration_scale;
    /* The zero-mode callback precedes the global pause check. */
    if (!arg1) { entity_transform_apply(arg0,1); return; }
    if (!D_801170FC) {
        obj=arg0->obj;
        model=obj->model;
        velocity=model->vel;
        /* Native snapshots this scale once, outside the three-axis loop. */
        acceleration_scale=D_801249CC;
        for(i=0;i<3;i++) {
            velocity[i] += D_80121DDC[i]*acceleration_scale;
            obj->accumulated[i] += velocity[i]+D_80121DDC[i]*acceleration_scale;
        }
        pos[0]=model->pos[0]*D_8002EB94;
        pos[1]=model->pos[1]*D_8002EB94;
        pos[2]=model->pos[2]*D_8002EB94;
        sound_position_set(pos,obj->position);
        arg0->timer-=D_8002EB94;
        if(arg0->timer<=0.0f) {
            if(D_80117530[obj->kind].flags&0x2000) entity_spawn_callback(obj->index,0,0);
            func_800AFA84(D_80143FC8,obj);
            entity_transform_apply(arg0,1);
        }
    }
}
