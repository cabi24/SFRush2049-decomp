H='''void best_times_display(s16 idx)
{
    Slot *s = &D_80140808[idx];
    s32 i;
    s->e[0].c = 0;
    s->e[0].a = 0.0f;
    s->e[0].b = 0.0f;
'''
T='''    scheduler_recv(D_80140AE0[idx]);
    D_80140AE0[idx] = -1;
    D_80140A08[idx] = 0;
    D_80140B10[idx] = 0.0f;
    D_80140BE0[idx] = 0.0f;
    D_80142518[idx] = 0.0f;
}
'''
V={
'dowhile': H+'''    i = 1;
    do {
        s->e[i].c = 0;
        s->e[i].a = 0.0f;
        s->e[i].b = 0.0f;
        i++;
    } while (i < 5);
'''+T,
'for1': H+'''    for (i = 1; i < 5; i++) {
        s->e[i].c = 0;
        s->e[i].a = 0.0f;
        s->e[i].b = 0.0f;
    }
'''+T,
'glob': '''void best_times_display(s16 idx)
{
    s32 i;
    D_80140808[idx].e[0].c = 0;
    D_80140808[idx].e[0].a = 0.0f;
    D_80140808[idx].e[0].b = 0.0f;
    for (i = 1; i < 5; i++) {
        D_80140808[idx].e[i].c = 0;
        D_80140808[idx].e[i].a = 0.0f;
        D_80140808[idx].e[i].b = 0.0f;
    }
'''+T,
}
