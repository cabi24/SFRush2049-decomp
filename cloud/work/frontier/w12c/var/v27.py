A=('''    if (level!=state->level) {
        state->level=level;
        client_sync(state->handle,level);
    }''','''    if (level!=state->level) {
        client_sync(state->handle,state->level=level);
    }''')
V={'asg':[A]}
