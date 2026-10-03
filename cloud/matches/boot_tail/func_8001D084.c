/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct StateNode {
    struct StateNode *next;
    struct StateNode *previous;
    u32 flags08;
    u8 unknown0C[40];
    u32 identifier34;
} StateNode;
extern StateNode *D_8004FD50;
extern int func_8001B8C4(u32);
void func_8001D084(StateNode *state)
{
    if (state->next != 0) state->next->previous = state->previous;
    if (state->previous != 0) state->previous->next = state->next;
    else D_8004FD50 = state->next;
    state->flags08 &= 0xFFFF;
    if (state->identifier34 != 0xFFFFFFFF) func_8001B8C4(state->identifier34);
}
