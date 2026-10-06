/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Runtime image B: 0x80393518..0x803936A8. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Player952 {
    u8 before_kind[0x384];
    s8 kind;
    s8 lives;
    u8 tail[0x32];
} Player952;
typedef struct TextPosition { s16 x, y; } TextPosition;
extern s16 D_80151AD0;
extern Player952 player_array[];
extern TextPosition D_803941D0[4][4];
extern char D_80394AB0[];
extern void render_helper(float);
extern int object_create(int);
extern void func_800B669C(unsigned int, unsigned int);
extern void fcvt_wrapper(char *, char *, ...);
extern void dispatch_handler(int);
extern void state_utility(s16, s16, char *);
int func_80393518(void *callback_context)
{
    s16 i;
    s16 x, y;
    char text[12];
    render_helper(0.0f);
    object_create(10);
    func_800B669C(1, 1);
    for (i = 0; i < D_80151AD0; i++) {
        if (i < D_80151AD0 && player_array[i].kind != 8) {
            x = D_803941D0[D_80151AD0 - 1][i].x;
            y = D_803941D0[D_80151AD0 - 1][i].y;
            if (player_array[i].lives >= 0) {
                fcvt_wrapper(text, D_80394AB0, player_array[i].lives);
                dispatch_handler(0);
                state_utility(x + 1, y + 1, text);
                dispatch_handler(1);
                state_utility(x, y, text);
            }
        }
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
