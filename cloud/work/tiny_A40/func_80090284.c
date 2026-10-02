/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Node {struct Node *next;s16 count,id,state;u8 pad10[2];void *object;f32 value;u32 flags;} Node;
extern Node *D_801392C8;extern s16 D_8012E66C,D_8012E678;
Node *func_80090284(void) {
 Node *node=D_801392C8;
 if(!node)return 0;
 D_8012E66C++;
 D_801392C8=node->next;
 if(D_8012E66C>D_8012E678)D_8012E678=D_8012E66C;
 node->next=0;node->count=0;node->id=-1;node->state=0;
 node->object=0;node->flags=0;node->value=0.0f;
 return node;
}
