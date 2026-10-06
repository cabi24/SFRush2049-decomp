STOP=('''                layer_stop(entry,MODE(model));''','''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''')
SETD=('''                layer_set(entry,level,style);''','''                if (level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if (style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }''')
D1=('''    s32 original_slot=model->index;''','''    s32 dummy;
    s32 original_slot=model->index;''')
NOPOS=('''                f32 *pos=(f32 *)((u8 *)model+556);''','')
NOPOS2=('''camera_clip_planes(entry->handle,pos,''','''camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),''')
V={'p0':[],'p1':[D1],'p2':[STOP],'p3':[STOP,SETD],'p4':[NOPOS,NOPOS2],'p5':[STOP,SETD,NOPOS,NOPOS2]}
