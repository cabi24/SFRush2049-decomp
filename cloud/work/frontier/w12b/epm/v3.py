OLD="""        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
V={}
V['dup']="""        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags & ~(1 << i);"""
V['dup2']="""        poly = &D_8015B268[v->objnum];
        poly->flags = (poly->flags | (1 << i)) ^ (1 << i);"""
