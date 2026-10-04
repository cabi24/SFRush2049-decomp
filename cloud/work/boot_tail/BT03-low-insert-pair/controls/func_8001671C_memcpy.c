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
extern void *memcpy(void *, const void *, unsigned int);
extern void func_80014594(void);
extern void func_800145DC(void);

int func_8001671C(u16 id, void *payload)
{
    ResourceRange *range;
    ResourceEntry *entry;
    int first;
    int position;
    int i;
    int j;
    func_80014594();
    range = &D_8003DA28[id >> 6];
    if (range->count == 0) {
        range->first = D_8003DA20;
        first = range->first;
        position = first;
    } else {
        first = range->first;
        for (i = 0; i < range->count && D_8003E228[first + i].id < id; ++i) {
        }
        position = first + i;
        if (i < range->count) {
            entry = &D_8003E228[position];
            if (entry->id == id) {
                ++entry->references;
                func_800145DC();
                return 0;
            }
        }
    }
    if (D_8003DA20 < 2048) {
        for (i = 0; i < 512; ++i) {
            if (first < D_8003DA28[i].first) {
                ++D_8003DA28[i].first;
            }
        }
        for (j = D_8003DA20 - 1; j >= position; --j) {
            memcpy(&D_8003E228[j + 1], &D_8003E228[j], sizeof(ResourceEntry));
        }
        entry = &D_8003E228[position];
        entry->payload = payload;
        entry->id = id;
        entry->references = 1;
        ++range->count;
        ++D_8003DA20;
        func_800145DC();
        return 1;
    }
    func_800145DC();
    return 0;
}
