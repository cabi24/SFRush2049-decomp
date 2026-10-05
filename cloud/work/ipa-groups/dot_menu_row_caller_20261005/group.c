/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native N64 option rows. No established arcade donor. See REPORT.md.
 * The packed RGBA word view is required by real inlined call sites; copying
 * the aggregate itself has the same setter bytes but different caller code.
 * All 14 row calls in func_8010A8D0 are genuine. That parent is unclaimed.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef union Color4 { struct { u8 r,g,b,a; } channels; u32 rgba; } Color4;
typedef struct OptionRow {
    u8 prefix[8];
    f32 angle;
    u8 middle[36];
    f32 position[3];
    u8 alpha;
    u8 tail[3];
} OptionRow;
typedef struct ResourceHeader {
    u8 prefix[32];
    u16 string_offset;
} ResourceHeader;
typedef struct MenuAssets {
    void *first;
    void **labels;
    void *third;
    ResourceHeader *header;
    void **values;
} MenuAssets;
extern Color4 D_801146BC, D_801146C0;
extern s32 D_80116D0C;
extern s16 D_80116D9C, D_8013FEC8;
extern OptionRow D_8011650C[];
extern void *D_801164D0[];
extern s8 D_80146108[];
extern MenuAssets D_8017A4E0;
extern s32 D_80118E20, D_80118E24;
extern f32 D_80116CFC[];
extern u8 D_80150B70[];
extern u8 D_801461D0[];
extern Color4 D_80118E28, D_80118E2C;
void func_800BEA3C(Color4 first, Color4 second)
{
    D_80118E28.rgba = first.rgba;
    D_80118E2C.rgba = second.rgba;
}
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, void *);
extern void render_helper(f32);
extern void brake_light_update(s32, f32 *, void *, f32 *, s16 *);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(void *, s16);
extern void func_800ED66C(f32);
extern void particle_velocity_set(void);

void func_8010A7A4(s32 index, s32 x, s32 y, void *label, void *value)
{
    s16 row_y;
    if (D_80116D0C == 1) {
        if (D_8011650C[index].angle < 1.57079637f) {
            func_800BEA3C(D_801146BC, D_801146C0);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC, D_801146C0);
            dispatch_handler(22);
        }
    } else {
        dispatch_handler(index == D_80116D9C ? 22 : 1);
    }
    row_y = y;
    state_utility(x - 105, row_y, label);
    state_utility(x + 40, row_y, value);
}

void func_8010A8D0(void)
{
    s32 index;
    s32 x, y;
    s16 screen[2];
    OptionRow *row;
    void *label;

    render_helper(0.0f);
    D_80118E24 = 1;
    D_80118E20 = 0;
    render_helper(32320.0f);
    brake_light_update(0, D_80116CFC, D_80150B70, 0, screen);
    x = screen[0];
    y = screen[1];
    osRecvMesg(D_801461D0, 0, 1);
    slot_state_setup(13);
    osJamMesg(D_801461D0, 0, 0);
    dispatch_handler(1);
    label = D_8017A4E0.labels[50];
    state_utility(x - (object_manager_update(label, -1) >> 1), y, label);
    osRecvMesg(D_801461D0, 0, 1);
    slot_state_setup(11);
    osJamMesg(D_801461D0, 0, 0);
    for (index = 0; index < 14; index++) {
        row = &D_8011650C[index];
        if ((row->angle > 0.69813168f && row->angle < 2.44346094f)
            || row->alpha == 0) {
            continue;
        }
        func_800ED66C((f32)row->alpha);
        render_helper(32384.0f);
        brake_light_update(0, row->position, D_80150B70, 0, screen);
        x = screen[0] + 50;
        y = screen[1];
        switch (index) {
        case 1:
            func_8010A7A4(1, x, y, D_8017A4E0.labels[13],
                D_8017A4E0.values[D_80146108[10] + D_8017A4E0.header->string_offset]);
            break;
        case 2:
            func_8010A7A4(2, x, y, D_8017A4E0.labels[15],
                D_8017A4E0.values[D_80146108[0] + D_8017A4E0.header->string_offset]);
            break;
        case 3:
            func_8010A7A4(3, x, y, D_8017A4E0.labels[16],
                D_8017A4E0.values[D_80146108[1] + D_8017A4E0.header->string_offset]);
            break;
        case 4:
            func_8010A7A4(4, x, y, D_8017A4E0.labels[17],
                D_8017A4E0.values[D_80146108[2] + D_8017A4E0.header->string_offset]);
            break;
        case 5:
            func_8010A7A4(5, x, y, D_8017A4E0.labels[18],
                D_8017A4E0.values[D_80146108[3] + D_8017A4E0.header->string_offset]);
            break;
        case 6:
            func_8010A7A4(6, x, y, D_8017A4E0.labels[19],
                D_8017A4E0.values[D_80146108[4] + D_8017A4E0.header->string_offset]);
            break;
        case 7:
            func_8010A7A4(7, x, y, D_8017A4E0.labels[20],
                D_8017A4E0.values[D_80146108[5] + D_8017A4E0.header->string_offset]);
            break;
        case 8:
            func_8010A7A4(8, x, y, D_8017A4E0.labels[21],
                D_8017A4E0.values[D_80146108[6] + D_8017A4E0.header->string_offset]);
            break;
        case 9:
            func_8010A7A4(9, x, y, D_8017A4E0.labels[22],
                D_8017A4E0.values[D_80146108[7] + D_8017A4E0.header->string_offset]);
            break;
        case 10:
            func_8010A7A4(10, x, y, D_8017A4E0.labels[23],
                D_8017A4E0.values[D_80146108[8] + D_8017A4E0.header->string_offset]);
            break;
        case 11:
            func_8010A7A4(11, x, y, D_8017A4E0.labels[24],
                D_8017A4E0.values[D_80146108[9] + D_8017A4E0.header->string_offset]);
            break;
        case 12:
            func_8010A7A4(12, x, y, D_8017A4E0.labels[26],
                D_8017A4E0.values[D_80146108[18] + D_8017A4E0.header->string_offset]);
            break;
        case 13:
            func_8010A7A4(13, x, y, D_8017A4E0.labels[14],
                D_8017A4E0.values[D_80146108[11] + D_8017A4E0.header->string_offset]);
            break;
        case 0:
            func_8010A7A4(0, x, y, D_8017A4E0.labels[25], D_801164D0[D_8013FEC8]);
            break;
        }
    }
    particle_velocity_set();
    D_80118E24 = 3;
    D_80118E20 = 0;
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC, D_801146C0);
    render_helper(-1.0f);
}
