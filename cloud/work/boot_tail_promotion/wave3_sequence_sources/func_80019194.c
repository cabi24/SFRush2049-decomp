/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80019194.c; canonical native types.
 * Wave 3 copy: includes the production include/sequence_context.h. */
#include "sequence_context.h"

void func_80019194(unsigned char channel,unsigned short duration,unsigned int identifier,unsigned char flags)
{
    unsigned int original;
    int i;
    original=identifier;
    identifier=func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if (!(identifier & 0x80000000U)) {
            func_8001B9F8(channel,duration,D_80043EB8[identifier].channelFC4,flags,original);
            for (i=0;i<64;i++) {
                if (D_80043EB8[identifier].channels528[i] != D_80043EB8[identifier].channelFC4) {
                    func_8001B9F8(channel,duration,D_80043EB8[identifier].channels528[i],0,0);
                }
            }
        } else {
            identifier &= 0x7FFFFFFFU;
            switch (flags & 15) {
            case 0: D_80043EB8[identifier].valueFE0=channel; break;
            case 1: D_80043EB8[identifier].pendingFF0=0; break;
            case 2:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 8;
                break;
            case 3:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 128;
                break;
            }
        }
    }
}
