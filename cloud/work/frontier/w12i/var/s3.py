ST='''                player_conditional_call(entry);'''
EH='''                    entity_hierarchy_update(entry->handle,style);
                }'''
V={
 's_live2':[(ST,ST+'\n                entry->level=0.0f;'),(EH,EH+'\n                entry->level=0.0f;')],
}
