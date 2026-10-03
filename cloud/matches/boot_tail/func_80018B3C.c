/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Entry {
    struct Entry *next;
    struct Entry *previous;
    u32 identifier;
} Entry;
typedef struct ContextPrefix {
    u8 unknown000[0xF68];
    u32 activeF68;
    u32 savedF6C;
    u32 lowF70;
    u32 highF74;
    u32 unknownF78;
    Entry *headF7C;
} ContextPrefix;
extern ContextPrefix *D_8004BE80;
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
