/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: six record-copy words differ at the selected flags. */
/* The native record is eight bytes: payload pointer, ID, reference count.
 * Fields permit byte alignment; whole-record shifts use aligned storage. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    void *payload;
    u16 id;
    u16 references;
} RecordFields;
#pragma pack()
typedef union {
    RecordFields fields;
    unsigned int words[2];
} RecordStorage;
extern int D_8003C610;
extern RecordStorage D_8003C618[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015720(u16 id, void *payload)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && ((RecordFields *)&D_8003C618[i])->id < id; i++) {
    }
    if (i < D_8003C610) {
        if (((RecordFields *)&D_8003C618[i])->id != id) {
            if (D_8003C610 < 2048) {
                for (j = D_8003C610 - 1; j >= i; j--) {
                    D_8003C618[j + 1] = D_8003C618[j];
                }
                D_8003C610++;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            ((RecordFields *)&D_8003C618[i])->references++;
            func_800145DC();
            return 0;
        }
    } else {
        if (D_8003C610 < 2048) {
            D_8003C610++;
        } else {
            func_800145DC();
            return 0;
        }
    }
    ((RecordFields *)&D_8003C618[i])->payload = payload;
    ((RecordFields *)&D_8003C618[i])->id = id;
    ((RecordFields *)&D_8003C618[i])->references = 1;
    func_800145DC();
    return 1;
}
