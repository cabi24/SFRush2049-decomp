/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

int func_8001671C(u16 id, void *payload)
{
    int main;
    int position;
    int base;
    int i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count == 0) {
        position = base = D_8003DA28[main].first = D_8003DA20;
    } else {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && D_8003E228[base + i].id < id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            position = base + i;
            if (id == D_8003E228[position].id) {
                D_8003E228[position].references++;
                func_800145DC();
                return 0;
            }
        } else {
            position = base + i;
        }
    }
    if (D_8003DA20 < 2048) {
        for (i = 0; i < 512; ++i) {
            if (D_8003DA28[i].first > base) {
                D_8003DA28[i].first++;
            }
        }
        i = D_8003DA20 - 1;
        for (; i >= position; --i) {
            D_8003E228[i + 1] = D_8003E228[i];
        }
        D_8003E228[position].payload = payload;
        D_8003E228[position].id = id;
        D_8003E228[position].references = 1;
        D_8003DA28[main].count++;
        D_8003DA20++;
        func_800145DC();
        return 1;
    }
    func_800145DC();
    return 0;
}
