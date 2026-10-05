/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018B8C.c to the production
 * canonical types (include/sequence_context.h, valueFC6); code unchanged. Wave 4. */
#include "sequence_context.h"

extern unsigned char D_8002C630;
unsigned short func_80018B8C(unsigned int identifier)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF && (index & 0x80000000) == 0) return D_80043EB8[index].valueFC6;
    }
    return 0;
}
