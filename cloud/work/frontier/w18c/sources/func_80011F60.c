/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80011F60 - lib_11640 (IDO 7.1). Allocate and link the audio buffer-node pool: COUNT
 * 0x14-byte nodes (D_80038338) and COUNT 256-byte buffers (D_8003833C, data cache invalidated)
 * from the allocator hook D_80038018; node i owns buffer i; the nodes form a doubly linked free
 * list headed by D_80038348; the active list D_80038344 and counter D_80038340 start empty.
 * Base: w17g lockonly/func_80011F60.c (D10 body verbatim, callee osInvalDCache; LOCK-exact under
 * 7.1, TU-blocked by its typed externs). w18c: the node type is the TU's AudioNode (func_80012200,
 * which reads its countdown at +0x10) completed to the 0x14-byte layout D10 recovered (next,
 * previous, buffer, +0x0C, countdown); the TU edit moves that typedef above this slot and gives
 * func_800121BC's `extern void *D_8003833C, *D_80038338` their element types (RESULTS.md
 * "TU edits", proven byte-neutral). Casting `void *` globals instead flips 15 addu operand
 * orders (true=150), so the typed globals are load-bearing. No shaping devices. */
typedef struct AudioNode { struct AudioNode *next; struct AudioNode *previous; unsigned char *buffer; unsigned int unknown0C; unsigned short countdown; unsigned short unknown12; } AudioNode;
extern AudioNode *D_80038344;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osInvalDCache(void *, int);
extern AudioNode *D_80038338;
extern unsigned char *D_8003833C;
extern unsigned int D_80038340;
extern AudioNode *D_80038348;

void func_80011F60(int count)
{
    int i;
    D_80038338 = D_80038018(count * sizeof(AudioNode), 0);
    D_8003833C = D_80038018(count * 256, 0);
    osInvalDCache(D_8003833C, count * 256);
    D_80038344 = 0;
    D_80038348 = D_80038338;
    D_80038338[0].buffer = D_8003833C;
    D_80038338[0].previous = 0;
    for (i = 1; i < count; i++) {
        D_80038338[i].previous = &D_80038338[i - 1];
        D_80038338[i - 1].next = &D_80038338[i];
        D_80038338[i].buffer = &D_8003833C[i * 256];
    }
    D_80038338[i - 1].next = 0;
    D_80038340 = 0;
}
