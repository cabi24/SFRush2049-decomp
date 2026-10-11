/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* engine_sound_sync (0x80098AE4, 1236 B) -- w20e best, NOT a match.
 * Unit (blob_unit --tag w20e, with aes_ctx.c --internal audio_effect_setup):
 *   FAIL engine_sound_sync: 270 of 309 words differ | norm 14 rows | frame 376/376, every sp home as retail.
 * Levers found in w20e (all measured, see RESULTS.md):
 *  - lock/unlock via the locked inlined helpers func_80091AF0/func_80091AF8;
 *  - projection is arcade BodyVector (rows . direction), no TransposeUV: retail never stores a transpose;
 *  - first listener loop indexed by i (strength-reduced by uopt to retail's per-source pointer init);
 *  - the four call arguments live in a local array out[4] (param, depth, pan, gain at 348..360):
 *    retail stores them and reloads them into $f22..$f28 before the IPA call (array = not a register candidate).
 * Shaping / hypotheses (disclosed):
 *  - pad1, pad2, pad3: UNUSED named locals = exact frame-layout residual. Retail frame 376; without them 360;
 *    3 slots (2 above direction at 316/320, 1 at 252 between basis and panValues). Identity unknown.
 *  - body_vector is defined but the code is written inline (inlined static = 16 B more frame); the static
 *    copy is kept only as the stub hypothesis for func_80098ADC (unverified).
 *  - audio_effect_setup takes `effect` in s0 (IPA register parameter) and clobbers s1/s2: it must be in the
 *    same group (provisional source aes_ctx.c) for retail's `jal; nop`.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct Effect {u8 pad0[16];s32 kind;u8 pad20[4];s8 state,delay;u8 persistent;u8 pad27[33];s32 voice;} Effect;
typedef struct Source {struct Source *next,*prev;u32 flags;f32 position[3];f32 radius,nearVolume,farVolume,parameter;s32 handle;} Source;
typedef struct Listener {struct Listener *next,*prev;u32 flags;f32 position[3];u8 pad24[12];f32 right[3],forward[3];} Listener;
typedef struct List {u8 flags[4];s32 count;Listener *head,*tail;} List;
extern List D_80146188;
extern Source *D_801461F0;
extern u8 D_80142728[];
void audio_effect_setup(Effect *);
Effect *func_80091BA8(s32);
s32 entity_state_check(s32);
f32 func_80098A54(f32 *);
void entity_transform_calc(s32,f32,f32,f32,f32);
s32 osRecvMesg(void *,void *,s32);
s32 osJamMesg(void *,void *,s32);
void func_80091AF0(void);
void func_80091AF8(void);

static void body_vector(const f32 uvec[3],f32 bodvec[3],const f32 uvs[][3])
{
    bodvec[0]=uvec[0]*uvs[0][0]+uvec[1]*uvs[0][1]+uvec[2]*uvs[0][2];
    bodvec[1]=uvec[0]*uvs[1][0]+uvec[1]*uvs[1][1]+uvec[2]*uvs[1][2];
    bodvec[2]=uvec[0]*uvs[2][0]+uvec[1]*uvs[2][1]+uvec[2]*uvs[2][2];
}

void engine_sound_sync(void)
{
    Source *source, *next;
    Effect *effect;
    f32 out[4];
    Listener *listener;
    f32 distance, total, totalPan, totalDepth, weight;
    s32 pad1, pad2;
    f32 direction[3], projected[3], basis[3][3];
    s32 pad3;
    f32 panValues[5], depthValues[5], gains[5];
    s32 i;

    func_80091AF0();
    source=D_801461F0;
    while (source) {
        next=source->next;
        effect=func_80091BA8(source->handle);
        if (effect->kind==2 && !effect->persistent && !effect->delay &&
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
                listener=listener->next;
            }
            out[3]=total<0.0f?0.0f:total>1.0f?1.0f:total;
            for (i=0; i<D_80146188.count; i++) {
                if (total!=0.0f) {
                    weight=gains[i]/total;
                    totalPan+=panValues[i]*weight;
                    totalDepth+=depthValues[i]*weight;
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
