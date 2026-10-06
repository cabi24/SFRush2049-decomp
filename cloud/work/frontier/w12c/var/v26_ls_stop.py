exec(open('v25.py').read().split('V=')[0])
STOP='''static void layer_stop(LayerState *e, s32 mode)
{
    if (mode==2) scheduler_recv(e->handle);
    else results_screen_update(e->handle);
    player_conditional_call(e);
}
'''
P2=(P[0],'''                layer_stop(entry,MODE(model));''')
START='''static s32 layer_start(ModelView *model, s32 slot)
{
    if(MODE(model)==2) {
        return frame_sync(36,model->index,2,2);
    } else {
        return camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
    }
}
'''
S0='''            if(MODE(model)==2) {
                entry->handle=frame_sync(36,model->index,2,2);
            } else {
                entry->handle=camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
            }'''
S1=(S0,'''            entry->handle=layer_start(model,slot);''')
PL='''static void layer_place(LayerState *e, f32 *pos, f32 level, f32 style) {
    if (style != e->style) e->style = style;
    else style = -2.0f;
    camera_clip_planes(e->handle, pos, (s32)D_801141B0, level, style);
}
'''
O='''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'''
PL1=(O,'                layer_place(entry,(f32 *)((u8 *)model+556),level,style);')
HD=('#define MODE(m)',H)
V={
 'ls_stop':[M2,P2,('#define MODE(m)',H.replace('#define MODE(m)',STOP+'#define MODE(m)'))],
 'ls_stop_start':[M2,P2,S1,('void func_800E05F0(ModelView *model)',START+'void func_800E05F0(ModelView *model)'),('#define MODE(m)',H.replace('#define MODE(m)',STOP+'#define MODE(m)'))],
 'ls_start':[M2,S1,('void func_800E05F0(ModelView *model)',START+'void func_800E05F0(ModelView *model)'),HD],
}

V={k:v for k,v in V.items() if k=='ls_stop'}
