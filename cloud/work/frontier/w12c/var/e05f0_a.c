#define MODE(m) FIELD(m,s8,1996)
void func_800E05F0(ModelView *model)
{
    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 style,level;
    f32 weighted,value;
    s32 i,handle;
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
        style=0.5f; level=0.0f;
        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {
                weighted=D_8011F060[i]*model->power[i];
                value=weighted-D_8011F060[i]*0.5f+1.0f;
                if(style<value)style=value;
                value=weighted*D_8012438C;
                if(level<value)level=value;
            }
        }
        entry=&D_80140640[slot];
        handle=entry->handle;
        if(handle==-1 && level>0.0f) {
            if(MODE(model)==2) {
                entry->handle=handle=frame_sync(36,model->index,2,2);
            } else {
                entry->handle=handle=camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,400.0f,0.0f,1.0f,0.0f,36,slot,0,130);
            }
        }
        if(handle!=-1) {
            if(level<=0.0f) {
                if(MODE(model)==2)scheduler_recv(handle);
                else results_screen_update(handle);
                player_conditional_call(entry);
            } else if(MODE(model)==2) {
                if(level!=entry->level) {
                    entry->level=level;
                    client_sync(handle,level);
                }
                if(style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }
            } else {
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);
            }
        }
    }
    if(MODE(model)!=2)return;
    func_800DFBA0(model);
    mode_select_handler(model);
    if(D_8002EB90<D_80110020[original_slot])D_80110020[original_slot]=0.0f;
    if(!(FIELD(model,s32,2004)&0x10) &&
       (FIELD(input_rec0,s32,original_slot*76+56)&FIELD(input_rec0,s32,original_slot*76+4)) &&
       D_8002EB90-D_80110020[original_slot]>D_80124390) {
        D_80110020[original_slot]=D_8002EB90;
        D_801407E0[original_slot]=frame_sync(D_801115CD[original_slot*13+FIELD(model,u8,8)]+26,original_slot,1,4);
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
