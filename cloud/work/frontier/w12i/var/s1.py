ST='''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(entry);'''
V={
 's_glob':[(ST,'''                if(MODE(model)==2) scheduler_recv(entry->handle);
                else results_screen_update(entry->handle);
                player_conditional_call(&D_80140640[slot]);''')],
 's_ifelse':[(ST,'''                if(MODE(model)==2) {scheduler_recv(entry->handle);
                player_conditional_call(entry);}
                else {results_screen_update(entry->handle);
                player_conditional_call(entry);}''')],
 's_pcc':[(ST,'''                player_conditional_check(entry,MODE(model)!=2);''')],
}
