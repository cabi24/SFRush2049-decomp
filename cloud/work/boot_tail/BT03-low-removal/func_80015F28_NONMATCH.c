/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: research source only. */
/* Eight-byte table records have a word-aligned copy representation and a
 * packed field view. Payload bits are opaque here. Only active records may
 * be searched or shifted; the native tables supply that bounded domain. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    unsigned int payload;
    u16 id;
    u16 references;
} RecordFields;
#pragma pack()
extern int D_80038608;
typedef union {
    RecordFields fields;
    unsigned int words[2];
} RecordStorage;
extern RecordStorage D_80038610[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015F28(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_80038608 && id != ((RecordFields *)&D_80038610[i])->id; i++) {
    }
    if (i != D_80038608) {
        if (--((RecordFields *)&D_80038610[i])->references == 0) {
            for (j = i + 1; j < D_80038608; j++) {
                D_80038610[j - 1] = D_80038610[j];
            }
            D_80038608--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}
