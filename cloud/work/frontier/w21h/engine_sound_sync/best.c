/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* engine_sound_sync (0x80098AE4, 1236 B) -- w21h best, NOT a match (8 normalised rows, frame 376 exact).
 * Unit: blob_unit --tag w21h score engine_sound_sync audio_effect_setup func_80098ADC --with aes.c --with best.c
 *       --internal audio_effect_setup --internal func_80098ADC --neighbours
 *       -> FAIL engine_sound_sync: 98 of 309 words differ (us.sh: words 117 ops 8 norm 8 | frame 376/376).
 * Levers kept from w20e: inlined lock/unlock func_80091AF0/AF8; BodyVector rows . direction; indexed loop 1;
 *   the four call arguments in a local array out[4]; audio_effect_setup takes effect in s0 (group-only).
 * New in w21h (each measured, RESULTS.md):
 *  - func_80098ADC (the locked caller-less stub right before this function) = inlined helper that clamps the
 *    total gain into out[3] through a pointer store. The pointer store keeps the list-count load of loop 2
 *    below the clamp (retail order) and fixes the a1/a2/a3 array bases; its 8-byte inline area replaces one
 *    w20e pad slot. HYPOTHESIS (helper identity); non-static so its stub keeps a symbol (w17j).
 *  - `kind = effect->kind` through a named local (former w20e pad slot): kind in v1, int temp ring as retail.
 *  - loop 2 without `weight`: `x * (gains[i] / total)` numbers the pan web before gains (a1/a2 as retail).
 * SHAPING (disclosed):
 *  - compiled-out compare `if (i == 5 || D_80146188.count == 1) {}` at the end of the listener loop: a
 *    code-free use of i (v1 in loop 2, as retail), of &D_80146188 (s5, as retail) and of the constants 5/1
 *    (s7 with no code, s8 = li 1 as retail; the array bases then fall out of the saved registers as retail).
 *    Identity unknown (an assert/debug macro that compiles to nothing is the natural candidate).
 *  - weight and pad3: UNUSED named locals = exact frame residual. Retail frame 376; without them 368; 2 slots
 *    (weight at 324 above direction, pad3 at 252 between basis and panValues).
 * Residual (8 norm rows): the FP temp ring starts two registers later in retail (out[0] load is $f8, ours $f4),
 *   which shifts every FP temp in loop 1 and the cross-product schedule; and the gains reload web is $f2 in
 *   retail ($f12 ours, tie with nearVolume at save 300 by web number; force p1:w155=c25,p1:w148=c26 fixes the
 *   registers but not the add/mul order).
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

void func_80098ADC(f32 *d, f32 v)
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
