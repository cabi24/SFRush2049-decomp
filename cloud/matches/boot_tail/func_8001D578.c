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
extern int func_8001D4CC(StateNode *);
void func_8001D578(void)
{
    StateNode *state;
    StateNode *next;
    state = D_8004FD50;
    while (state != 0) {
        next = state->next;
        func_8001D4CC(state);
        state = next;
    }
}
