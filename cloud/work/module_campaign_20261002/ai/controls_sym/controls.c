/* Adapted native reconstruction guided by arcade source.
 * controls.c donor notice: Copyright 1996 Time Warner Interactive.
 * Unauthorized reproduction, adaptation, distribution, performance or
 * display of this computer program or the associated audiovisual work
 * is strictly prohibited.
 * Exact native semantics and N64 field offsets govern this research source.
 * Flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 */
#include "physics_types.h"
void func_800E3430(Physics *object)
{
    Model952 *slot=&player_array[object->slot];
    int i;
    Wheel92 *td;
    f32 factor;
    object->steerangle=object->steergain*object->wheel_input;
    object->dt=object->modeltime;
    object->idt=1.0f/object->dt;
    object->clutch=object->clutch_input;
    if(slot->place_locked==1) object->throttle=0.0f;
    else object->throttle=object->throttle_input;
    if(D_80152718!=0 || slot->place_locked!=0) {
        object->throttle=0.0f;object->clutch=1.0f;object->brake=0.7f;
    } else if(D_8013FECB!=0) {
        object->throttle=0.0f;object->clutch=1.0f;object->brake=0.33f;
    } else object->brake=object->brake_input;
    if(object->brake>0.99f) object->brake=1.0f;
    if(object->autotrans==0) {object->gear=object->gear_input;object->commandgear=object->gear_input;}
    else object->commandgear=object->gear_input;
    if(object->control_flags<0) {object->commandgear=0;object->gear=0;object->clutch=1.0f;object->brake=1.0f;}
    for(i=0,td=&object->wheel[0];i<4;i++,td++) {
        if(td->velocity>0.0f) {
            if(td->velocity<10.0f) {
                if(td->velocity<2.0f) factor=0.0f;
                else factor=td->velocity*0.05f;
            } else factor=1.0f;
        } else {
            if(td->velocity>-10.0f) {
                if(td->velocity>-2.0f) factor=0.0f;
                else factor=td->velocity*0.05f;
            } else factor=-1.0f;
        }
        object->torque[i]=-object->brakegain[i]*object->brake*factor;
    }
}

