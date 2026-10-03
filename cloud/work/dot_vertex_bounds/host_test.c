#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "func_800B9740.c"
s16 D_801407D4[3], D_801407B4[3];
VertexSelection D_801407F0;
Vertex *D_801409E8;
u16 D_801527A4;
static Vertex points[128];
static VertexRange ranges[255];
static uint32_t state=0xb9740;
static uint32_t next(void) {state=state*1664525u+1013904223u;return state;}
int main(void)
{
    unsigned test,i,j,k,start,count,primary,range_count;
    s16 low[3],high[3];int included;
    assert(sizeof(Vertex)==6 && offsetof(VertexRange,count)==10);
    assert(offsetof(VertexSelection,range_count)==8);
    if(sizeof(void*)==4) {
        assert(sizeof(VertexRange)==16 && offsetof(VertexRange,points)==12);
        assert(sizeof(VertexSelection)==16 && offsetof(VertexSelection,ranges)==12);
    }
    D_801409E8=points;D_801407F0.ranges=ranges;
    for(test=0;test<20000;test++) {
        count=next()%129;primary=(test%7==0)?65535:next()%129;
        range_count=(test%7==0)?255:next()%16;
        D_801527A4=(u16)count;D_801407F0.primary_count=(u16)primary;
        D_801407F0.range_count=(u8)range_count;
        for(i=0;i<count;i++) for(k=0;k<3;k++) points[i][k]=(s16)(next()&65535);
        for(j=0;j<range_count;j++) {
            start=next()%(count+1);ranges[j].points=points+start;
            ranges[j].count=(u16)(next()%(count-start+1));ranges[j].kind=(u8)(next()%3);
        }
        for(k=0;k<3;k++) {low[k]=32767;high[k]=-32767;}
        for(i=0;i<count;i++) {
            included=i<primary;
            for(j=0;j<range_count;j++) {
                start=(unsigned)(ranges[j].points-points);
                if(i>=start && i<start+ranges[j].count && ranges[j].kind==1)included=1;
            }
            if(included)for(k=0;k<3;k++) {
                if(points[i][k]<low[k])low[k]=points[i][k];
                if(points[i][k]>high[k])high[k]=points[i][k];
            }
        }
        func_800B9740();
        assert(memcmp(low,D_801407D4,sizeof(low))==0);
        assert(memcmp(high,D_801407B4,sizeof(high))==0);
        assert(D_801527A4==count && D_801407F0.primary_count==primary);
    }
    /* Retail deliberately uses -32767, so an isolated -32768 vertex leaves
       the maximum sentinel at -32767. Do not normalize that behavior. */
    D_801527A4=1;D_801407F0.primary_count=1;
    for(k=0;k<3;k++)points[0][k]=-32768;
    func_800B9740();
    for(k=0;k<3;k++)assert(D_801407D4[k]==-32768 && D_801407B4[k]==-32767);
    return 0;
}
