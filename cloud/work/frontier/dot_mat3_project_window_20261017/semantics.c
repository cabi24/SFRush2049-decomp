/* Finite host differential tests; source boundary checks supplement native proof. */
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.c"
#include "historical_body.inc"

Port D_8017A510[4];
void *func_800A6078(s32 idx) { assert(idx>=0 && idx<4); return &D_8017A510[idx]; }
void func_800A61B0(f32 *v, f32 *out, f32 *m) {
    unsigned i;
    for(i=0;i<3;i++) out[i]=v[0]*m[i*3]+v[1]*m[i*3+1]+v[2]*m[i*3+2];
}
static unsigned count, saturated, optional;
static void check(int idx, float *point, View *view, int body) {
    float a[3]={-99.0f,-99.0f,-99.0f},b[3]={-99.0f,-99.0f,-99.0f};
    s16 sa[2]={111,222},sb[2]={111,222};
    historical_project(idx,point,view,body?a:0,sa);
    brake_light_update(idx,point,view,body?b:0,sb);
    if(memcmp(sa,sb,sizeof sa)!=0 || memcmp(a,b,sizeof a)!=0) {
        fputs("project-window differential mismatch\n",stderr); exit(3);
    }
    saturated+=(sa[0]==-32768||sa[0]==32767||sa[1]==-32768||sa[1]==32767);
    optional+=!body;count++;
}
static uint32_t seed=0x20496385u;
static float next(void) { seed=seed*1664525u+1013904223u; return (float)((int)(seed>>8)%200001-100000)*0.0625f; }
int main(void) {
    static const float xy[]={-1000000.0f,-32768.5f,-32768.0f,-32767.5f,-1.0f,-0.0f,0.0f,1.0f,32766.5f,32767.0f,32767.5f,1000000.0f};
    static const float z[]={-100.0f,-0.0f,0.0f,0.01f,2.499f,2.5f,2.501f,100.0f};
    View v; float p[3]; unsigned i,j,k,w,b,n;
    memset(&v,0,sizeof v);v.uvs[0][0]=v.uvs[1][1]=v.uvs[2][2]=1.0f;
    for(w=0;w<4;w++){Port *q=&D_8017A510[w];memset(q,0,sizeof *q);q->unk1C=1.0f;q->unk20=1.0f;q->unk24=2.5f;q->unk28=2.5f;q->unk2C=(float)w*0.25f;q->unk30=-(float)w*0.25f;}
    for(w=0;w<4;w++)for(i=0;i<12;i++)for(j=0;j<12;j++)for(k=0;k<8;k++)for(b=0;b<2;b++) {p[0]=xy[i];p[1]=xy[j];p[2]=z[k];check((int)w,p,&v,(int)b);}
    for(n=0;n<10000;n++) {
        w=n%4;
        for(i=0;i<3;i++){v.pos[i]=next()*0.1f;p[i]=next();}
        /* Orthogonal 90-degree views, avoiding dependence on host trig details. */
        memset(v.uvs,0,sizeof v.uvs);v.uvs[1][1]=1.0f;
        switch(n%4){case 0:v.uvs[0][0]=1;v.uvs[2][2]=1;break;case 1:v.uvs[0][2]=1;v.uvs[2][0]=-1;break;case 2:v.uvs[0][0]=-1;v.uvs[2][2]=-1;break;default:v.uvs[0][2]=-1;v.uvs[2][0]=1;break;}
        D_8017A510[w].unk1C=0.25f+(float)(n%11)*0.125f;
        D_8017A510[w].unk20=0.5f+(float)(n%13)*0.0625f;
        D_8017A510[w].unk24=160.0f;D_8017A510[w].unk28=120.0f;
        D_8017A510[w].unk2C=160.0f;D_8017A510[w].unk30=120.0f;
        check((int)w,p,&v,(int)(n%2));
    }
    assert(saturated>0 && optional>0);
    printf("%u finite differential cases; %u saturation cases; %u null-body cases; depth and signed-short boundaries exercised; UBSan clean",count,saturated,optional);
    return 0;
}
