/* Existing complete w12h pause-menu caller, without its unused shaping locals. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
extern s8 D_8011AD40[];
extern s32 D_8011AD44;
void func_800B59F0(void);
extern s32 D_801174B4;
extern s32 D_801174B8;
extern s32 D_8015694C;
extern s32 D_80149784;
extern s32 D_80156944;
extern s32 D_8015698C;
extern s32 D_80156998[];
extern s32 D_80143A00[];
extern s32 D_80156978[];
extern s8 D_8011AD30;
extern s8 D_8011AD54;
extern s8 D_8011AD58;
extern s8 D_8011AD5C;
extern s32 D_8011AD3C;
extern s32 D_8011AD48;
extern s32 D_8011AD4C;
extern s32 D_8011AD50;
extern s8 D_8011AD6C;
extern s8 D_80157244;
extern s32 D_80161398;
extern u8 D_80146108[];
typedef struct { s8 b0; u8 pad[15]; } Pad16;
extern Pad16 D_80156CF0[];

s32 viDeadlinePassed(void);
void update_viewport(s32 x, s32 y);
void particle_collision(void);
void resource_type_select(s32 a);
void audio_doppler(s32 a);
void audio_distance_atten(void);
void sound_loop_set(void);
void func_800B5688(void);
void func_800B4FB0(s32 a);

static void menu_exit(s32 code) {
    func_800B5688();
    if (D_801174B4 & 0x7C03FFFE)
        D_801174B8 = code;
    else
        func_800B4FB0(1);
}

void physics_sym(void) {
    Pad16 *p;
    s32 v;
    s32 btn;
    s32 rep;
    s32 hold;
    s32 dz;
    s32 ds;

    if (D_801174B4 & 0x7C03FFFE) {
        btn = D_8015694C;
        rep = D_80149784;
        hold = D_80156944;
    } else {
        btn = D_80156998[D_8015698C];
        rep = D_80143A00[D_8015698C];
        hold = D_80156978[D_8015698C];
    }
    if (D_8011AD30 == 0)
        particle_collision();
    if (D_8011AD54 == 1) {
        if (viDeadlinePassed()) {
            D_8011AD54 = 0;
            D_8011AD58 = 1;
            D_8011AD5C = 0;
        }
    } else if (D_8011AD58 == 1) {
        if (btn & 7) {
            resource_type_select(btn);
            if (btn & 4)
                D_8011AD5C = 0;
            D_8011AD58 = 0;
        } else if (btn & 0x3000) {
            audio_doppler(btn);
            D_8011AD5C = !D_8011AD5C;
        }
    } else if (btn & 5) {
        resource_type_select(btn);
        D_8011AD3C = 2;
    } else if (hold & 0x3000) {
        if (rep & 0x3000) {
            dz = (rep & 0x1000) ? -1 : 1;
            audio_doppler(btn);
            switch (D_8011AD44) {
            case 0:
                D_8011AD4C += dz;
                if (D_8011AD4C < -32)
                    D_8011AD4C = -32;
                else if (D_8011AD4C > 32)
                    D_8011AD4C = 32;
                D_80146108[20] = D_8011AD4C;
                update_viewport(D_8011AD50, D_8011AD4C);
                break;
            case 1:
                D_8011AD50 += dz;
                if (D_8011AD50 < -32)
                    D_8011AD50 = -32;
                else if (D_8011AD50 > 32)
                    D_8011AD50 = 32;
                D_80146108[19] = D_8011AD50;
                update_viewport(D_8011AD50, D_8011AD4C);
                break;
            }
        }
    } else if (btn & 0xC00) {
        ds = (btn & 0x400) ? -1 : 1;
        audio_distance_atten();
        do {
            D_8011AD44 += ds;
            if (D_8011AD44 > 3)
                D_8011AD44 = 0;
            else if (D_8011AD44 < 0)
                D_8011AD44 = 3;
        } while ((v = D_8011AD40[D_8011AD44]) == 0);
    } else if (btn & 2) {
        if (D_8011AD44 == 3) {
            D_8011AD4C = 0;
            D_8011AD50 = 0;
            D_80146108[19] = 0;
            D_80146108[20] = 0;
            D_8011AD48 = 0;
            D_80161398 = 0;
            update_viewport(0, 0);
        }
    }
    func_800B59F0();
    if (D_801174B4 & 0x7C0000) {
        p = &D_80156CF0[D_8015698C];
        if (!p->b0 != D_8011AD6C) {
            D_8011AD6C = !p->b0;
            sound_loop_set();
        }
    }
    if (D_80157244)
        menu_exit(4);
    else if (D_8011AD3C)
        menu_exit(16);
}
