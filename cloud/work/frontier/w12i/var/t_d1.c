static void layer_set(LayerState *state, f32 level, f32 style)
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
static void layer_stop(LayerState *e, s32 mode)
{
    if (mode==2) scheduler_recv(e->handle);
    else results_screen_update(e->handle);
    player_conditional_call(e);
}
void func_800E0048(LayerState *e, f32 *pos, f32 level, f32 style)
{
    camera_clip_planes(e->handle,pos,D_801141B0,level,style);
}
/*
 * func_800E05F0 (w12c): NEAR-MISS, 12 ops rows / 43 word rows in the unit (w11f: 60 ops rows).
 *  - natural literals 0.6f / 0.1f (retail .rodata 0x8012438C / 0x80124390 are this function's own
 *    literals, not globals; verify at splice); D_8002EB90 volatile; frame_sync's 2nd parameter s32 here;
 *  - `style = style<value ? value : style` ternary max updates; `D_8011F060[i]/2.0f + 1` (int 1);
 *  - `weighted = model->power[i]*D_8011F060[i]` (uopt swaps operands: this gives retail's D*power);
 *  - `level = value = 0.0f` (dead init of value: makes value's web precede weighted's -> f2/f12);
 *  - entry->handle read directly (no handle local); `& 0x10` flag test with the w4/w56 operand order.
 *  - layer_set / layer_stop static inlines + block-local pos only reproduce the 224-byte frame: a
 *    hypothesis (the image has no stubs for two deleted statics near here).
 * Open residual (RESULTS.md): a1/a2 tie (&D_8011F060 vs const 4), `.alias $8,$sp` after
 * player_conditional_call (proven by listing edit to reorder mfc1/swc1 at client_sync), as1 hoisting of
 * the camera_clip_planes a2/a3 setup into the compare block, and two stack-home offsets.
 */
#define MODE(m) FIELD(m,s8,1996)
void func_800E05F0(ModelView *model)
{
    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 style;
    f32 weighted,value;
    s32 i;
    s32 d1,d2,d3;
    f32 level;
    if (!D_8010FFC0) return;
    if (D_8010FFCC[original_slot]) {
        D_8010FFCC[original_slot]=0;
        return;
    }
    D_8010FFCC[original_slot]=1;
    if (!D_8010FFC4[original_slot]) return;
    func_800E0050((MODELDAT *)model);
    slot=model->index;
    if (MODE(model)==2) {
        style=0.5f; level=value=0.0f;
        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {
                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;
            }
        }
        if (D_8011F060[0] < level) {}
        entry=&D_80140640[slot];
        if(entry->handle==-1 && level>0.0f) {
            if(MODE(model)==2) {
                entry->handle=frame_sync(36,model->index,2,2);
            } else {
                entry->handle=camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
            }
        }
        if(entry->handle!=-1) {
            if(level<=0.0f) {
                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);
            } else if(MODE(model)==2) {
                if (level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if (style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }
            } else {
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                func_800E0048(entry,(f32 *)((u8 *)model+556),level,style);
            }
        }
    }
    if(MODE(model)!=2)return;
    func_800DFBA0(model);
    mode_select_handler(model);
    if(D_8002EB90<D_80110020[original_slot])D_80110020[original_slot]=0.0f;
    if(!(FIELD(model,s32,2004)&0x10) &&
       (FIELD(input_rec0,s32,original_slot*76+4)&FIELD(input_rec0,s32,original_slot*76+56)) &&
       D_8002EB90-D_80110020[original_slot]>0.1f) {
        D_80110020[original_slot]=D_8002EB90;
        D_801407E0[original_slot]=frame_sync(D_801115CD[original_slot][FIELD(model,u8,8)]+26,original_slot,1,4);
    }
    if(FIELD(model,s16,1628)==1) {
        D_801407C0[original_slot]=frame_sync(25,original_slot,2,1);
        FIELD(model,s16,1628)=2;
    } else if(FIELD(model,s16,1628)==3) {
        scheduler_recv(D_801407C0[original_slot]);
        D_801407C0[original_slot]=-1;
        FIELD(model,s16,1628)=0;
    }
}
