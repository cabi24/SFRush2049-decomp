/* Host-only test bridge; never part of matching source. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "func_8010C588.c"
EnabledView2056 D_8014AA3A[4];
PlayerView952 D_80152818[4];
int run_case(int index, int enabled, const unsigned int *input, int alias,
             unsigned int *output)
{
    float origin[3], radius, error;
    float *destination;
    short selector;
    int result;
    assert(sizeof(float)==4 && sizeof(unsigned int)==4);
    assert(sizeof(EnabledView2056)==2056 && sizeof(PlayerView952)==952);
    assert(offsetof(PlayerView952,position)==8);
    assert(index>=0 && index<4 && alias>=0 && alias<=8);
    memset(D_8014AA3A,0x55,sizeof D_8014AA3A);
    memset(D_80152818,0xA5,sizeof D_80152818);
    selector=(short)index;
    D_8014AA3A[index].enabled=(signed char)enabled;
    memcpy(origin,input,12);
    memcpy(D_80152818[index].position,input+3,12);
    memcpy(&radius,input+6,4);
    memcpy(&error,input+7,4);
    destination=&error;
    if(alias==1)destination=0;
    else if(alias>=2 && alias<=4)destination=&origin[alias-2];
    else if(alias==5)destination=&radius;
    else if(alias>=6)destination=&D_80152818[index].position[alias-6];
    result=func_8010C588(&selector,origin,&radius,destination);
    memcpy(output,origin,12);
    memcpy(output+3,D_80152818[index].position,12);
    memcpy(output+6,&radius,4);
    memcpy(output+7,&error,4);
    assert(selector==index && D_8014AA3A[index].enabled==enabled);
    return result;
}
#ifdef STANDALONE
static unsigned int state=0xc588;
static unsigned int next_word(void) {state=state*1664525U+1013904223U;return state;}
int main(void)
{
    unsigned int input[8],output[8],i,j;
    int result;
    for(i=0;i<100000;i++) {
        for(j=0;j<8;j++)input[j]=next_word();
        result=run_case(i%4,(i%3)?1:0,input,i%9,output);
        assert(result==0 || result==1);
        if(i%3==0) {assert(result==0);assert(memcmp(input,output,sizeof input)==0);}
    }
    puts("100000 sanitizer cases passed");
    return 0;
}
#endif
