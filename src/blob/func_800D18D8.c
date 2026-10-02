/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef unsigned int u32;
typedef struct Node16 {u32 pad0,pad4,value;u8 busy,pad13[3];} Node16;
typedef struct List16 {u8 indirect,doubly,pad2[2];u32 count;Node16 *head,*tail;} List16;
extern List16 D_80149808;extern u32 D_80110258,D_801460F8,D_80146104;
void func_8009211C(List16 *,Node16 *);
Node16 *func_800D18D8(void) {
 Node16 *node=D_80149808.head;
 func_8009211C(&D_80149808,node);
 node->busy=0;
 node->value=(D_80110258<<D_801460F8)|(node->value&D_80146104);
 if(D_80110258>=((~D_80146104)>>(D_801460F8+1)))D_80110258=1;
 else D_80110258++;
 return node;
}
