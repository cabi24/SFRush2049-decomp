/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018184 - lib_17dc0 (IDO 7.1). Service the current context's timed-node list
 * (D_8004BE80->active, head at 0xF78): a node whose clock (whole + half120) has reached its
 * deadline fires func_8001B8C4(identifier) (lib_1a660, locked) and is released
 * (func_800174D0, locked, same TU); otherwise its 16.16 clock advances by the tempo increment.
 * Returns 0 when the list is empty, else 1.
 * Base: w17g lockonly/func_80018184.c (D10 body; LOCK-exact under 7.1, TU-blocked by its private
 * Context/TimedNode views). w18c: same body on include/sequence_context.h with the fields named
 * (w18c/include_patch: SequenceNode deadline0C/fraction10/whole14, SequenceContext
 * fraction118/whole11C; layout unchanged). Casts are D10's. */
#include "sequence_context.h"
extern int func_8001B8C4(unsigned int);

int func_80018184(void)
{
    SequenceNode *node;
    SequenceNode *next;
    int sum;
    if (D_8004BE80->active == 0) return 0;
    node = D_8004BE80->active;
    while (node != 0) {
        next = node->next;
        sum = (int)((unsigned int)node->whole14 + (unsigned int)D_8004BE80->half120);
        if (sum >= node->deadline0C) {
            func_8001B8C4(node->identifier);
            func_800174D0(node);
        } else {
            sum = (int)(node->fraction10 + D_8004BE80->fraction118);
            node->fraction10 = (unsigned int)sum & 65535;
            sum >>= 16;
            node->whole14 = (int)((unsigned int)node->whole14 + (unsigned int)sum +
                (unsigned int)D_8004BE80->whole11C);
        }
        node = next;
    }
    return 1;
}
