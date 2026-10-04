/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: research source only. */
/* Eight-byte table records have a word-aligned copy representation and a
 * packed field view. Payload bits are opaque here. Only active records may
 * be searched or shifted; the native tables supply that bounded domain. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    u16 id;
    u16 metadata;
    unsigned int payload;
} RecordFields;
#pragma pack()
extern int D_80042228;
typedef union {
    RecordFields fields;
    unsigned int words[2];
} RecordStorage;
extern RecordStorage D_80042230[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_8001661C(u16 id)
{
    int i;
    int j;
    for (i = 0; i < D_80042228 && id != ((RecordFields *)&D_80042230[i])->id; i++) {
    }
    if (i != D_80042228) {
        func_80014594();
        for (j = i + 1; j < D_80042228; j++) {
            D_80042230[j - 1] = D_80042230[j];
        }
        D_80042228--;
        func_800145DC();
        return 1;
    }
    return 0;
}
