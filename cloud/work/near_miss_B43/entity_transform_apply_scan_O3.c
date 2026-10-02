/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef struct Node {
    struct Node *next;
    unsigned short tag;
    s16 handle;
    unsigned char opaque[12];
    int state;
} Node;
extern Node *gPhysicsObjectListHead;
extern Node *D_801492C8;
extern s16 D_8012E66C,D_8012E678;
extern void model_data_load(int,int,unsigned int);
void entity_transform_apply(Node *node,int unlink) {
    Node *previous,*current;
    if (node->handle>=0) model_data_load(node->handle,1,15);
    node->state=0;
    node->handle=-1;
    if (unlink) {
        current=gPhysicsObjectListHead;
        if (current==node) gPhysicsObjectListHead=current->next;
        else {
            previous=0;
            while (current && current!=node) {
                previous=current;
                current=current->next;
            }
            if (previous && previous->next==node) previous->next=node->next;
        }
    }
    node->next=D_801492C8;
    D_8012E66C--;
    D_801492C8=node;
    if (D_8012E678<D_8012E66C) D_8012E678=D_8012E66C;
}
