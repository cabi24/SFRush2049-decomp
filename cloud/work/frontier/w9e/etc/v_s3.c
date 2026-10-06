/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { f32 x,y,z; } Vec3;
typedef struct Node112 Node112;
typedef struct { u8 prefix[64]; u32 flags; } Resource68;
typedef struct {
    u8 prefix[16]; u32 flags; s16 count,action;
    u32 field24; Resource68 *resources; u32 field32;
} Scene36;
struct Node112 {
    Node112 *next; u8 flags; u8 gap5[3]; void *key;
    s16 field12,resource,metadata; u16 field18;
    Vec3 position; u8 gap32[36]; Vec3 normal;
    u8 gap80[8]; s16 texture; u8 gap90[11];
    s8 index; u8 gap102[6]; u8 *state;
};
typedef struct {
    const char *name; u32 field4; void (*callback)(Node112 *);
    void *animation; s16 kind; u16 flags; u8 gap20[2];
    s8 category,element; f32 value; u8 tail[20];
} Metadata48;
typedef struct { s32 object[3]; u8 gap12[122]; s16 active; } Slot136;
typedef struct { void *record; f32 matrix[9]; Vec3 position,velocity; } Matrix64;
typedef struct Animation24 {
    struct Animation24 *next; s16 state; u16 field6; u32 field8;
    Node112 *node; f32 time; void *data;
} Animation24;
typedef struct { u8 prefix[16]; Node112 *head; } SceneManager20;
extern u32 D_801174B4;
extern s8 D_80156994;
extern s32 D_801493D4;
extern Scene36 *D_801493D0;
extern s32 D_8014A110;
extern s8 D_80152570,D_80150DD0;
extern s8 D_80150E28[];
extern u8 D_80117510[];
extern u8 *D_80150E68[],*D_80150E98[],*D_80118DDC[];
extern s32 D_80150F78,D_80150F80;
extern Matrix64 *D_80150F38;
extern s32 D_801497EC;
extern void **D_801497C0;
extern SceneManager20 D_80143FC8;
extern Metadata48 D_80117530[];
extern u16 D_801427C0[];
extern Resource68 D_8012E700[];
extern Animation24 *D_801391F0;
extern f32 D_8011418C[];
extern u8 D_80118C10[];
extern Vec3 D_80118DFC;
extern void *audio_dma_sync(void *,u32);
extern void func_800B2CB4(s16);
extern void listener_position_set(s16);
s32 transmission_ratio_get(Scene36 *,s16,Vec3 *,void *,s32,s32,s32);
extern void *func_8008E3C0(void *);
extern void math_utility(f32 *,f32 *);
extern Animation24 *func_80090284(void);
extern void func_800AB750(s8,Vec3 *,Vec3 *,Vec3 *);
/* The original direct overlay call supplies this single signed-byte input.
   Its implementation and overlay semantics are intentionally unclaimed. */
extern void func_8039133C(s32);
void func_800AB7D0(Node112 *node,Metadata48 *metadata)
{
    s32 category,index,j;
    Matrix64 *matrix;
    Scene36 *scene;
    Slot136 *slot;
    category=metadata->category;
    if((s32)(D_801174B4<<9)>=0) {
        if(metadata->flags&8) {
            node->state=D_80150E98[category];
            if(!(node->flags&16) && category==4) {
                scene=&D_801493D0[node->index];
                *(Scene36 **)node->state=scene;
                if((*(Scene36 **)node->state)->flags&16) node->flags&=~8;
            }
            D_80150E98[category]+=D_80117510[category];
        } else if(category!=7) {
            node->state=D_80118DDC[category]+D_80117510[category]*metadata->element;
        }
        if(metadata->kind==4) {
            node->index=D_80150F80++;
            matrix=&D_80150F38[node->index];
            math_utility(D_8011418C,matrix->matrix);
            for(j=0;j<3;j++) {
                ((f32 *)&matrix->velocity)[j]=0.0f;
                ((f32 *)&matrix->position)[j]=0.0f;
            }
            index=(s32)D_80117530[node->metadata].value;
            matrix->record=D_80118C10+index*32;
        }
    } else if((metadata->flags&8) && category==5) {
        slot=(Slot136 *)D_80150E68[5];
        for(index=0;index<16;index++,slot++) {
            if(!slot->active && slot->object[0]==-1 && slot->object[1]==-1 && slot->object[2]==-1) break;
        }
        slot->active=1;
        node->state=D_80150E98[category]+D_80117510[category]*index;
    }
}
void engine_torque_calc(Node112 *node)
{
    Metadata48 *metadata;
    Animation24 *animation;
    Vec3 transform;
    metadata=&D_80117530[node->metadata];
    func_800AB7D0(node,metadata);
    if(node->texture!=-1) *(u16 *)((u8 *)&D_8012E700[node->resource]+20)=D_801427C0[node->texture];
    if(metadata->flags&2) {
        animation=func_80090284();
        if(animation) {
            animation->state=0;
            animation->node=node;
            animation->time=0.0f;
            animation->data=metadata->animation;
            animation->next=D_801391F0;
            D_801391F0=animation;
            if((metadata->flags&4) && metadata->callback) metadata->callback(node);
        }
    }
    if(metadata->flags&0x4000) {
        if(!(node->flags&16)) metadata->callback(node);
        else {
            transform=D_80118DFC;
            func_800AB750(node->index,&transform,&node->normal,&node->position);
        }
    }
}
void engine_sound_update(void)
{
    Vec3 zero;
    s32 i,j,offset,enabled,other,found;
    Scene36 *scene;
    Resource68 *resource;
    Node112 *node;
    void *key;
    Slot136 *slot;
    if((D_801174B4&8) && !D_80156994) return;
    if(D_801493D4) {
        zero.z=zero.x=zero.y=0.0f;
        for(i=0,offset=0;i<D_801493D4;i++,offset+=36) {
            scene=(Scene36 *)((u8 *)D_801493D0+offset);
            if(D_8014A110==2 && scene->action>0) {
                found=0;
                resource=scene->resources;
                for(j=0;j<scene->count;j++,resource++) if(resource->flags&0x01000000) {found=1;break;}
                if(!found) { if(scene->action>0) func_800B2CB4(scene->action); continue; }
            }
            if(((scene->flags&4) && (scene->flags&8)) ||
               (D_80152570 && (scene->flags&8)) || (!D_80152570 && (scene->flags&4))) {
                enabled=(scene->flags&0x8000)?0:1;
                other=(scene->flags&16)?0:1;
                for(j=0;j<scene->count;j++) {
                    if(D_80150DD0) {
                        scene->resources[j].flags&=~0x1000;
                        scene=(Scene36 *)((u8 *)D_801493D0+offset);
                    }
                    if(scene->resources[j].flags&1) {
                        transmission_ratio_get(scene,-1,&zero,0,i,enabled,other);
                        scene=(Scene36 *)((u8 *)D_801493D0+offset);
                    }
                }
                if(D_80150DD0 && scene->action>0) listener_position_set(scene->action);
            } else if(scene->flags&0x800) func_800B2CB4(scene->action);
        }
    }
    if(!D_80150DD0) {
        for(i=0;i<8;i++) if(D_80150E28[i]) D_80150E68[i]=audio_dma_sync(0,D_80150E28[i]*D_80117510[i]);
        if(D_80150F78) D_80150F38=audio_dma_sync(0,D_80150F78*64);
        for(i=0;i<D_801497EC;i++) {
            key=D_801497C0[i];
            node=D_80143FC8.head;
            while(node) {
                if((node->flags&8) && node->key==key) {D_801497C0[i]=node; break;}
                node=node->next;
            }
            if(key==D_801497C0[i]) D_801497C0[i]=0;
        }
    }
    if(D_8014A110==6) {
        for(offset=0;offset<2176;offset+=544) {
            ((Slot136 *)(D_80150E68[5]+offset))[0].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[2]=-1;
        }
    }
    D_80150F80=0;
    for(i=0;i<8;i++) if(D_80150E28[i]) D_80150E98[i]=D_80150E68[i];
    node=D_80143FC8.head;
    while(node) {engine_torque_calc(node); node=node->next;}
    if(D_8014A110==6) func_8039133C(D_80150DD0);
}

s32 transmission_ratio_get(Scene36 *scene,s16 a,Vec3 *v,void *p,s32 i,s32 e,s32 o)
{
    Node112 *n;
    n=func_8008E3C0(&D_80143FC8);
    if(n==0) return 1;
    n->index=i;
    engine_torque_calc(n);
    return 1;
}
