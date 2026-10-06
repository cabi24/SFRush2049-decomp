typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[0x24];
    f32 pos[3];
    f32 matrix[9];
    u8 pad54[152 - 0x54];
} View;
typedef struct {
    u16 pad0;
    u16 flags;
} Poly;

extern s16 D_80151AD0;
extern View D_80150B70[];
extern Poly *D_80154368[];
extern f32 D_801140F8[4][3];
extern u8 D_801140F4[];
extern u16 D_80154398;
extern u16 D_801543A4;
extern s16 D_80152032;
extern volatile u8 D_80140BDC;
extern s8 D_8010FFC0;

f32 viGetTimeToDeadline();
void func_8008C544(f32 *in, f32 *out, f32 *m);
s32 func_800B24EC();
Poly *func_800A78BC(s32 count, f32 *vertices, u16 texture, u8 *color, u16 flags, s32 indexed);
void func_8008C074(Poly *poly, s32 count, f32 *vertices, u16 texture, u8 *color, u16 flags, s32 indexed);
void func_8008D0C0(Poly *poly);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
int sprintf(char *, const char *, ...);

s32 func_800B61A8(s32 id, s32 a1, s32 a2, u8 a3);

void audio_doppler_full(s32 show) {
    f32 quad[4][3];
    s32 player;
    s32 i;
    View *view;
    f32 t;
    f32 size;
    char name[8];


    for (player = 0; player < D_80151AD0; player++) {
        if (show) {
            view = &D_80150B70[player];
            t = viGetTimeToDeadline() - 0.001f;
            size = (t - (s32)t) * 12.0f;
            if (size < 5.0f)
                size = 5.0f;
            for (i = 0; i < 4; i++) {
                D_801140F8[i][2] = size;
                func_8008C544(D_801140F8[i], quad[i], view->matrix);
                quad[i][0] += view->pos[0];
                quad[i][1] += view->pos[1];
                quad[i][2] += view->pos[2];
            }
            if (D_80154368[player] == 0) {
                func_800B24EC("CNTDWN3", &D_80154398, 0, (s8)(D_80140BDC - 1), 1);
                D_80154368[player] = func_800A78BC(4, quad[0], D_80154398, D_801140F4, (1 << player) | 0x82C0, 1);
                D_801543A4 = 4;
            } else if (D_80152032 < D_801543A4) {
                if (player + 1 == D_80151AD0) {
                    sprintf(name, "CNTDWN%d", D_80152032);
                    func_800B24EC(name, &D_80154398, 0, (s8)(D_80140BDC - 1), 1);
                    D_801543A4 = D_80152032;
                    if (D_80152032 == 3) {
                        func_800B61A8(75, 0, 1, 0);
                    } else if (D_80152032 == 2) {
                        func_800B61A8(76, 0, 1, 0);
                    } else {
                        func_800B61A8(77, 0, 1, 0);
                    }
                }
                func_8008C074(D_80154368[player], 4, quad[0], D_80154398, D_801140F4, 0, 1);
                D_80154368[player]->flags &= 0x7FFF;
            } else {
                func_8008C074(D_80154368[player], 4, quad[0], D_80154398, D_801140F4, 0, 1);
            }
        } else if (D_80154368[player] != 0) {
            func_8008D0C0(D_80154368[player]);
            D_80154368[player] = 0;
            D_80152032 = -1;
        }
    }
}
