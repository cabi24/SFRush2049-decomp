/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010AEAC = GC animText@optionscr.c, with func_8010A8D0 = animButtonText and func_8010A7A4 =
 * printOption(x, y, item, name, value) (register parameters s1=x, s2=y, s3=name, s4=value, a0=item come out of
 * IPA with this prototype order). DRAFT unit (`--internal func_8010A7A4 --internal func_8010A8D0`): A7A4 16/75
 * (ops 7, frame 56/56), A8D0 325/375, AEAC 357/413. TB_FlashTextColors(D_801146BC, D_801146C0) is an inlined
 * helper whose two by-value colour params have stack homes (retail 40/44 and 32/36) - here forced with address-
 * taken pointers (colour residual: our params get v0/v1, retail uses ring temps).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef u32 Color;

typedef struct TextHdr {
    char *str[64];
} TextHdr;

typedef struct LangInfo {
    u8 pad0[32];
    u16 base;
} LangInfo;

typedef struct Text {
    u8 pad0[4];
    TextHdr *hdr;
    u8 pad8[4];
    LangInfo *info;
    char **strings;
} Text;

typedef struct ButtonInfo {
    u8 pad0[8];
    f32 ang;
    u8 padC[48 - 12];
    f32 pos[3];
    u8 alpha;
    u8 pad3D[3];
} ButtonInfo;

typedef struct Pair {
    s32 h, v;
} Pair;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s32 D_8015698C;
extern Text D_8017A4E0;
extern s32 D_80116D0C;
extern s16 D_80116D9C;
extern ButtonInfo D_8011650C[];
extern f32 D_80116CFC[3];
extern u8 gCamera[];
extern Color D_801146BC;
extern Color D_801146C0;
extern Color D_80118E28;
extern Color D_80118E2C;
extern Pair D_80118E20;
extern s8 D_80146108[];
extern s16 D_8013FEC8;
extern char *D_801164D0[];
extern s32 D_80116D14[14];
extern s16 D_80149DA2;
extern s16 D_80149B84;
extern s8 D_80116DA8;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void render_helper(f32);
void func_800ED66C(f32);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);
u32 object_manager_update(void *, s32);
s16 object_bytes_sum_global(void);
void brake_light_update(s32, f32 *, void *, f32 *, s16 *);
void particle_velocity_set(void);
void fcvt_wrapper(char *, char *, ...);

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

static void TB_FlashTextColors(Color a, Color b)
{
    Color *pa = &a;
    Color *pb = &b;

    D_80118E28 = *pa;
    D_80118E2C = *pb;
}

static void TB_SetJust(s32 h, s32 v)
{
    D_80118E20.v = v;
    D_80118E20.h = h;
}

void func_8010A7A4(s32 x, s32 y, s32 item, char *name, char *value)
{
    if (D_80116D0C == 1) {
        if (D_8011650C[item].ang < 1.5707964f) {
            TB_FlashTextColors(D_801146BC, D_801146C0);
            dispatch_handler(1);
        } else {
            TB_FlashTextColors(D_801146BC, D_801146C0);
            dispatch_handler(22);
        }
    } else {
        dispatch_handler(item == D_80116D9C ? 22 : 1);
    }
    state_utility(x - 105, y, name);
    state_utility(x + 40, y, value);
}

#define OPT_VALUE(k) D_8017A4E0.strings[D_8017A4E0.info->base + D_80146108[k]]

void func_8010A8D0(void)
{
    s32 i;
    s32 x;
    s32 y;
    char *str;
    s16 screenPos[2];

    render_helper(0.0f);
    TB_SetJust(0, 1);
    render_helper(32320.0f);
    brake_light_update(0, D_80116CFC, gCamera, 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    font_set(13);
    dispatch_handler(1);
    str = D_8017A4E0.hdr->str[50];
    state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
    font_set(11);
    for (i = 0; i < 14; i++) {
        if ((0.69813168f < D_8011650C[i].ang && D_8011650C[i].ang < 2.443461f) || !D_8011650C[i].alpha) {
            continue;
        }
        func_800ED66C(D_8011650C[i].alpha);
        render_helper(32384.0f);
        brake_light_update(0, D_8011650C[i].pos, gCamera, 0, screenPos);
        x = screenPos[0] + 50;
        y = screenPos[1];
        switch (i) {
        case 0:
            func_8010A7A4(x, y, 0, D_8017A4E0.hdr->str[25], D_801164D0[D_8013FEC8]);
            break;
        case 1:
            func_8010A7A4(x, y, 1, D_8017A4E0.hdr->str[13], OPT_VALUE(10));
            break;
        case 2:
            func_8010A7A4(x, y, 2, D_8017A4E0.hdr->str[15], OPT_VALUE(0));
            break;
        case 3:
            func_8010A7A4(x, y, 3, D_8017A4E0.hdr->str[16], OPT_VALUE(1));
            break;
        case 4:
            func_8010A7A4(x, y, 4, D_8017A4E0.hdr->str[17], OPT_VALUE(2));
            break;
        case 5:
            func_8010A7A4(x, y, 5, D_8017A4E0.hdr->str[18], OPT_VALUE(3));
            break;
        case 6:
            func_8010A7A4(x, y, 6, D_8017A4E0.hdr->str[19], OPT_VALUE(4));
            break;
        case 7:
            func_8010A7A4(x, y, 7, D_8017A4E0.hdr->str[20], OPT_VALUE(5));
            break;
        case 8:
            func_8010A7A4(x, y, 8, D_8017A4E0.hdr->str[21], OPT_VALUE(6));
            break;
        case 9:
            func_8010A7A4(x, y, 9, D_8017A4E0.hdr->str[22], OPT_VALUE(7));
            break;
        case 10:
            func_8010A7A4(x, y, 10, D_8017A4E0.hdr->str[23], OPT_VALUE(8));
            break;
        case 11:
            func_8010A7A4(x, y, 11, D_8017A4E0.hdr->str[24], OPT_VALUE(9));
            break;
        case 12:
            func_8010A7A4(x, y, 12, D_8017A4E0.hdr->str[26], OPT_VALUE(18));
            break;
        case 13:
            func_8010A7A4(x, y, 13, D_8017A4E0.hdr->str[14], OPT_VALUE(11));
            break;
        }
    }
    particle_velocity_set();
    TB_SetJust(0, 3);
    func_800ED66C(-1.0f);
    TB_FlashTextColors(D_801146BC, D_801146C0);
    render_helper(-1.0f);
}

s32 func_8010AEAC(void *item)
{
    s32 i;
    s32 y;
    char *str;
    s16 itemsDrawn;
    char str1[40];

    if (D_80116D0C == 1) {
        func_8010A8D0();
        return 1;
    }
    render_helper(0.0f);
    font_set(13);
    dispatch_handler(1);
    if ((D_801174B4 & 0x7C0000) && D_80116DA8) {
        fcvt_wrapper(str1, "CONTROLLER %d %s", D_8015698C + 1, D_8017A4E0.hdr->str[233]);
        state_utility(160 - (object_manager_update(str1, -1) >> 1), 10, str1);
    } else {
        str = D_8017A4E0.hdr->str[50];
        state_utility(160 - (object_manager_update(str, -1) >> 1), 10, str);
    }
    y = object_bytes_sum_global() * 2 + 10;
    font_set(11);
    itemsDrawn = 0;
    for (i = 0; i < 14; i++) {
        if (!D_80116D14[i]) {
            continue;
        }
        if (i < D_80149DA2) {
            continue;
        }
        if (itemsDrawn >= D_80149B84) {
            continue;
        }
        switch (i) {
        case 0:
            func_8010A7A4(160, y, 0, D_8017A4E0.hdr->str[25], D_801164D0[D_8013FEC8]);
            break;
        case 1:
            func_8010A7A4(160, y, 1, D_8017A4E0.hdr->str[13], OPT_VALUE(10));
            break;
        case 2:
            func_8010A7A4(160, y, 2, D_8017A4E0.hdr->str[15], OPT_VALUE(0));
            break;
        case 3:
            func_8010A7A4(160, y, 3, D_8017A4E0.hdr->str[16], OPT_VALUE(1));
            break;
        case 4:
            func_8010A7A4(160, y, 4, D_8017A4E0.hdr->str[17], OPT_VALUE(2));
            break;
        case 5:
            func_8010A7A4(160, y, 5, D_8017A4E0.hdr->str[18], OPT_VALUE(3));
            break;
        case 6:
            func_8010A7A4(160, y, 6, D_8017A4E0.hdr->str[19], OPT_VALUE(4));
            break;
        case 7:
            func_8010A7A4(160, y, 7, D_8017A4E0.hdr->str[20], OPT_VALUE(5));
            break;
        case 8:
            func_8010A7A4(160, y, 8, D_8017A4E0.hdr->str[21], OPT_VALUE(6));
            break;
        case 9:
            func_8010A7A4(160, y, 9, D_8017A4E0.hdr->str[22], OPT_VALUE(7));
            break;
        case 10:
            func_8010A7A4(160, y, 10, D_8017A4E0.hdr->str[23], OPT_VALUE(8));
            break;
        case 11:
            func_8010A7A4(160, y, 11, D_8017A4E0.hdr->str[24], OPT_VALUE(9));
            break;
        case 12:
            func_8010A7A4(160, y, 12, D_8017A4E0.hdr->str[26], OPT_VALUE(18));
            break;
        case 13:
            func_8010A7A4(160, y, 13, D_8017A4E0.hdr->str[14], OPT_VALUE(11));
            break;
        }
        y += object_bytes_sum_global();
        itemsDrawn++;
    }
    func_800ED66C(-1.0f);
    TB_FlashTextColors(D_801146BC, D_801146C0);
    render_helper(-1.0f);
    return 1;
}
