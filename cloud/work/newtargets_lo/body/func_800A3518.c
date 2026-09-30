typedef struct { u8 pad0[140]; struct { s32 v; u8 pad[36]; } e[16]; } Car;
typedef struct { u8 b[772]; } CarSlot;
extern CarSlot D_80144030[];
s32 func_800A3518(s32 i) {
    s32 n = 0;
    s32 k;
    Car *c = (Car *)&D_80144030[i];
    for (k = 0; k < 16; k++) {
        if (c->e[k].v == 0) {
            n++;
        }
    }
    return n;
}
