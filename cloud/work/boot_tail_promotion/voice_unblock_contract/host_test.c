#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
VoiceState D_8004BEB8[32];
static VoiceState oracle[32];
static unsigned selected, seed, active, stage;
/* Native endian-independent record oracle uses actual host word objects. */
static void mutate(unsigned s, int expected){
    VoiceState *p= expected ? oracle : D_8004BEB8;
    if(s==0)p[selected].flags24=0xa5000000U|seed;
    else if(s==1)p[(selected+1)%32].activeBD=(seed*7)&255;
    else {p[selected].identifier60=0xbeef0000U|seed;p[selected].activeBD=255;}
}
u8 func_8001467C(int i){assert((unsigned)i==selected&&stage++==0);mutate(0,0);return active;}
void func_80014AF0(int i){assert((unsigned)i==selected&&stage++==1&&active);mutate(1,0);}
void func_8001F6EC(VoiceState *s){assert(s==D_8004BEB8+selected&&stage++==1+active);assert(s->identifier60==selected);mutate(2,0);}
int main(void){unsigned i,n=0;unsigned char *m=(unsigned char *)(void *)D_8004BEB8;VoiceState *e=oracle;
    /* Host pointer sizes differ; IDO verifies the native layout separately. */
    for(seed=0;seed<32;seed++)for(active=0;active<2;active++){
        for(i=0;i<sizeof(D_8004BEB8);i++)m[i]=(i*13+seed*17)&255;
        memcpy(oracle,D_8004BEB8,sizeof(oracle));stage=0;func_8001F954(0xffffffffU);assert(stage==0&&memcmp(oracle,D_8004BEB8,sizeof(oracle))==0);n++;
        for(selected=0;selected<32;selected++){
            for(i=0;i<sizeof(D_8004BEB8);i++)m[i]=(i*13+seed*17)&255;
            memcpy(oracle,D_8004BEB8,sizeof(oracle));mutate(0,1);if(active)mutate(1,1);e[selected].identifier60=selected;mutate(2,1);e[selected].activeBD=0;
            stage=0;func_8001F954(selected);assert(stage==2+active&&memcmp(oracle,D_8004BEB8,sizeof(oracle))==0);n++;
        }
    }
    printf("%u actual-source cases passed\n",n);return 0;
}
