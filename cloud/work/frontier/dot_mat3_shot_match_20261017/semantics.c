/* Bounded finite host differential check, not an emulation of the N64 FPU. */
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "candidate.c"
#include "historical_body.inc"

static QNode nodes[2];
static Poly polys[3];
static u8 stream[0x10010];
QNode *D_80124EEC = nodes;
Poly *D_801497F8 = polys;
u8 *D_80152460 = stream;

struct Trace { u32 words[512]; unsigned count; };
struct Result { struct Trace trace; f32 end[3], mat[3][3]; int poly; };
static struct Trace trace;
static int query_count, first_miss, same_cell, quad_value, collision_calls;
static int hit_outside, hit_mode;
static s16 hit_state;
static f32 hit[3];
static unsigned cases, traversals, hits, snaps, normalizations;

static void word(u32 n) { assert(trace.count < 512); trace.words[trace.count++] = n; }
static void real(f32 f) { u32 w; memcpy(&w, &f, sizeof w); word(w); }
static void vector(const f32 *v, unsigned n) { unsigned k; for (k=0;k<n;k++) real(v[k]); }

void *handbrake_apply(QNode *node, s16 x, s16 z, s16 *out) {
    assert(node == nodes); word(1); word((u16)x); word((u16)z);
    *out = (s16)quad_value;
    query_count++;
    if (query_count == 1) return first_miss ? NULL : nodes;
    if (query_count == 2) return same_cell ? nodes : nodes+1;
    traversals++;
    assert(query_count == 3);
    return NULL;
}
void func_800ADCE0(u8 *input, s32 count, u16 *front, u32 marker) {
    unsigned k; word(2); word((u32)(input-stream)); word((u32)count); word(marker);
    assert(count == 3); for(k=0;k<3;k++) front[k]=(u16)k;
}
s16 input_process_controller(Poly *poly, f32 *p1, f32 *p2, f32 *out, f32 *vcOut, f32 *mat, s32 flag, s16 *outIdx, f32 rad2) {
    unsigned k; word(3); word((u32)(poly-polys)); vector(p1,3); vector(p2,3); word((u32)flag); real(rad2);
    collision_calls++;
    for(k=0;k<3;k++) { out[k]=hit[k]; vcOut[k]=(f32)(k+11); }
    if(hit_outside) out[0]=300.0f;
    for(k=0;k<9;k++) mat[k]=(f32)(k+1)*(f32)(poly-polys+1);
    *outIdx=42;
    return hit_mode ? hit_state : 0;
}
void math_utility(void *a, void *b) { word(4); memcpy(b,a,9*sizeof(f32)); hits++; }
void steering_sensitivity(s32 arg, u16 idx, f32 *position, f32 *out, f32 (*mat)[3], f32 threshold) {
    unsigned k; word(5); word((u32)arg); word(idx); vector(position,3); real(threshold);
    for(k=0;k<3;k++) { out[k]=position[k]+1.0f; mat[1][k]+=2.0f; } snaps++;
}
void traction_control(s32 arg, u16 idx, f32 *position, f32 *out, f32 (*mat)[3]) {
    unsigned k; word(6); word((u32)arg); word(idx); vector(position,3);
    for(k=0;k<3;k++) { out[k]=position[k]+2.0f; mat[2][k]+=3.0f; } snaps++;
}
f32 func_8008E0B8(f32 *v) {
    unsigned k; f32 len=sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); word(7); vector(v,3);
    for(k=0;k<3;k++) v[k]/=len;
    normalizations++; return len;
}

static struct Result run(int historical, f32 *start, f32 *end, f32 radius, int snap, int skip) {
    struct Result r; Poly *p;
    memset(&r,0,sizeof r); memset(&trace,0,sizeof trace); query_count=collision_calls=0;
    memcpy(r.end,end,sizeof r.end);
    p=historical ? historical_shot(start,r.end,r.mat,radius,snap,skip)
                 : input_deadzone_apply(start,r.end,r.mat,radius,snap,skip);
    r.poly=p ? (int)(p-polys)+1 : 0;
    r.trace=trace;
    return r;
}
static void compare(f32 *start, f32 *end, f32 radius, int snap, int skip) {
    struct Result a=run(1,start,end,radius,snap,skip),b=run(0,start,end,radius,snap,skip);
    assert(memcmp(&a,&b,sizeof a)==0); cases++;
}
static void reset(int quad, int surfaces) {
    unsigned k; memset(nodes,0,sizeof nodes); memset(polys,0,sizeof polys); memset(stream,0,sizeof stream);
    nodes[0].x0=nodes[0].y0=-100; nodes[0].x1=nodes[0].y1=100;
    for(k=0;k<4;k++) nodes[0].child[k]=surfaces ? 1 : 0;
    stream[1]=stream[0x10001]=3;
    polys[0].type=15; polys[1].type=7; polys[2].type=1;
    quad_value=quad; first_miss=0; same_cell=1; hit_outside=0; hit_mode=1; hit_state=1;
    hit[0]=(quad&1)?50.0f:-50.0f;hit[1]=4.5f;hit[2]=(quad&2)?-50.0f:50.0f;
}
int main(void) {
    f32 a[3],b[3]; int q,dir,snap,skip,flags,outside,state; unsigned k;
    static const f32 radii[]={0.0f,-0.0f,0.25f,2.0f,-3.5f};
    /* Exhaust the supplied signed-short return domain with a stable hit. */
    reset(0,1); a[0]=-50.25f;a[1]=2.5f;a[2]=50.25f;b[0]=-40.5f;b[1]=3.5f;b[2]=60.5f;
    for(state=-32768;state<=32767;state++){hit_state=(s16)state;compare(a,b,2.0f,0,7);}
    /* Surface filtering, both snap paths, outside hits, extended list offsets. */
    for(q=0;q<4;q++) for(snap=0;snap<2;snap++) for(skip=0;skip<16;skip++)
    for(flags=0;flags<4;flags++) for(outside=0;outside<2;outside++) for(k=0;k<5;k++) {
        reset(q,1); nodes[0].mask=(u8)((flags&1)?(16<<q):0);
        polys[2].type=(u16)(1|((flags&1)?0x1000:0)|((flags&2)?0x2000:0));
        hit_outside=outside; hit_state=(s16)(q%2?-1:32767);
        a[0]=hit[0]+0.25f;a[1]=-3.5f;a[2]=hit[2]-0.25f;
        b[0]=hit[0]+1.5f;b[1]=9.5f;b[2]=hit[2]+2.5f;
        compare(a,b,radii[k],snap,skip);
    }
    /* All face directions and corner-tie branches, with fractional floors. */
    for(q=0;q<4;q++) for(dir=0;dir<12;dir++) for(k=0;k<5;k++) {
        reset(q,0); same_cell=0;
        a[0]=hit[0];a[1]=1.25f;a[2]=hit[2];
        b[0]=a[0]+0.25f;b[1]=-17.5f;b[2]=a[2]-0.25f;
        if(dir<4) { if(dir<2)b[0]=dir?-150.5f:150.5f;else b[2]=dir==2?-150.5f:150.5f; }
        else { b[0]=a[0]+((dir&1)?-200.0f:200.0f); b[2]=a[2]+((dir&2)?-200.0f:200.0f); if(dir>=8)b[0]=a[0]; }
        compare(a,b,radii[k],0,7);
    }
    reset(0,0);first_miss=1;compare(a,b,1.0f,0,7);
    reset(0,0);same_cell=1;compare(a,b,1.0f,0,7);
    assert(traversals>0 && hits>0 && snaps>0 && normalizations>0);
    printf("%u finite differential cases; all 65536 s16 returns; %u traversal exits; %u accepted hits; %u snap calls; %u normalizations; UBSan clean",cases,traversals,hits,snaps,normalizations);
    return 0;
}
