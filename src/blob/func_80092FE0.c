/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct ModelSlot { unsigned int flags; char pad4[0x38]; unsigned int v3C; char pad40[4]; } ModelSlot;
typedef struct PlayerRec { short p0; short a; char p4[14]; short b; char p14[10]; short c; short q; short d; short q2; short e; short q3; short f; char p2C[20]; } PlayerRec;
extern ModelSlot D_8012E700[];
extern PlayerRec D_80139320[];
void func_80092FE0(short idx, unsigned int *val) {
    PlayerRec *r = &D_80139320[idx];
    short a, b, c, d, e, f;
    a = r->a; D_8012E700[a].v3C = *val;
    b = r->b; D_8012E700[b].v3C = *val;
    c = r->c; D_8012E700[c].v3C = *val;
    d = r->d; D_8012E700[d].v3C = *val;
    e = r->e; D_8012E700[e].v3C = *val;
    f = r->f; D_8012E700[f].v3C = *val;
}
