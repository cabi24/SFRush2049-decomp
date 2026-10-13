/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80012234: fetch one 0x100-byte audio book/cache block by source key.
 * Static lib_11640, IDO 7.1. No proven arcade ancestor. The existing AudioNode
 * layout and active/free heads are shared with accepted 11F60/12200; the
 * two-argument command writer and twelve-byte DMA record follow accepted 12730.
 * A hit refreshes countdown to two and returns its buffer without queuing.
 * A miss prepends a free node, or reuses the first active countdown-zero node,
 * then queues {0x100, source, buffer}. The caller must provide a hit, a free
 * node, or an expired active node; retail also dereferences null on exhaustion.
 * A consumed free head keeps its previous link, as retail does.
 * List/queue storage and capacity must be valid. No artificial guards, helpers,
 * formals, padding, volatile accesses, or code-shaping devices are used.
 * The complete 372-byte function matches standalone. The independent checker
 * owns static installation, locking, the cartridge gate and merging. */
typedef struct AudioNode { struct AudioNode *next; struct AudioNode *previous; unsigned char *buffer; unsigned int unknown0C; unsigned short countdown; unsigned short unknown12; } AudioNode;
typedef struct DmaEntry_80012234 { int length; unsigned int source; unsigned char *destination; } DmaEntry_80012234;
extern AudioNode *D_80038344;
extern AudioNode *D_80038348;
extern void *D_8003802C;
extern unsigned int D_80038038;
extern unsigned int D_8003803C;
extern unsigned short D_80038028;
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
unsigned char *func_80012234(unsigned int source)
{
    AudioNode *node;
    for (node = D_80038344; node != 0; node = node->next) {
        if (source == node->unknown0C) {
            node->countdown = 2;
            return node->buffer;
        }
    }
    if (D_80038348 != 0) {
        node = D_80038348;
        D_80038348 = node->next;
        if (D_80038348 != 0) {
            D_80038348->previous = 0;
        }
        if (D_80038344 != 0) {
            D_80038344->previous = node;
        }
        node->next = D_80038344;
        D_80038344 = node;
    } else {
        for (node = D_80038344; node != 0; node = node->next) {
            if (node->countdown == 0) {
                break;
            }
        }
    }
    node->countdown = 2;
    node->unknown0C = source;
    D_80038038 += 12;
    D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].source = source;
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].length = 0x100;
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].destination = node->buffer;
    D_80038028++;
    return node->buffer;
}
