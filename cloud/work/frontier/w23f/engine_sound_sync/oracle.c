/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* ORACLE SOURCE (w23f), not a match candidate: w21h/engine_sound_sync/best.c's engine_sound_sync body VERBATIM,
 * plus static copies of the callees that retail calls with whole-program IPA conventions, so that a standalone
 * IDO compile (recomp replace.py mode ido) calls them correctly:
 *   audio_effect_setup     retail: effect in s0, s1/s2 clobbered unsaved (frontier show: register_params s0)
 *   entity_transform_calc  retail: x,y,z,w in $f22/$f24/$f26/$f28, $f20 clobbered (call site 0x80098F30..3C)
 *   func_80091AF0/AF8      retail: jr-ra stubs (lock/unlock inlined into every caller); the draft calls them
 *   func_80098ADC          retail: jr-ra stub; made static so the clamp is the draft's own code
 * The w22r verdict (DIFFERENT at frame 706) compiled best.c alone: the call to the retail entity_transform_calc
 * passed the four floats in a1..a3/stack while retail reads $f22..$f28, and audio_effect_setup got no s0. */
typedef signed char s8; typedef unsigned char u8;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct Effect {u8 pad0[16];s32 kind;u8 pad20[4];s8 state,delay;u8 persistent;u8 pad27[33];s32 voice;} Effect;
typedef struct Source {struct Source *next,*prev;u32 flags;f32 position[3];f32 radius,nearVolume,farVolume,parameter;s32 handle;} Source;
typedef struct Listener {struct Listener *next,*prev;u32 flags;f32 position[3];u8 pad24[12];f32 right[3],forward[3];} Listener;
typedef struct List {u8 flags[4];s32 count;Listener *head,*tail;} List;
extern List D_80146188;
extern Source *D_801461F0;
extern u8 D_80142728[];
Effect *func_80091BA8(s32);
s32 entity_state_check(s32);
f32 func_80098A54(f32 *);
s32 osRecvMesg(void *,void *,s32);
s32 osJamMesg(void *,void *,s32);


/* ---- oracle-only static copies of the callees whose retail conventions are IPA-specific ---- */
typedef struct ALink {struct ALink *next, *prev;} ALink;
typedef struct AList {u8 indirect, doubly, pad[2];u32 count;ALink *head, *tail;} AList;
typedef struct AOwner { ALink link; s8 active,queued; } AOwner;
typedef struct AEffect { ALink link; AList *list; u32 handle; s32 kind,mode; s8 state,delay; u8 persistent,priority; u32 rest[9]; AOwner *owner; } AEffect;
void func_80091FBC(AList *list, ALink *object, ALink *before);
void func_8009211C(AList *list, ALink *object);
extern u32 D_80146104;
extern AList D_801461E8,D_801461B0,D_80143FF8;
/* audio_effect_setup (0x80095800): retail takes effect in s0 and clobbers s1/s2 unsaved; body = w21h aes.c */
static void audio_effect_setup(Effect *input) {
    AEffect *effect=(AEffect *)input;
    AOwner *owner;
    ALink *pending;
    effect->handle &= D_80146104;
    effect->kind=0;
    if (effect->mode==1) {
        if (effect->owner) {
            owner=effect->owner;
            pending=&owner->link;
            if (owner->queued) {
                func_8009211C(&D_801461E8,&owner->link);
                ((AOwner *)pending)->queued=0;
            }
            func_80091FBC(&D_801461B0,pending,D_801461B0.head);
            ((AOwner *)pending)->active=1;
            effect->owner=0;
        }
    }
    if (!effect->persistent) {
        func_8009211C(effect->list,&effect->link);
        func_80091FBC(&D_80143FF8,&effect->link,D_80143FF8.head);
        effect->list=&D_80143FF8;
    }
}
/* entity_transform_calc (0x80091CA4): retail takes x,y,z,w in $f22/$f24/$f26/$f28 and clobbers $f20;
 * body = locked src/blob/groups/codex_transform_b109/group.c */
typedef signed short s16;
typedef struct TEntity {
    u8 pad0[12]; s32 id; s32 state; u8 pad14[6];
    u8 msgCount; u8 pad1B[9]; f32 x,y,z,w; u8 pad34[16];
} TEntity;
typedef struct TMsg {
    s16 id; s8 type,used;
    union { struct { f32 x,y,z,w; } value; TEntity *target; } payload;
    TEntity *ent;
} TMsg;
extern s32 D_801427A8;
TMsg *func_80091B00(void);
static void entity_transform_calc(s32 h,f32 x,f32 y,f32 z,f32 w) {
    TMsg *m=0;
    TEntity *e=(TEntity *)func_80091BA8(h);
    if(e!=0){
        if((x!=-2.0f && x!=e->x) || (y!=-2.0f && y!=e->y) ||
           (z!=-2.0f && z!=e->z) || (w!=-2.0f && w!=e->w)) {
            m=func_80091B00();
            m->type=4;
            m->ent=e;
            e->msgCount++;
            if(x!=-2.0f && x!=e->x)m->payload.value.x=x; else m->payload.value.x=-2.0f;
            if(y!=-2.0f && y!=e->y)m->payload.value.y=y; else m->payload.value.y=-2.0f;
            if(z!=-2.0f && z!=e->z)m->payload.value.z=z; else m->payload.value.z=-2.0f;
            if(w!=-2.0f && w!=e->w)m->payload.value.w=w; else m->payload.value.w=-2.0f;
        }
    }
    if(m!=0)osJamMesg(&D_801427A8,m,0);
}
/* func_80091AF0/AF8: retail 2-word stubs (deleted statics, inlined); the draft calls them */
static void func_80091AF0(void) { osRecvMesg(D_80142728, 0, 1); }
static void func_80091AF8(void) { osJamMesg(D_80142728, 0, 0); }
/* ---- end of oracle-only copies ---- */

static void body_vector(const f32 uvec[3],f32 bodvec[3],const f32 uvs[][3])
{
    bodvec[0]=uvec[0]*uvs[0][0]+uvec[1]*uvs[0][1]+uvec[2]*uvs[0][2];
    bodvec[1]=uvec[0]*uvs[1][0]+uvec[1]*uvs[1][1]+uvec[2]*uvs[1][2];
    bodvec[2]=uvec[0]*uvs[2][0]+uvec[1]*uvs[2][1]+uvec[2]*uvs[2][2];
}

static void func_80098ADC(f32 *d, f32 v)
{
    *d = v<0.0f?0.0f:v>1.0f?1.0f:v;
}

void engine_sound_sync(void)
{
    Source *source, *next;
    Effect *effect;
    f32 out[4];
    Listener *listener;
    f32 distance, total, totalPan, totalDepth, weight;
    s32 i, kind;
    f32 direction[3], projected[3], basis[3][3];
    s32 pad3;
    f32 panValues[5], depthValues[5], gains[5];

    func_80091AF0();
    source=D_801461F0;
    while (source) {
        next=source->next;
        effect=func_80091BA8(source->handle);
        kind=effect->kind;
        if (kind==2 && !effect->persistent && !effect->delay &&
            !entity_state_check(effect->voice)) {
            audio_effect_setup(effect);
        } else if (!effect->state) {
            i=0;
            total=0.0f;
            totalPan=0.0f;
            totalDepth=0.0f;
            out[0]=source->parameter;
            listener=D_80146188.head;
            while (listener) {
                direction[0]=source->position[0]-listener->position[0];
                direction[1]=source->position[1]-listener->position[1];
                direction[2]=source->position[2]-listener->position[2];
                distance=func_80098A54(direction);
                basis[2][0]=listener->right[0];
                basis[2][1]=listener->right[1];
                basis[2][2]=listener->right[2];
                basis[1][0]=listener->forward[0];
                basis[1][1]=listener->forward[1];
                basis[1][2]=listener->forward[2];
                basis[0][0]=basis[1][1]*basis[2][2]-basis[2][1]*basis[1][2];
                basis[0][1]=basis[1][2]*basis[2][0]-basis[2][2]*basis[1][0];
                basis[0][2]=basis[1][0]*basis[2][1]-basis[2][0]*basis[1][1];
                projected[0]=direction[0]*basis[0][0]+direction[1]*basis[0][1]+direction[2]*basis[0][2];
                projected[1]=direction[0]*basis[1][0]+direction[1]*basis[1][1]+direction[2]*basis[1][2];
                projected[2]=direction[0]*basis[2][0]+direction[1]*basis[2][1]+direction[2]*basis[2][2];
                panValues[i]=projected[0];
                depthValues[i]=projected[2];
                if (source->radius>0.0f) {
                    gains[i]=source->nearVolume-(source->nearVolume-source->farVolume)*(distance/source->radius);
                    if (gains[i]<0.0f) gains[i]=0.0f;
                } else gains[i]=source->nearVolume;
                total+=gains[i];
                panValues[i] = panValues[i] - (panValues[i] * gains[i]);
                i++;
                if (i == 5 || D_80146188.count == 1) {}
                listener=listener->next;
            }
            func_80098ADC(&out[3],total);
            for (i=0; i<D_80146188.count; i++) {
                if (total!=0.0f) {
                    totalPan+=panValues[i]*(gains[i]/total);
                    totalDepth+=depthValues[i]*(gains[i]/total);
                }
            }
            out[2]=totalPan < -1.0f ? -1.0f : totalPan>1.0f ? 1.0f : totalPan;
            out[1]=totalDepth < -1.0f ? -1.0f : totalDepth>1.0f ? 1.0f : totalDepth;
            entity_transform_calc(source->handle,out[3],out[2],out[1],out[0]);
        }
        source=next;
    }
    func_80091AF8();
}
