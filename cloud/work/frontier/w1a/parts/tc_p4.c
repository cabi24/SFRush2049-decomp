void *audio_task_complete(Heap *heap, u32 size)
{
    Heap *h;
    u32 *slot;
    HandleTable *t;
    u32 i;

    osRecvMesg(D_80152770, 0, 1);
    h = heap_or_default(heap);
    t = &h->tbl;
    if (t->slots == 0) {
        t->count = h->maxHandles;
        t->slots = audio_helper(h->maxHandles * 4, h, 0, 1);
        memset(t->slots, 0, h->maxHandles * 4);
        t->next = 0;
    }
    i = 0;
    for (;;) {
        slot = t->slots;
        for (; i < t->count; i++, slot++) {
            if (*slot == 0) {
                break;
            }
        }
        if (i < t->count) {
            break;
        }
        i = 0;
        if (t->next == 0) {
            t->next = audio_helper(12, h, 0, 1);
            t->next->count = h->maxHandles;
            t->next->slots = audio_helper(h->maxHandles * 4, h, 0, 1);
            memset(t->next->slots, 0, h->maxHandles * 4);
            t->next->next = 0;
        }
        t = t->next;
    }
    *slot = (u32)audio_helper(size, h, slot, 0);
    osJamMesg(D_80152770, 0, 0);
    return slot;
}
