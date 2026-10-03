#include "scheduler_native.h"
/* SDK client registration; native physical helper is osSetGlobalIntMask. */
void osScAddClient(OSSched *scheduler,OSScClient *client,OSMesgQueue *queue) {
 u32 mask;
 mask=osSetGlobalIntMask(1);
 client->msgQueue=queue;
 client->next=scheduler->clientList;
 scheduler->clientList=client;
 osSetGlobalIntMask(mask);
}
