/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct MesgQueue MesgQueue;
typedef struct Client {struct Client *next;MesgQueue *queue;} Client;
typedef struct Scheduler {u8 opaque0[608];Client *clients;} Scheduler;
extern u32 osSetGlobalIntMask(u32);
void osScAddClient(Scheduler *scheduler,Client *client,MesgQueue *queue) {
    u32 mask;
    mask=osSetGlobalIntMask(1);
    client->queue=queue;
    client->next=scheduler->clients;
    scheduler->clients=client;
    osSetGlobalIntMask(mask);
}
