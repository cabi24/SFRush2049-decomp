OLD = """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);"""
V = {
 'q1': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse((s32)m->buffer, n);""",
 'q2': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        if (n) {}
        format_string_parse(m->buffer, n);""",
 'q3': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        if (n) {}
        m->count = n / 3;
        format_string_parse(m->buffer, n);""",
 'q4': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, (s32)n);""",
 'q5': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n + 0);""",
 'q6': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(*&m->buffer, n);""",
 'q7': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(((MenuData *)m)->buffer, n);
        if (m) {}""",
 'q8': """        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);
        if (n) {}""",
}
