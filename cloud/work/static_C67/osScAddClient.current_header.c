/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
/* Grounded missing declaration; private replay only, not a shared header edit. */
extern u32 osSetGlobalIntMask(u32);
void osScAddClient(OSSched *scheduler, OSScClient *client, OSMesgQueue *queue) {
    u32 mask;
    mask = osSetGlobalIntMask(1);
    client->msgQueue = queue;
    client->next = scheduler->clientList;
    scheduler->clientList = client;
    osSetGlobalIntMask(mask);
}
