/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct ResourceOffsets {
    unsigned int offsets[4];
} ResourceOffsets;

extern void *func_80014E1C(unsigned short, void *);

void *func_80014EE8(unsigned short id, ResourceOffsets *resource)
{
    return func_80014E1C(id, (unsigned char *)resource + resource->offsets[3]);
}
