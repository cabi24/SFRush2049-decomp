A='''                    entry->level=level;
                    client_sync(entry->handle,level);'''
V={
 'o1':[(A,'''                    client_sync(entry->handle,entry->level=level);''')],
 'o2':[(A,'''                    client_sync(entry->handle,(entry->level=level));''')],
}
