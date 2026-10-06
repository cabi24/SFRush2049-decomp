SET=('''                layer_set(entry,level,style);''','''                func_800E0048(entry,level,style);''')
HELP=('''void func_800E0050(MODELDAT *m) {''','''void func_800E0048(LayerState *state, f32 level, f32 style)
{
    if (level!=state->level) {
        state->level=level;
        client_sync(state->handle,level);
    }
    if (style!=state->style) {
        state->style=style;
        entity_hierarchy_update(state->handle,style);
    }
}
void func_800E0050(MODELDAT *m) {''')
STOP=('''                layer_stop(entry,MODE(model));''','''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''')
PCC=('''                layer_stop(entry,MODE(model));''','''                player_conditional_check(entry,MODE(model)!=2);''')
NOPOS=('''                f32 *pos=(f32 *)((u8 *)model+556);''','')
NOPOS2=('''camera_clip_planes(entry->handle,pos,''','''camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),''')
V={'e48':[SET,HELP],'e48_stop':[SET,HELP,STOP],'e48_pcc':[SET,HELP,PCC],'e48_stop_nopos':[SET,HELP,STOP,NOPOS,NOPOS2],'e48_pcc_nopos':[SET,HELP,PCC,NOPOS,NOPOS2]}
