/* Actual complete caller, reconstructed from native func_800DB1E0. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern u32 D_801174B4, D_801174B8;
extern u32 D_8015694C, D_80149784, D_80156998[], D_80143A00[];
extern s32 D_8015698C;
extern s8 D_80116D94, D_80116DA8;
extern s16 D_80116D9C, D_80149B84, D_80149DA2, D_8013FEC8;
extern s32 D_80116D14[14], D_80116DA0;
extern s8 D_80146108[19];
extern s8 D_80156CF0[][16], D_80157244;
void replay_save_prompt(void);
void resource_type_select(u32);
void audio_distance_atten(void);
void audio_doppler(u32);
void func_800A510C(void);
void init_state_begin(void);
void time_result_display(void);
void func_800DA0BC(void);
void func_800B4FB0(s32);
void func_800DA2C0(void);

void func_800DB1E0(void) {
    u32 pressed;
    u32 repeated;
    s32 step;
    if (D_801174B4 & 0x7C03FFFE) {
        pressed = D_8015694C;
        repeated = D_80149784;
    } else {
        pressed = D_80156998[D_8015698C];
        repeated = D_80143A00[D_8015698C];
    }
    if (D_80116D94 == 0) {
        replay_save_prompt();
    }
    if (pressed & 5) {
        resource_type_select(pressed);
        D_80116DA0 = 2;
    } else if (repeated & 0x400) {
        audio_distance_atten();
        do {
            D_80116D9C--;
            if (D_80116D9C < 0) D_80116D9C = 13;
        } while (D_80116D14[D_80116D9C] == 0);
    } else if (repeated & 0x800) {
        audio_distance_atten();
        do {
            D_80116D9C++;
            if (D_80116D9C >= 14) D_80116D9C = 0;
        } while (D_80116D14[D_80116D9C] == 0);
    }
    if (D_80149DA2 < D_80116D9C - D_80149B84 / 2) D_80149DA2 = D_80116D9C - D_80149B84 / 2;
    if (D_80116D9C - D_80149B84 / 2 < D_80149DA2) D_80149DA2 = D_80116D9C - D_80149B84 / 2;
    if (14 - D_80149B84 < D_80149DA2) D_80149DA2 = 14 - D_80149B84;
    if (D_80149DA2 < 0) D_80149DA2 = 0;
    if (pressed & 0x3000) {
        step = pressed & 0x1000 ? -1 : 1;
        audio_doppler(pressed);
        switch ((u16)D_80116D9C) {
        case 0:
            D_8013FEC8 += step;
            if (D_8013FEC8 < 0) D_8013FEC8 = 5;
            else if (D_8013FEC8 >= 6) D_8013FEC8 = 0;
            D_80146108[16] = D_8013FEC8;
            D_80146108[17] = 1;
            func_800A510C();
            break;
        case 1: D_80146108[10] = !D_80146108[10]; break;
        case 2: D_80146108[0] ^= 1; break;
        case 3: D_80146108[1] ^= 1; break;
        case 4: D_80146108[2] ^= 1; break;
        case 5: D_80146108[3] ^= 1; break;
        case 6: D_80146108[4] ^= 1; break;
        case 7: D_80146108[5] ^= 1; break;
        case 8: D_80146108[6] ^= 1; break;
        case 9: D_80146108[7] ^= 1; break;
        case 10: D_80146108[8] ^= 1; break;
        case 11: D_80146108[9] ^= 1; break;
        case 12: D_80146108[18] ^= 1; break;
        case 13: D_80146108[11] ^= 1; break;
        }
        if (!(D_801174B4 & 0x7C03FFFE)) init_state_begin();
    }
    func_800DA2C0();
    if (D_801174B4 & 0x007C0000) {
        if (!D_80156CF0[D_8015698C][0] != D_80116DA8) {
            D_80116DA8 = !D_80156CF0[D_8015698C][0];
            time_result_display();
        }
    }
    if (D_80157244 != 0) {
        func_800DA0BC();
        if (D_801174B4 & 0x7C03FFFE) D_801174B8 = 4;
        else func_800B4FB0(1);
    } else if (D_80116DA0 != 0) {
        func_800DA0BC();
        if (D_801174B4 & 0x7C03FFFE) D_801174B8 = 16;
        else func_800B4FB0(1);
    }
}
