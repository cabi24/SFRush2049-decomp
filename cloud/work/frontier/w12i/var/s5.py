ST='''                player_conditional_call(entry);'''
V={
 'k1s':[(ST,ST+'\n                if (entry) {}')],
 'k2s':[(ST,ST+'\n                if (entry->handle) {}')],
 'k3s':[(ST,ST+'\n                if (entry->level < level) {}')],
}
