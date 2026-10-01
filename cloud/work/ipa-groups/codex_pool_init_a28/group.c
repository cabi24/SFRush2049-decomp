/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Node {struct Node *prev,*next;u32 pad8;void *resource;} Node;
typedef struct Pool {u8 doubly;u8 pad1[3];s32 count,size;void *records;Node *active,*free;} Pool;
extern Pool D_80155220;extern u8 D_80155B30[],D_80155290[];
void func_8008D0C0(void *);void func_800AFA84(Pool *,Node *);void pool_linked_list_init(Pool *);void *memset(void *,s32,u32);
void func_800B0580(void) {
 Pool *pool=&D_80155220;Node *node;
 while((node=pool->active)!=0) {func_8008D0C0(node->resource);func_800AFA84(pool,node);}
 pool->records=D_80155B30;
 pool->count=100;
 pool->size=36;
 pool->doubly=1;
 pool_linked_list_init(pool);
 memset(D_80155290,0,2208);
}
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
