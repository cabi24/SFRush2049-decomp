/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 adaptation of MusyX dataRemoveMacro; exact donor/version not claimed. */
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
    int main;
    int base;
    int i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count != 0) {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && id != D_8003E228[base + i].id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            if (--D_8003E228[base + i].references == 0) {
                for (i = base + i + 1; i < D_8003DA20; ++i) {
                    D_8003E228[i - 1] = D_8003E228[i];
                }
                for (i = 0; i < 512; ++i) {
                    if (D_8003DA28[i].first > base) {
                        --D_8003DA28[i].first;
                    }
                }
                --D_8003DA28[main].count;
                --D_8003DA20;
            }
        }
    }
    func_800145DC();
    return 0;
}
