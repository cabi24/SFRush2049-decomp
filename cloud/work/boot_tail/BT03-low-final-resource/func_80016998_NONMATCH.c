/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceEntry {
    void *payload;
    u16 id;
    u16 references;
} ResourceEntry;
typedef struct ResourceRange {
    u16 count;
    u16 first;
} ResourceRange;
#pragma pack(0)
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern ResourceEntry D_8003E228[2048];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80016998(u16 id)
{
    ResourceRange *range;
    ResourceEntry *entry;
    int first;
    int i;
    int j;
    func_80014594();
    range = &D_8003DA28[id >> 6];
    if (range->count != 0) {
        first = range->first;
        for (i = 0; i < range->count && D_8003E228[first + i].id != id; ++i) {
        }
        if (i < range->count) {
            entry = &D_8003E228[first + i];
            --entry->references;
            if (entry->references == 0) {
                for (j = first + i + 1; j < D_8003DA20; ++j) {
                    D_8003E228[j - 1] = D_8003E228[j];
                }
                for (i = 0; i < 512; ++i) {
                    if (first < D_8003DA28[i].first) {
                        --D_8003DA28[i].first;
                    }
                }
                --range->count;
                --D_8003DA20;
            }
        }
    }
    func_800145DC();
    return 0;
}
