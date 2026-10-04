/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018B3C.c: file-local type names ContextPrefix, Entry suffixed _80018B3C so several bodies share one ROM TU; no other change. */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Entry_80018B3C {
    struct Entry_80018B3C *next;
    struct Entry_80018B3C *previous;
    u32 identifier;
} Entry_80018B3C;
typedef struct ContextPrefix_80018B3C {
    u8 unknown000[0xF68];
    u32 activeF68;
    u32 savedF6C;
    u32 lowF70;
    u32 highF74;
    u32 unknownF78;
    Entry_80018B3C *headF7C;
} ContextPrefix_80018B3C;
extern ContextPrefix_80018B3C *D_8004BE80;
extern void func_80018A30(void);
extern void func_8001897C(void);
void func_80018B3C(u32 value)
{
    if (D_8004BE80->activeF68) {
        D_8004BE80->savedF6C = D_8004BE80->activeF68;
        D_8004BE80->highF74 = value;
        D_8004BE80->lowF70 = 0;
        func_80018A30();
        func_8001897C();
    }
}
