/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A availability-screen input root, func_80394058 (1292 bytes).
 * Reconstructed from native targets; no proven arcade-source ancestor.
 * Complete genuine 923B0 renderer, 9338C input and 93B3C initializer are
 * nonmatching context. Their real IPA clobbers, particularly six FP save
 * pairs from the renderer, are required to reproduce the kept root's ABI.
 * The native mode read has its own s32 carrier. Two actual signed-byte
 * input steps retain separate homes; every root statement is inherited.
 * SHAPING: residual_slot0 and residual_slot1 are unused s32 declarations
 * before the byte locals. Each reserves four bytes in the declared-local
 * area. Native frame 128, frame without these slots 120; total residual
 * 2 * 4 = 8 bytes. Save area 24..111 and byte homes 118/119 stay exact.
 * This is an explicitly permitted frame reconstruction, not evidence of
 * original variable names, types or declarations. No padding arrays.
 * Earlier strict helpers are preservation members, with no new credit.
 * Context FP/data discrepancies remain unclaimed; root owns no data.
 */
/* Complete genuine A:931C0 -> {930B8,93004,92F28} call family. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s8 D_803BA860;
extern s8 D_803BA7FC[];
extern s8 D_803BA7FE[];

void func_80392F28(void)
{
    s32 i;
    s32 flag;
    s8 level = D_803BA858;

    if (level < 12) {
        D_803BA880 = 0;
        flag = 1;
    } else if (level < 20) {
        D_803BA880 = 1;
        flag = 1;
    } else if (level < 24) {
        D_803BA880 = 2;
        flag = 1;
    } else if (level < 25) {
        D_803BA880 = 3;
        flag = 1;
    } else if (level < 29) {
        D_803BA880 = 4;
        flag = 1;
        D_803BA860 = level - 25;
    } else {
        D_803BA880 = 5;
        flag = 1;
    }
    for (i = 0; i < 2; i++) {
        D_803BA7FC[i] = flag;
        D_803BA7FE[i] = flag;
    }
}

extern s8 D_803BA7E0[];
extern s8 D_803BA7F0[];
extern s32 D_803BA830[];
extern s8 D_803B65A4;

void func_80393004(void)
{
    unsigned int i;
    s32 count;

    D_803BA7E0[0] = 1;
    D_803BA7F0[0] = 1;
    D_803BA7E0[1] = 1;
    D_803BA7F0[1] = 1;
    D_803BA7E0[2] = 0;
    count = D_803BA830[D_803B65A4];
    D_803BA7F0[2] = count < 1;
    for (i = 0; i < 8; i++) {
        D_803BA7E0[i + 3] = (s32)i < count;
        D_803BA7F0[i + 3] = (s32)i < count;
    }
}


/* Bounded storage views follow only observed offsets; names are descriptive. */
typedef struct { s8 enabled; unsigned char remaining[15]; } PlayerSlot;
typedef struct { unsigned char prefix[16]; unsigned char slot; } ListOwner;
typedef struct { ListOwner *owner; } ListObject;
typedef struct ListNode ListNode;
typedef struct { ListNode *next, *prev; ListObject *object; unsigned char unknown0C[8]; char text[24]; s32 flags2C; } ListPayload;
struct ListNode { ListPayload *payload; };
typedef struct { s32 prefix[2]; ListNode *head, *tail; } ListRoot;
extern PlayerSlot D_80156CF0[];
extern ListRoot D_8012E6D8;
extern s32 func_800A1A3C(s32);
extern s32 func_800A35F8(s32);
extern s32 func_80094FC4(s32);
void func_803930B8(void) {
    s32 slot;
    ListNode *node;
    for (slot = 0; slot < 4; slot++) {
        if (!D_80156CF0[slot].enabled) { D_803BA830[slot] = -4; }
        else if (!func_800A1A3C(slot)) { D_803BA830[slot] = -1; }
        else if (func_800A35F8(slot)) { D_803BA830[slot] = -2; }
        else if (func_80094FC4(slot)) { D_803BA830[slot] = -3; }
        else {
            node = D_8012E6D8.head;
            D_803BA830[slot] = 0;
            while (node != 0) {
                if (node->payload->object->owner->slot == slot) {
                    D_803BA830[slot]++;
                }
                node = node->payload->next;
            }
            if (D_803BA830[slot] == 0) { D_803BA830[slot] = 0; }
        }
    }
}

void func_803930B8(void);
void func_80393004(void);
void func_80392F28(void);
extern s32 D_803BA804;
extern ListNode *D_803BA800;
extern s8 D_803BA84D;
extern s8 D_803BA854;
extern s8 D_803BA850;
extern s8 D_803BA80C;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s32 D_803BA818[];
extern s8 D_803B6554[][13];
extern s32 D_803BA7D4;

void func_803931C0(s32 mode)
{
    D_803BA804 = mode;
    switch (D_803BA804) {
    case 0:
        func_803930B8();
        func_80393004();
        D_803BA84D = 0;
        D_803BA854 = 0;
        D_803BA850 = 0;
        D_803BA80C = 0;
        break;
    case 1:
        if (D_803BA800 == 0 && D_803BA858 >= 29) {
            D_803BA858 = 0;
            while (D_803B6554[D_803BA880][D_803BA818[D_803BA880]] == 0) {
                if (++D_803BA818[D_803BA880] >= 13) {
                    D_803BA818[D_803BA880] = 0;
                }
            }
        }
        func_80392F28();
        break;
    }
    D_803BA7D4 = 1;
}

/* Image A: complete 168-byte cleanup. Native clobbers s0-s2; standalone is research. */
typedef struct { s32 fields[2]; f32 angle; f32 matrix[3][3]; f32 position[3]; u8 alpha; u8 remaining[3]; } MenuRecord;
extern MenuRecord D_803B5F28[23];
extern s32 D_803BA7D0;
extern void *D_803B6530, *D_803B652C;
extern s8 D_803B6528;
extern void sound_call_minimal(s16);
extern void sound_handles_array_clear(void *);
extern void sound_stop(void *);
extern void ambient_sounds_clear(void);
void func_803932E4(void) {
    s32 i;
    for (i = 0; i < 23; i++) {
        if (D_803B5F28[i].fields[1] != -1) {
            sound_call_minimal((s16)D_803B5F28[i].fields[1]);
            D_803B5F28[i].fields[1] = -1;
        }
    }
    D_803BA7D0 = 0;
    if (D_803B6530 != 0) {
        sound_handles_array_clear(D_803B6530);
        D_803B6530 = 0;
    }
    if (D_803B652C != 0) {
        sound_stop(D_803B652C);
        D_803B652C = 0;
    }
    ambient_sounds_clear();
    D_803B6528 = 0;
}

/* Full image-A entry root. Only witnessed fields and control flow are named. */
extern s8 D_80157244;
extern s32 D_80156944, D_8015694C, D_80149784;
extern s32 D_803BA810;
extern s8 D_803B6534[];
extern void func_80393B3C(void);
extern void func_8039338C(void);
extern void func_803923B0(void);
extern void func_800B5570(s32);
extern void func_800B5F88(s32);
extern void func_800B5F4C(s32);
extern void func_800B5FC4(s32);
extern void func_800A3424(ListObject *, s32);
extern void func_800C7308(ListNode *);

void func_80394058(void)
{
    s32 residual_slot0; s32 residual_slot1;
    s8 level_step;
    s8 item_step;
    s32 mode;
    if (D_803B6528 == 0) {
        func_80393B3C();
    }
    func_803930B8();
    func_80393004();
    if (D_803BA830[D_803B65A4] <= 0) {
        if (D_803BA800 != 0) {
            func_803931C0(0);
        }
        D_803BA800 = 0;
    }
    if (D_80157244 != 0) {
        func_803932E4();
        func_800B5570(16);
    } else {
        mode = D_803BA804;
        switch (mode) {
        case 0:
            func_8039338C();
            break;
        case 1:
            if (D_80156944 & 0x3000) {
                if (D_803BA810 == 0) {
                    level_step = D_80149784 & 0x1000 ? -1 : (D_80149784 & 0x2000 ? 1 : 0);
                    if (level_step != 0) {
                        func_800B5F88(D_80149784);
                        do {
                            D_803BA858 += level_step;
                            if (D_803BA858 < 0) {
                                D_803BA858 = D_803BA800 != 0 ? 29 : 28;
                            }
                            if (D_803BA858 > (D_803BA800 != 0 ? 29 : 28)) {
                                D_803BA858 = 0;
                            }
                        } while (D_803B6534[D_803BA858] == 0);
                        func_80392F28();
                    }
                } else {
                    item_step = D_80149784 & 0x1000 ? -1 : (D_80149784 & 0x2000 ? 1 : 0);
                    if (item_step != 0) {
                        func_800B5F88(D_80149784);
                        do {
                            D_803BA818[D_803BA880] += item_step;
                            if (D_803BA818[D_803BA880] < 0) {
                                D_803BA818[D_803BA880] = 12;
                            }
                            if (D_803BA818[D_803BA880] >= 13) {
                                D_803BA818[D_803BA880] = 0;
                            }
                        } while (D_803B6554[D_803BA880][D_803BA818[D_803BA880]] == 0);
                        func_80392F28();
                    }
                }
            } else if (D_80149784 & 0x400) {
                func_800B5F4C(D_80149784);
                do {
                    if (--D_803BA810 < 0) {
                        D_803BA810 = 1;
                    }
                } while (D_803BA7FC[D_803BA810] == 0);
            } else if (D_80149784 & 0x800) {
                func_800B5F4C(D_80149784);
                do {
                    if (++D_803BA810 >= 2) {
                        D_803BA810 = 0;
                    }
                } while (D_803BA7FC[D_803BA810] == 0);
            }
            while (D_803B6554[D_803BA880][D_803BA818[D_803BA880]] == 0) {
                if (++D_803BA818[D_803BA880] >= 13) {
                    D_803BA818[D_803BA880] = 0;
                }
            }
            while (D_803BA7FC[D_803BA810] == 0) {
                if (--D_803BA810 < 0) {
                    D_803BA810 = 1;
                }
            }
            if (D_8015694C & 7) {
                func_800B5FC4(D_8015694C);
                func_803931C0(0);
                if (D_803BA800 != 0) {
                    func_800A3424(D_803BA800->payload->object, 0);
                    func_800C7308(D_803BA800);
                }
            }
            break;
        }
    }
    func_803923B0();
}

extern s32 D_803BA808;
extern char D_803BA840[];
extern ListNode D_80146150[4];
typedef struct { unsigned char prefix[0x2C4]; const char *label; } ResourceNames;
extern ResourceNames *D_8017A4E4;
extern s32 func_800CD544(ListNode *);
extern s32 func_800CCEFC(ListNode *);
extern void func_800BE6A4(char *, const char *);
extern void func_800C9404(void);
extern void func_800F769C(ListNode *);
extern void func_800C734C(ListNode *);

void func_8039338C(void)
{
    ListNode *node;
    if (D_803BA7E0[D_803BA808] == 0) {
        D_803BA80C = 0;
        do {
            if (--D_803BA808 < 0) {
                D_803BA808 = 10;
            }
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800 != 0) {
                    do {
                        D_803BA800 = D_803BA800->payload->prev;
                    } while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800 == 0) {
                    D_803BA800 = D_8012E6D8.tail;
                }
            } else {
                D_803BA800 = 0;
            }
        } while (D_803BA7E0[D_803BA808] == 0);
    }
    if (D_803BA854 == 1) {
        /* This state suppresses navigation but still accepts the action below. */
    } else if (D_803BA84D == 1) {
        if (D_80149784 & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA850 = !D_803BA850;
        }
    } else if (D_803BA808 == 1 && (D_80156944 & 0x3000)) {
        if (D_80149784 & 0x1000) {
            func_800B5F88(D_80149784);
            if (--D_803B65A4 < 0) {
                D_803B65A4 = 3;
            }
        } else if (D_80149784 & 0x2000) {
            func_800B5F88(D_80149784);
            if (++D_803B65A4 >= 4) {
                D_803B65A4 = 0;
            }
        }
        func_80393004();
        D_803BA7D4 = 1;
    } else if (D_803BA808 >= 3 && D_803BA808 < 12 && (D_80156944 & 0x3000)) {
        if (D_8015694C & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA80C = !D_803BA80C;
        }
    } else if (D_80149784 & 0x400) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 3 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800 != 0) {
                    do {
                        D_803BA800 = D_803BA800->payload->prev;
                    } while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800 != 0) {
                    break;
                }
            }
            if (--D_803BA808 < 0) {
                D_803BA808 = 10;
            }
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800 != 0) {
                    do {
                        D_803BA800 = D_803BA800->payload->prev;
                    } while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800 == 0) {
                    D_803BA800 = D_8012E6D8.tail;
                    while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4) {
                        D_803BA800 = D_803BA800->payload->prev;
                    }
                }
            } else {
                D_803BA800 = 0;
            }
        } while (D_803BA7E0[D_803BA808] == 0);
    } else if (D_80149784 & 0x800) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 10 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800 != 0) {
                    do {
                        D_803BA800 = D_803BA800->payload->next;
                    } while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800 != 0) {
                    break;
                }
            }
            if (++D_803BA808 >= 11) {
                D_803BA808 = 0;
            }
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800 != 0) {
                    do {
                        D_803BA800 = D_803BA800->payload->next;
                    } while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800 == 0) {
                    D_803BA800 = D_8012E6D8.head;
                    while (D_803BA800 != 0 && D_803BA800->payload->object->owner->slot != D_803B65A4) {
                        D_803BA800 = D_803BA800->payload->next;
                    }
                }
            } else {
                D_803BA800 = 0;
            }
        } while (D_803BA7E0[D_803BA808] == 0);
    }
    if (D_8015694C & 2) {
        if (D_803BA80C == 1) {
            func_800B5FC4(D_8015694C);
            if (D_803BA854 == 1) {
                D_803BA854 = 0;
            } else if (D_803BA84D == 1) {
                if (D_803BA850 == 1) {
                    if (D_803BA800 != 0) {
                        if (func_800CD544(D_803BA800)) {
                            D_803BA854 = 1;
                        }
                    } else {
                        for (node = D_80146150; node != D_80146150 + 4; node++) {
                            func_800CD544(node);
                        }
                        D_803BA854 = 1;
                    }
                }
                D_803BA84D = 0;
            } else {
                if (D_803BA800 != 0) {
                    func_800BE6A4(D_803BA840, D_803BA800->payload->text);
                } else {
                    func_800BE6A4(D_803BA840, D_8017A4E4->label);
                }
                D_803BA84D = 1;
                D_803BA850 = 0;
            }
        } else if (D_803BA808 == 0 || (D_803BA808 >= 3 && D_803BA808 < 11)) {
            func_800B5FC4(D_8015694C);
            if (D_803BA800 != 0) {
                if (func_800CCEFC(D_803BA800)) {
                    func_800A3424(D_803BA800->payload->object, 1);
                    func_803931C0(1);
                }
            } else {
                func_800C9404();
                for (node = D_80146150; node != D_80146150 + 4; node++) {
                    func_800F769C(node);
                    func_800C734C(node);
                }
                for (node = D_8012E6D8.head; node != 0; node = node->payload->next) {
                    if (node->payload->flags2C != 0) {
                        func_800F769C(node);
                        func_800C734C(node);
                    } else {
                        if (!func_800CCEFC(node)) {
                            break;
                        }
                        func_800F769C(node);
                        func_800C734C(node);
                        func_800C7308(node);
                    }
                }
                if (node == 0) {
                    func_803931C0(1);
                }
            }
        }
    } else if (D_8015694C & 5) {
        func_800B5FC4(D_8015694C);
        if (D_803BA854 == 1) {
            D_803BA854 = 0;
        } else if (D_803BA84D == 1) {
            D_803BA84D = 0;
        } else {
            func_803932E4();
            func_800B5570(16);
        }
    }
}

/* Complete initializer; slot geometry follows the witnessed 64-byte records. */
typedef unsigned int u32;
typedef struct { u8 red, green, blue, alpha; } Color4;
extern Color4 D_803B65A8;
extern char *D_803B5EF0[];
extern f32 D_8011418C[3][3];
extern u8 D_80140BDC;
extern char D_803B8C10[], D_803B8C20[], D_803B8C28[], D_803B8C2C[];
extern unsigned char D_803B6504[];
extern void particle_lifetime_set(void), particle_velocity_set(void);
extern void *memcpy(void *, const void *, u32);
extern s32 string_copy_format(char *, s8, s8, s8);
extern s32 sign_extend_call(s32, f32 (*)[3], s32, u32);
extern void func_8008E06C(s16, Color4 *);
extern void model_data_load(s32, s32, s32), model_transform_setup(s32, s32, s32);
extern void *func_800B24EC(char *, s16 *, s32, s8, s32);
extern void func_8008D870(s16, void *, s32);
extern void func_800B5898(f32, f32 (*)[3]), func_800B5940(f32, f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3], f32 (*)[3], f32);
extern void *func_800B3B4C(s32, s32, s32, s32, s32, s32, s32, s32);
extern void *sound_control(s16, s16, void *, s16);

void func_80393B3C(void)
{
    Color4 color;
    MenuRecord *record;
    s16 texture;
    s32 object;
    s32 i;
    f32 scale = 1.0f;
    u32 flags;
    f32 (*matrix)[3];
    D_803BA808 = 0;
    D_803BA800 = 0;
    func_803931C0(0);
    particle_lifetime_set();
    color = D_803B65A8;
    for (i = 0; i < 23; i++) {
        record = &D_803B5F28[i];
        if (record->fields[1] == -1) {
            matrix = record->matrix;
            memcpy(matrix, D_8011418C, 36);
            object = string_copy_format(D_803B5EF0[record->fields[0]], 0, D_80140BDC - 1, 1);
            if (record->fields[0] == 0 || record->fields[0] == 1 || record->fields[0] == 2 ||
                record->fields[0] == 7 || record->fields[0] == 8 || record->fields[0] == 9 ||
                record->fields[0] == 10 || record->fields[0] == 11) {
                flags = 0x42000;
            } else {
                flags = 0;
            }
            record->fields[1] = sign_extend_call(object, matrix, -1, flags);
            func_8008E06C((s16)record->fields[1], &color);
            model_data_load(record->fields[1], 1, 15);
            model_transform_setup(record->fields[1], 0, 15);
        }
    }
    func_8008D870((s16)D_803B5F28[17].fields[1], func_800B24EC(D_803B8C10, &texture, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B5F28[17].matrix, D_8011418C, 36);
    func_800B5898(-1.5707963705062866f, D_803B5F28[17].matrix);
    D_803B5F28[17].position[0] = 0.0f;
    D_803B5F28[17].position[1] = 115.0f;
    D_803B5F28[17].position[2] = 200.0f;
    D_803B5F28[17].position[0] *= scale;
    D_803B5F28[17].position[1] *= scale;
    D_803B5F28[17].position[2] *= scale;
    func_8008D870((s16)D_803B5F28[18].fields[1], func_800B24EC(D_803B8C20, &texture, 0, D_80140BDC - 1, 1), -1);
    func_8008D870((s16)D_803B5F28[19].fields[1], func_800B24EC(D_803B8C28, &texture, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B5F28[19].matrix, D_8011418C, 36);
    func_800B5898(-1.5707963705062866f, D_803B5F28[19].matrix);
    D_803B5F28[19].position[0] = 75.0999984741211f;
    D_803B5F28[19].position[1] = 55.20000076293945f;
    D_803B5F28[19].position[2] = 100.0f;
    func_8008B32C(D_803B5F28[19].matrix, D_803B5F28[19].matrix, 0.5f);
    memcpy(D_803B5F28[20].matrix, D_8011418C, 36);
    func_800B5898(-1.5707963705062866f, D_803B5F28[20].matrix);
    func_800B5940(3.1415927410125732f, D_803B5F28[20].matrix);
    D_803B5F28[20].position[0] = 87.0999984741211f;
    D_803B5F28[20].position[1] = 55.20000076293945f;
    D_803B5F28[20].position[2] = 100.0f;
    func_8008D870((s16)D_803B5F28[21].fields[1], func_800B24EC(D_803B8C2C, &texture, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B5F28[21].matrix, D_8011418C, 36);
    func_800B5898(-1.5707963705062866f, D_803B5F28[21].matrix);
    D_803B5F28[21].position[0] = -75.0999984741211f;
    D_803B5F28[21].position[1] = 55.20000076293945f;
    D_803B5F28[21].position[2] = 100.0f;
    D_803B5F28[21].position[0] *= scale;
    D_803B5F28[21].position[1] *= scale;
    D_803B5F28[21].position[2] *= scale;
    func_8008B32C(D_803B5F28[21].matrix, D_803B5F28[21].matrix, 0.5f);
    memcpy(D_803B5F28[22].matrix, D_8011418C, 36);
    func_800B5898(-1.5707963705062866f, D_803B5F28[22].matrix);
    D_803B5F28[22].position[0] = -87.0999984741211f;
    D_803B5F28[22].position[1] = 55.20000076293945f;
    D_803B5F28[22].position[2] = 100.0f;
    D_803B5F28[22].position[0] *= scale;
    D_803B5F28[22].position[1] *= scale;
    D_803B5F28[22].position[2] *= scale;
    D_803BA7D0 = 1;
    D_803BA7D4 = 1;
    particle_velocity_set();
    D_803B6530 = func_800B3B4C(-20, 0, -10, 10, 192, 0, 0, 0);
    D_803B652C = sound_control(0, 0, D_803B6504, 1);
    D_803BA84D = 0;
    D_803BA854 = 0;
    D_803BA850 = 0;
    D_803BA80C = 0;
    D_803B6528 = 1;
}

extern void func_803915E8(void);
extern Color4 D_803B65B0;
extern f32 D_803B64E8, D_803B64EC, D_803B64F0, D_803B64F4;
extern f32 D_803B64F8, D_803B64FC, D_803B6500;
extern f32 D_803BA7D8, D_803BA7DC, D_8002EB94;
extern char D_803B8C60[], D_803B8C70[];
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_803923B0(void)
{
    f32 old_position[3];
    f32 selected_y;
    f32 y;
    s32 i, count, before;
    MenuRecord *record;
    Color4 color;
    s16 texture;
    void *object;
    s8 settled;
    if (D_803BA7D0) {
        switch (D_803BA804) {
        case 0:
            func_803915E8();
            break;
        case 1:
            settled = 1;
            color = D_803B65B0;
            count = 0;
            before = 0;
            for (i = 0; i < 2; i++) {
                if (D_803BA7FE[i]) {
                    count++;
                    if (i < D_803BA810) { before++; }
                }
            }
            if ((count - 1) * D_803B64EC * D_803B64E8 > D_803B64F4 - D_803B64F8) {
                selected_y = D_803B64F4 - before * (D_803B64F4 - D_803B64F8) / (count - 1);
            } else {
                selected_y = D_803B64F4 - before * D_803B64EC * D_803B64E8;
            }
            record = &D_803B5F28[18];
            memcpy(old_position, record->position, 12);
            record->angle += 6.2831854820251465f * D_8002EB94;
            if (record->angle > 6.2831854820251465f) {
                record->angle -= 6.2831854820251465f;
            }
            memcpy(record->matrix, D_8011418C, 36);
            record->position[0] = D_803B64F0 * D_803B6500 / D_803B64FC - 165.0f * D_803B64E8;
            record->position[1] = selected_y * D_803B6500 / D_803B64FC;
            record->position[2] = D_803B6500;
            if (D_803BA7D4 == 0) {
                if (D_803BA7D8 < fabsf(record->position[1] - old_position[1]) * 60.0f * D_803B64E8 / 10.0f) {
                    D_803BA7D8 = fabsf(record->position[1] - old_position[1]) * 60.0f * D_803B64E8 / 10.0f;
                    D_803BA7D8 = D_803BA7D8 < 540.0f * D_803B64E8 ? D_803BA7D8 : 540.0f * D_803B64E8;
                    D_803BA7D8 = D_803BA7D8 > 180.0f * D_803B64E8 ? D_803BA7D8 : 180.0f * D_803B64E8;
                }
                if (old_position[1] + D_803BA7D8 * D_8002EB94 < record->position[1]) {
                    record->position[1] = old_position[1] + D_803BA7D8 * D_8002EB94;
                } else if (old_position[1] - D_803BA7D8 * D_8002EB94 > record->position[1]) {
                    record->position[1] = old_position[1] - D_803BA7D8 * D_8002EB94;
                } else {
                    D_803BA7D8 = 180.0f * D_803B64E8;
                }
            }
            func_800B5940(record->angle, record->matrix);
            func_800B5898(1.5707963705062866f, record->matrix);
            func_8008B32C(record->matrix, record->matrix, D_803B64E8 * 0.4000000059604645f);
            y = selected_y + before * D_803B64EC * D_803B64E8;
            for (record = D_803B5F28; record < D_803B5F28 + 11; record++) {
                model_data_load(record->fields[1], 1, 15);
            }
            for (i = 11; i < 13; i++) {
                record = &D_803B5F28[i];
                if (i == D_803BA810 + 11) {
                    if (record->angle < 3.1415927410125732f) {
                        record->angle += 12.566370964050293f * D_8002EB94;
                    }
                    if (record->angle > 3.1415927410125732f) { record->angle = 3.1415927410125732f; }
                    if (D_803BA7D4) { record->angle = 3.1415927410125732f; }
                } else {
                    if (record->angle > 0.0f) {
                        record->angle -= 12.566370964050293f * D_8002EB94;
                    }
                    if (record->angle < 0.0f) { record->angle = 0.0f; }
                    if (D_803BA7D4) { record->angle = 0.0f; }
                }
                memcpy(old_position, record->position, 12);
                record->position[0] = D_803B64F0 - 140.0f * D_803B64E8;
                record->position[1] = y;
                record->position[2] = D_803B64FC;
                if (D_803BA7D4 == 0) {
                    if (D_803BA7DC < fabsf(record->position[1] - old_position[1]) * 60.0f / 10.0f) {
                        D_803BA7DC = fabsf(record->position[1] - old_position[1]) * 60.0f / 10.0f;
                        D_803BA7DC = D_803BA7DC < 540.0f * D_803B64E8 ? D_803BA7DC : 540.0f * D_803B64E8;
                        D_803BA7DC = D_803BA7DC > 180.0f * D_803B64E8 ? D_803BA7DC : 180.0f * D_803B64E8;
                    }
                    if (old_position[1] + D_803BA7DC * D_8002EB94 < record->position[1]) {
                        record->position[1] = old_position[1] + D_803BA7DC * D_8002EB94;
                        settled = 0;
                    } else if (old_position[1] - D_803BA7DC * D_8002EB94 > record->position[1]) {
                        record->position[1] = old_position[1] - D_803BA7DC * D_8002EB94;
                        settled = 0;
                    }
                }
                func_8008B32C(D_8011418C, record->matrix, D_803B64E8);
                func_800B5898(record->angle - 1.5707963705062866f, record->matrix);
                if (record->angle > 1.5707963705062866f) {
                    object = func_800B24EC(D_803B8C60, &texture, 0, D_80140BDC - 1, 1);
                } else {
                    object = func_800B24EC(D_803B8C70, &texture, 0, D_80140BDC - 1, 1);
                }
                func_8008D870((s16)record->fields[1], object, -1);
                if (record->position[1] > D_803B64F4 + D_803B64EC * D_803B64E8 ||
                    record->position[1] < D_803B64F8 - D_803B64EC * D_803B64E8) {
                    record->alpha = 0;
                } else if (record->position[1] > D_803B64F4) {
                    record->alpha = 255.0f - (record->position[1] - D_803B64F4) * 255.0f / (D_803B64EC * D_803B64E8);
                } else if (record->position[1] < D_803B64F8) {
                    record->alpha = 255.0f - (D_803B64F8 - record->position[1]) * 255.0f / (D_803B64EC * D_803B64E8);
                } else {
                    record->alpha = 255;
                }
                if (D_803BA7FE[i - 11] == 0) { record->alpha = 0; }
                color.alpha = record->alpha;
                func_8008E06C((s16)record->fields[1], &color);
                if (D_803BA7FE[i - 11] == 0 || record->alpha == 0) {
                    model_data_load(record->fields[1], 1, 15);
                } else {
                    model_transform_setup(record->fields[1], 0, 15);
                }
                if (i == 11) {
                    D_803B5F28[15].position[0] = D_803B64F0 + 100.0f * D_803B64E8;
                    D_803B5F28[15].position[1] = record->position[1];
                    D_803B5F28[15].position[2] = record->position[2];
                    D_803B5F28[15].angle = record->angle;
                    func_8008B32C(D_8011418C, D_803B5F28[15].matrix, D_803B64E8);
                    func_800B5898(D_803B5F28[15].angle - 1.5707963705062866f, D_803B5F28[15].matrix);
                    func_8008D870((s16)D_803B5F28[15].fields[1], object, -1);
                    D_803B5F28[15].alpha = record->alpha;
                    func_8008E06C((s16)D_803B5F28[15].fields[1], &color);
                    if (D_803BA7FE[i] == 0 || record->alpha == 0) {
                        model_data_load(D_803B5F28[15].fields[1], 1, 15);
                    } else {
                        model_transform_setup(D_803B5F28[15].fields[1], 0, 15);
                    }
                } else if (i == 12) {
                    D_803B5F28[16].position[0] = D_803B64F0 + 100.0f * D_803B64E8;
                    D_803B5F28[16].position[1] = record->position[1];
                    D_803B5F28[16].position[2] = record->position[2];
                    D_803B5F28[16].angle = record->angle;
                    func_8008B32C(D_8011418C, D_803B5F28[16].matrix, D_803B64E8);
                    func_800B5898(D_803B5F28[16].angle - 1.5707963705062866f, D_803B5F28[16].matrix);
                    func_8008D870((s16)D_803B5F28[16].fields[1], object, -1);
                    D_803B5F28[16].alpha = record->alpha;
                    func_8008E06C((s16)D_803B5F28[16].fields[1], &color);
                    if (D_803BA7FE[i - 11] == 0 || record->alpha == 0) {
                        model_data_load(D_803B5F28[16].fields[1], 1, 15);
                    } else {
                        model_transform_setup(D_803B5F28[16].fields[1], 0, 15);
                    }
                }
                if (D_803BA7FE[i - 11] == 1) { y -= D_803B64EC * D_803B64E8; }
            }
            for (; i < 23; i++) {
                if (i < 15) { model_data_load(D_803B5F28[i].fields[1], 1, 15); }
                else { model_transform_setup(D_803B5F28[i].fields[1], 0, 15); }
            }
            if (settled == 1) { D_803BA7DC = 0.0f; }
            D_803BA7D4 = 0;
            break;
        }
    }
}
