SET=('''            } else if(MODE(model)==2) {
                layer_set(entry,level,style);
            } else {
                f32 *pos=(f32 *)((u8 *)model+556);
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style);
            }''','''            } else if(MODE(model)==2) {
                mode_select_input(entry,level,style);
            } else {
                func_800E0048(entry,(f32 *)((u8 *)model+556),level,style);
            }''')
STOP=('''                layer_stop(entry,MODE(model));''','''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''')
STOP2=(STOP[0],'''                player_conditional_check(entry,MODE(model)!=2);''')
HELP=('''void func_800E0050(MODELDAT *m) {''','''void func_800E0048(LayerState *e, f32 *pos, f32 level, f32 style)
{
    if (style!=e->style) e->style=style;
    else style=-2.0f;
    camera_clip_planes(e->handle,pos,(s32)D_801141B0,level,style);
}
void func_800E0050(MODELDAT *m) {''')
V={'x':[SET,STOP,HELP],'x_pcc':[SET,STOP2,HELP]}
