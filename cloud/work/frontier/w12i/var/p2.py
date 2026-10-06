MSI=('''                layer_set(entry,level,style);''','''                mode_select_input(entry,level,style);''')
STOP=('''                layer_stop(entry,MODE(model));''','''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);''')
PCC=('''                layer_stop(entry,MODE(model));''','''                player_conditional_check(entry,MODE(model)!=2);''')
V={'msi':[MSI],'msi_stop':[MSI,STOP],'pcc':[PCC],'msi_pcc':[MSI,PCC]}
