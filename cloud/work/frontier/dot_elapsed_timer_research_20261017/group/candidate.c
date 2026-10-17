typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Blit Blit;
struct Blit { const char *name;void *palette;void *info;u16 texIndex;s16 x,y;u16 state;
 s16 width,height;u8 alpha,flip;s8 hide,init;s16 top,bot,left,right,color;u16 pad26;
 s32 (*animFunc)(Blit *);u32 animID;s32 animDTA;u16 blIdx,pad36;void *data;Blit *child; };
extern void Input_ApplyPadConfig(Blit *);
extern s16 D_80151AD0;extern u32 D_801174B4;extern s8 D_8015723C;extern u8 D_80116187;
static s32 elapsedTimerPos[4][2] = {{272,142},{272,142},{240,197},{160,134}};
s32 object_create(s32);u32 camera_shake_update(u16);s8 object_byte9_set(s8);

s32 Hidden(Blit *blt,s32 hide)
{
    if (hide != blt->hide) {
        blt->hide = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hide;
}
s32 checkpoint_hit(Blit *blt)
{
    s32 win;
    s32 digitW;
    s32 colonW;
    s8 fixedWidth;
    win = blt->animID;
    if (Hidden(blt,(win == 1 && D_80151AD0 > 1) ||
                   (win == 2 && D_80151AD0 < 2))) {
        blt->animFunc = 0;
        return 1;
    }
    if (Hidden(blt,(D_801174B4 & 8) ||
                   !(D_801174B4 & 0x400000) || !D_8015723C))
        return 1;
    if (D_80151AD0 == 1) object_create(1);
    else object_create(2);
    digitW = camera_shake_update('8') + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(':') + 1;
    object_byte9_set(fixedWidth);
    blt->x = elapsedTimerPos[D_80151AD0 - 1][0] - (7 * digitW + 2 * colonW) / 2;
    blt->y = elapsedTimerPos[D_80151AD0 - 1][1] - 1;
    blt->alpha = D_80116187; /* Native HUD alpha, versus GC128. */
    blt->state = 100; /* Native-only depth field. */
    Input_ApplyPadConfig(blt);
    return 1;
}

/* Existing locked text callback, retained as genuine second reader of the
 * same owned static. Its historical unused formal/body spelling is inherited.
 * Font/justification calls now name the actual single existing helpers. */
extern f32 D_801543CC;
extern void render_helper(f32),dispatch_handler(s32),state_utility(s16,s16,void *);
typedef enum { J_LEFT=0,J_CENTER=1,J_RIGHT=2,J_BOTTOM=3 } JustEnum;
extern void func_800B669C(JustEnum,JustEnum);
s32 animElapsedTimerText(s32 arg0)
{
    s32 t;
    char digits[9];
    char str[2];
    s32 digitW;
    s32 colonW;
    s8 fixedWidth;
    s32 x;
    s32 y;
    s32 digitIdx;

    if ((D_801174B4 & 8) || !(D_801174B4 & 0x400000) || !D_8015723C) {
        return 1;
    }
    t = D_801543CC * 1000.0f;
    if (t < 0) t = 0;
    digits[0] = t / 600000 % 6 + '0';
    digits[1] = t / 60000 % 10 + '0';
    digits[2] = ':';
    digits[3] = t / 10000 % 6 + '0';
    digits[4] = t / 1000 % 10 + '0';
    digits[5] = '.';
    digits[6] = t / 100 % 10 + '0';
    digits[7] = t / 10 % 10 + '0';
    digits[8] = t % 10 + '0';
    render_helper(0.0f);
    func_800B669C(1, 3);
    if (D_80151AD0 == 1) {
        object_create(1);
    } else {
        object_create(2);
    }
    digitW = camera_shake_update('8') + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(':') + 1;
    object_byte9_set(fixedWidth);
    str[1] = 0;
    y = elapsedTimerPos[D_80151AD0 - 1][1];
    x = elapsedTimerPos[D_80151AD0 - 1][0] - (digitW * 5 + colonW * 2) / 2;
    for (digitIdx = 0; digitIdx < 8; digitIdx++) {
        if ((str[0] = digits[digitIdx]) != ':' && str[0] != '.') {
            str[0] = '8';
            dispatch_handler(0);
            state_utility(x, y, str);
            str[0] = digits[digitIdx];
        }
        dispatch_handler(1);
        state_utility(x, y, str);
        if (digitIdx == 1 || digitIdx == 2 || digitIdx == 4 || digitIdx == 5) {
            x += (digitW + colonW) / 2;
        } else {
            x += digitW;
        }
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
