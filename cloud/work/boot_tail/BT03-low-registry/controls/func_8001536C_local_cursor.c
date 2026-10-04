/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SampleRecord {
    u16 identifier;
    u16 references;
    u32 offset;
    void *data;
    u8 descriptor[16];
} SampleRecord;
typedef struct ResourceOffsets { u32 offsets[4]; } ResourceOffsets;
typedef struct ResourceGroup {
    u32 next_offset;
    u16 identifier;
    u16 type;
    u32 offsets[6];
} ResourceGroup;
extern u8 D_8002C630;
extern short D_80038390;
extern ResourceGroup *D_80038398[128];
extern void func_80014F14(u16 *, ResourceOffsets *);
extern void func_80015228(u16 *, void *, SampleRecord *);
extern void func_80014F80(u16 *, ResourceOffsets *);
extern void func_80014FEC(u16 *, ResourceOffsets *);
extern void func_80015058(u16 *, ResourceOffsets *);
extern void func_80015318(u16, u16 *);

int func_8001536C(ResourceGroup *groups, u16 identifier, void *address,
                  SampleRecord *records, ResourceOffsets *resources)
{
    ResourceGroup *group;
    if (D_8002C630 && D_80038390 < 128) {
        group = groups;
        while (group->next_offset != 0xFFFFFFFFU) {
            if (identifier == group->identifier) {
                D_80038398[D_80038390] = group;
                func_80014F14((u16 *)((u8 *)group + group->offsets[0] + 8), resources);
                func_80015228((u16 *)((u8 *)group + group->offsets[1] + 8), address, records);
                func_80014F80((u16 *)((u8 *)group + group->offsets[2] + 8), resources);
                func_80014FEC((u16 *)((u8 *)group + group->offsets[3] + 8), resources);
                func_80015058((u16 *)((u8 *)group + group->offsets[4] + 8), resources);
                if (group->type == 1) {
                    func_80015318(identifier, (u16 *)((u8 *)group + group->offsets[5] + 8));
                }
                ++D_80038390;
                return 1;
            }
            group = (ResourceGroup *)((u8 *)group + group->next_offset);
        }
    }
    return 0;
}
