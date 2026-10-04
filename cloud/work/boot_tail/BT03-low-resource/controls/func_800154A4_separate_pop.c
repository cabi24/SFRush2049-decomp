/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ResourceGroup {
    u32 unknown00;
    u16 id;
    u16 type;
    u32 offsets[5];
} ResourceGroup;
extern u8 D_8002C630;
extern short D_80038390;
extern ResourceGroup *D_80038398[];
extern void func_800150C8(u16 *);
extern void func_800152A8(u16 *);
extern void func_80015120(u16 *);
extern void func_80015178(u16 *);
extern void func_800151D0(u16 *);
extern int func_80015348(u16);

int func_800154A4(void)
{
    ResourceGroup *group;
    if (D_8002C630 != 0 && D_80038390 != 0) {
        --D_80038390;
        group = D_80038398[D_80038390];
        func_800150C8((u16 *)((unsigned char *)group + group->offsets[0] + 8));
        func_800152A8((u16 *)((unsigned char *)group + group->offsets[1] + 8));
        func_80015120((u16 *)((unsigned char *)group + group->offsets[2] + 8));
        func_80015178((u16 *)((unsigned char *)group + group->offsets[3] + 8));
        func_800151D0((u16 *)((unsigned char *)group + group->offsets[4] + 8));
        if (group->type == 1) {
            func_80015348(group->id);
        }
        return 1;
    }
    return 0;
}
