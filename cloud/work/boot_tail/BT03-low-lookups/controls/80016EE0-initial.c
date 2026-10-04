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
extern LookupKey D_80042680;
extern int D_8003C610;
extern LookupEntry D_8003C618[];
extern LookupEntry *D_80042688;
extern int func_80016E40(const void *, const void *);
unsigned int func_80016EE0(unsigned short key)
{
    D_80042680.key = key;
    D_80042688 = func_8001E864(&D_80042680, D_8003C618,
        D_8003C610, 8, func_80016E40);
    if (D_80042688 != 0) {
        return D_80042688->value;
    }
    return 0;
}
