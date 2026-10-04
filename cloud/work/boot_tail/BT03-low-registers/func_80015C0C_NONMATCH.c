/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: research source only. */
/* Twelve-byte table records have a word-aligned copy representation and a
 * packed field view. Payload bits are opaque here. Only active records may
 * be searched or shifted; the native tables supply that bounded domain. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    unsigned int payload;
    u16 id;
    u16 type;
    u16 references;
    u16 metadata;
} RecordFields;
#pragma pack()
extern int D_8003CE18;
typedef union {
    RecordFields fields;
    unsigned int words[3];
} RecordStorage;
extern RecordStorage D_8003CE20[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015C0C(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003CE18 && ((RecordFields *)&D_8003CE20[i])->id != id; i++) {
    }
    if (i != D_8003CE18) {
        if (--((RecordFields *)&D_8003CE20[i])->references == 0) {
            for (j = i + 1; j < D_8003CE18; j++) {
                D_8003CE20[j - 1] = D_8003CE20[j];
            }
            D_8003CE18--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}
