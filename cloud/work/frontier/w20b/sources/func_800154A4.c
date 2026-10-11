/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800154A4 - lib_15a10 (IDO 7.1). Pops the top resource group off the group stack
 * D_80038398[--D_80038390] and runs the five unloaders (func_800150C8 / 800152A8 / 80015120 / 80015178 /
 * 800151D0) on its five id lists (each at group + offsets[i] + 8), plus func_80015348(group id) for type 1
 * groups. Returns 1, or 0 when the system is off (D_8002C630) or the stack is empty. Inverse of func_8001536C.
 * w20b lever: each id-list address is a block-scoped running pointer `{ u8 *p = (u8 *)group;
 * p += offset; p += 8; f(p); }`: 7.1 keeps base-first `addu a0,s0,tN` only for the `p += x` form;
 * every one-expression spelling canonicalises to `addu a0,tN,s0` (w18c g1: true=50); `p + 8` as the
 * argument instead of `p += 8` colours p into v0/v1 (true=90); a function-level p goes to s1 (true=400).
 * No devices beyond the block locals, no unused locals.
 * TU context: needs D_80038390 declared `short` (retail lh + signed decrement; the TU's func_80014E10
 * block declares it unsigned short): w20b tu_edits.py, layout-neutral, proven identical alone. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ResourceGroup { int size; u16 id; u16 type; u32 offsets[6]; } ResourceGroup;
extern u8 D_8002C630;
extern short D_80038390;
extern ResourceGroup *D_80038398[];
extern void func_800150C8(u16 *ids);
extern void func_800152A8(u16 *ids);
extern void func_80015120(u16 *ids);
extern void func_80015178(u16 *ids);
extern void func_800151D0(u16 *ids);
extern void func_80015348(unsigned short id);

int func_800154A4(void)
{
    ResourceGroup *group;
    if (D_8002C630 != 0 && D_80038390 != 0) {
        group = D_80038398[--D_80038390];
        { u8 *p = (u8 *)group; p += group->offsets[0]; p += 8; func_800150C8((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[1]; p += 8; func_800152A8((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[2]; p += 8; func_80015120((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[3]; p += 8; func_80015178((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[4]; p += 8; func_800151D0((u16 *)p); }
        if (group->type == 1) {
            func_80015348(group->id);
        }
        return 1;
    }
    return 0;
}
