/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioCacheNode {
    struct AudioCacheNode *next;
    struct AudioCacheNode *previous;
} AudioCacheNode;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
/* Caller 80012730 passes its fourth word; native homes this genuine unused formal. */
AudioCacheNode *func_80012660(unsigned int tag)
{
    AudioCacheNode *node;
    if (D_8003835C != 0) {
        node = D_8003835C;
        D_8003835C = node->next;
        if (D_8003835C != 0) {
            D_8003835C->previous = 0;
        }
        if (D_80038358 == 0) {
            node->next = D_80038354;
            if (D_80038354 != 0) {
                D_80038354->previous = node;
            }
            D_80038354 = node;
            D_80038358 = node;
        } else {
            node->next = 0;
            node->previous = D_80038358;
            D_80038358->next = node;
            D_80038358 = node;
        }
        return node;
    }
    node = D_80038354;
    node->next->previous = 0;
    D_80038354 = node->next;
    D_80038358->next = node;
    node->next = 0;
    node->previous = D_80038358;
    D_80038358 = node;
    return node;
}
