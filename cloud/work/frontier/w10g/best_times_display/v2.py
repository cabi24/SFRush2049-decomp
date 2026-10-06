T='''    scheduler_recv(D_80140AE0[idx]);
    D_80140AE0[idx] = -1;
    D_80140A08[idx] = 0;
    D_80140B10[idx] = 0.0f;
    D_80140BE0[idx] = 0.0f;
    D_80142518[idx] = 0.0f;
}
'''
V={
'for0p': '''void best_times_display(s16 idx)
{
    Slot *s = &D_80140808[idx];
    s32 i;
    for (i = 0; i < 5; i++) {
        s->e[i].c = 0;
        s->e[i].a = 0.0f;
        s->e[i].b = 0.0f;
    }
'''+T,
'for0g': '''void best_times_display(s16 idx)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        D_80140808[idx].e[i].c = 0;
        D_80140808[idx].e[i].a = 0.0f;
        D_80140808[idx].e[i].b = 0.0f;
    }
'''+T,
}
