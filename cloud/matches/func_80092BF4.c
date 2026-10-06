/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80092BF4: copy two words (*v1, *v2) into fields 0x3C/0x40 of the 0x44-byte
 * model slot D_8012E700[r->w14], where r = &D_80139320[idx] (0x40-byte player record).
 * Sibling of the locked func_80092FE0 and written in its style.  Also matches at -O2.
 * Shaping quirks: the slot index is read into two separate `short` locals (one
 * sign-extension each, and `li 68; multu` instead of a shift expansion), and the
 * second store goes through a `ModelSlot *` local (puts the final address in a3).
 */
typedef struct ModelSlot { unsigned int flags; char pad4[0x38]; unsigned int v3C; unsigned int v40; } ModelSlot;
typedef struct PlayerRec { char p0[0x14]; int w14; char p18[0x28]; } PlayerRec;
extern ModelSlot D_8012E700[];
extern PlayerRec D_80139320[];
void func_80092BF4(short idx, unsigned int *v1, unsigned int *v2) {
    PlayerRec *r = &D_80139320[idx];
    short a, b; ModelSlot *m;
    a = r->w14; D_8012E700[a].v3C = *v1;
    b = r->w14; m = &D_8012E700[b]; m->v40 = *v2;
}
