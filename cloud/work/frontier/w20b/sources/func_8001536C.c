/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001536C - lib_15a10 (IDO 7.1). Walks a chain of resource groups (each starts with its byte
 * size; size -1 ends the chain) for the group with this id; on a hit pushes it on the group stack
 * D_80038398[D_80038390] (128 entries) and runs the group's five loaders (func_80014F14 / 80015228 /
 * 80014F80 / 80014FEC / 80015058) on its five id lists (each at group + offsets[i] + 8), plus
 * func_80015318 on the sixth list for type 1 groups. Returns 1 on a hit, else 0. Inverse of
 * func_800154A4 (pop + unload).
 * w20b levers: (1) each id-list address is a block-scoped running pointer
 * `{ u8 *p = group; p += offset; p += 8; f(p); }`: 7.1 keeps base-first `addu a0,s0,tN` only for the
 * `p += x` form; every one-expression form canonicalises to `addu a0,tN,s0` (w18c/w19b: 50-60), and a
 * function-level p is coloured into s1 (true=400). (2) the walking cursor is a local `u8 *group`
 * assigned inside the guard (the first chain test then reads the parameter in a0 and the copy to s0
 * lands after it, as retail); compare written `group->id == id`.
 * Shaping device (disclosed): the end test reads the size word through a `u32` view
 * (`*(u32 *)group != 0xFFFFFFFF`) while the advance reads the int field: with one type uopt CSEs the
 * two loads (retail reloads the size after the test; true=630). Type views: the cursor is `u8 *` with
 * casts to the group view at each field access. No unused locals.
 * TU context: needs D_80038390 declared `short` (retail lh/slti; the TU's func_80014E10 block declares
 * it unsigned short): w20b tu_edits.py, layout-neutral, proven identical alone. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ResourceGroup { int size; u16 id; u16 type; u32 offsets[6]; } ResourceGroup;
extern u8 D_8002C630;
extern short D_80038390;
extern ResourceGroup *D_80038398[];
typedef struct ResourceOffsets { unsigned int offsets[4]; } ResourceOffsets;
typedef struct SampleRecord { u16 identifier; u16 references; u32 offset; void *data; u8 descriptor[16]; } SampleRecord;
extern void func_80014F14(u16 *ids, ResourceOffsets *resources);
extern void func_80015228(u16 *ids, void *address, SampleRecord *records);
extern void func_80014F80(u16 *ids, ResourceOffsets *resources);
extern void func_80014FEC(u16 *ids, ResourceOffsets *resources);
extern void func_80015058(u16 *ids, ResourceOffsets *resources);
extern void func_80015318(unsigned short id, unsigned short *data);

int func_8001536C(u8 *groups, u16 id, void *address, SampleRecord *records, ResourceOffsets *resources)
{
    u8 *group;
    if (D_8002C630 != 0 && D_80038390 < 128) {
        group = groups;
        while (*(u32 *)group != 0xFFFFFFFF) {
            if (((ResourceGroup *)group)->id == id) {
                D_80038398[D_80038390] = (ResourceGroup *)group;
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[0]; p += 8; func_80014F14((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[1]; p += 8; func_80015228((u16 *)p, address, records); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[2]; p += 8; func_80014F80((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[3]; p += 8; func_80014FEC((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[4]; p += 8; func_80015058((u16 *)p, resources); }
                if (((ResourceGroup *)group)->type == 1) {
                    u8 *p = group; p += ((ResourceGroup *)group)->offsets[5]; p += 8; func_80015318(id, (u16 *)p);
                }
                D_80038390++;
                return 1;
            }
            group += ((ResourceGroup *)group)->size;
        }
    }
    return 0;
}
