/* Finite final-result check using the actual complete group source.
 * Ordered native stores are proven separately by the full MIPS/GNU match. */
#include <stdio.h>
#include <string.h>
/* Unused context has the native 32-bit libc signature; keep that declaration
 * separate from the host libc. D300 has no calls and does not use this name. */
#define memcpy native_context_memcpy
#include "group.c"
#undef memcpy
s8 D_803B9FD0[4];
s8 D_80110E85[4][13],D_80111049[4][13],D_8011108D[4][13],D_801111A9[4][13],D_8011123D[4][13];
float D_803B28C8[3][4],D_803B2948[3][4],D_803B2978[3][4],D_803B2A08[3][4],D_803B2A58[3][4];
float D_803BA190[4][4];
int main(void)
{
    s8 (*selectors[5])[13];
    float (*vectors[5])[4];
    s8 selector_before[5][4][13],selection_before[4];
    float vector_before[5][3][4],output_before[4][4],expected[4];
    static const float values[12]={0.0f,-0.0f,0.5f,1.0f,2.0f,-1.0f,0.25f,-4.0f,0.1f,0.3f,-0.7f,1.1f};
    int player,selection,choice,j,row,component,p,k,code;
    unsigned long cases=0;
    volatile float product;
    selectors[0]=D_80110E85;selectors[1]=D_80111049;selectors[2]=D_8011108D;selectors[3]=D_801111A9;selectors[4]=D_8011123D;
    vectors[0]=D_803B28C8;vectors[1]=D_803B2948;vectors[2]=D_803B2978;vectors[3]=D_803B2A08;vectors[4]=D_803B2A58;
    for(player=0;player<4;player++)for(selection=0;selection<13;selection++)for(choice=0;choice<243;choice++) {
        for(p=0;p<4;p++) {
            D_803B9FD0[p]=(s8)selection;
            for(component=0;component<4;component++)D_803BA190[p][component]=64.0f+(float)(4*p+component);
        }
        code=choice;
        for(j=0;j<5;j++) {
            for(p=0;p<4;p++)for(k=0;k<13;k++)selectors[j][p][k]=(s8)((p+k+j)%3);
            selectors[j][player][selection]=(s8)(code%3);code/=3;
            for(row=0;row<3;row++)for(component=0;component<4;component++)vectors[j][row][component]=values[(choice+j*3+row+component+selection)%12];
            memcpy(selector_before[j],selectors[j],sizeof(selector_before[j]));
            memcpy(vector_before[j],vectors[j],sizeof(vector_before[j]));
        }
        memcpy(selection_before,D_803B9FD0,sizeof(selection_before));
        memcpy(output_before,D_803BA190,sizeof(output_before));
        for(component=0;component<4;component++) {
            product=vectors[0][selectors[0][player][selection]][component]*vectors[1][selectors[1][player][selection]][component];
            product=product*vectors[2][selectors[2][player][selection]][component];
            product=product*vectors[3][selectors[3][player][selection]][component];
            product=product*vectors[4][selectors[4][player][selection]][component];
            expected[component]=product;
        }
        func_8039D300(player);
        if(memcmp(expected,D_803BA190[player],sizeof(expected)))return 1;
        for(p=0;p<4;p++)if(p!=player && memcmp(output_before[p],D_803BA190[p],sizeof(output_before[p])))return 2;
        if(memcmp(selection_before,D_803B9FD0,sizeof(selection_before)))return 3;
        for(j=0;j<5;j++)if(memcmp(selector_before[j],selectors[j],sizeof(selector_before[j])) || memcmp(vector_before[j],vectors[j],sizeof(vector_before[j])))return 4;
        cases++;
    }
    printf("%lu finite product cases; unrelated output and inputs unchanged\n",cases);
    return 0;
}
