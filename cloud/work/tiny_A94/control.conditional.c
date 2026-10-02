/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Node {u8 other0[4];struct Node *next;u8 other8[4];u32 size;u8 other16[4];s8 disabled;u8 other21[3];} Node;
typedef struct Owner {u8 other0[8];Node *head;} Owner;
typedef struct Queue Queue;
extern Queue D_80152770;
extern Owner *D_801527C8;
extern int osRecvMesg(Queue *,void **,int);
extern int osJamMesg(Queue *,void *,int);
u32 audio_output_setup(Owner *owner)
{
    Node *node;
    Owner *active;
    u32 size;
    osRecvMesg(&D_80152770,0,1);
    active=owner!=0?owner:D_801527C8;
    size=0;
    for(node=active->head;node!=0;node=node->next) {
        if(node->disabled==0)size+=node->size;
    }
    osJamMesg(&D_80152770,0,0);
    return size;
}
