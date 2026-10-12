/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* engine_sound_sync (0x80098AE4, 1236 B) + func_80098ADC (0x80098ADC, locked 2-word stub given a body).
 * w23f: EQUAL in the whole-program unit with audio_effect_setup_b132.c (this group):
 *   blob_unit --tag w23f score engine_sound_sync audio_effect_setup func_80098ADC --with audio_effect_setup_b132.c
 *     --with engine_sound_sync.c --internal audio_effect_setup --internal func_80098ADC --neighbours
 * PACKAGING: func_80091AF0/func_80091AF8 (locked jr-ra stubs of codex_heap_release_a25, the slot-queue lock and
 *   unlock) are defined again at the END of this file as group context, same bodies as the locked copies. Their
 *   position sets the source line of the inlined `la a0,D_80142728`, which as1 uses to break scheduling ties: as
 *   separate files (73 "func_80091AF0.c" line 14) the lui stays after the register saves (16 words differ in
 *   score.py group); defined before the function it is hoisted above `sw ra`; after the function it lands at +8
 *   as in retail (and as in the whole-program unit, where the inlined code carries the call-site line).
 * Semantics: per frame, under the slot-queue lock (inlined func_80091AF0/AF8), walk the active sound sources
 * (D_801461F0); an effect of kind 2 that is not persistent, not delayed and whose voice has finished is released
 * (audio_effect_setup); otherwise, for every listener (D_80146188 list, at most 5), project source - listener onto
 * the listener's basis (forward, right and their arcade crossprod), compute a distance-attenuated gain
 * (near - (near - far) * d / radius, clamped at 0) and accumulate gain-weighted pan/depth; then send gain (clamped
 * 0..1 by func_80098ADC), pan, depth (clamped -1..1) and the source parameter to entity_transform_calc.
 * History: w17h/w20e/w21h drafts (see w21h/RESULTS.md for the out[4] array, kind local, clamp helper levers).
 * w23f levers (each measured in the unit, cloud/work/frontier/w23f/RESULTS.md):
 *  - cross product in the arcade vecmath.h crossprod(a,b,r) operand order (a = basis[1], b = basis[2]);
 *  - projection written basis-first (`basis[k][j]*direction[j]`), as the arcade BodyVector-style dot products;
 *  - listener loop as a for loop (`listener = listener->next, i++` in the increment), `weight = gains[i]` as the
 *    pan multiplier (a plain variable is the second operand) and `total += gains[i]`.
 * HYPOTHESIS (helper identity): func_80098ADC = the inlined gain clamp writing through a pointer (w21h).
 * SHAPING (disclosed; identity unknown, both compile to no code):
 *  - `if (gains[i] != 0.0f) {}` before the radius test: an early code-free read of gains[i] that numbers the
 *    gains[i] reload expression (PRE bit) before the nearVolume load, so the reload web wins the save-300 tie
 *    against nearVolume and takes $f2 (retail), nearVolume $f12 (retail);
 *  - `if (i == 5 || D_80146188.count == 1) {}` at the end of the listener loop (w21h): code-free uses that give
 *    retail's s5 = &D_80146188, s8 = 1 and the unsaved array bases.
 *  An assert/debug macro that compiles to nothing is the natural candidate for both.
 * UNUSED LOCAL (exact frame residual): pad3, 1 slot at sp+252 between basis and panValues. Retail frame 376;
 *   without it the frame still rounds to 376 but panValues/depthValues/gains move up 4 bytes (6 words differ).
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
            for (listener=D_80146188.head; listener; listener=listener->next, i++) {
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
                basis[0][0]=basis[1][1]*basis[2][2]-basis[1][2]*basis[2][1];
                basis[0][1]=basis[1][2]*basis[2][0]-basis[1][0]*basis[2][2];
                basis[0][2]=basis[1][0]*basis[2][1]-basis[1][1]*basis[2][0];
                projected[0]=basis[0][0]*direction[0]+basis[0][1]*direction[1]+basis[0][2]*direction[2];
                projected[1]=basis[1][0]*direction[0]+basis[1][1]*direction[1]+basis[1][2]*direction[2];
                projected[2]=basis[2][0]*direction[0]+basis[2][1]*direction[1]+basis[2][2]*direction[2];
                panValues[i]=projected[0];
                depthValues[i]=projected[2];
                if (gains[i] != 0.0f) {}
                if (source->radius>0.0f) {
                    gains[i]=source->nearVolume-(source->nearVolume-source->farVolume)*(distance/source->radius);
                    if (gains[i]<0.0f) gains[i]=0.0f;
                } else gains[i]=source->nearVolume;
                weight=gains[i];
                total+=gains[i];
                panValues[i] = panValues[i] - (panValues[i] * weight);
                if (i == 5 || D_80146188.count == 1) {}
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

void func_80091AF0(void) {
    osRecvMesg(D_80142728, 0, 1);
}

void func_80091AF8(void) {
    osJamMesg(D_80142728, 0, 0);
}
