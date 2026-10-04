/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct TimedNode {
    struct TimedNode *next;
    struct TimedNode *previous;
    u32 identifier;
    s32 deadline;
    u32 fraction;
    s32 whole;
} TimedNode;
typedef struct Context {
    u8 unknown000[0x118];
    u32 fraction118;
    s32 whole11C;
    s32 half120;
    u8 unknown124[0xE54];
    TimedNode *headF78;
} Context;
extern Context *D_8004BE80;
extern int func_8001B8C4(u32);
extern void func_800174D0(TimedNode *);
int func_80018184(void)
{
    TimedNode *node;
    TimedNode *next;
    s32 sum;
    if (D_8004BE80->headF78 == 0) return 0;
    node = D_8004BE80->headF78;
    while (node != 0) {
        next = node->next;
        sum = (s32)((u32)node->whole + (u32)D_8004BE80->half120);
        if (sum >= node->deadline) {
            func_8001B8C4(node->identifier);
            func_800174D0(node);
        } else {
            sum = (s32)(node->fraction + D_8004BE80->fraction118);
            node->fraction = (u32)sum & 65535;
            sum >>= 16;
            node->whole = (s32)((u32)node->whole + (u32)sum +
                (u32)D_8004BE80->whole11C);
        }
        node = next;
    }
    return 1;
}
