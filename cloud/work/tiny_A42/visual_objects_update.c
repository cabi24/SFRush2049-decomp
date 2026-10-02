/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Node {void *body;} Node;
typedef struct List {u32 pad0,pad4;Node *head;u32 pad12;} List;
extern List D_80144D60[4];
s32 MaxPathZeroControls(Node *,s32);
void visual_objects_update(s32 action) {
 List *list;Node *node;
 for(list=D_80144D60;list!=D_80144D60+4;list++) {
  node=list->head;
  while(node) {
   if(FIELD(node->body,u32,72)!=0)MaxPathZeroControls(node,action);
   node=FIELD(node->body,Node *,0);
  }
 }
}
