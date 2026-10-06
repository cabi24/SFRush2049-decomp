ST='''                player_conditional_call(entry);'''
EH='''                    entity_hierarchy_update(entry->handle,style);
                }'''
V={
 'k1':[(ST,ST+'\n                if (entry) {}'),(EH,EH+'\n                if (entry) {}')],
 'k2':[(ST,ST+'\n                if (entry->handle) {}'),(EH,EH+'\n                if (entry->handle) {}')],
 'k3':[(ST,ST+'\n                if (entry->level) {}'),(EH,EH+'\n                if (entry->level) {}')],
}
