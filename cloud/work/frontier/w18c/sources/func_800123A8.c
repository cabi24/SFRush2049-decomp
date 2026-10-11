/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800123A8 - lib_11640 (IDO 7.1). Allocate and link the audio cache-node pool: COUNT
 * 1536-byte buffers (D_8003834C, data cache invalidated) and COUNT 0x18-byte nodes (D_80038350)
 * from the allocator hook D_80038018; node i owns buffer i; all nodes go on the free list
 * D_8003835C; the in-use list (head D_80038354, tail D_80038358) starts empty.
 * Base: w17g lockonly/func_800123A8.c (D10 body verbatim, callee osInvalDCache; LOCK-exact under
 * 7.1, TU-blocked by its typed externs). w18c: the node type is the TU's AudioCacheNode
 * (func_80012660 uses next/previous) completed to D10's 0x18-byte layout; the TU edit moves that
 * typedef and its three list externs above this slot and gives func_8001261C's
 * `extern void *D_80038350, *D_8003834C` their element types (RESULTS.md "TU edits", proven
 * byte-neutral). No shaping devices. */
typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; unsigned int unknown08; unsigned int unknown0C; unsigned int unknown10; unsigned char *buffer; } AudioCacheNode;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osInvalDCache(void *, int);
extern AudioCacheNode *D_80038350;
extern unsigned char *D_8003834C;

void func_800123A8(unsigned short count)
{
    int i;
    D_8003834C = D_80038018(count * 1536, 0);
    osInvalDCache(D_8003834C, count * 1536);
    D_80038350 = D_80038018(count * sizeof(AudioCacheNode), 0);
    D_80038354 = 0;
    D_80038358 = 0;
    D_8003835C = D_80038350;
    D_80038350[0].previous = 0;
    D_80038350[0].buffer = D_8003834C;
    for (i = 1; i < count; i++) {
        D_80038350[i - 1].next = &D_80038350[i];
        D_80038350[i].previous = &D_80038350[i - 1];
        D_80038350[i].buffer = &D_8003834C[i * 1536];
    }
    D_80038350[i - 1].next = 0;
}
