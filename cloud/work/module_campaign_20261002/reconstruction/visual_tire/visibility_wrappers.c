/* Genuine complete logical bodies, reconstructed from the native
 * 18/25-word routines. Private native register conventions require actual
 * source context; this file makes no ordinary singleton match claim. */
#include "types.h"
s32 model_bounds_calc(s32 show,Visual *v)
{
    if(show) model_transform_setup(v->object,0,15);
    else model_data_load(v->object,1,15);
    return show;
}
s32 matrix_scale_apply(Visual *v,s32 show,s32 part)
{
    if(part<0) return model_bounds_calc(show,v);
    if(show) model_transform_setup(v->object,0,1<<part);
    else model_data_load(v->object,1,1<<part);
    return show;
}
