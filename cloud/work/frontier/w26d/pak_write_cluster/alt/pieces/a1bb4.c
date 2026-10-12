void func_800A1BB4(s32 index) {
    C96Handle *p, *next;
    C96File *data;

    if (D_80144030[index].modified == 0) return;
    next = p = D_80144D68[index].head;
    while (p != 0) {
        data = p->file;
        next = data->next;
        if (data->data != 0) {
            if (D_80144030[index].slots[data->slot].modified != 0) break;
        }
        p = next;
    }
    if (p == 0) D_80144030[index].modified = 0;
}
