s32 track_collision_setup(s32 index, s32 mode) {
    C96Handle *node;
    C96Handle *next;
    D_80144030[index].result = 0;
    if (!D_80144030[index].modified) return 1;
    pfs_lock();
    D_80144030[index].busy = 1;
    node = D_80144D68[index].head;
    while (node) {
        next = node->file->next;
        if (node->file->data && D_80144030[index].slots[node->file->slot].modified) {
            if (!track_data_decompress(node, mode)) break;
            D_80144030[index].result = 1;
        }
        node = next;
    }
    D_80144030[index].busy = 0;
    pfs_unlock();
    return node == 0;
}
