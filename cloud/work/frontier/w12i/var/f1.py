A='''            if(level<=0.0f) {
                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);
            } else if(MODE(model)==2) {'''
SET='''                if (level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if (style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }'''
CAM='''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                func_800E0048(entry,(f32 *)((u8 *)model+556),level,style);'''
OLD=A+'\n'+SET+'''
            } else {
'''+CAM+'''
            }'''
STOP='''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);'''
F1='''            if(level>0.0f) {
                if(MODE(model)==2) {
'''+SET+'''
                } else {
'''+CAM+'''
                }
            } else {
'''+STOP+'''
            }'''
F1b='''            if(!(level<=0.0f)) {
                if(MODE(model)==2) {
'''+SET+'''
                } else {
'''+CAM+'''
                }
            } else {
'''+STOP+'''
            }'''
V={'F1':[(OLD,F1)],'F1b':[(OLD,F1b)]}
