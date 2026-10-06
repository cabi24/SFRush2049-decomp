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
typedef struct Item {u8 other[16];u8 player;} Item;
typedef struct Selection {Item *item;} Selection;
typedef struct Resource {u8 *bytes;} Resource;
typedef struct OwnerBody {MenuOwner *next,*previous;Selection *selection;u8 other[32];Resource *resource;} OwnerBody;
struct MenuOwner {OwnerBody *body;};
typedef struct Player {u8 other0[8];u32 flags;u8 other12[60];MenuOwner *owner;} Player;
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

extern Blit *D_803B8288,*D_803B828C;
extern s32 D_803B8284;
/* Native polling contract also used in the accepted input-aux/audio groups. */
extern volatile s16 D_8002EB70;
extern void sound_stop(Blit *);
extern void func_80090254(s16);
extern void ambient_sounds_clear(void);
extern u32 state_word_a,D_801174BC;
extern void func_800A5B3C(void);
extern void func_800960D4(void *);
typedef struct ResourceQuad {u32 words[4];} ResourceQuad;
typedef struct ModelResource {u8 other[24];ResourceQuad quad;u8 tail[48];} ModelResource;
typedef struct ModelBank {ModelResource *models;u32 other;} ModelBank;
typedef struct Slot72 {s32 handle;f32 offset_x,offset_y,angle,scale_from,scale_to;f32 matrix[3][3],position[3];} Slot72;
extern Slot72 D_803B7CE4[20];
extern s8 D_803BAAE0;
extern ResourceQuad D_803BAB58[7],D_803BAAE8[7];
extern void *D_803BABC8[7],*D_803BABE8[7];
extern ModelBank D_801161F4[];
extern char *D_803B7738[];

void func_8038AE50(void)
{
    if (D_803B8288) { sound_stop(D_803B8288); D_803B8288 = 0; }
    if (D_803B828C) { sound_stop(D_803B828C); D_803B828C = 0; }
}

void func_8038B388(void)
{
    func_8038AE50();
    if (D_803B8284 != -1) { func_80090254((s16)D_803B8284); D_803B8284 = -1; }
    while (D_8002EB70) {}
}

void func_8038B940(void)
{
    s32 i,model;
    for (i = 0; i < 48; i++) {
        if (D_803B6B14[i].handle != -1) {
            if (state_word_a & 0x7C03FFFE) func_80090254((s16)D_803B6B14[i].handle);
            D_803B6B14[i].handle = -1;
        }
    }
    D_803B7714 = 0;
    if (D_803B7CD0) { sound_stop(D_803B7CD0); D_803B7CD0 = 0; }
    ambient_sounds_clear();
    func_8038AE50();
    if (D_803B8284 != -1) { func_80090254((s16)D_803B8284); D_803B8284 = -1; }
    while (D_8002EB70) {}
    if (D_803BAAE0) {
        for (i = 0; i < 20; i++) {
            if (D_803B7CE4[i].handle != -1) {
                func_80090254((s16)D_803B7CE4[i].handle);
                D_803B7CE4[i].handle = -1;
            }
        }
        while (D_8002EB70) {}
        for (i = 0; i < 7; i++) {
            model = string_copy_format(D_803B7738[i < 6 ? i : 19],0,D_80140BDC - 1,1);
            D_801161F4[model >> 10].models[model & 0x3FF].quad = D_803BAB58[i];
            if (D_803BABC8[i]) func_800960D4(D_803BABC8[i]);
            D_803BABC8[i] = 0;
            if (D_803BABE8[i]) func_800960D4(D_803BABE8[i]);
            D_803BABE8[i] = 0;
            D_803BAAE8[i].words[2] = 0;
            D_803BAAE8[i].words[3] = 0;
        }
        D_803BAAE0 = 0;
    }
    func_800A5B3C();
    D_803B7CD4 = 0;
}

extern s8 D_80157244,D_8013F1D9,D_803BA908;
extern u8 D_801543D4;
extern s32 D_8015694C;
extern u32 D_80156944,D_80149784;
extern f32 D_803B7CE0;
extern void func_800B5570(s32);
extern s8 func_800A361C(s32);
extern void func_8038C5EC(void);
extern void resource_type_select(s32);
extern void func_800B5F88(s32);
extern u32 entity_flags_apply(u32,u32,u32,u8);
extern void audio_bus_mix(MenuOwner *,u8,s8);
extern void func_8038AEB0(void);
extern void func_8038B3EC(void);
extern void func_8038A820(void);

void func_8038E5DC(void)
{
    s32 i,j,direction;
    if (D_801174BC != 1) {
        if (D_801174BC != state_word_a) {func_800B5570(256);return;}
        D_801174BC = 1;
    }
    if (!D_803B7CD4) func_8038DFEC();
    func_8038C5EC();
    for (i = 0; i < D_8014A108; i++) {
        if (D_8014A118[i].owner->body->selection) {
            if (func_800A361C(D_8014A118[i].owner->body->selection->item->player)) {
                func_8038B940();func_800B5570(32);return;
            }
        }
    }
    if (D_80157244) {func_8038B940();func_800B5570(4);return;}
    if ((D_8015694C & 3) && (D_80156994 || D_8014978C != 5)) {
        resource_type_select(D_8015694C);func_800B5570(256);func_8038B940();
        for (i = 0; i < D_8014A108; i++)
            for (j = 21; j < 41; j++) audio_bus_mix(D_8014A118[i].owner,(u8)j,D_80146108[j]);
        return;
    }
    if (D_8015694C & 4) {
        entity_flags_apply(38,0,1,0);func_8038B940();
        if (D_8014A110 == 3) func_800B5570(0x4000);
        else func_800B5570(64);
        return;
    }
    if (D_80156944 & 0x3080) {
        direction = (D_80149784 & 0x1000) ? -1 : (D_80149784 & 0x2000) ? 1 : 0;
        if (D_8014A110 == 3 && (D_80154628 > 0 || D_803BA8B0[D_803BA85A] != 20)) goto after_scroll;
        if (!direction && !D_803BA910) goto after_scroll;
        if (D_8014A110 == 2 && D_803BA8B0[D_803BA85A] == 2) goto after_scroll;
        func_800B5F88(D_80149784);
        if (D_8013F1D9 >= 2) direction = -direction;
        switch ((u16)D_803BA8B0[D_803BA85A]) {
        case 0:
            if (direction) {
                D_803BA856 += direction;
                if (D_803BA856 < 0) D_803BA856 = D_803BA852 - 1;
                else if (D_803BA856 >= D_803BA852) D_803BA856 = 0;
                do {
                    D_8014978C += direction;
                    if (D_8014978C < 0) D_8014978C = 18;
                    else if (D_8014978C >= 19) D_8014978C = 0;
                } while (!func_8038B834(D_8014978C));
                for (j = 21; j < 41; j++) D_80146108[j] = reverb_setup(D_8014A118[0].owner,(u8)j);
                if (!D_80156994) D_803B7718 = 1;
            }
            break;
        case 1:
            if (direction) {
                D_80146108[25] += direction;
                if (D_80146108[25] < 0) D_80146108[25] = 1;
                else if (D_80146108[25] > 1) D_80146108[25] = 0;
            } else D_80146108[25] = 0;
            break;
        case 2:
            if (direction) {
                D_80146108[21] += direction;
                if (D_80146108[21] < 1) D_80146108[21] = 8;
                else if (D_80146108[21] > 8) D_80146108[21] = 1;
            } else D_80146108[21] = 3;
            break;
        case 3:
            if (direction) {
                D_80146108[32] += direction;
                if (D_80146108[32] < 0) D_80146108[32] = 2;
                else if (D_80146108[32] > 2) D_80146108[32] = 0;
            } else D_80146108[32] = 0;
            break;
        case 4:
            if (direction) {
                if (!D_80146108[33]) D_80146108[33] = 1;
                else D_80146108[33] = 0;
            }
            else D_80146108[33] = 0;
            break;
        case 5:
            if (direction) {
                D_80146108[34] += direction;
                if (D_80146108[34] < 1) D_80146108[34] = 100;
                else if (D_80146108[34] > 100) D_80146108[34] = 1;
            } else D_80146108[34] = 69;
            break;
        case 6:
            if (direction) {
                D_80146108[35] += direction;
                if (D_80146108[35] < 1) D_80146108[35] = 20;
                else if (D_80146108[35] > 20) D_80146108[35] = 1;
            } else D_80146108[35] = 5;
            break;
        case 7:
            if (direction) {
                D_80146108[36] += direction;
                if (D_80146108[36] < 1) D_80146108[36] = 50;
                else if (D_80146108[36] > 50) D_80146108[36] = 1;
            } else D_80146108[36] = 30;
            break;
        case 8:
            if (direction) {
                if (!D_80146108[37]) D_80146108[37] = 1;
                else D_80146108[37] = 0;
            }
            else D_80146108[37] = 0;
            break;
        case 9:
            if (direction) {
                D_80146108[38] += direction * 5;
                if (D_80146108[38] < 5) D_80146108[38] = 50;
                else if (D_80146108[38] > 50) D_80146108[38] = 5;
            } else D_80146108[38] = 10;
            break;
        case 10:
            if (direction) {
                D_80146108[39] += direction;
                if (D_80146108[39] < 1) D_80146108[39] = 20;
                else if (D_80146108[39] > 20) D_80146108[39] = 1;
            } else D_80146108[39] = 8;
            break;
        case 11:
            if (direction) D_80146108[40] = D_80146108[40] ^ 1;
            else D_80146108[40] = 0;
            break;
        case 12:
            if (direction) D_80146108[27] ^= 1;
            else D_80146108[27] = 0;
            func_8038B388();func_8038AEB0();
            D_803B7CE0 = 1.0f - D_803B7CE0;
            break;
        case 13:
            if (direction) {
                if ((D_8014A118[0].flags & 0x3C000) == 0x3C000) D_80146108[28] = 2;
                else if (!D_80146108[28]) D_80146108[28] = 1;
                else D_80146108[28] = 0;
            } else D_80146108[28] = 0;
            func_8038B388();func_8038AEB0();
            break;
        case 14:
            if (direction) {
                D_80146108[29] += direction;
                if (D_80146108[29] < 0) D_80146108[29] = 3;
                else if (D_80146108[29] > 3) D_80146108[29] = 0;
            } else D_80146108[29] = 0;
            break;
        case 15:
            if (direction) {
                D_80146108[30] += direction;
                if (D_80146108[30] < 0) D_80146108[30] = 4;
                else if (D_80146108[30] > 4) D_80146108[30] = 0;
            } else D_80146108[30] = 0;
            break;
        case 16:
            if (direction) {
                D_80146108[23] += direction;
                if (D_80146108[23] < 0) D_80146108[23] = 3;
                else if (D_80146108[23] > 3) D_80146108[23] = 0;
            } else D_80146108[23] = 1;
            break;
        case 17:
            if (direction) {
                D_80146108[22] += direction;
                if (D_80146108[22] > 5) D_80146108[22] = 0;
                else if (D_80146108[22] < 0) D_80146108[22] = 5;
            } else D_80146108[22] = 5;
            break;
        case 18:
            if (direction) {
                D_80146108[24] += direction;
                if (D_80146108[24] < 0) D_80146108[24] = 5;
                else if (D_80146108[24] > 5) D_80146108[24] = 0;
            } else D_80146108[24] = 2;
            break;
        case 19:
            if (direction) {
                D_80146108[26] += direction;
                if (D_80146108[26] < 0) D_80146108[26] = 2;
                else if (D_80146108[26] > 2) D_80146108[26] = 0;
            } else D_80146108[26] = 2;
            break;
        case 20:
            if (direction) D_80146108[31] ^= 1;
            else D_80146108[31] = 0;
            if (D_8014A110 == 3) D_8014A118[D_801543D4].owner->body->resource->bytes[0x6F9] = D_80146108[31];
            break;
        }
        init_state_begin();
        for (j = 21; j < 41; j++) audio_bus_mix(D_8014A118[0].owner,(u8)j,D_80146108[j]);
    } else if (D_80149784 & 0x400) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(40,0,1,0);D_803BA85A--;
            if (D_803BA85A < 0) D_803BA85A = D_803BA898 - 1;
        }
    } else if (D_80149784 & 0x800) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(39,0,1,0);D_803BA85A++;
            if (D_803BA85A >= D_803BA898) D_803BA85A = 0;
        }
    }
    if (D_803BA85A - 1 < D_803BA878) D_803BA878 = D_803BA85A - 1;
    if (D_803BA878 < D_803BA85A - 2) D_803BA878 = D_803BA85A - 2;
    if (D_803BA898 - 4 < D_803BA878) D_803BA878 = D_803BA898 - 4;
    if (D_803BA878 < 0) D_803BA878 = 0;
 after_scroll:
    D_803BA908 = D_8014A110 != 3 || (D_80154628 == 0 && D_803BA8B0[D_803BA85A] == 20);
    if (D_803BA84E != D_8014978C) {
        D_803BAC90 = D_8014978C;
        D_803BA84E = D_8014978C;
        for (i = 0; i < D_8014A108; i++) {
            switch (D_8014A110) {
            case 0: case 1: case 2:
                if (D_8014978C >= 0 && D_8014978C < 6)
                    menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)D_8014978C);
                break;
            case 4:
                if (D_8014978C >= 14 && D_8014978C < 18)
                    menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)(D_8014978C - 14));
                break;
            case 6:
                if (D_8014978C >= 6 && D_8014978C < 14)
                    menu_text_input(D_8014A118[i].owner,D_8014A110,(u8)(D_8014978C - 6));
                break;
            }
        }
    }
    func_8038B3EC();func_8038A820();func_8038A634(0);
    particle_position_set(1);Effects_UpdateEmitters();
}

struct Blit {u8 other0[18];u16 priority;u8 other20[32];u16 resource;};
typedef struct ImageMeta {u8 other0[21];u8 flags;u8 other22[10];} ImageMeta;
typedef struct DisplayCommand {u32 w0,w1;} DisplayCommand;
extern ImageMeta D_80140BF0[];
extern char *D_803B7788[];
extern char D_803B9260[];
extern Color D_803B8328;
extern s32 D_803B7CD8;
extern DisplayCommand *D_803BAB50;
extern f32 D_803B8254[12],D_803BAC30[12],D_803B832C[3];
extern Blit *func_800B3704(char *,s16,s16,s16);
extern void func_80094EC8(Blit *);
/* N64 display-list operations: command fields, not CPU instruction shaping. */
#define DISPLAY_WORDS(p,a,b) do {DisplayCommand *command=(p)++; command->w0=(a);command->w1=(b);} while(0)
#define DISPLAY_TEXTURE(p,on) DISPLAY_WORDS(p,0xD7000000u|((on)<<1),0xFFFFFFFFu)
#define DISPLAY_VERTEX(p,v,n,first) DISPLAY_WORDS(p,0x01000000u|((n)<<12)|(((first)+(n))<<1),(u32)(v))
#define DISPLAY_TRIANGLES(p,a,b,c,d,e,f) DISPLAY_WORDS(p,0x06000000u|((a)*2<<16)|((b)*2<<8)|((c)*2),((d)*2<<16)|((e)*2<<8)|((f)*2))

void func_8038AEB0(void)
{
    /* Native performs this real initial aggregate copy; no later use is seen. */
    Color initial_color = D_803B8328;
    DisplayCommand *commands;
    s32 i,vertex,model;
    func_8038AE50();
    if ((D_8014978C >= 0 && D_8014978C < 6)
        || (D_8014978C >= 6 && D_8014978C < 14)
        || (D_8014978C >= 14 && D_8014978C < 18)
        || (D_8014978C >= 18 && D_8014978C < 19)) {
        D_803B8288 = func_800B3704(D_803B7788[D_8014978C],176,32,0);
        D_803B8288->priority = 0x7F00;
        func_80094EC8(D_803B8288);
        D_80140BF0[D_803B8288->resource].flags |= 1;
        if (func_8038ACB0(14) == 1) {
            D_803B828C = func_800B3704(D_803B9260,176,32,0);
            D_803B828C->priority = 0x7E00;
            func_80094EC8(D_803B828C);
        }
    }
    if (D_803BAC90 >= 0 && D_803BAC90 < 6) {
        if (D_803B7CE4[19].handle == -1) goto finish;
        memcpy(D_803BABE8[6],D_803BABE8[D_803BAC90],D_803B7CD8 * 64 + 64);
        memcpy(D_803BABC8[6],D_803BABC8[D_803BAC90],D_803B7CD8 * 40 + 56);
        commands = D_803BAB50;
        if ((D_8014A110 == 3 && D_801543D8[D_80154628].other[1] == 1)
            || (D_8014A110 != 3 && D_80146108[28])) {
            DISPLAY_TEXTURE(commands,0);
            DISPLAY_WORDS(commands,0xE7000000u,0);
            DISPLAY_WORDS(commands,0xE200001Cu,0xC8112230u);
            DISPLAY_WORDS(commands,0xFCFFFFFFu,0xFFFE7C38u);
            for (i = 0, vertex = 0; i < D_803B7CD8; i++, vertex += 4) {
                if (i + 1 == D_803B7CD8) {
                    DISPLAY_VERTEX(commands,(u8 *)D_803BABE8[6]+vertex*16,4,0);
                    DISPLAY_VERTEX(commands,D_803BABE8[6],4,4);
                } else {
                    DISPLAY_VERTEX(commands,(u8 *)D_803BABE8[6]+vertex*16,8,0);
                }
                DISPLAY_TRIANGLES(commands,1,5,0,4,0,5);
                DISPLAY_TRIANGLES(commands,0,4,3,7,3,4);
                DISPLAY_TRIANGLES(commands,3,7,2,6,2,7);
                DISPLAY_TRIANGLES(commands,2,6,1,5,1,6);
            }
            DISPLAY_TEXTURE(commands,1);
            DISPLAY_WORDS(commands,0xDF000000u,0);
        }
        func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803B8254,2.0f);
        func_800B5898(-0.785398185f,(f32 (*)[3])D_803B8254);
        D_803B7CE4[19].position[0] = D_803B832C[0];
        D_803B7CE4[19].position[1] = D_803B832C[1];
        D_803B7CE4[19].position[2] = D_803B832C[2];
    } else if ((D_803BAC90 >= 6 && D_803BAC90 < 14)
        || (D_803BAC90 >= 14 && D_803BAC90 < 18)) {
        if (D_803B8284 == -1) {
            model = string_copy_format(D_803B7738[D_803BAC90],0,D_80140BDC - 1,1);
            D_803B8284 = sign_extend_call(model,(u32)D_803BAC30,-1,0);
            func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803BAC30,2.0f);
            func_800B5898(-0.785398185f,(f32 (*)[3])D_803BAC30);
            D_803BAC30[9] = D_803B832C[0];
            D_803BAC30[10] = D_803B832C[1];
            D_803BAC30[11] = D_803B832C[2];
        }
    }
 finish:
    D_803BAC78 = D_803BAC90;
}

extern f32 D_803B8350,D_803B8338[3],D_803B8344[3];
/* Same-symbol clock contract already present in fixed-base matching sources. */
extern volatile f32 D_8002EB94;
extern void func_800FD754(f32 (*)[3],f32 (*)[3],f32,f32,f32);
extern void gfx_setup_fc(f32,f32 (*)[3]);

void func_8038B3EC(void)
{
    f32 inverse;
    D_803B8350 += D_8002EB94 * 6.28318548f / 10.0f;
    if (D_803B8350 > 6.28318548f) D_803B8350 -= 6.28318548f;
    if (D_803BAC90 != D_803BAC78) {
        D_803BACCC -= D_8002EB94 * 4.0f;
        if (D_803BACCC < 0.0f) {
            D_803BACCC = 0.0f;
            func_8038AE50();
            if (D_803B8284 != -1) {func_80090254((s16)D_803B8284);D_803B8284 = -1;}
            while (D_8002EB70) {}
            func_8038AEB0();
        }
    } else {
        D_803BACCC += D_8002EB94 * 4.0f;
        if (D_803BACCC > 1.0f) D_803BACCC = 1.0f;
    }
    D_803B7CE4[19].position[0] = D_803B8338[0];
    D_803B7CE4[19].position[1] = D_803B8338[1];
    D_803B7CE4[19].position[2] = D_803B8338[2];
    D_803B7CE4[19].position[0] += D_803B7CE4[D_803BAC78].offset_x;
    D_803B7CE4[19].position[1] += D_803B7CE4[D_803BAC78].offset_y;
    inverse = 1.0f - D_803BACCC;
    D_803B7CE4[19].position[0] *= inverse;
    D_803B7CE4[19].position[1] *= inverse;
    D_803B7CE4[19].position[2] *= inverse;
    D_803B7CE4[19].position[0] += D_803B8344[0] * D_803BACCC;
    D_803B7CE4[19].position[1] += D_803B8344[1] * D_803BACCC;
    D_803B7CE4[19].position[2] += D_803B8344[2] * D_803BACCC;
    D_803BAC30[9] = D_803B7CE4[19].position[0];
    D_803BAC30[10] = D_803B7CE4[19].position[1];
    D_803BAC30[11] = D_803B7CE4[19].position[2];
    func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803BAC30,
        D_803B7CE4[D_803BAC78].scale_from * inverse + D_803BACCC * D_803B7CE4[D_803BAC78].scale_to);
    func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803B8254,
        D_803B7CE4[D_803BAC78].scale_from * (1.0f - D_803BACCC) + D_803BACCC * D_803B7CE4[D_803BAC78].scale_to);
    if ((D_8014A110 == 3 && D_801543D8[D_80154628].other[1] == 1)
        || (D_8014A110 != 3 && D_8014A110 != 4 && D_8014A110 != 6 && D_8014A110 != 5 && D_80146108[28])) {
        func_800FD754((f32 (*)[3])D_803BAC30,(f32 (*)[3])D_803BAC30,-1.0f,1.0f,1.0f);
        func_800FD754((f32 (*)[3])D_803B8254,(f32 (*)[3])D_803B8254,-1.0f,1.0f,1.0f);
    }
    gfx_setup_fc(D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC) + D_803B8350 * D_803BACCC,(f32 (*)[3])D_803BAC30);
    func_800B5898(-0.52359879f * D_803BACCC + (1.0f - D_803BACCC) * -1.57079637f,(f32 (*)[3])D_803BAC30);
    gfx_setup_fc(D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC) + D_803B8350 * D_803BACCC,(f32 (*)[3])D_803B8254);
    func_800B5898(-0.52359879f * D_803BACCC + (1.0f - D_803BACCC) * -1.57079637f,(f32 (*)[3])D_803B8254);
}
