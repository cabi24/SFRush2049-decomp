void scheduler_recv(s32 h)
{
    Entity *e;
    Msg *m = 0;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e != 0) {
        m = func_80091B00();
        m->type = 6;
        m->ent = e;
        e->msgCount++;
    }
    osJamMesg(&D_80142728, 0, 0);
    if (m != 0) {
        osJamMesg(&D_801427A8, m, 0);
    }
}