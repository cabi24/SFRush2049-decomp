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
extern int D_8003C610;
typedef union {
    RecordFields fields;
    unsigned int words[2];
} RecordStorage;
extern RecordStorage D_8003C618[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_800158D8(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && id != ((RecordFields *)&D_8003C618[i])->id; i++) {
    }
    if (i != D_8003C610) {
        if (--((RecordFields *)&D_8003C618[i])->references == 0) {
            for (j = i + 1; j < D_8003C610; j++) {
                D_8003C618[j - 1] = D_8003C618[j];
            }
            D_8003C610--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}
