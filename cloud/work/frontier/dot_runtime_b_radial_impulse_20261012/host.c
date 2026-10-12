/* Actual-source LP64 logical fixture. Native layout is checked separately. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "candidate.c"
Player D_80152818[4];
Vehicle D_8014A250[4];
Scene D_8012E700[3];
volatile s16 D_801543CA;
static Effect effect;
static BObject object;
static int mutation;
static uint32_t events[96];
static int event_count;

static uint32_t word(float x) { uint32_t u; memcpy(&u,&x,4); return u; }
static void event(uint32_t x) { events[event_count++]=x; }
void func_8038D3A4(Player *source, Player *player, s32 magnitude)
{
    event(1); event((uint32_t)(source-D_80152818));
    event((uint32_t)(player-D_80152818)); event((uint32_t)magnitude);
    if (mutation) {
        effect.owner=(effect.owner+1)%4;
        player->uv[0][0]=2.0f;
        D_8014A250[player->index].velocity[0]=17.5f;
    }
}
void func_800A61B0(float *in,float *out,float *matrix)
{
    float a[3],v[3]; int j;
    Player *player=(Player *)((unsigned char *)matrix - offsetof(Player,uv));
    for(j=0;j<3;j++)v[j]=in[j];
    event(2); event((uint32_t)(player-D_80152818));
    for(j=0;j<3;j++)event(word(v[j]));
    for(j=0;j<3;j++)a[j]=(v[0]*matrix[j]+v[1]*matrix[j+3])+v[2]*matrix[j+6];
    for(j=0;j<3;j++)out[j]=a[j];
    if(mutation) {
        player->index=(player->index+1)%4;
        if(D_801543CA>3)D_801543CA=3;
    }
}
int host_run(int count,float radius,int owner,int mask,int variant,int mutate,uint32_t *out)
{
    int i,j,k,n=0,which;float d[3];
    memset(&effect,0,sizeof effect); memset(&object,0,sizeof object);
    memset(D_80152818,0,sizeof D_80152818); memset(D_8014A250,0,sizeof D_8014A250);
    memset(D_8012E700,0,sizeof D_8012E700);
    effect.owner=owner;effect.hit_mask=mask;effect.primary=&object;
    object.scene_index=1;D_8012E700[1].radius=radius;
    D_801543CA=count;mutation=mutate;event_count=0;
    for(j=0;j<3;j++)object.position[j]=(j-1)*1.5f;
    for(i=0;i<4;i++){
        Player *p=&D_80152818[i];Vehicle *v=&D_8014A250[i];
        p->index=i;p->active=(variant%5==1 && i==2)?0:1;
        v->attached=(variant%5==2 && i==1)?0:-1;
        which=(i+variant)%4;
        if(which==0){d[0]=1;d[1]=2;d[2]=3;}
        else if(which==1){d[0]=4*radius;d[1]=0;d[2]=0;}
        else if(which==2){d[0]=4*radius+1;d[1]=0;d[2]=0;}
        else {d[0]=-2;d[1]=-1;d[2]=1;}
        if(radius==0){d[0]=1;d[1]=0;d[2]=0;}
        for(j=0;j<3;j++){
            p->position[j]=object.position[j]+d[j];
            v->velocity[j]=(i*4+j-7)*.25f;v->planar_velocity[j]=(i*3-j+2)*.125f;
            for(k=0;k<3;k++)p->uv[j][k]=(j==k);
        }
    }
    func_8038D498(&effect);
    out[n++]=(uint32_t)(unsigned char)effect.owner;
    out[n++]=(uint32_t)(unsigned char)effect.hit_mask;
    out[n++]=(uint32_t)(unsigned short)D_801543CA;
    for(i=0;i<4;i++){
        out[n++]=(uint32_t)(unsigned char)D_80152818[i].index;
        out[n++]=word(D_80152818[i].uv[0][0]);
        for(j=0;j<3;j++)out[n++]=word(D_8014A250[i].velocity[j]);
        for(j=0;j<3;j++)out[n++]=word(D_8014A250[i].planar_velocity[j]);
    }
    out[n++]=event_count;
    for(i=0;i<event_count;i++)out[n++]=events[i];
    return n;
}
#ifdef HOST_MAIN
#include <stdio.h>
int main(void)
{
    int counts[]={0,1,3,4},masks[]={0,1,4,128,255};float radii[]={0,4,8,8.5f,20,20.5f};
    uint32_t out[160];int c,r,o,m,v,u,n,i;
    for(c=0;c<4;c++)for(r=0;r<6;r++)for(o=0;o<2;o++)for(m=0;m<5;m++)for(v=0;v<5;v++)for(u=0;u<2;u++){
        n=host_run(counts[c],radii[r],o*2,masks[m],v,u,out);
        for(i=0;i<n;i++)printf("%08x%s",out[i],i+1==n?"\n":" ");
    }
    return 0;
}
#endif
