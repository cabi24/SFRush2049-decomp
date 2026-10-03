/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/hud.c:AnimateDot.
 * N64 maps the signed world bounds into a centered per-viewport rectangle,
 * then applies the native mirror, eligibility, flash and icon rules. */
#include "types.h"
s32 func_80109A60(Sprite *blt)
{
    s16 slot,flash,icon_size;
    s32 hidden,world_width,world_height,map_width,map_height,size;
    Car952 *car;
    Model2056 *m;
    slot=D_80142DB4[blt->selector&15];
    hidden=D_80151AD0>=5 || slot==-1 || !D_80156BDC;
    if(hidden!=blt->hidden) {
        blt->hidden=hidden;
        Input_ApplyPadConfig(blt);
    }
    if(blt->hidden) return 1;
    world_width=D_801407B4.x-D_801407D4.x;
    world_height=D_801407B4.z-D_801407D4.z;
    icon_size=blt->height;
    size=D_801161C4;
    if(world_height<world_width) {
        map_width=size-8;
        map_height=world_height*map_width/world_width;
    } else {
        map_height=size-8;
        map_width=world_width*map_height/world_height;
    }
    blt->x=(D_801160A8[D_80151AD0-1].x+
            (size-map_width-8)/2+4)-(icon_size+size)/2;
    blt->y=(D_801160A8[D_80151AD0-1].y+
            (size-map_height-8)/2+4)-(icon_size+size)/2;
    car=&player_array[slot];
    if(D_80140A04) {
        blt->x=(s32)((f32)blt->x+
            ((car->position[0]-(f32)D_801407D4.x)*(f32)map_width)/(f32)world_width);
    } else {
        blt->x=(s32)((f32)blt->x+((f32)(map_width-1)-
            ((car->position[0]-(f32)D_801407D4.x)*(f32)map_width)/(f32)world_width));
    }
    blt->y=(s32)((f32)blt->y+
        ((car->position[2]-(f32)D_801407D4.z)*(f32)map_height)/(f32)world_height);
    D_80140BF0[blt->render_slot].flags|=1;
    m=&D_8014A250[slot];
    flash=car->dead || m->crash || !m->collidable || m->hide || m->hit_target!=-1;
    if(car->active>0) flash=0;
    if(slot<D_801543CA) {
        blt->alpha=((D_8002E8E8.tick&8) && flash)?64:255;
        if(m->mode==2) stat_race_update(blt,slot,icon_size,icon_size);
        else if(m->mode==1) stat_race_update(blt,7,icon_size,icon_size);
        Input_ApplyPadConfig(blt);
        return 1;
    }
    if(blt->hidden!=1) {
        blt->hidden=1;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}
