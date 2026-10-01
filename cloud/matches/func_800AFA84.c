/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Node {struct Node *prev,*next;} Node;
typedef struct Pool {u8 doubly;u8 pad1[15];Node *active,*free;} Pool;
void func_800AFA84(Pool *pool,Node *node) {
 Node *cur,*prev;
 if(node) {
  if(pool->doubly) {
   if(node->prev)node->prev->next=node->next;
   cur=node->next;
   if(cur)cur->prev=node->prev;
   else pool->active=node->prev;
  }else{
   cur=pool->active;prev=0;
   while(cur) {
    if(node==cur) {
     if(prev)prev->prev=node->prev;
     else pool->active=node->prev;
     break;
    }
    prev=cur;cur=cur->prev;
   }
  }
  node->prev=pool->free;pool->free=node;
 }
}
