typedef struct ModelSlot { unsigned int flags; char pad4[0x38]; unsigned int v3C; unsigned int v40; } ModelSlot;
typedef struct PlayerRec { char p0[0x14]; int w14; char p18[0x28]; } PlayerRec;
extern ModelSlot D_8012E700[];
extern PlayerRec D_80139320[];
void func_80092BF4(short idx, unsigned int *v1, unsigned int *v2) {
    PlayerRec *r = &D_80139320[idx];
    short a, b;
    a = r->w14; D_8012E700[a].v3C = *v1;
    b = r->w14; D_8012E700[b].v40 = *v2;
}
