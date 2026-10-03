/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#pragma pack(1)
typedef struct PackedKey {
    unsigned char unknown00[4];
    unsigned short key;
} PackedKey;
#pragma pack()

int func_80016BF8(const PackedKey *left, const PackedKey *right)
{
    return left->key - right->key;
}
