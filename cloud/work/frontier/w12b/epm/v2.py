OLD="""        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
V={}
V['m_pad']="""        pad1 = 1 << i;
        poly = &D_8015B268[v->objnum];
        poly->flags = ~pad1 & poly->flags;"""
V['m_pad2']="""        poly = &D_8015B268[v->objnum];
        pad1 = 1 << i;
        poly->flags = ~pad1 & poly->flags;"""
V['m_xlu']="""        poly = &D_8015B268[v->objnum];
        xlu = 1 << i;
        poly->flags = ~xlu & poly->flags;"""
V['m_body']="""        poly = &D_8015B268[v->objnum];
        body = 1 << i;
        poly->flags = ~body & poly->flags;"""
V['m_padn']="""        poly = &D_8015B268[v->objnum];
        pad1 = ~(1 << i);
        poly->flags = pad1 & poly->flags;"""
V['m_pad_lf']="""        poly = &D_8015B268[v->objnum];
        pad1 = 1 << i;
        poly->flags = poly->flags & ~pad1;"""
