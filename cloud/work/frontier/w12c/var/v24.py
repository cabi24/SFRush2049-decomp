M=('''                if(level!=entry->level) {
                    entry->level=level;
                    client_sync(entry->handle,level);
                }
                if(style!=entry->style) {
                    entry->style=style;
                    entity_hierarchy_update(entry->handle,style);
                }''','''                mode_select_input(entry,level,style);''')
P=('''                if(MODE(model)==2)scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''','''                player_conditional_check(entry,MODE(model)!=2);''')
V={'msi':[M],'msi_pcc':[M,P]}
