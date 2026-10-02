/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef int s32;
typedef struct Link {struct Link *next,*previous;} Link;
typedef struct Node {Link links;s8 active,queued;} Node;
typedef struct List16 {u8 flags[4];u32 count;Link *head,*tail;} List16;
typedef struct Player76 {u8 pad0[68];s32 node;u32 pad72;} Player76;
extern s16 D_8014A108;extern Player76 D_8014A118[];
extern List16 D_80146170,D_80146188;extern u8 D_80142728[];
extern void func_800D52CC(Node *);extern void func_8009211C(List16 *,Link *);extern void func_80091FBC(List16 *,Link *,Link *);
extern s32 osRecvMesg(void *,void **,s32);extern s32 osJamMesg(void *,void *,s32);
void func_800D5374(void) {
 Player76 *player,*end;Node *node;s32 count=D_8014A108;
 if(count>0) {
 player=D_8014A118;do {
  node=(Node *)player->node;
  if(node!=(Node *)-1) {
   osRecvMesg(D_80142728,0,1);func_800D52CC(node);
   if(node->queued){func_8009211C(&D_80146188,&node->links);node->queued=0;}
   func_80091FBC(&D_80146170,&node->links,D_80146170.head);node->active=1;
   osJamMesg(D_80142728,0,0);count=D_8014A108;
  }
  end=D_8014A118+count;player->node=-1;player++;
 } while(player<end);
 }
}
