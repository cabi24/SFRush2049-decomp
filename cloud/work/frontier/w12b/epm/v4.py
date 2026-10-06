OLD="""    i = car->unk35C;
    if (i >= 0 && (car->unk35D == 0 || car->unk35D == 1)) {
        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
V={}
V['dead_pre']="""    i = car->unk35C;
    pad1 = 1 << i;
    if (i >= 0 && (car->unk35D == 0 || car->unk35D == 1)) {
        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
V['dis_pre']="""    i = car->unk35C;
    if (1 << i);
    if (i >= 0 && (car->unk35D == 0 || car->unk35D == 1)) {
        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
V['dead_pre_lf']="""    i = car->unk35C;
    pad1 = 1 << i;
    if (i >= 0 && (car->unk35D == 0 || car->unk35D == 1)) {
        poly = &D_8015B268[v->objnum];
        poly->flags = poly->flags & ~(1 << i);"""
V['dead_in']="""    i = car->unk35C;
    if (i >= 0 && (pad1 = 1 << i, car->unk35D == 0 || car->unk35D == 1)) {
        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
