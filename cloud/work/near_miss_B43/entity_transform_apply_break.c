/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef struct Node {
    struct Node *next;
    unsigned short tag;
    s16 handle;
    unsigned char opaque[12];
    int state;
} Node;
extern Node *D_801391F0;
extern Node *D_801392C8;
extern s16 D_8012E66C,D_8012E678;
extern void model_data_load(int,int,unsigned int);
void entity_transform_apply(Node *node,int unlink) {
    Node *previous,*current;
    if (node->handle>=0) model_data_load(node->handle,1,15);
    node->state=0;
    node->handle=-1;
    if (unlink) {
        current=D_801391F0;
        if (node==current) D_801391F0=current->next;
        else {
            previous=current;
            while (current) {
                current=current->next;
                if (node==current) break;
                previous=current;
            }
            if (previous && previous->next==node) previous->next=node->next;
        }
    }
    node->next=D_801392C8;
    D_8012E66C--;
    D_801392C8=node;
    if (D_8012E678<D_8012E66C) D_8012E678=D_8012E66C;
}
