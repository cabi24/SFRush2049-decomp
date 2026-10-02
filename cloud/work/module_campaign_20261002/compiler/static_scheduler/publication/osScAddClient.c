/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
/* SDK client registration; native physical helper is osSetGlobalIntMask. */
void osScAddClient(OSSched *scheduler,OSScClient *client,OSMesgQueue *queue) {
 u32 mask;
 mask=osSetGlobalIntMask(1);
 client->msgQueue=queue;
 client->next=scheduler->clientList;
 scheduler->clientList=client;
 osSetGlobalIntMask(mask);
}
