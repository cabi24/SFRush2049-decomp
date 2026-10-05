typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Config {
    /* 0 */ s8 type;
    /* 1 */ u8 pad1[4];
    /* 5 */ u8 selection;
    /* 6 */ u8 flags;
    /* 7 */ u8 kind;
} Config;

typedef struct Player {
    /* 0 */ u8 pad0;
    /* 1 */ u8 id;
    /* 2 */ u8 pad2[74];
} Player; /* 0x4C */

extern s16 D_8014A108;
extern s16 D_80142724;
extern s16 D_801543CA;
extern s32 D_8014A110;
extern s8 D_80154628;
extern s8 D_8014978C;
extern u8 D_801543D4;
extern Player D_80154450[];
extern Config D_80153E88[];
extern int D_8011735C;

int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

f32 frand(f32 range) {
    return func_8008B2B4() * range / 32768.0f;
}
s32 pick(s32 slot) {
    if (slot == 0) {
        return D_801543D4;
    }
    return (slot == 1) ? D_801543D4 ^ 1 : slot;
}
void init_state_continue(void)
{
    s32 extra;
    s32 slot;
    s32 selected;
    s32 prior;
    u8 next = 0;
    Config *config;

    extra = D_80142724;
    if (6 - D_8014A108 < extra) {
        extra = 6 - D_8014A108;
    }
    if (D_8014A110 == 4 || D_8014A110 == 5 || D_8014A110 == 6) {
        extra = 0;
    }
    D_801543CA = D_8014A108 + extra;
    config = D_80153E88;
    for (slot = 0; slot < 6; slot++, config++) {
        if (D_8014A110 == 3 && slot < D_801543CA && D_80154628 > 0) {
            selected = (slot == 0) ? D_801543D4 : ((slot == 1) ? D_801543D4 ^ 1 : slot);
            config->selection = D_80154450[selected].id;
            config->flags = 176;
            if (slot < D_8014A108) {
                config->kind = 6;
            } else {
                config->kind = 0;
            }
        } else if (slot < D_8014A108) {
            config->selection = extra++;
            config->kind = 6;
            config->flags = 176;
        } else if (slot < D_801543CA) {
            if (D_8014A110 == 3) {
                for (;;) {
                    config->selection = frand(D_801543CA);
                    for (prior = 0; prior < slot; prior++) {
                        if (config->selection == D_80153E88[prior].selection) {
                            break;
                        }
                    }
                    if (slot != prior) {
                        continue;
                    }
                    break;
                }
            } else {
                config->selection = next++;
            }
            config->kind = 0;
            config->flags = 176;
        } else {
            config->kind = 7;
            config->flags = 0;
        }
        config->type = D_8014978C;
    }
}

void standin_caller(s32 a)
{
    if (a) {
        init_state_continue();
    }
    init_state_continue();
}
