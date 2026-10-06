P=('''                if(MODE(model)==2)scheduler_recv(handle);
                else results_screen_update(handle);
                player_conditional_call(entry);''','''                player_conditional_check(entry,MODE(model)!=2);''')
V={'pcc':[P]}
