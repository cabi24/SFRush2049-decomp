/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;
typedef struct Node {
 const char *name;void *fallback,*resource;
 u16 index;s16 first,second,state;u16 width,height;
 u8 color;s8 pending,busy,enabled;
 s16 bound[5];u8 gap[2];
 int word40,word44,word48;u16 slot;u8 gap54[2];int word56;
 struct Node *next;
} Node;
extern int D_80149788;
extern Node *D_80149450[];
extern void collision_sound_play(Node *);
extern int func_800A79F4(u16,void *,void *,int,int,int,int);
extern void Input_ApplyPadConfig(Node *);
Node *func_800B3704(const char *name,int first,int second) {
 Node *node;
 if(D_80149788>=200)return 0;
 node=D_80149450[D_80149788];
 D_80149788++;
 node->name=name;
 node->first=first;
 node->busy=0;
 node->pending=0;
 node->enabled=1;
 node->next=0;
 node->word40=0;
 node->word44=-1;
 node->word48=-1;
 node->word56=0;
 node->second=second;
 collision_sound_play(node);
 node->slot=func_800A79F4(node->index,0,0,first,second,-1,-1);
 Input_ApplyPadConfig(node);
 return node;
}
