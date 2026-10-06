H='''void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 n;

    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
'''
T='''        m->loaded = 1;
    }
}
'''
V={
'base': H+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);
'''+T,
'cnt': H+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        {s32 c = n / 3; m->count = c;}
        format_string_parse(m->buffer, n);
'''+T,
'assign': H+'''        m->count = (n = inflate_entry_alt(m->data, m->size, m->buffer)) / 3;
        format_string_parse(m->buffer, n);
'''+T,
'nodecl': H.replace('    s32 n;\n','')+'''        {s32 n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);}
'''+T,
'ptrarg': H+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(*(s32 *)&m->buffer, n);
'''+T,
}
