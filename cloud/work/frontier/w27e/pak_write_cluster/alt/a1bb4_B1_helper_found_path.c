C96Handle *a1bb4_find(s32 index) {
    C96Handle *p, *next;
    C96File *data;

    next = p = D_80144D68[index].head;
    while (p != 0) {
        data = p->file;
        next = data->next;
        if (data->data != 0) {
            if (D_80144030[index].slots[data->slot].modified != 0) return p;
        }
        p = next;
    }
    return p;
}
void func_800A1BB4(s32 index) {
    if (D_80144030[index].modified == 0) return;
    if (a1bb4_find(index) == 0) D_80144030[index].modified = 0;
}
