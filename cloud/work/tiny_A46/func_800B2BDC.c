/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;typedef unsigned int u32;
typedef struct Node24 {struct Node24 *next;s16 p4,id;u32 p8,p12,p16,p20;} Node24;
extern Node24 D_80138880[100];extern Node24 *D_801392C8;extern u32 D_801391F0;extern s16 D_8012E66C;
void func_800B2BDC(void) {
 Node24 *node;
 D_801392C8=D_80138880;
 for(node=D_80138880;node<D_80138880+99;node++) {
  node->id=-1;
  node->next=node+1;
 }
 node->next=0;node->id=-1;
 D_801391F0=0;D_8012E66C=0;
}
