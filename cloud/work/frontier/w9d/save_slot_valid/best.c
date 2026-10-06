/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * save_slot_valid (0x800AF5E0; historical label): create a scene record (func_8008E26C: allocate a
 * 0x44-byte record in D_8012E700[], word0 = flags, owner pointer, id, linked under `parent`) whose
 * owner is either a fresh object from func_800A7D6C() (dynamic != 0), the fixed object D_8011418C
 * (col == -1, which also sets flag 0x80), or the pointer table entry D_803B9AA0[row / 13 * 12 + col].
 * The flag word is ((layer << 8) ^ 0xF00) | flags, the same encoding camera_reset uses.
 *
 * Shaping (w9d): the table is indexed flat (`row / 13 * 12 + col`); a 2-D `[][12]` declaration gives
 * the same instructions with ugen's mul-by-48 in one register (8 words of register naming).
 * -O3 only (26/44 at -O2).
 */
typedef short s16;
typedef int s32;

extern s32 D_8011418C;
extern s32 *D_803B9AA0[];
void *func_800A7D6C(void);
s32 func_8008E26C(s32 id, void *owner, s16 parent, s32 flags);

void save_slot_valid(s32 id, s16 layer, s16 parent, s32 flags, s16 row, s16 col, s32 dynamic)
{
    void *owner;

    if (dynamic != 0) {
        owner = func_800A7D6C();
    } else if (col == -1) {
        owner = &D_8011418C;
        flags |= 0x80;
    } else {
        owner = D_803B9AA0[row / 13 * 12 + col];
    }
    func_8008E26C(id, owner, parent, ((layer << 8) ^ 0xF00) | flags);
}
