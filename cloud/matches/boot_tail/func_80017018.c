/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#pragma pack(1)
typedef struct PackedLeadingKey {
    unsigned short key;
} PackedLeadingKey;
#pragma pack()

int func_80017018(const PackedLeadingKey *left, const PackedLeadingKey *right)
{
    return left->key - right->key;
}
