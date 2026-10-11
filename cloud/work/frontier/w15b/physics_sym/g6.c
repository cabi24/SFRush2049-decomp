/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w12h DRAFT (scout) of physics_sym + func_800B59F0 (internal). physics_sym: FAIL 208/313 words
 * (compiled 315); func_800B59F0: EQUAL. With 8 forced colours (tools/trace/force.sh, see
 * ../RESULTS.md) physics_sym is 4 rows off (only the update_viewport(0, 0) delay slot), so the
 * structure, frame (152) and homes are right and the residual is colouring + one as1 choice.
 * physics_sym: pause-menu input handler (called through a pointer table; no jal callers).
 * btn/rep/hold = pressed/repeat/held pad words (D_8015694C/D_80149784/D_80156944 when
 * D_801174B4 & 0x7C03FFFE, else per-controller D_80156998/D_80143A00/D_80156978[D_8015698C]).
 * Unused locals c, pad, e (and the declaration slots of p/v) size the frame (152): disclosed. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    f32 ang;             /* 0x08 */
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 pad3C[4];
} Button; /* 0x40 */

extern s32 D_8011AC94;
extern s32 D_8011AC98;
extern f32 D_8011AC9C;
extern Button D_8011A994[];
extern s8 D_8011AD40[];
extern s32 D_8011AD44;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_80152678;

f32 fabsf(f32);
#pragma intrinsic(fabsf)
void *memcpy(void *, const void *, u32);
s32 func_800B24EC();
void func_800B5898(f32 angle, f32 uv[][3]);
void func_800B5940(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void model_data_load();
void model_transform_setup();
void func_8008D870(s16 arg0, s32 arg1, s32 arg2);

extern s8 D_8011AD30;
extern s32 D_8011AD34;
extern s8 D_8011AD38;
extern s32 D_8011AD68;
void visual_objects_update(s32 arg0);
void sound_handles_clear(s32 arg0);
void sound_stop(s32 sound_id);
void ambient_sounds_clear(void);
void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);

void func_800B5688(void) {
    s32 *slot;

    if (D_8011AD30 != 0) {
        visual_objects_update(1);
        slot = (s32 *)D_8011A994;
        do {
            if (slot[1] != -1) {
                entity_spawn_callback((s16)slot[1], 0, 0);
                slot[1] = -1;
            }
            slot += 16;
        } while ((s32)slot != (s32)&D_8011AC94);
        D_8011AC94 = 0;
        sound_handles_clear(0);
        D_8011AD68 = 0;
        if (D_8011AD34 != 0) {
            sound_stop(D_8011AD34);
            D_8011AD34 = 0;
        }
        ambient_sounds_clear();
        D_8011AD38 = 0;
        D_8011AD30 = 0;
    }
}

s32 func_800B59E8(s32 i) {
    s32 v;
    v = D_8011AD40[i];
    return v;
}

void func_800B59F0(void) {
    f32 cur[4];
    f32 old[3];
    f32 d;
    f32 sel_y;
    s32 vis;
    s16 tex;
    s32 i;
    s32 name;
    f32 y;

    if (D_8011AC94 == 0)
        return;
    y = 100.0f;
    for (i = 0; i < 4; i++) {
        if (i == D_8011AD44) {
            sel_y = y;
            if (D_8011A994[i].ang < 3.1415927f)
                D_8011A994[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_8011A994[i].ang || D_8011AC98 == 1)
                D_8011A994[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_8011A994[i].ang)
                D_8011A994[i].ang -= 12.566371f * D_8002EB94;
            if (D_8011A994[i].ang < 0.0f || D_8011AC98 == 1)
                D_8011A994[i].ang = 0.0f;
        }
        memcpy(D_8011A994[i].mat, D_8011418C, 36);
        memcpy(old, D_8011A994[i].pos, 12);
        D_8011A994[i].pos[0] = -90.0f;
        D_8011A994[i].pos[1] = y;
        D_8011A994[i].pos[2] = 300.0f;
        if (D_8011AC98 == 0) {
            if (old[1] + 180.0f * D_8002EB94 < D_8011A994[i].pos[1])
                D_8011A994[i].pos[1] = old[1] + 180.0f * D_8002EB94;
            if (D_8011A994[i].pos[1] < old[1] - 180.0f * D_8002EB94)
                D_8011A994[i].pos[1] = old[1] - 180.0f * D_8002EB94;
        }
        D_8011A994[i + 4].ang = D_8011A994[i].ang;
        D_8011A994[i + 4].pos[0] = 150.0f;
        D_8011A994[i + 4].pos[1] = D_8011A994[i].pos[1];
        D_8011A994[i + 4].pos[2] = D_8011A994[i].pos[2];
        func_800B5898(D_8011A994[i].ang - 1.5707964f, D_8011A994[i].mat);
        memcpy(D_8011A994[i + 4].mat, D_8011A994[i].mat, 36);
        if (1.5707964f < D_8011A994[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(D_80140BDC - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(D_80140BDC - 1), 1);
        func_8008D870((s16)D_8011A994[i].handle, name, -1);
        func_8008D870((s16)D_8011A994[i + 4].handle, name, -1);
        if ((vis = D_8011AD40[i]) == 0) {
            model_data_load(D_8011A994[i].handle, 1, 15);
            model_data_load(D_8011A994[i + 4].handle, 1, 15);
        } else {
            model_transform_setup(D_8011A994[i].handle, 0, 15);
            if (i < 3)
                model_transform_setup(D_8011A994[i + 4].handle, 0, 15);
            else
                model_data_load(D_8011A994[i + 4].handle, 1, 15);
            y -= 40.0f;
        }
    }
    memcpy(old, D_8011A994[8].pos, 12);
    D_8011A994[8].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_8011A994[8].ang)
        D_8011A994[8].ang -= 6.2831855f;
    memcpy(D_8011A994[8].mat, D_8011418C, 36);
    D_8011A994[8].pos[0] = -160.0f;
    D_8011A994[8].pos[2] = 225.0f;
    D_8011A994[8].pos[1] = sel_y * 225.0f / 300.0f;
    if (D_8011AC98 == 0) {
        y = D_8011A994[8].pos[1];
        if (D_80152678 < fabsf(y - old[1]) * 60.0f / 10.0f) {
            D_80152678 = fabsf(y - old[1]) * 60.0f / 10.0f;
            D_80152678 = (D_80152678 < 540.0f * D_8011AC9C) ? D_80152678 : 540.0f * D_8011AC9C;
            D_80152678 = (180.0f * D_8011AC9C < D_80152678) ? D_80152678 : 180.0f * D_8011AC9C;
        }
        if (old[1] + D_80152678 * D_8002EB94 < y)
            D_8011A994[8].pos[1] = old[1] + D_80152678 * D_8002EB94;
        else if (y < old[1] - D_80152678 * D_8002EB94)
            D_8011A994[8].pos[1] = old[1] - D_80152678 * D_8002EB94;
        else
            D_80152678 = 180.0f * D_8011AC9C;
    }
    func_800B5940(D_8011A994[8].ang, D_8011A994[8].mat);
    func_800B5898(1.5707964f, D_8011A994[8].mat);
    func_8008B32C(D_8011A994[8].mat, D_8011A994[8].mat, 0.4f);
    D_8011AC98 = 0;
}


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
typedef struct { volatile s8 b0; u8 pad[15]; } Pad16;
extern Pad16 D_80156CF0[];

s32 viDeadlinePassed(void);
void update_viewport(s32 x, s32 y);
void particle_collision(void);
void resource_type_select(s32 a);
void audio_doppler(s32 a);
void audio_distance_atten(s32 a);
void sound_loop_set(void);
void func_800B5688(void);
void func_800B4FB0(s32 a);

void physics_sym(void) {
    Pad16 *p;
    s32 v;
    s32 c;
    s32 btn;
    s32 rep;
    s32 hold;
    s32 pad;
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
        audio_distance_atten(btn);
        do {
            D_8011AD44 += ds;
            if (D_8011AD44 > 3)
                D_8011AD44 = 0;
            else if (D_8011AD44 < 0)
                D_8011AD44 = 3;
        } while (!func_800B59E8(D_8011AD44));
    } else if (btn & 2) {
        if (D_8011AD44 == 3) {
            D_8011AD4C = 0;
            D_8011AD50 = 0;
            D_80146108[19] = D_8011AD50;
            D_80146108[20] = D_8011AD4C;
            D_8011AD48 = 0;
            D_80161398 = 0;
            update_viewport(D_8011AD50, D_8011AD4C);
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
    if (D_80157244) {
        func_800B5688();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 4;
        else
            func_800B4FB0(1);
    } else if (D_8011AD3C) {
        func_800B5688();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 16;
        else
            func_800B4FB0(1);
    }
}
