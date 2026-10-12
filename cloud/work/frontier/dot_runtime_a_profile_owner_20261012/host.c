/* Bounded behavior check compiled with the actual, unchanged group source. */
#include <stdio.h>
#include <string.h>
#include "group.c"
s16 D_8014A108;
Player D_8014A118[4];
static Profile *handles[4];
int main(void)
{
    int assignment,count,skip,owner,k,v,expected,actual;
    unsigned long calls=0;
    Player before[4];
    Profile **choices[5];
    choices[0]=0;
    for(k=0;k<4;k++)choices[k+1]=&handles[k];
    for(assignment=0;assignment<625;assignment++) {
        v=assignment;
        for(k=0;k<4;k++){D_8014A118[k].owner=choices[v%5];v/=5;}
        memcpy(before,D_8014A118,sizeof(before));
        for(count=-2;count<=4;count++)for(skip=-2;skip<=5;skip++)for(owner=0;owner<5;owner++) {
            D_8014A108=(s16)count;expected=0;
            if(choices[owner])for(k=0;k<count;k++)
                if(k!=skip && D_8014A118[k].owner==choices[owner])expected=1;
            actual=func_80395E24(choices[owner],skip);
            if(actual!=expected || D_8014A108!=count || memcmp(before,D_8014A118,sizeof(before)))return 1;
            calls++;
        }
    }
    printf("%lu checked owner selections; no writes\n",calls);
    return 0;
}
