/* Bounded host differential harness. The exact candidate is included, and replay.py
 * appends the same body with only the multiply-by-one shaping removed. This tests
 * finite game-like geometry, state writes and callback order. It does not prove
 * IEEE exception equivalence or reproduce every N64 FCSR configuration. */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "camera_play_script.c"
#include "natural_body.inc"

static PV vertices[4] = {{0,0,0,0}, {4,0,0,0}, {4,0,4,0}, {0,0,4,0}};
PV *D_8015201C = vertices;
s32 D_80152568;
s8 D_801174CC[16] = {0,1,2,3,0,1,2,3,1,2,3,0,4,5,7,6};
s8 D_801174C4[8] = {0,1,3,2,0,1,3,2};
PCar player_array[8];
Link D_80153E88[8];
static int events[64], count;
static void event(int v) { assert(count < 64); events[count++] = v; }
void effect_cleanup(s8 a, s8 b, s8 c) { event(1000 + a*100 + b*10 + c); }
void func_800C54F0(s16 a, s32 b) { event(2000 + a*100 + b); }
s32 func_800B61A8(s32 a, s32 b, s32 c, unsigned char d) {
    event(3000 + a*100 + b*10 + c + d); return 0;
}
s32 func_800AD5D0(u8 *p, s32 n, s16 *out) {
    int i; (void)p; assert(n == 4); for (i=0; i<n; i++) out[i]=(s16)i; return 0;
}
void func_800AD650(void *a, void *b) {
    s16 x[9]; int i; f32 *out=b; memcpy(x,a,sizeof x);
    for(i=0;i<9;i++) out[i] = x[i] * (1.0f/16384.0f);
}
void func_800A61B0(void *a, void *b, void *c) {
    const f32 *v=a, *m=c; f32 *out=b; int i;
    for(i=0;i<3;i++) out[i]=v[0]*m[i*3]+v[1]*m[i*3+1]+v[2]*m[i*3+2];
}
void math_utility(void *a, void *b) { memcpy(b,a,9*sizeof(f32)); }
void func_800C36A0(CCar *car, CCol *col) {
    (void)car; assert(col->poly != NULL); event(4000 + col->idx*10 + col->edge);
}
int main(void) {
    unsigned c; int hits=0, callbacks=0;
    for(c=0;c<1024;c++) {
        CCar original, shaped, natural; CCol first, second;
        Poly p, previous; s16 matrix[9]={16384,0,0,0,16384,0,0,0,16384};
        int first_events[64], first_count, i; float x=(float)(c%8)-2.0f;
        float z=(float)((c/8)%8)-2.0f;
        memset(&original,0,sizeof original); memset(&p,0,sizeof p);
        memset(&first,0,sizeof first); memset(player_array,0,sizeof player_array);
        memset(D_80153E88,0,sizeof D_80153E88);
        p.cnt=4; p.type=(u16)(c%8); memcpy(p.body,matrix,sizeof matrix); previous=p;
        original.pidx=(s16)(c%4); original.f6C4=(c&16)?-1:0;
        original.mat[0]=1; original.mat[4]=(c&32)?-1.0f:1.0f; original.mat[8]=1;
        player_array[original.pidx].f358=(c&64)?1:0;
        D_80153E88[original.pidx].kind=(c&128)?6:0;
        for(i=0;i<4;i++) {
            original.B628[i][0]=x+(float)(i&1);
            original.B628[i][1]=(c&256)?0.25f:((i&1)?1.0f:-1.0f);
            original.B628[i][2]=z+(float)((i>>1)&1);
            memcpy(original.C676[i],original.B628[i],3*sizeof(f32));
            original.C676[i][1]+=1.0f;
        }
        if(c&512) { first.poly=&previous; first.k=(c&1)?0.125f:8.0f; }
        second=first; shaped=original; natural=original;
        count=0; camera_play_script(&shaped,&p,&first);
        first_count=count; memcpy(first_events,events,sizeof events);
        count=0; natural_camera_play_script(&natural,&p,&second);
        assert(memcmp(&shaped,&natural,sizeof shaped)==0);
        assert(memcmp(&first,&second,sizeof first)==0);
        assert(first_count==count);
        assert(memcmp(first_events,events,(size_t)count*sizeof(int))==0);
        hits+=(first.poly==&p); callbacks+=count;
    }
    assert(hits > 0); assert(callbacks > 0);
    printf("1024 bounded finite-geometry cases passed; %d contacts, %d callbacks\n",hits,callbacks);
    return 0;
}
