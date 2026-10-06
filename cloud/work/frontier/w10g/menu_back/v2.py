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
'len': H.replace('s32 n;','s32 n;\n    s32 len;')+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        len = n;
        m->count = n / 3;
        format_string_parse(m->buffer, len);
'''+T,
'len2': H.replace('s32 n;','s32 n;\n    s32 len;')+'''        len = n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = len / 3;
        format_string_parse(m->buffer, n);
'''+T,
'u32': H.replace('s32 n;','u32 n;')+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = (s32)n / 3;
        format_string_parse(m->buffer, n);
'''+T,
'cntfirst': H+'''        m->count = inflate_entry_alt(m->data, m->size, m->buffer);
        n = m->count;
        m->count /= 3;
        format_string_parse(m->buffer, n);
'''+T,
'reg': H.replace('s32 n;','register s32 n;')+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);
'''+T,
'twoif': H+'''        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);
        m->loaded = 1;
    }
}
''',
}
del V['twoif']
