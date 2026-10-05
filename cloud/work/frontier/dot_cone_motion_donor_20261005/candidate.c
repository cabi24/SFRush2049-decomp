/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;
/* N64-native offset views. Opaque bytes describe real record fields.
 * AnimateCone ancestry: historicalsource/rushtherock 845329d7, targets.c.
 * The clock qualifier preserves the four separately observed native reads;
 * accepted adjacent callbacks use the same qualifier. Original declaration
 * ownership and asynchronous-update behavior remain unestablished. */
typedef struct { f32 angular_velocity[3], velocity[3]; } Motion;
typedef struct { u8 unknown00[14]; s16 index, kind; u8 unknown12[2]; f32 basis[3][3]; f32 position[3]; u8 unknown44[40]; Motion *motion; } Object;
typedef struct { u8 unknown00[12]; Object *obj; f32 timer; } State;
extern s32 D_801170FC;
extern volatile f32 D_8002EB94;
extern f32 D_80121DDC[3];
typedef struct { u8 unknown00[18]; u16 flags; u8 unknown14[28]; } Kind;
extern Kind D_80117530[];
extern u8 D_80143FC8[];
extern void entity_transform_apply(void *,s32);
extern void entity_spawn_callback(s32,s32,s32);
extern void func_800AFA84(void *,void *);
extern void sound_position_set(f32 *,void *);
void func_8010E4E4(State *arg0, s16 arg1) {
    s16 i;
    Kind *kind;
    Object *obj;
    f32 acceleration_scale = 0.15f;
    f32 pos[3];
    Motion *motion;
    f32 *velocity;
    /* The zero-mode callback precedes the global pause check. */
    if (!arg1) { entity_transform_apply(arg0,1); return; }
    if (!D_801170FC) {
        obj=arg0->obj;
        motion=obj->motion;
        velocity=motion->velocity;
        for(i=0;i<3;i++) {
            velocity[i] += D_80121DDC[i]*acceleration_scale;
            obj->position[i] += velocity[i]+D_80121DDC[i]*acceleration_scale;
        }
        pos[0]=motion->angular_velocity[0]*D_8002EB94;
        pos[1]=motion->angular_velocity[1]*D_8002EB94;
        pos[2]=motion->angular_velocity[2]*D_8002EB94;
        sound_position_set(pos,obj->basis);
        arg0->timer-=D_8002EB94;
        if(arg0->timer<=0.0f) {
            kind = &D_80117530[obj->kind];
            if(kind->flags&0x2000) entity_spawn_callback(obj->index,0,0);
            func_800AFA84(D_80143FC8,obj);
            entity_transform_apply(arg0,1);
        }
    }
}
