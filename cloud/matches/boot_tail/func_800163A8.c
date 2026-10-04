/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Release every matching subrecord in the first matching registry entry. */
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct {
    u16 id;
    u16 references;
    u32 size;
    u32 metadata;
    unsigned char data[16];
} Resource;
typedef struct {
    Resource *resources;
    u32 metadata[2];
} RegistryEntry;
extern u32 D_800385A0;
extern RegistryEntry D_800385A8[];
extern void func_80014D08(void *, u32);
extern int func_800161A0(Resource *);
int func_800163A8(u16 id)
{
    Resource *resource;
    int used;
    int found;
    u32 i;
    found = 0;
    for (i = 0; i < D_800385A0; i++) {
        used = 0;
        for (resource = D_800385A8[i].resources; resource->id != 0xffff; resource++) {
            if (resource->id == id) {
                found = 1;
                if (--resource->references == 0) {
                    func_80014D08(resource->data, resource->size);
                }
            }
            if (resource->references != 0) {
                used = 1;
            }
        }
        if (found) {
            if (!used) {
                func_800161A0(D_800385A8[i].resources);
            }
            return 1;
        }
    }
    return 0;
}
