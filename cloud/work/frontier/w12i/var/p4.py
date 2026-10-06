SETD=('''                layer_set(entry,level,style);''','''                if (level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if (style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }''')
STOP=('''                layer_stop(entry,MODE(model));''','''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''')
CAM=('''                f32 *pos=(f32 *)((u8 *)model+556);
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style);''','''                func_800E0048(entry,(f32 *)((u8 *)model+556),level,style);''')
HELP=('''void func_800E0050(MODELDAT *m) {''','''void func_800E0048(LayerState *e, f32 *pos, f32 level, f32 style)
{
    if (style!=e->style) e->style=style;
    else style=-2.0f;
    camera_clip_planes(e->handle,pos,(s32)D_801141B0,level,style);
}
void func_800E0050(MODELDAT *m) {''')
D3=('''    f32 style,level;''','''    s32 d1,d2,d3;
    f32 style,level;''')
D3b=('''    f32 style,level;''','''    s32 d1,d2,d3;
    f32 style;
    f32 weighted,value;
    s32 i;
    f32 level;''')
D3c=('''    f32 weighted,value;
    s32 i;''','')
V={'c1':[SETD,STOP,CAM,HELP],'c1d':[SETD,STOP,CAM,HELP,D3],'c1e':[SETD,STOP,CAM,HELP,D3b,D3c]}
