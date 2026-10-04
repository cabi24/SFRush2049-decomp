/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Vector {float x,y,z;} Vector;
typedef struct Emitter {
    struct Emitter *next,*previous;
    u32 flags;
    Vector position,velocity;
    float range,gain,minimum,curve;
    u32 handle,context;
    u16 identifier,counter;
    float fade;
} Emitter;
typedef struct SpatialEntry {
    struct SpatialEntry *next;
    float volume,pan,span,send,pitch;
    Emitter *emitter;
} SpatialEntry;
typedef struct SpatialGroup {u32 key;SpatialEntry *pending,*active;} SpatialGroup;
extern SpatialGroup D_8004FDA0[];
extern u8 D_8004FF20;
extern float D_8002D904,D_8002D908;
extern u32 func_8001B1D0(u16,u8,u8);
extern void func_8001CCDC(Emitter *,float,float,float,float,float);
void func_8001DC08(void)
{
    int i;
    SpatialGroup *group;
    SpatialEntry *entry;
    Emitter *state;
    float difference,upper,lower;
    i=0;
    if (D_8004FF20>0) {
      lower=D_8002D908;
      upper=D_8002D904;
      group=D_8004FDA0;
      do {
        for (entry=group->pending;entry!=0;entry=entry->next) {
            if (group->active!=0) {
                difference=entry->volume-group->active->volume;
                if (difference<=lower) continue;
                if (difference<=upper) {
                    entry->emitter->counter++;
                    if (entry->emitter->counter<20) continue;
                } else entry->emitter->counter=0;
            }
            state=entry->emitter;
            state->handle=func_8001B1D0(state->identifier,127,64);
            if (state->handle==0xFFFFFFFFU) {
                if (!(state->flags&2)) {
                    state->flags|=0x40000;
                    state->flags&=~0x20000U;
                }
            } else {
                state->fade=0.0f;
                state->flags|=0x100000;
                func_8001CCDC(state,entry->volume,entry->pan,entry->span,entry->send,entry->pitch);
                state->flags&=~0x20000U;
                if (group->active!=0) group->active=group->active->next;
            }
        }
        i++;group++;
      } while (i<D_8004FF20);
    }
}
