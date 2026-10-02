/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "../src/rom/rom_tu.h"
#include "static_scheduler_context.h"
void osScAddClient(OSSched *scheduler, OSScClient *client, OSMesgQueue *queue) {
    u32 mask;
    mask = osSetGlobalIntMask(1);
    client->msgQueue = queue;
    client->next = scheduler->clientList;
    scheduler->clientList = client;
    osSetGlobalIntMask(mask);
}
