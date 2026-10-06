/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Local group candidate: func_8039B00C, 69/69 words / 276 bytes.
 * Complete real C140 caller is unclaimed context. This minimal two-function
 * group does not duplicate other configuration-family matching claims.
 * Native data views express accessed offsets/strides, not original type names.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Cfg {u8 other0[4],u4,other5[3];s8 b8,b9;} Cfg;
typedef struct Item {u8 other[16];u8 player;} Item;
typedef struct Selection {Item *item;} Selection;
typedef struct Resource {u8 *bytes;} Resource;
typedef struct MenuOwner MenuOwner;
typedef struct OwnerBody {
    MenuOwner *next,*previous;
    Selection *selection;
    u8 other[32];
    Resource *resource;
} OwnerBody;
struct MenuOwner {OwnerBody *body;};
typedef struct Player {u8 other[72];MenuOwner *owner;} Player;
typedef struct Blit Blit;
typedef struct FiveParts FiveParts;
typedef struct MenuObjectSlot {
    s32 other,handle;
    u8 unaccessed[56];
} MenuObjectSlot;
extern Cfg *D_803BAD90;
extern u8 D_801543D4;
extern u32 D_801174BC,state_word_a;
extern s8 D_803B340C,D_803B342C,D_803B3414,D_803B3418,D_803BADC4;
extern s8 D_803B3424,D_803B341C,D_803B3420,D_80157244;
extern s32 D_803BACF8,D_8015694C,D_803B3448,D_803B344C,D_803B33C4;
extern s32 D_803B3410,D_803BAD38,D_803B33C0;
extern s16 D_803B9A80,D_803B9A82,D_803B9A84,D_8014A108;
extern u8 D_803BAD98[20],D_803B8738[];
extern f32 D_803B9A50[12],D_8002EB94,D_803B95B8;
extern Player D_8014A118[];
extern MenuObjectSlot D_803B30C0[12];
extern Blit *D_803B3408;
extern FiveParts *D_803BACF0;
extern void func_800B5570(s32);
extern void func_80090E9C(f32,f32 (*)[3]);
extern void func_80090254(s16);
extern void resource_type_select_simple(s32);
extern void func_8039B214(void);
extern void func_8039BE48(void);
extern s32 func_8039A448(s32);
extern u32 entity_flags_apply(u32,u32,u32,u8);
extern void net_session_update(void);
extern u8 *func_800BE6A4(u8 *,u8 *);
extern void func_800FDF88(s32);
extern void audio_distance_atten(s32);
extern void func_800B5F88(s32);
extern s32 func_800F7564(s32);
extern s8 func_800A361C(s32);
extern void func_8039A4D4(void);
extern void sound_stop(Blit *);
extern void sound_handles_array_clear(s8 *);
extern void ambient_sounds_clear(void);

void func_8039B00C(void)
{
    s32 i;
    for (i = 0; i < 12; i++) {
        if (D_803B30C0[i].handle != -1) {
            if (state_word_a & 0x7C03FFFE)
                func_80090254((s16)D_803B30C0[i].handle);
            D_803B30C0[i].handle = -1;
        }
    }
    D_803B33C0 = 0;
    if (D_803B3408) {
        sound_stop(D_803B3408);
        D_803B3408 = 0;
    }
    if (D_803BACF0) {
        sound_handles_array_clear((s8 *)D_803BACF0);
        D_803BACF0 = 0;
    }
    if (D_803B3448 != -1) {
        func_80090254((s16)D_803B3448);
        D_803B3448 = -1;
    }
    if (D_803B344C != -1) {
        func_80090254((s16)D_803B344C);
        D_803B344C = -1;
    }
    ambient_sounds_clear();
    D_803B340C = 0;
}

void func_8039C140(void)
{
    s32 i;
    D_803BAD90 = (Cfg *)(D_8014A118[D_801543D4].owner->body->resource->bytes + 0x6F4);
    if (D_801174BC != 1) {
        if (D_801174BC != state_word_a) {
            func_800B5570(0x80);
            return;
        }
        D_801174BC = 1;
    }
    if (!D_803B340C)
        func_8039BE48();
    while (!func_8039A448(D_803BACF8)) {
        D_803BACF8++;
        if (D_803BACF8 >= 4)
            D_803BACF8 = 0;
    }
    if (D_803B342C) {
        func_80090E9C(-90.0f * D_8002EB94 * D_803B95B8, (f32 (*)[3])D_803B9A50);
        if (D_8015694C & 7) {
            if (D_803B3448 != -1) {
                func_80090254((s16)D_803B3448);
                D_803B3448 = -1;
            }
            if (D_803B344C != -1) {
                func_80090254((s16)D_803B344C);
                D_803B344C = -1;
            }
            resource_type_select_simple(D_8015694C);
            D_803B342C = 0;
            D_803B33C4 = 1;
        }
    } else if (D_803B3414) {
        func_8039B214();
    } else if (D_8015694C & 1) {
        entity_flags_apply(0x2E, 0, 1, 0);
        if (D_803B3418) {
            D_803B3418 = 0;
            D_803BADC4 = 0;
            D_803B33C4 = 1;
        } else if (!func_8039A448(0)) {
            D_803BACF8 = 2;
            goto confirm_option;
        } else {
            net_session_update();
            D_803B3410 = 1;
        }
    } else if (D_8015694C & 2) {
        entity_flags_apply(0x25, 0, 1, 0);
 confirm_option:
        switch (D_803BACF8) {
        case 0:
            net_session_update();
            D_803B3410 = 1;
            break;
        case 1:
            break;
        case 2:
            if (D_803BADC4 || !D_803B3424) {
                func_800FDF88(D_803BAD38);
                net_session_update();
                D_803B3410 = 1;
            } else if (D_803B3418) {
                D_803B3418 = 0;
                D_803BADC4 = 0;
                D_803B33C4 = 1;
            } else {
                D_803BADC4 = 0;
                D_803B3418 = 1;
            }
            break;
        case 3:
            D_803B3414 = 1;
            D_803B9A82 = 0;
            D_803B9A80 = D_803B9A82;
            func_800BE6A4(D_803BAD98, D_803B8738);
            D_803B341C = 0;
            D_803B3420 = 0;
            break;
        }
    } else if (D_8015694C & 4) {
        entity_flags_apply(0x26, 0, 1, 0);
        if (D_803B3418) {
            D_803B3418 = 0;
            D_803BADC4 = 0;
            D_803B33C4 = 1;
        } else {
            D_803B3410 = 2;
        }
    } else if (D_8015694C & 0x400) {
        if (!D_803B3418) {
            audio_distance_atten(D_8015694C);
            do {
                D_803BACF8--;
                if (D_803BACF8 < 0)
                    D_803BACF8 = 3;
            } while (!func_8039A448(D_803BACF8));
        }
    } else if (D_8015694C & 0x800) {
        if (!D_803B3418) {
            audio_distance_atten(D_8015694C);
            do {
                D_803BACF8++;
                if (D_803BACF8 >= 4)
                    D_803BACF8 = 0;
            } while (!func_8039A448(D_803BACF8));
        }
    } else if (D_8015694C & 0x3000) {
        if (D_803B3418) {
            func_800B5F88(D_8015694C);
            D_803BADC4 = !D_803BADC4;
        } else if (D_803BACF8 == 2) {
            func_800B5F88(D_8015694C);
            do {
                if (D_8015694C & 0x1000)
                    D_803BAD38--;
                else if (D_8015694C & 0x2000)
                    D_803BAD38++;
                if (D_803BAD38 < 0)
                    D_803BAD38 = 3;
                if (D_803BAD38 >= 4)
                    D_803BAD38 = 0;
            } while (!func_800F7564(D_803BAD38));
        } else if (D_803BACF8 == 1) {
            func_800B5F88(D_8015694C);
            if (D_8015694C & 0x1000)
                D_803B9A84--;
            else if (D_8015694C & 0x2000)
                D_803B9A84++;
            if (D_803B9A84 < 0)
                D_803B9A84 = D_803BAD90->b9;
            if (D_803B9A84 > D_803BAD90->b9)
                D_803B9A84 = 0;
        }
    }
    for (i = 0; i < D_8014A108; i++) {
        if (D_8014A118[i].owner->body->selection) {
            if (func_800A361C(D_8014A118[i].owner->body->selection->item->player)) {
                func_8039B00C();
                func_800B5570(0x20);
                return;
            }
        }
    }
    if (D_80157244) {
        func_8039B00C();
        func_800B5570(4);
    } else if (D_803B3410) {
        func_8039B00C();
        if (D_803B3410 == 1) {
            switch (D_803BACF8) {
            case 0: func_800B5570(0x80); break;
            case 2: func_800B5570(0x80); break;
            }
        } else if (D_803B3410 == 2) {
            func_800B5570(0x40);
        }
        D_803B3410 = 0;
    }
    func_8039A4D4();
}
