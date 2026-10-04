typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 pad00[8];
    s32 unk08;
    u8 pad0C[8];
    char name[1];
} PlayerProfile;

typedef struct {
    u8 pad00;
    u8 active;
    u8 pad02[0x48 - 2];
    PlayerProfile **profile;
} InputRecord;

extern s16 active_player_count;
extern InputRecord input_rec0[];
extern s32 D_801424F0[];
extern s8 D_80143F18[];
extern char D_80143A50[][13];
extern s16 D_80144010[];
extern s16 D_80144C40[];
extern s8 D_80151AC0[3][5];
extern char D_80151968[][13];
extern s32 D_801148D4[];

void *memset(void *dst, s32 c, u32 n);
u32 strlen(const char *s);
void func_800A473C(char *dst, char *src);

void func_800F2888(void)
{
    s32 i;
    s32 j;
    s32 k;
    PlayerProfile **profile;

    for (i = 0; i < active_player_count; i++) {
        profile = input_rec0[i].profile;
        if ((*profile)->unk08 == 0) {
            D_801424F0[i] = D_801148D4[input_rec0[i].active];
        } else {
            D_801424F0[i] = (s32)profile;
        }
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 5; k++) {
                if (D_80151AC0[j][k] == i) {
                    D_80143F18[i] = 1;
                }
            }
        }
        if (D_80143F18[i] == 0) {
            continue;
        }
        if ((*profile)->unk08 == 0) {
            D_80143F18[i] = 1;
            D_80144010[i] = -1;
            if (D_801424F0[i] == 0xFFFFFFFF) {
                memset(D_80143A50[i], 0, 13);
            } else {
                func_800A473C(D_80143A50[i], D_80151968[D_801424F0[i]]);
            }
            D_80144C40[i] = strlen(D_80143A50[i]);
        } else {
            D_80143F18[i] = 0;
            func_800A473C(D_80143A50[i], (*profile)->name);
        }
    }
}

void __standin_a(void) { func_800F2888(); }
void __standin_b(void) { func_800F2888(); }
