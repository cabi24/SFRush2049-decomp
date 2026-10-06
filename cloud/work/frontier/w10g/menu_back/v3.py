V={
'early': '''void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 n;

    if (m->buffer != 0) return;
    (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
    m->buffer = *(s32 *)(*h)->h36;
    n = inflate_entry_alt(m->data, m->size, m->buffer);
    m->count = n / 3;
    format_string_parse(m->buffer, n);
    m->loaded = 1;
}''',
'nolocal': '''void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 n;

    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
        format_string_parse(m->buffer, n = inflate_entry_alt(m->data, m->size, m->buffer), m->count = n / 3);
        m->loaded = 1;
    }
}''',
'size': '''void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 size;
    s32 n;

    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
        n = inflate_entry_alt(m->data, m->size, m->buffer);
        size = n;
        m->count = size / 3;
        format_string_parse(m->buffer, size);
        m->loaded = 1;
    }
}''',
'retval': '''void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 n;

    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
        m->count = (n = inflate_entry_alt(m->data, m->size, m->buffer)) / 3;
        m->loaded = format_string_parse(m->buffer, n), 1;
    }
}''',
}
del V['nolocal']
