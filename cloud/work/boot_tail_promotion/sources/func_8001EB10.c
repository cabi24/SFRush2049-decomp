/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001EB10.c: file-local type names SequenceNode, VoiceState suffixed _8001EB10 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
#pragma pack(1)
typedef struct SequenceNode_8001EB10 {struct SequenceNode_8001EB10 *next,*previous;u32 key,value;} SequenceNode_8001EB10;
typedef struct VoiceState_8001EB10 {
    u8 unknown00[16];
    u32 child10,parent14;
    SequenceNode_8001EB10 *entry18;
    u8 unknown1C[68];
    u32 identifier60;
    u8 unknown64[316];
} VoiceState_8001EB10;
#pragma pack(0)
extern VoiceState_8001EB10 D_8004BEB8[];
extern SequenceNode_8001EB10 *D_80050C50,*D_80050C54;
extern void func_80021844(VoiceState_8001EB10 *);
void func_8001EB10(VoiceState_8001EB10 *state)
{
    func_80021844(state);
    if (state->identifier60 != 0xFFFFFFFFU) {
        if (state->parent14 != 0xFFFFFFFFU) {
            D_8004BEB8[(state->parent14 & 255)].child10=state->child10;
            if (state->child10 != 0xFFFFFFFFU) {
                D_8004BEB8[(state->child10 & 255)].parent14=state->parent14;
            }
        } else if (state->child10 != 0xFFFFFFFFU) {
            state->entry18->value=state->child10;
            D_8004BEB8[(state->child10 & 255)].parent14=0xFFFFFFFFU;
            D_8004BEB8[(state->child10 & 255)].entry18=state->entry18;
        } else {
            if (state->entry18->previous != 0) {
                state->entry18->previous->next=state->entry18->next;
            } else {
                D_80050C50=state->entry18->next;
            }
            if (state->entry18->next != 0) {
                state->entry18->next->previous=state->entry18->previous;
            }
            state->entry18->next=D_80050C54;
            if (D_80050C54 != 0) D_80050C54->previous=state->entry18;
            state->entry18->previous=0;
            D_80050C54=state->entry18;
        }
    }
}
