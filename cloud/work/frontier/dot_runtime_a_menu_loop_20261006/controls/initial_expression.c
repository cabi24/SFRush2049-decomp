/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Image A menu callback. N64-specific; direct arcade equivalent not identified. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef float f32;
typedef struct Texture { u8 pad0[18]; u16 height; u8 pad20[16]; } Texture;
typedef struct MenuIndices { u8 pad0[26]; u16 first; } MenuIndices;
typedef struct MenuData {
    u8 pad0[12];
    MenuIndices *indices;
    char **text;
} MenuData;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern s8 D_803BA028[];
extern volatile u8 D_80140BDC;
extern char D_803B85E8[];
extern MenuData D_8017A4E0;
void render_helper(f32);
void *object_create(s32);
void dispatch_handler(s32);
Texture *func_800B24EC(char *, s16 *, s8, s8, s32);
s16 object_bytes_sum_global(void);
void state_utility(s16, s16, void *);

s32 func_803A4340(s32 arg0)
{
    s32 i;
    s32 j;
    s16 index;
    render_helper(0.0f);
    object_create(12);
    dispatch_handler(1);
    for (i = 0; i < D_8014A108; i++) {
        if (D_803BA028[i] != 1) {
            for (j = 0; j < 4; j++) {
                if (i < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
                    state_utility(10, 48 + 2 * j * (func_800B24EC(D_803B85E8, &index, 0, D_80140BDC - 1, 1)->height / 8) - object_bytes_sum_global(), D_8017A4E0.text[D_8017A4E0.indices->first + j]);
                }
            }
        }
    }
    render_helper(-1.0f);
    return 1;
}
