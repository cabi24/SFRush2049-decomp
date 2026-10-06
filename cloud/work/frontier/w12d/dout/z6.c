/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Node104 {
    u8 name[16];
    f32 transform[12];
    u32 flags;
    s16 sibling,child;
    u8 opaque72[8];
    f32 minimum[3],maximum[3];
} Node104;
extern Node104 *D_80149B80,*D_80149D94;
extern s16 D_80149D90;
extern volatile u8 D_80140BDC;
extern u32 state_word_a;
extern int transmission_ratio_get(Node104 *,s16,int,int,int,int,int);
extern int string_copy_format(void *,s8,s8,int),func_8008E26C(int,f32 *,s16,u32);
extern void func_800A7E10(f32 *,f32 *,s16);
static u32 node_flags(Node104 *n) { return n->flags; }
void differential_output(Node104 *node,s16 parent)
{
    Node104 *current;
    s16 first;
    int result;
    int model;
    first=-1;
    current=node;
    while(current) {
        D_80149D94=current;
        if(!transmission_ratio_get(current,parent,0,1,-1,1,0)) {
            result=parent;
            model=string_copy_format(current,0,D_80140BDC-1,1);
            if(state_word_a & 8)
                result=(s16)func_8008E26C(model,current->transform,result,node_flags(current)|0xE00);
            else
                result=(s16)func_8008E26C(model,current->transform,result,current->flags);
            if(current->minimum[0]!=current->maximum[0] || current->minimum[1]!=current->maximum[1]) {
                if(parent==-1)func_800A7E10(current->minimum,current->maximum,result);
            }
            if(D_80149D90==-1)D_80149D90=result;
            if(first==-1)first=result;
        }
        if(current->sibling>=0)current=&D_80149B80[current->sibling];
        else current=0;
    }
    current=node;
    while(current) {
        if(current->child>=0)differential_output(&D_80149B80[current->child],first);
        first++;
        if(current->sibling>=0)current=&D_80149B80[current->sibling];
        else current=0;
    }
    if(parent&&parent){}
}
