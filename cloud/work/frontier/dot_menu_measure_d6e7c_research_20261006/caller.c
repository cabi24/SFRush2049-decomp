/* Genuine D7634 caller from frozen credits_scroll_grp/gr3_e.c. */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((s8 *)(p)+(o)))
typedef struct { f32 unk0,unk4; } D_80156958_Entry;
typedef struct { u8 pad00[12];u8 unk0C,unk0D,unk0E; } D_80146108_Record;
extern s32 state_word_a;
extern s32 state_word_b;
extern s32 D_80156998[4];
extern D_80156958_Entry D_80156958[4];
extern s32 D_8015698C;
extern u8 D_80156994, D_80156CF0, D_80157244, D_8015F72D;
extern D_80146108_Record D_80146108;
extern s8 D_8011062C;
extern s32 D_80110638;
extern s32 D_8011063C;
extern s32 D_80110640;
extern s32 D_80110644;
extern s8 D_8011064C;
extern f32 D_801241D4;
extern f32 D_801241D8;
extern f32 D_801241DC;
extern f32 D_801241E0;
extern f32 D_801241E4;
extern s8 D_8014000D;
extern s32 D_80142728;
extern s32 D_801427A8;
extern s32 D_8015694C;
extern f32 D_8015695C;
extern s32 osRecvMesg(OSMesgQueue *,void *,s32),osJamMesg(OSMesgQueue *,void *,s32);
extern void *func_80091B00(void);
extern void drone_pathfind_main(void),speed_set(f32,f32,s32,s32),func_800D6160(s32);
extern void resource_type_select(s32),audio_distance_atten(s32),audio_doppler(s32);
extern void credits_screen(void),credits_scroll(void),func_800D6914(void);
extern void sync_entry_register(s32,s32),func_800D6E00(s32),func_800B4FB0(s32);
void func_800D7634(void) {
    s32 spF4;
    f32 spF0;
    f32 spEC;
    s32 spE8;
    s32 spE4;
    void *spD4;
    void *sp74;
    f32 *var_a1;
    s32 temp_t8;
    void *temp_v0;
    void *temp_v0_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    s8 *temp_v1_2;
    void *temp_v1;

    if (state_word_a & 0x7C03FFFE) {
        var_a1 = &D_8015695C;
        var_a0 = D_8015694C;
        spEC = D_80156958->unk0;
        spF0 = D_8015695C;
    } else {
        var_a1 = &D_80156958->unk0;
        temp_v1 = (f32 *) ((u8 *) &D_80156958->unk0 + (D_8015698C * 8));
        var_a0 = D_80156998[D_8015698C];
        spEC = M2C_FIELD(temp_v1, f32 *, 0);
        spF0 = M2C_FIELD(temp_v1, f32 *, 4);
    }
    if (D_8011062C == 0) {
        spF4 = var_a0;
        drone_pathfind_main();
    }
    if (var_a0 & 5) {
        resource_type_select(var_a0);
        D_8011063C = 2;
    } else if (var_a0 & 2) {
        if ((D_80110638 != 0) && (D_80110638 != 1) && (D_80110638 == 2)) {
            osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
            temp_v0 = func_80091B00();
            spD4 = temp_v0;
            M2C_FIELD(temp_v0, s8 *, 2) = 1;
            osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
            osJamMesg((OSMesgQueue *) &D_801427A8, (void *) spD4, 0);
            speed_set((f32) (s8) D_80146108.unk0C / 10.0f, D_801241D4, 1, 0);
            if (D_80110640 < 0xC) {
                sync_entry_register(D_80110640, 1);
            } else if (D_80110640 == 0xC) {
                if (state_word_a & 0x7C03FFFE) {
                    sync_entry_register(6, 1);
                } else {
                    func_800D6160(0);
                }
            }
        }
    } else if (var_a0 & 0xC00) {
        var_a2 = 1;
        if (var_a0 & 0x400) {
            var_a2 = -1;
        }
        spE8 = var_a2;
        audio_distance_atten(var_a0);
        var_v1 = D_80110638;
        do {
            temp_t8 = var_v1 + var_a2;
            D_80110638 = temp_t8;
            var_v1 = temp_t8;
            if (temp_t8 >= 4) {
                D_80110638 = 0;
                var_v1 = 0;
            } else if (var_v1 < 0) {
                D_80110638 = 3;
                var_v1 = 3;
            }
        } while (1 == 0);
        if ((var_v1 == 1) || (var_v1 == 2)) {
            speed_set((f32) (s8) D_80146108.unk0C / 10.0f, D_801241D8, 1, 0);
            if (D_80110640 < 0xC) {
                sync_entry_register(D_80110640, 1);
            } else if (D_80110640 == 0xC) {
                if (state_word_a & 0x7C03FFFE) {
                    sync_entry_register(6, 1);
                } else {
                    func_800D6160(0);
                }
            }
        }
    } else if (var_a0 & 0x3000) {
        var_a3 = 1;
        if (var_a0 & 0x1000) {
            var_a3 = -1;
        }
        spE4 = var_a3;
        audio_doppler(var_a0);
        switch (D_80110638) {                       /* irregular */
        case 0:
            D_80146108.pad00[0] = (s8) D_80146108.pad00[0] + var_a3;
            if ((s8) D_80146108.pad00[0] >= 0xB) {
                D_80146108.pad00[0] = 0;
            } else if ((s8) D_80146108.pad00[0] < 0) {
                D_80146108.pad00[0] = 0xA;
            }
            speed_set((f32) (s8) D_80146108.pad00[0] / 10.0f, D_801241DC, 0, 1);
            break;
        case 1:
            D_80146108.unk0C = (s8) D_80146108.unk0C + var_a3;
            if ((s8) D_80146108.unk0C >= 0xB) {
                D_80146108.unk0C = 0;
            } else if ((s8) D_80146108.unk0C < 0) {
                D_80146108.unk0C = 0xA;
            }
            speed_set((f32) (s8) D_80146108.unk0C / 10.0f, D_801241E0, 1, 0);
            break;
        case 2:
            spE4 = var_a3;
            osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
            temp_v0_2 = func_80091B00();
            sp74 = temp_v0_2;
            M2C_FIELD(temp_v0_2, s8 *, 2) = 1;
            spE4 = var_a3;
            osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
            osJamMesg((OSMesgQueue *) &D_801427A8, (void *) sp74, 0);
            var_a0_2 = D_80110640 + var_a3;
            D_80110640 = var_a0_2;
            if (var_a0_2 >= 0xE) {
                D_80110640 = 0;
                var_a0_2 = 0;
            } else if (var_a0_2 < 0) {
                var_a0_2 = 0xD;
                D_80110640 = 0xD;
            }
            D_80110644 = var_a0_2;
            D_80146108.unk0E = (u8) var_a0_2;
            speed_set((f32) (s8) D_80146108.unk0C / 10.0f, D_801241E4, 1, 0);
            if (D_80110640 < 0xC) {
                sync_entry_register(D_80110640, 1);
            } else if (D_80110640 == 0xC) {
                if (state_word_a & 0x7C03FFFE) {
                    sync_entry_register(6, 1);
                } else {
                    func_800D6160(0);
                }
            }
            break;
        case 3:
            M2C_FIELD(&(&D_8014000D)[0x6108], s8 *, 0xF) = (s8) (M2C_FIELD(&(&D_8014000D)[0x6108], s8 *, 0xF) == 0);
            func_800D6E00(M2C_FIELD(&(&D_8014000D)[0x6108], s8 *, 0xF));
            break;
        }
    }
    func_800D6914();
    if (state_word_a & 0x7C0000) {
        temp_v1_2 = (D_8015698C * 0x10) + &D_80156CF0;
        if ((*temp_v1_2 == 0) != D_8011064C) {
            D_8011064C = *temp_v1_2 == 0;
            credits_scroll();
        }
    }
    if ((s8) D_80157244 != 0) {
        credits_screen();
        if (state_word_a & 0x7C03FFFE) {
            state_word_b = 4;
            return;
        }
        func_800B4FB0(1);
        return;
    }
    if (D_8011063C != 0) {
        credits_screen();
        if (state_word_a & 0x7C03FFFE) {
            state_word_b = 0x10;
            return;
        }
        func_800B4FB0(1);
    }
}


