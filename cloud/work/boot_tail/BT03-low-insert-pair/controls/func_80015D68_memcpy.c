/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceEntry {
    void *payload;
    u16 id;
    u16 references;
} ResourceEntry;
#pragma pack(0)
extern int D_80038608;
extern ResourceEntry D_80038610[2048];
extern void *memcpy(void *, const void *, unsigned int);
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015D68(u16 id, void *payload)
{
    int i;
    int j;
    ResourceEntry *entry;
    func_80014594();
    for (i = 0; i < D_80038608 && D_80038610[i].id < id; ++i) {
    }
    if (i < D_80038608) {
        entry = &D_80038610[i];
        if (entry->id != id) {
            if (D_80038608 < 2048) {
                for (j = D_80038608 - 1; j >= i; --j) {
                    memcpy(&D_80038610[j + 1], &D_80038610[j], sizeof(ResourceEntry));
                }
                ++D_80038608;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            func_800145DC();
            ++entry->references;
            return 0;
        }
    } else if (D_80038608 < 2048) {
        entry = &D_80038610[i];
        ++D_80038608;
    } else {
        func_800145DC();
        return 0;
    }
    entry->payload = payload;
    entry->id = id;
    entry->references = 1;
    func_800145DC();
    return 1;
}
