/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef unsigned int u32;
typedef signed char s8;
typedef struct Node {s32 word0;struct Node *next;s32 word8;u32 value;s32 word10;s8 status;} Node;
typedef struct List {s32 words[2];Node *head;} List;
extern char D_80152770[];
extern List *D_801527C8;
extern s32 osRecvMesg(void*,void**,s32),osJamMesg(void*,void*,s32);
u32 func_800E79F8(List *list) {
    Node *node;
    u32 maximum=0;
    osRecvMesg(D_80152770,0,1);
    if (list==0) list=D_801527C8;
    node=list->head;
    while(node!=0) {
        if(node->status==0 && maximum<node->value) maximum=node->value;
        node=node->next;
    }
    osJamMesg(D_80152770,0,0);
    return maximum;
}
