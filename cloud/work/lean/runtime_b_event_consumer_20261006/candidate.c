/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B: draw player counters and consume the four-record HUD event ring. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct Player { u8 unknown000[0x3A3]; s8 remaining; u8 unknown3A4[0x14]; } Player;
typedef struct Event {
    s8 active, age, column, row;
    f32 from_x, from_y, to_x, to_y;
    s8 delta;
    u8 unknown15[3];
} Event;
typedef struct Texts { u8 before108[108]; char *same_player; char *other_player; } Texts;
typedef struct LanguageState { s32 unknown0; Texts *texts; } LanguageState;
typedef struct NameData { u8 before_name[20]; char name[1]; } NameData;
typedef struct NameRef { NameData *data; } NameRef;
typedef struct PlayerInfo { NameRef *name_ref; u8 unknown04[72]; } PlayerInfo;
extern Player D_80152818[];
extern Event D_80395E70[4];
extern s16 D_8014A108, D_80151AD0;
extern s32 D_803940D0[4][4][2], D_80115F28[4][4][2];
extern char D_80394AF0[], D_80394AF4[], D_80394AF8[];
extern LanguageState countdown_state;
extern PlayerInfo D_8014A160[];
extern s8 D_8012E67C[], D_80142690;
extern s32 D_801146AC[], D_801170FC;
extern void render_helper(f32);
extern s32 object_create(s32);
extern void func_800B669C(unsigned int, unsigned int);
extern int sprintf(char *, const char *, ...);
extern void fcvt_wrapper(char *, char *, ...);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, void *);
extern void camera_auto_follow(s16, s16, s16, s16, s16, s16, u8 *);
extern void func_800F7E30(s8, s8, s8);

s32 func_803936A8(void *callback_context)
{
    s32 i, x, y;
    Event *event;
    char text[32];
    render_helper(0.0f);
    object_create(10);
    func_800B669C(1, 1);
    for (i = 0; i < D_8014A108; i++) {
        x = D_803940D0[D_80151AD0 - 1][i][0];
        y = D_803940D0[D_80151AD0 - 1][i][1];
        sprintf(text, D_80394AF0, D_80152818[i].remaining);
        dispatch_handler(0);
        state_utility(x + 1, y + 1, text);
        dispatch_handler(1);
        state_utility(x, y, text);
    }
    for (i = 0; i < 4; i++) {
        event = &D_80395E70[i];
        if (event->active) {
            if (event->age > 40) {
                event->active = 0;
                if (event->delta > 0) {
                    func_800F7E30(event->row, event->column, event->delta);
                }
                D_80152818[event->row].remaining += event->delta;
            } else {
                x = ((event->to_x - event->from_x) / 40.0f) * event->age + event->from_x;
                y = ((event->to_y - event->from_y) / 40.0f) * event->age + event->from_y;
                if (event->column == event->row) {
                    dispatch_handler(1);
                    state_utility(x, y, D_80394AF4);
                    x = D_80115F28[D_80151AD0 - 1][event->column][0];
                    y = D_80115F28[D_80151AD0 - 1][event->column][1];
                    if (D_8014A108 >= 3) {
                        object_create(11);
                        dispatch_handler(0);
                        state_utility(x + 1, y + 1, countdown_state.texts->same_player);
                        dispatch_handler(1);
                        state_utility(x, y, countdown_state.texts->same_player);
                    }
                } else {
                    dispatch_handler(1);
                    state_utility(x, y, D_80394AF8);
                    x = D_80115F28[D_80151AD0 - 1][event->column][0];
                    y = D_80115F28[D_80151AD0 - 1][event->column][1];
                    fcvt_wrapper(text, countdown_state.texts->other_player,
                                 D_8014A160[event->row].name_ref->data->name);
                    if (D_8014A108 >= 3) {
                        object_create(11);
                        dispatch_handler(0);
                        camera_auto_follow(x + 1, y + 1, 144, 110, -1, 0, (u8 *)text);
                        dispatch_handler(D_801146AC[D_8012E67C[event->row]]);
                        camera_auto_follow(x, y, 144, 110, -1, 0, (u8 *)text);
                    }
                }
                if (!D_80142690 && !D_801170FC) {
                    event->age++;
                }
            }
        }
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
