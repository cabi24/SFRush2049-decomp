/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D764 (static, boot 0x8001D764, 332 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 21 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_runtime_followon_8001D764.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-runtime-followon/nonmatch/func_8001D764.c (Astra / BT03-high-runtime-followon).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef struct Vector { float x,y,z; } Vector;
typedef struct SpatialState {
    struct SpatialState *next,*previous;
    u32 flags08;
    Vector position,velocity,forward,side,up;
    float inverse[12];
    float first78,second7C,third80,gain84;
} SpatialState;
extern u8 D_8002C630;
extern SpatialState *D_8004FD54;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001D5C0(SpatialState *);
int func_8001D764(SpatialState *state,const Vector *position,const Vector *velocity,
                  const Vector *forward,const Vector *up,float first,float second,
                  float third,u32 flags,u8 level)
{
    if (D_8002C630) {
        func_80014594();
        if ((state->next=D_8004FD54) != 0) D_8004FD54->previous=state;
        state->previous=0;
        D_8004FD54=state;
        state->position=*position;
        state->velocity=*velocity;
        state->forward=*forward;
        state->up.x=-up->x;
        state->up.y=-up->y;
        state->up.z=-up->z;
        state->first78=first;
        state->second7C=second;
        state->third80=third;
        func_8001D5C0(state);
        state->flags08=flags;
        state->gain84=(float)level/127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}
