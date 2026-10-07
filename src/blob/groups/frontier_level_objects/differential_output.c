/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * differential_output (historical label; 0x800AC3D8, 648 bytes) and its node-flags getter
 * func_800AC660 (0x800AC660, the caller-less retail stub right after it).
 * Recursive placement of a 104-byte node tree (pool D_80149B80, sibling s16 @68, child s16 @70):
 * phase 1 walks the sibling chain, publishes the current node in D_80149D94, instantiates it through
 * transmission_ratio_get(node, parent, 0, 1, -1, 1, 0), and when that returns 0 resolves the name id
 * (string_copy_format) and registers it with func_8008E26C (flags | 0xE00 when state_word_a bit 3),
 * records bounds when parent == -1, and remembers the first id; phase 2 recurses into each child with
 * consecutive ids starting at that first id. No arcade ancestor identified.
 *
 * EQUAL in the whole-program unit (w13d), with frontier_level_objects' transmission_ratio_get using the
 * kept entity getter func_8008AE64 instead of func_800AC660 (see group.json / RESULTS.md).
 *
 * Recovered (w12d): s16 parameter (entry cvt and a1 home store); int result with (s16) casts on the
 * func_8008E26C returns (keeps the per-iteration result = parent copy in s1); volatile D_80140BDC;
 * the inlined getter func_800AC660 in the 0xE00 branch only (retail's lw v0 / ori a3,v0 delay slot).
 *
 * Shaping devices (all code-free; disclosed per the owner rules; they may not be the original
 * spelling, probably compiled-out debug checks):
 *  - four compiled-out reads `if(parent){}` at the end: they lower parent's colouring priority
 *    (54/13 = 4.15) below the constant -1's (30/7 = 4.29), so -1 takes s3 and parent s4 as in retail;
 *  - `if(D_80149D94){}` inside the first loop: keeps the &D_80149D94 address web above the
 *    per-procedure callee-saved cost the reads above raise (tot 20, else it splits and the frame is 72);
 *  - `if(current) { if(D_80149D94){} do {...} while(current); }` (a while loop written as guarded
 *    do-while with a compiled-out read in the preheader): adds one block to the &D_80149D94 web
 *    (20/8 = 2.5), so it ranks below &D_80149D90 (20/7) and takes s8 (retail) instead of s7. The read
 *    must be on the same source line as the `do {` (a separate line changes as1's preheader schedule).
 */
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
u32 func_800AC660(Node104 *n);
void differential_output(Node104 *node,s16 parent)
{
    Node104 *current;
    s16 first;
    int result;
    int model;
    first=-1;
    current=node;
    if(current) { if(D_80149D94){} do {
        D_80149D94=current;
        if(!transmission_ratio_get(current,parent,0,1,-1,1,0)) {
            result=parent;
            model=string_copy_format(current,0,D_80140BDC-1,1);
            if(state_word_a & 8)
                result=(s16)func_8008E26C(model,current->transform,result,func_800AC660(current)|0xE00);
            else
                result=(s16)func_8008E26C(model,current->transform,result,current->flags);
            if(current->minimum[0]!=current->maximum[0] || current->minimum[1]!=current->maximum[1]) {
                if(parent==-1)func_800A7E10(current->minimum,current->maximum,result);
            }
            if(D_80149D90==-1)D_80149D90=result;
            if(first==-1)first=result;
        }
        if(D_80149D94){}
        if(current->sibling>=0)current=&D_80149B80[current->sibling];
        else current=0;
    } while(current);
    }
    current=node;
    while(current) {
        if(current->child>=0)differential_output(&D_80149B80[current->child],first);
        first++;
        if(current->sibling>=0)current=&D_80149B80[current->sibling];
        else current=0;
    }
    if(parent){}
    if(parent){}
    if(parent){}
    if(parent){}
}

u32 func_800AC660(Node104 *n)
{
    return n->flags;
}
