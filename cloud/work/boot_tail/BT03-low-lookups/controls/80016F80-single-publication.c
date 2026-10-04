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
extern unsigned int D_80042690;
extern unsigned short D_80042694;
extern int D_8003CE18;
extern unsigned char D_8003CE20[];
extern LookupEntry *D_8004269C;
extern int func_80016F58(const void *, const void *);
void *func_80016F80(unsigned short key, unsigned short *auxiliary)
{
    LookupEntry *entry;
    void *value;
    value = 0;
    D_80042694 = key;
    entry = func_8001E864(&D_80042690, D_8003CE20, D_8003CE18, 12, func_80016F58);
    if (entry != 0) {
        *auxiliary = entry->auxiliary;
        value = entry->value;
    }
    D_8004269C = entry;
    return value;
}
