void *audio_helper(u32 size, Heap *heap, void *owner, s32 tag)
{
    Block *b;
    Block *n;
    u32 total = 0;
    u32 largest = 0;
    u32 bs;

    size = (size + 31) & ~31;
    for (b = heap->first; b != 0; b = b->next) {
        if (b->used == 0) {
            bs = b->size;
            total += bs;
            if (largest < bs) {
                largest = bs;
            }
            if (bs >= size) {
                break;
            }
        }
    }
    bs = b->size;
    if (bs - size >= 64) {
        n = (Block *)((u8 *)b + size + 32);
        n->next = b->next;
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->prev = b;
        n->size = b->size - size - 32;
        n->owner = 0;
        n->used = 0;
        n->tag = 0;
        n->pad16[0] = 0;
        n->magic = 0xFEDCBA98;
        b->next = n;
        b->size = size;
    }
    b->used = 1;
    b->owner = owner;
    b->tag = tag;
    return (u8 *)b + 32;
}