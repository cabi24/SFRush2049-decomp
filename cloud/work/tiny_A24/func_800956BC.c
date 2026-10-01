typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct Node {u32 pad0;struct Node *next;u8 type,pad9[3];void *payload;f32 amount;} Node;
typedef struct {u32 pad[3];Node *first;} List;
extern List D_80146160;
Node *func_800956BC(void *payload) {Node *p=D_80146160.first;while(p){if(p->payload==payload && p->type==3)return p;p=p->next;}return NULL;}
