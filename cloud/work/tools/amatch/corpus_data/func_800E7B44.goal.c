void func_800E7B44(Heap *heap, u16 count, u16 a)
{

    if (a == 0) {
    }
    heap->magic = 0xFEDCBA98;
    heap->next = 0;
    heap->first = (Block *)(((u32)heap + 67) & ~31);
    heap->first->magic = 0xFEDCBA98;
    heap->first->next = 0;
    heap->first->prev = 0;
    heap->first->size = heap->end - (u32)heap->first - 32;
    heap->first->owner = 0;
    heap->first->used = 0;
    heap->first->tag = 0;
    heap->first->pad16[0] = 0;
    heap->last = heap->first;
    heap->tbl.count = count;
    heap->maxHandles = a;
    heap->tbl.next = 0;
    if (count > 0) {
        heap->tbl.slots = audio_helper(count * 4, heap, 0, 1);
        memset(heap->tbl.slots, 0, count * 4);
    } else {
        heap->tbl.slots = 0;
    }
}