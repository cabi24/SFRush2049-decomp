/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* CONTEXT (byte-identical bodies of the locked menu_save_options members func_800A370C = ll.c ClearLL and
 * func_800A44E8 = InitLL, from src/blob/groups/menu_save_options/save.c) so the group compile inlines them into
 * PS_PakServer / PS_InitPakSys as the whole-program unit does. Not claimed. */
typedef signed char s8;
typedef signed int s32;

typedef struct ListHead {
    s8 unk0;
    s8 unk1;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} ListHead;

void func_800A370C(ListHead *list) {
    list->unk8 = 0;
    list->unkC = 0;
    list->unk4 = 0;
}

void func_800A44E8(ListHead *list, s8 a, s8 b) {
    list->unk0 = a;
    list->unk1 = b;
    func_800A370C(list);
}
