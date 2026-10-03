#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "func_8010C448.c"
ProximityEnabled D_8014AA3A[8];
ProximityPlayer player_array[8];
typedef char enabled_stride[(sizeof(ProximityEnabled)==0x808)?1:-1];
typedef char player_stride[(sizeof(ProximityPlayer)==0x3B8)?1:-1];
typedef char position_offset[(offsetof(ProximityPlayer,position)==8)?1:-1];
/* data: point[3], player position[3], radius, separate output; bit patterns. */
int run_case(int index, int enabled, uint32_t *data, int alias) {
    float point[3], radius, output, *destination;
    int result; short player_index=(short)index;
    memset(D_8014AA3A,0xA5,sizeof(D_8014AA3A));
    memset(player_array,0xA5,sizeof(player_array));
    D_8014AA3A[index].enabled=(signed char)enabled;
    memcpy(point,data,12); memcpy(player_array[index].position,data+3,12);
    memcpy(&radius,data+6,4); memcpy(&output,data+7,4);
    destination = alias < 0 ? 0 : alias < 3 ? &point[alias] :
        alias < 6 ? &player_array[index].position[alias-3] :
        alias == 6 ? &radius : &output;
    result=func_8010C448(&player_index,point,&radius,destination);
    assert(player_index==index);
    memcpy(data,point,12); memcpy(data+3,player_array[index].position,12);
    memcpy(data+6,&radius,4); memcpy(data+7,&output,4);
    {
        unsigned i,j; unsigned char *p;
        for(i=0;i<8;i++) {
            p=(unsigned char *)&D_8014AA3A[i];
            for(j=0;j<sizeof(ProximityEnabled);j++)
                assert(p[j]==((i==(unsigned)index && j==0)?(unsigned char)enabled:0xA5));
            p=(unsigned char *)&player_array[i];
            for(j=0;j<sizeof(ProximityPlayer);j++)
                if(i!=(unsigned)index || j<8 || j>=20) assert(p[j]==0xA5);
        }
    }
    return result;
}
#ifdef HOST_MAIN
static uint32_t seed=0x123abcde;
static uint32_t random_word(void) { seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed; }
static int equivalent(uint32_t a,uint32_t b) {
    float x,y;memcpy(&x,&a,4);memcpy(&y,&b,4);
    return a==b || (isnan(x)&&isnan(y));
}
int main(void) {
    unsigned n,j; uint32_t data[8],expected[8];
    for(n=0;n<20000;n++) {
        float p[3],c[3],r,dx,dy,dz,distance,error;
        int alias=(int)(n%9)-1,index=n%8,enabled=(n%5)?(int)(signed char)n:0, want=0,got;
        for(j=0;j<8;j++) data[j]=random_word();
        if(n%2) for(j=0;j<7;j++) { float v=(float)(int)(random_word()%81)-40.0f;memcpy(data+j,&v,4); }
        memcpy(expected,data,sizeof(data));memcpy(p,data,12);memcpy(c,data+3,12);memcpy(&r,data+6,4);
        if(enabled) {
            dx=c[0]-p[0];dy=c[1]-p[1];dz=c[2]-p[2];r=r+3.5f;
            distance=dz*dz+dx*dx;error=distance-r*r;
            if(alias>=0) memcpy(expected+alias,&error,4);
            want=!(error>0.0f) && dy>-2.0f && dy<18.0f;
        }
        got=run_case(index,enabled,data,alias);assert(got==want);
        for(j=0;j<8;j++) assert(equivalent(data[j],expected[j]));
    }
    puts("PASS 20000 randomized and floating-edge host cases, 9 output alias modes, full global canaries");
    return 0;
}
#endif
