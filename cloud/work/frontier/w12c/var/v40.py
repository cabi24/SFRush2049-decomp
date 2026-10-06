A='''        state->level=level;
        client_sync(state->handle,level);'''
V={'oneline':[(A,'''        state->level=level; client_sync(state->handle,level);''')],
   'swap_lines':[(A,'''        client_sync(state->handle,
        state->level=level);''')]}
