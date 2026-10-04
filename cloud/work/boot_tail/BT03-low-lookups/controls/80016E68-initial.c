/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#pragma pack(1)
typedef struct LookupEntry {
    unsigned int value;
    unsigned short key;
    unsigned short auxiliary;
} LookupEntry;
#pragma pack()
typedef struct LookupKey {
    unsigned int value;
    unsigned short key;
    unsigned short auxiliary;
} LookupKey;
extern void *func_8001E864(const void *, const void *, int, int,
                         int (*)(const void *, const void *));
extern LookupKey D_80042670;
extern int D_80038608;
extern LookupEntry D_80038610[];
extern LookupEntry *D_80042678;
extern int func_80016E40(const void *, const void *);
unsigned int func_80016E68(unsigned short key)
{
    D_80042670.key = key;
    D_80042678 = func_8001E864(&D_80042670, D_80038610,
        D_80038608, 8, func_80016E40);
    if (D_80042678 != 0) {
        return D_80042678->value;
    }
    return 0;
}
