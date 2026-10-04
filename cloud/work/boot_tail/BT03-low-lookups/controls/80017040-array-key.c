/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#pragma pack(1)
typedef struct LookupBank {
    unsigned short unknown00;
    unsigned short count;
    void *entries;
} LookupBank;
#pragma pack()
extern int D_80042228;
extern LookupBank D_80042230[];
extern unsigned short D_800426A0[];
extern int func_80017018(const void *, const void *);
extern void *func_8001E864(const void *, const void *, int, int,
                         int (*)(const void *, const void *));
void *func_80017040(unsigned short key)
{
    int i;
    void *entry;
    D_800426A0[0] = key;
    for (i = 0; i < D_80042228; i++) {
        entry = func_8001E864(D_800426A0, D_80042230[i].entries,
            D_80042230[i].count, 12, func_80017018);
        if (entry != 0) {
            return entry;
        }
    }
    return 0;
}
