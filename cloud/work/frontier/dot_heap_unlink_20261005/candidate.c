/* Research NONMATCH: unlink a selected heap from the default chain, then release
 * it with tag 1 under the real message-queue lock. The unchanged accepted
 * func_800A51D8 selector supplies null/default selection. The incoming pointer
 * is then reused for the successor traversal; all values serve live operations.
 * Types and declarations come unchanged from the accepted heap-compactor TU.
 */
void func_800E7A98(Heap *heap) {
    Heap *chosen, *current;
    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);
    chosen = func_800A51D8(heap);
    current = D_801527C8;
    while (current != NULL) {
        heap = current->next;
        if (chosen == heap) {
            current->next = chosen->next;
            break;
        }
        current = heap;
    }
    audio_reverb_update((u32)chosen, 1);
    osJamMesg((OSMesgQueue *)&D_80152770, NULL, 0);
}
