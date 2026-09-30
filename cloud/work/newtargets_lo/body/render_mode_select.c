typedef struct { u8 pad0[22]; s16 s22; s16 s24; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
extern s16 D_8015B254;
s32 func_8008E144(s32);
void render_mode_select(s16 a, s16 b) {
    if (b < 0) {
        if (D_8015B254 < 0) {
            D_8015B254 = a;
        } else {
            D_8012E700[func_8008E144(D_8015B254)].s24 = a;
        }
    } else {
        if (D_8012E700[b].s22 == -1) {
            D_8012E700[b].s22 = a;
        } else {
            D_8012E700[func_8008E144(D_8012E700[b].s22)].s24 = a;
        }
    }
}
