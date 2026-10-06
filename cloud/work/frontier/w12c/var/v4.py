PCC='''void player_conditional_check(LayerState *rec, s32 arg1) {
    if (arg1 != 0) {
        results_screen_update(rec->handle);
    } else {
        scheduler_recv(rec->handle);
    }
    player_conditional_call(rec);
}'''
V={
 'base':[],
 'proto':[(PCC,'void player_conditional_check(LayerState *rec, s32 arg1);')],
 'e0048':[(PCC,PCC.replace('void player_conditional_check','void func_800E0048')),('player_conditional_check(&set->rec[3], 1);','func_800E0048(&set->rec[3], 1);')],
 'e0048s':[(PCC,PCC.replace('void player_conditional_check','static void func_800E0048')),('player_conditional_check(&set->rec[3], 1);','func_800E0048(&set->rec[3], 1);')],
}
