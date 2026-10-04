/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80014A74.c: file-local type names AudioSlot suffixed _80014A74 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct AudioSlot_80014A74 {
    u8 active;
    u8 pending;
    u8 unknown02[2];
    u16 rate;
    u8 unknown06[14];
    u32 position;
    u8 unknown18[16];
    u16 release_count;
    u8 unknown2A[22];
    u16 value40;
    u16 value42;
    u16 value44;
    u16 value46;
    u8 unknown48[32];
} AudioSlot_80014A74;
extern AudioSlot_80014A74 *D_80038294;

extern void func_8001E0E0(u16 *, u16 *, u32, u32, u32, u16 *, u32, u16 *);

void func_80014A74(int index, u32 volume, u32 pan, u32 span, u32 aux)
{
    AudioSlot_80014A74 *slot;
    slot = &D_80038294[index];
    func_8001E0E0(&slot->value42, &slot->value40, volume, pan, span,
                  &slot->value44, aux, &slot->value46);
}
