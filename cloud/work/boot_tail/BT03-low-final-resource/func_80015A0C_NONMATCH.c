/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceEntry {
    void *payload;
    u16 id;
    u16 parameter;
    u16 references;
    unsigned char unknown0A[2];
} ResourceEntry;
#pragma pack(0)
extern int D_8003CE18;
extern ResourceEntry D_8003CE20[256];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015A0C(u16 id, void *payload, u16 parameter)
{
    int i;
    int j;
    ResourceEntry *entry;
    func_80014594();
    for (i = 0; i < D_8003CE18 && D_8003CE20[i].id < id; ++i) {
    }
    if (i < D_8003CE18) {
        entry = &D_8003CE20[i];
        if (entry->id != id) {
            if (D_8003CE18 < 256) {
                for (j = D_8003CE18 - 1; j >= i; --j) {
                    D_8003CE20[j + 1] = D_8003CE20[j];
                }
                ++D_8003CE18;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            ++entry->references;
            func_800145DC();
            return 0;
        }
    } else if (D_8003CE18 < 256) {
        entry = &D_8003CE20[i];
        ++D_8003CE18;
    } else {
        func_800145DC();
        return 0;
    }
    entry->payload = payload;
    entry->id = id;
    entry->parameter = parameter;
    entry->references = 1;
    func_800145DC();
    return 1;
}
