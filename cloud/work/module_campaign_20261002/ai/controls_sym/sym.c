/* Native adaptation guided by arcade drivsym.c:sym().
 * Original model Copyright 1985 Milliken Engineering.
 * Translation started 10/20/85, Copyright 1985 Atari Inc.
 * Translated by Max Behensky.
 * Copyright 1996 Time Warner Interactive.
 * Unauthorized reproduction, adaptation, distribution, performance or
 * display of this computer program or the associated audiovisual work
 * is strictly prohibited.
 * Research only. Flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 */
#include "physics_types.h"
void func_800E3724(Physics *object)
{
    int i,j,absolute;
    f32 x,y,z;
    func_800E3430(object);
    if(object->frozen!=0 || player_array[object->slot].state!=0) {
        object->steerangle=0.0f;object->clutch=0.0f;object->brake=0.0f;object->throttle=0.0f;
    }
    func_800E32CC(object);
    object_update_full(object);
    func_800E0B20(object);
    object->rpm=(s32)(object->engangvel*9.549305f*0.9f);
    absolute=object->rpm;
    if(absolute<0) absolute=-absolute;
    if(object->autotrans==0 && object->throttle>0.5f && absolute<600) {
        if(object->state==0) object->state=1;
    } else if(object->state!=0) object->state=3;
    if(object->mode==2) {
        for(i=0;i<3;i++) {
            if(object->peak_center_force[0][i]<object->CENTERFORCE[i])object->peak_center_force[0][i]=object->CENTERFORCE[i];
            if(object->CENTERFORCE[i]<object->peak_center_force[1][i])object->peak_center_force[1][i]=object->CENTERFORCE[i];
            for(j=0;j<4;j++) {
                if(object->peak_body_force[0][i]<object->BODYFORCE[j][i])object->peak_body_force[0][i]=object->BODYFORCE[j][i];
                if(object->BODYFORCE[j][i]<object->peak_body_force[1][i])object->peak_body_force[1][i]=object->BODYFORCE[j][i];
            }
        }
    }
    x=object->CENTERFORCE[0];y=object->CENTERFORCE[1];z=object->CENTERFORCE[2];
    object->CENTERFORCE[2]=0.0f;object->CENTERFORCE[1]=0.0f;object->CENTERFORCE[0]=0.0f;
    object->CENTERMOMENT[0]=0.0f;object->CENTERMOMENT[1]=0.0f;object->CENTERMOMENT[2]=0.0f;
    object->last_CENTERFORCE[0]=x;object->last_CENTERFORCE[1]=y;object->last_CENTERFORCE[2]=z;
}
