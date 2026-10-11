/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native linked-source/listener update; historical name retained as identity.
 * Arrays of five floats follow the three native 20-byte scratch spans.
 * TransposeUV/WorldVector are source-shape hypotheses from arcade LIB/fmath.c;
 * their mathematical operations are present in retail, their original names
 * and inline boundaries are not established. No padding locals are added.
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

static void transpose_basis(f32 uv[][3])
{
    f32 temp;
    temp=uv[0][1]; uv[0][1]=uv[1][0]; uv[1][0]=temp;
    temp=uv[0][2]; uv[0][2]=uv[2][0]; uv[2][0]=temp;
    temp=uv[1][2]; uv[1][2]=uv[2][1]; uv[2][1]=temp;
}

static void world_vector(const f32 source[3],f32 result[3],const f32 matrix[][3])
{
    result[0]=source[0]*matrix[0][0]+source[1]*matrix[1][0]+source[2]*matrix[2][0];
    result[1]=source[0]*matrix[0][1]+source[1]*matrix[1][1]+source[2]*matrix[2][1];
    result[2]=source[0]*matrix[0][2]+source[1]*matrix[1][2]+source[2]*matrix[2][2];
}

void engine_sound_sync(void)
{
    Source *source, *next;
    Effect *effect;
    Listener *listener;
    f32 gain, pan, depth, parameter;
    f32 distance, total, totalPan, totalDepth, weight;
    f32 direction[3], projected[3], basis[3][3];
    f32 panValues[5], depthValues[5], gains[5];
    f32 *panOut, *depthOut, *gainOut;
    s32 i;

    osRecvMesg(D_80142728,0,1);
    source=D_801461F0;
    while (source) {
        next=source->next;
        effect=func_80091BA8(source->handle);
        if (effect->kind==2 && !effect->persistent && !effect->delay &&
            !entity_state_check(effect->voice)) {
            audio_effect_setup(effect);
        } else if (!effect->state) {
            total=0.0f;
            totalPan=0.0f;
            totalDepth=0.0f;
            parameter=source->parameter;
            listener=D_80146188.head;
            if (listener) {
                panOut=panValues;
                depthOut=depthValues;
                gainOut=gains;
                do {
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
                transpose_basis(basis);
                world_vector(direction,projected,basis);
                *panOut=projected[0];
                *depthOut=projected[2];
                if (source->radius>0.0f) {
                    *gainOut=source->nearVolume-(source->nearVolume-source->farVolume)*(distance/source->radius);
                    if (*gainOut<0.0f) *gainOut=0.0f;
                } else *gainOut=source->nearVolume;
                total+=*gainOut;
                *panOut = *panOut - (*panOut * *gainOut);
                panOut++;depthOut++;gainOut++;
                listener=listener->next;
                } while (listener);
            }
            gain=total<0.0f?0.0f:total>1.0f?1.0f:total;
            i=0;
            if (D_80146188.count>0) do {
                if (total!=0.0f) {
                    weight=gains[i]/total;
                    totalPan+=panValues[i]*weight;
                    totalDepth+=depthValues[i]*weight;
                }
                i++;
            } while (i<D_80146188.count);
            pan=totalPan < -1.0f ? -1.0f : totalPan>1.0f ? 1.0f : totalPan;
            depth=totalDepth < -1.0f ? -1.0f : totalDepth>1.0f ? 1.0f : totalDepth;
            entity_transform_calc(source->handle,gain,pan,depth,parameter);
        }
        source=next;
    }
    osJamMesg(D_80142728,0,0);
}
