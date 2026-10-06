/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef short s16;
typedef int s32;

extern s32 D_8011418C;
extern s32 *D_803B9AA0[][12];
void *func_800A7D6C(void);
s32 func_8008E26C(s32 parent, void *owner, s16 id, s32 flags);

void save_slot_valid(s32 parent, s16 arg1, s16 id, s32 flags, s16 row, s16 col, s32 dynamic)
{
    void *owner;

    if (dynamic != 0) {
        owner = func_800A7D6C();
    } else if (col == -1) {
        owner = &D_8011418C;
        flags |= 0x80;
    } else {
        owner = D_803B9AA0[row / 13][col];
    }
    func_8008E26C(parent, owner, id, ((arg1 << 8) ^ 0xF00) | flags);
}
