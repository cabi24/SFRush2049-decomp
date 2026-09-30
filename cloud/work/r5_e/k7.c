typedef struct ModelSlot { unsigned int flags; char pad4[0x38]; unsigned int v3C; char pad40[4]; } ModelSlot;
typedef struct PlayerRec { short p0; short a; char p4[14]; short b; char p14[10]; short c; short q; short d; short q2; short e; short q3; short f; char p2C[20]; } PlayerRec;
extern ModelSlot D_8012E700[];
extern PlayerRec D_80139320[];
void func_80092FE0(short idx, unsigned int *val) {
    
    D_8012E700[D_80139320[idx].a].v3C = *val;
    D_8012E700[D_80139320[idx].b].v3C = *val;
    D_8012E700[D_80139320[idx].c].v3C = *val;
    D_8012E700[D_80139320[idx].d].v3C = *val;
    D_8012E700[D_80139320[idx].e].v3C = *val;
    D_8012E700[D_80139320[idx].f].v3C = *val;
}
