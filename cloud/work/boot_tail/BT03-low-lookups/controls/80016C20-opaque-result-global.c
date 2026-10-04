/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#pragma pack(1)
typedef struct LookupEntry {
    void *value;
    unsigned short key;
    unsigned short auxiliary;
} LookupEntry;
#pragma pack()
typedef struct LookupKey {
    void *value;
    unsigned short key;
    unsigned short auxiliary;
} LookupKey;
extern void *func_8001E864(const void *, const void *, int, int,
                         int (*)(const void *, const void *));
#pragma pack(1)
typedef struct LookupGroup {
    unsigned short count;
    unsigned short first;
} LookupGroup;
#pragma pack()
extern LookupGroup D_8003DA28[];
extern LookupEntry D_8003E228[];
extern int D_80042630;
extern int D_80042634;
extern unsigned int D_80042638;
extern unsigned short D_8004263C;
extern void *D_80042640;
extern int func_80016BF8(const void *, const void *);
void *func_80016C20(unsigned short key)
{
    int group;
    int first;
    int count;
    group = key >> 6;
    count = D_8003DA28[group].count;
    D_80042634 = group;
    if (count != 0) {
        first = D_8003DA28[group].first;
        D_8004263C = key;
        D_80042630 = first;
        D_80042640 = func_8001E864(&D_80042638, &D_8003E228[first],
            count, 8, func_80016BF8);
        if (D_80042640 != 0) {
            return ((LookupEntry *)D_80042640)->value;
        }
    }
    return 0;
}
