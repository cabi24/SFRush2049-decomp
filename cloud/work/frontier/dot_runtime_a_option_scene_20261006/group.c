/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    char *name;s16 x,y,width,height,top,bottom,left,right;
    u32 z,alpha;s32 (*callback)(Blit *);u32 descriptor;
} MultiBlit;
typedef struct MenuOwner MenuOwner;
typedef struct Player {u8 other[72];MenuOwner *owner;} Player;
typedef struct MenuObjectSlot {s32 other,handle;f32 angle,matrix[3][3],position[3];u8 alpha,pad[3];} MenuObjectSlot;
typedef union Color {u32 word;u8 channel[4];} Color;
typedef struct Texture Texture;
typedef struct Choice {s8 option;u8 pad1[3];f32 current_x,y,z,target_x;s8 relative;u8 pad21[3];} Choice;
typedef struct SavedOption {u8 value;u8 other[4];} SavedOption;
extern s16 D_8014A108;
extern s32 D_8014A110,D_803AF980;
extern s32 D_803B77D4[][21];
extern s8 D_80146108[],D_8014978C,D_80156994;
extern s8 D_803B8314[19];
extern s32 func_800F7644(s32);

s32 func_8038ACB0(s32 index)
{
    s32 mode = D_8014A110;
    s32 allowed = D_803B77D4[mode][index];
    if (mode == 0 && index == 17) allowed = D_8014A108 < 3;
    if (mode == 4) {
        if (index == 5) allowed = D_80146108[32] == 1;
        if (index == 6) allowed = D_80146108[32] == 0;
        if (index == 7) allowed = D_80146108[32] == 2;
    }
    if (mode == 6) {
        if (index == 9) allowed = D_80146108[37] == 0;
        if (index == 11) allowed = D_80146108[37] == 0;
        if (index == 10) allowed = D_80146108[37] == 1;
    }
    if (index == 19 && D_8014A108 == 1) allowed = 0;
    if (index == 12) allowed = D_8014978C >= 0 && D_8014978C < 6;
    if (!D_80156994 && D_8014978C == 5 && index != 0) allowed = 0;
    if (!D_803AF980 && index == 1) allowed = 0;
    return allowed;
}

s32 func_8038B834(s32 index)
{
    s8 *option = &D_803B8314[index];
    if (!*option) return 0;
    if (!func_800F7644(index)) return 0;
    if (D_8014A110 == 4) return option >= D_803B8314 + 14 && option < D_803B8314 + 18;
    if (D_8014A110 == 5) return option >= D_803B8314 + 18 && option < D_803B8314 + 19;
    if (D_8014A110 == 6) return option >= D_803B8314 + 6 && option < D_803B8314 + 14;
    return option >= D_803B8314 && option < D_803B8314 + 6;
}

extern f32 D_8011418C[12];
extern u8 D_80140BDC;
extern s32 sign_extend_call(u32,u32,s32,u32);
extern s32 string_copy_format(char *,s8,s8,s8);
extern void *memcpy(void *,const void *,u32);
extern Texture *func_800B24EC(char *,s16 *,s8,s8,s32);
extern void func_8008D870(s16,s32,s32);
extern void func_8008E06C(s16,s32 *);
extern void func_8008B32C(f32 [3][3],f32 [3][3],f32);
extern void func_800B5898(f32,f32 [][3]);
extern void func_800B5940(f32,f32 [][3]);
extern void particle_velocity_set(void);
extern MenuObjectSlot D_803B6B14[48];
extern s32 D_803B7714,D_803B7718;
extern Color D_803B8394;
extern char *D_803B6AF0[];
extern char D_803B9264[],D_803B9274[],D_803B927C[],D_803B9280[];
extern f32 D_803B930C,D_803B9310,D_803B9314,D_803B9318,D_803B931C;
extern f32 D_803B9320,D_803B9324,D_803B9328;

void func_8038C93C(void)
{
    Color color = D_803B8394;
    s32 i;
    s16 texture_index;
    f32 yaw;
    f32 y;
    for (i = 0; i < 48; i++) {
        if (D_803B6B14[i].handle == -1) {
            memcpy(D_803B6B14[i].matrix, D_8011418C, 36);
            D_803B6B14[i].handle = sign_extend_call(
                string_copy_format(D_803B6AF0[D_803B6B14[i].other], 0, D_80140BDC - 1, 1),
                (u32)D_803B6B14[i].matrix, -1,
                (D_803B6B14[i].other == 0 || D_803B6B14[i].other == 1 || D_803B6B14[i].other == 2) ? 0x42000 : 0);
            func_8008E06C((s16)D_803B6B14[i].handle, (s32 *)&color);
        }
    }
    func_8008D870((s16)D_803B6B14[42].handle,
        (s32)func_800B24EC(D_803B9264, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[42].matrix, D_8011418C, 36);
    yaw = D_803B930C;
    func_800B5898(yaw, D_803B6B14[42].matrix);
    D_803B6B14[42].position[0] = 0.0f;
    D_803B6B14[42].position[1] = 115.0f;
    D_803B6B14[42].position[2] = 200.0f;
    func_8008D870((s16)D_803B6B14[43].handle,
        (s32)func_800B24EC(D_803B9274, &texture_index, 0, D_80140BDC - 1, 1), -1);
    func_8008D870((s16)D_803B6B14[44].handle,
        (s32)func_800B24EC(D_803B927C, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[44].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[44].matrix);
    y = D_803B9310;
    D_803B6B14[44].position[0] = D_803B9314;
    D_803B6B14[44].position[1] = y;
    D_803B6B14[44].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[44].matrix, D_803B6B14[44].matrix, 0.5f);
    memcpy(D_803B6B14[45].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[45].matrix);
    func_800B5940(D_803B9318, D_803B6B14[45].matrix);
    D_803B6B14[45].position[0] = D_803B931C;
    D_803B6B14[45].position[1] = y;
    D_803B6B14[45].position[2] = 100.0f;
    func_8008D870((s16)D_803B6B14[46].handle,
        (s32)func_800B24EC(D_803B9280, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[46].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[46].matrix);
    y = D_803B9320;
    D_803B6B14[46].position[0] = D_803B9324;
    D_803B6B14[46].position[1] = y;
    D_803B6B14[46].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[46].matrix, D_803B6B14[46].matrix, 0.5f);
    memcpy(D_803B6B14[47].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[47].matrix);
    D_803B6B14[47].position[0] = D_803B9328;
    D_803B6B14[47].position[1] = y;
    D_803B6B14[47].position[2] = 100.0f;
    D_803B7714 = 1;
    D_803B7718 = 1;
    particle_velocity_set();
}

extern s16 D_803BA85A,D_803BA878,D_803BA898,D_803BA8B0[];
extern s16 D_803BA84E,D_803BA856,D_803BA852,D_803BA222;
extern u16 D_803BA220;
extern s8 D_803BA910,D_80154628,D_80142726,D_803B7CD4;
extern s32 D_803BAC78,D_803BAC90;
extern SavedOption D_801543D8[];
extern Player D_8014A118[];
extern Choice D_803BA918[];
extern f32 D_803BAE10[3],D_803B7A20[],D_803BACCC;
extern MultiBlit D_803B7A6C[];
extern Blit *D_803B7CD0;
extern void func_8038CD14(void);
extern u8 func_800CDC3C(MenuOwner *,s32);
extern void menu_text_input(MenuOwner *,s32,u8);
extern s32 reverb_setup(MenuOwner *,u8);
extern void init_state_begin(void);
extern void particle_lifetime_set(void);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void func_8038A634(s32);
extern void particle_position_set(s16);
extern void Effects_UpdateEmitters(void);
extern void entity_audio_update(void);

void func_8038DFEC(void)
{
    s32 i,j,phase,step;
    f32 position;
    Choice *choice;
    D_803BA85A = 0;
    D_803BA878 = 0;
    D_803BA910 = 0;
    D_803BAC78 = -1;
    D_803BA898 = 0;
    for (i = 0; i < 21; i++) {
        if (func_8038ACB0(i)) {
            D_803BA8B0[D_803BA898] = i;
            D_803BA898++;
        }
    }
    func_8038CD14();
    if (D_8014A110 == 3) {
        D_8014978C = D_801543D8[D_80154628].value;
        D_803BA84E = D_8014978C;
    } else {
        if (D_8014A110 == 4) {
            D_8014978C = func_800CDC3C(D_8014A118[0].owner,D_8014A110) + 14;
            D_803BA84E = D_8014978C;
        } else if (D_8014A110 == 5) {
            D_8014978C = func_800CDC3C(D_8014A118[0].owner,D_8014A110) + 18;
            D_803BA84E = D_8014978C;
        } else if (D_8014A110 == 6) {
            D_8014978C = func_800CDC3C(D_8014A118[0].owner,D_8014A110) + 6;
            D_803BA84E = D_8014978C;
        } else {
            D_8014978C = func_800CDC3C(D_8014A118[0].owner,D_8014A110);
            D_803BA84E = D_8014978C;
        }
        if (!func_8038B834(D_8014978C)) {
            if (D_8014A110 == 4) {
                D_8014978C = 14;
                D_803BA84E = 14;
            } else if (D_8014A110 == 5) {
                D_8014978C = 18;
                D_803BA84E = 18;
            } else if (D_8014A110 == 6) {
                D_8014978C = 6;
                D_803BA84E = D_8014978C;
            } else {
                D_8014978C = 0;
                D_803BA84E = 0;
            }
        }
        for (i = 0; i < D_8014A108; i++) {
            if (D_8014A110 == 4)
                menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)(D_8014978C - 14));
            else if (D_8014A110 == 5)
                menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)(D_8014978C - 18));
            else if (D_8014A110 == 6)
                menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)(D_8014978C - 6));
            else
                menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)D_8014978C);
        }
        for (i = 21; i < 41; i++)
            D_80146108[i] = reverb_setup(D_8014A118[0].owner,(u8)i);
    }
    init_state_begin();
    particle_lifetime_set();
    func_8038C93C();
    D_803B7CD0 = sound_control(0,0,D_803B7A6C,17);
    j = 0;
    for (i = 0; i < 19; i++) {
        if (func_8038B834(i)) {
            if (i == D_8014978C) {
                D_803BA856 = j;
                break;
            }
            j++;
        }
    }
    j = 0;
    D_803BA852 = 19;
    position = 0.0f;
    for (i = 0; i < 19; i++) {
        if (func_8038B834(i)) {
            choice = &D_803BA918[j];
            choice->relative = j - D_803BA856;
            choice->option = i;
            if (choice->relative < 0) choice->relative += D_803BA852;
            if (j == D_803BA856) choice->target_x = 0.0f;
            else if (D_803BA856 >= D_803BA852 / 2)
                choice->target_x = position - (D_803BA852 - D_803BA856) * 100;
            else
                choice->target_x = position + D_803BA856 * 100;
            choice->current_x = choice->target_x;
            choice->y = -5.0f;
            choice->z = 0.0f;
            position -= 100.0f;
            j++;
            if (position < -(D_803BA852 * 100 / 2)) position += D_803BA852 * 100;
        }
    }
    D_803BAE10[0] = 0.0f;
    D_803BAE10[1] = D_803B7A20[D_803BA918[D_803BA856].option] - 5.0f;
    D_803BAE10[2] = 0.0f;
    func_8038A634(1);
    step = D_80142726 ? D_80142726 * 5 : 1;
    phase = D_803BA220 + step;
    if (phase < 0) phase += 2048;
    if (phase >= 2048) phase -= 2048;
    D_803BA220 = phase;
    D_803BA222 = 0;
    particle_position_set(1);
    Effects_UpdateEmitters();
    entity_audio_update();
    D_803B7CD4 = 1;
    D_803BAC78 = -1;
    D_803BAC90 = D_8014978C;
    D_803BACCC = 0.0f;
}
