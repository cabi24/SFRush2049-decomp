/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w36f superseding revision (wave 36): func_803923B0 is now a claimed member, a direct typed port of MAT3
 * recordscr.c updateButtons + inlined updateViewButtons (see its header below).  The six old members are unchanged
 * and still MATCH; the only shared-declaration change is `extern volatile u8 D_80140BDC` (gNumModels; its other
 * user here is the context func_80393B3C, whose retail code also has the volatile lui/addiu/lbu 0(reg) form). */
/* w36i merge (wave 36): w36f revision as installed + w36c (func_803915E8, func_8039338C) + w36b (func_80393B3C,
 * func_80393B34); each lane block is taken verbatim, shared declarations agree (volatile D_80140BDC/D_8002EB94). */
/* w36b superseding revision: func_80393B3C (GC initRecordScreen@recordscr.c + inlined initButtons) is now a member, with its
 * deleted inlined-procedure stub func_80393B34; see the header at the function. Every earlier member is unchanged. */
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
 * w36c (wave 36): adds members/claims func_803915E8 = updateSelectButtons@recordscr.c (MAT3
 * GameSource recordscr.c, GC 0x80084f38) and func_8039338C = controlSelect@recordscr.c (GC
 * 0x80080874; replaces the earlier native-led context body), direct typed ports. The family is
 * MAT3 recordscr.c: 94058 updateRecordScreen (controlView inlined), 9338C controlSelect,
 * 93B3C initRecordScreen, 923B0 updateButtons (updateViewButtons inlined), 931C0 setRecordState.
 * D_8002EB94 gFrameSeconds and D_80140BDC gNumModels are volatile (MAT3 rush_protos.h).
 * SHAPING in func_803915E8: unusedA/unusedB are two unused s32 locals before prevPos (the same
 * two-slot residual as updateButtons@playerscr func_80394DD8): retail frame 128, prevPos at sp+100;
 * without them prevPos is at sp+108 and the frame is 120. 2 slots, 8 bytes. No padding arrays.
 * 255.f / 255.0f are deliberately different spellings (one shared 255 web is hoisted into $f30
 * and spills buttonY); the type-11 x is `buttonX + 0 * gButtonScale` (int zero, as 94DD8).
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

/* func_8039338C = controlSelect@recordscr.c (MAT3 GameSource recordscr.c, GC 0x80080874, size 0x788).
 * Direct typed port. Globals: D_803BA808 gSelectMenuSelect, D_803BA7E0 gSelectMenuValid[11], D_803BA80C
 * gSelectMenuClear, D_803BA800 gStatPlayer (tPlayerProfile **), D_803B65A4 gSelectPakIdx, D_8012E6D8 gPlayerFiles,
 * D_803BA84D gDeleteConfirm, D_803BA850 gYes, D_803BA854 gDeleteNotify, D_803BA840 gDeleteName[13], D_803BA7D4
 * buttonsJustInited, D_80146150 gJustPlayers[4], D_80149784 gActiveAutoEdges, D_80156944 gActiveLevels,
 * D_8015694C gActiveEdges. Calls: func_800B5F88 PlayLRSound, func_800B5F4C PlayUDSound, func_800B5FC4
 * PlayStartABSound, func_80393004 updateSelectInfo, func_800CD544 ClearPlayerRecords, func_800BE6A4 TB_Strcpy,
 * func_800CCEFC ReadPlayerData, func_800A3424 PS_LockFile, func_803931C0 setRecordState, func_800C9404
 * ClearTotalStats, func_800F769C AddPlayerStats, func_800C734C AddPlayerScores, func_800C7308 PurgePlayerData,
 * func_803932E4 uninitRecordScreen, func_800B5570 setGameState.
 * N64 differences: no MtapConnected test (pak index always wraps at 4), gText.single[177] (GC 187). */
void func_8039338C(void)
{
    s32 p, i;
    while (!D_803BA7E0[D_803BA808]) {
        D_803BA80C = 0;
        D_803BA808--;
        if (D_803BA808 < 0) D_803BA808 = 10;
        if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
            if (D_803BA800) {
                do { D_803BA800 = D_803BA800->payload->prev; }
                while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
            }
            if (!D_803BA800) D_803BA800 = D_8012E6D8.tail;
        } else D_803BA800 = 0;
    }
    if (D_803BA854 == 1) {
    } else if (D_803BA84D == 1) {
        if (D_80149784 & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA850 = !D_803BA850;
        }
    } else if (D_803BA808 == 1 && (D_80156944 & 0x3000)) {
        if (D_80149784 & 0x1000) {
            func_800B5F88(D_80149784);
            D_803B65A4--;
            if (D_803B65A4 < 0) D_803B65A4 = 3;
        } else if (D_80149784 & 0x2000) {
            func_800B5F88(D_80149784);
            D_803B65A4++;
            if (D_803B65A4 > 3) D_803B65A4 = 0;
        }
        func_80393004();
        D_803BA7D4 = 1;
    } else if (D_803BA808 >= 3 && D_803BA808 <= 11 && (D_80156944 & 0x3000)) {
        if (D_8015694C & 0x3000) {
            func_800B5F88(D_80149784);
            D_803BA80C = !D_803BA80C;
        }
    } else if (D_80149784 & 0x400) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 3 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->prev; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800) break;
            }
            D_803BA808--;
            if (D_803BA808 < 0) D_803BA808 = 10;
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->prev; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (!D_803BA800) {
                    D_803BA800 = D_8012E6D8.tail;
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4) D_803BA800 = D_803BA800->payload->prev;
                }
            } else D_803BA800 = 0;
        } while (!D_803BA7E0[D_803BA808]);
    } else if (D_80149784 & 0x800) {
        func_800B5F4C(D_80149784);
        D_803BA80C = 0;
        do {
            if (D_803BA808 == 10 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->next; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (D_803BA800) break;
            }
            D_803BA808++;
            if (D_803BA808 > 10) D_803BA808 = 0;
            if (D_803BA808 >= 3 && D_803BA808 < 11 && D_803BA7E0[D_803BA808] == 1) {
                if (D_803BA800) {
                    do { D_803BA800 = D_803BA800->payload->next; }
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4);
                }
                if (!D_803BA800) {
                    D_803BA800 = D_8012E6D8.head;
                    while (D_803BA800 && D_803BA800->payload->object->owner->slot != D_803B65A4) D_803BA800 = D_803BA800->payload->next;
                }
            } else D_803BA800 = 0;
        } while (!D_803BA7E0[D_803BA808]);
    }
    if (D_8015694C & 2) {
        if (D_803BA80C == 1) {
            func_800B5FC4(D_8015694C);
            if (D_803BA854 == 1) D_803BA854 = 0;
            else if (D_803BA84D == 1) {
                if (D_803BA850 == 1) {
                    if (D_803BA800) {
                        if (func_800CD544(D_803BA800)) D_803BA854 = 1;
                    } else {
                        for (p = 0; p < 4; p++) func_800CD544(&D_80146150[p]);
                        D_803BA854 = 1;
                    }
                }
                D_803BA84D = 0;
            } else {
                if (D_803BA800) func_800BE6A4(D_803BA840, D_803BA800->payload->text);
                else func_800BE6A4(D_803BA840, D_8017A4E4->label);
                D_803BA84D = 1;
                D_803BA850 = 0;
            }
        } else if (D_803BA808 == 0 || (D_803BA808 >= 3 && D_803BA808 < 11)) {
            func_800B5FC4(D_8015694C);
            if (D_803BA800) {
                if (func_800CCEFC(D_803BA800)) {
                    func_800A3424(D_803BA800->payload->object, 1);
                    func_803931C0(1);
                }
            } else {
                ListNode *node;
                func_800C9404();
                for (i = 0; i < 4; i++) {
                    func_800F769C(&D_80146150[i]);
                    func_800C734C(&D_80146150[i]);
                }
                for (node = D_8012E6D8.head; node; node = node->payload->next) {
                    if (node->payload->flags2C) {
                        func_800F769C(node); func_800C734C(node);
                    } else {
                        if (!func_800CCEFC(node)) break;
                        func_800F769C(node); func_800C734C(node);
                        func_800C7308(node);
                    }
                }
                if (!node) func_803931C0(1);
            }
        }
    } else if (D_8015694C & 5) {
        func_800B5FC4(D_8015694C);
        if (D_803BA854 == 1) D_803BA854 = 0;
        else if (D_803BA84D == 1) D_803BA84D = 0;
        else { func_803932E4(); func_800B5570(16); }
    }
}

/* Complete initializer; slot geometry follows the witnessed 64-byte records. */
typedef unsigned int u32;
typedef struct { u8 red, green, blue, alpha; } Color4;
extern Color4 D_803B65A8;
extern char *D_803B5EF0[];
extern f32 D_8011418C[3][3];
extern volatile u8 D_80140BDC;
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

/* func_80393B3C = initRecordScreen@recordscr.c (MAT3 reference/repos/mat3/decomp/src/GameSource/src/recordscr.c, GC 0x8008077C)
 * with initButtons@recordscr.c (GC 0x80084A5C) inlined; w36b direct typed port. The caller-less `jr ra; nop` at 0x80393B34
 * is the deleted procedure of the inlined initButtons: it is defined here as the non-static func_80393B34 (not on keep, so
 * uld/umerge inline it exactly as a static) so that its stub carries a symbol and cannot be counted as extra words of a
 * neighbouring member (w17j packaging rule). Names: gSelectMenuSelect D_803BA808, gStatPlayer D_803BA800,
 * setRecordState func_803931C0, createBG particle_lifetime_set, buttonInfo D_803B5F28 (23 x 64 B), buttonNames D_803B5EF0,
 * MBOX_NewObject sign_extend_call, MBOX_FindObject_WORK string_copy_format, ZOID_SetColor func_8008E06C, MBOX_HideObject
 * model_data_load, MBOX_ShowObject model_transform_setup, MBOX_FindTexture_WORK func_800B24EC, ZOID_SetTexture func_8008D870,
 * WPitchUV/WRollUV func_800B5898/func_800B5940, ScaleUV func_8008B32C, buttonsInited/JustInited D_803BA7D0/D_803BA7D4,
 * ZeroCameras particle_velocity_set, InitPane func_800B3B4C (viewPane D_803B6530), NewMultiBlit sound_control (blit
 * D_803B652C, mblit D_803B6504), gDeleteConfirm/gDeleteNotify/gYes/gSelectMenuClear D_803BA84D/854/850/80C, inited D_803B6528.
 * N64 differences from the GC bodies (from the retail words): no gSelectPakIdx store; the button y is 55.2f (GC 59.2f);
 * where the GC object has its "no-op copies" (records 17, 21, 22) the N64 multiplies all three position coordinates by a
 * constant 1.0: written `*= (1.0f / 1.0f)`, a constant expression that cfe folds to 1.0f but does not treat as the literal
 * 1.0f (a plain `*= 1.0f` is deleted; a propagated `f32 scale = 1.0f` keeps the mul but stores through absolute addresses,
 * retail stores through the s0 base). initRecordScreen is defined before initButtons, as in the MAT3 file: as1 breaks
 * scheduling ties by source line and the &buttonColor address (caller line) must win over the loop preheader.
 * Own data: .data white colour template {255,255,255,255} (retail 0x803B65A8 = FFFFFFFF) and 7 .rodata floats
 * (-1.5707964f 55.2f 75.1f pi 87.1f -75.1f -87.1f at 0x803B9454..0x803B946C), all checked against image A (tools/rocheck.py). */
void func_80393B34(void);

void func_80393B3C(void)
{
    D_803BA808 = 0;
    D_803BA800 = 0;
    func_803931C0(0);
    particle_lifetime_set();
    func_80393B34();
    D_803B6530 = func_800B3B4C(-20, 0, -10, 10, 192, 0, 0, 0);
    D_803B652C = sound_control(0, 0, D_803B6504, 1);
    D_803BA84D = 0;
    D_803BA854 = 0;
    D_803BA850 = 0;
    D_803BA80C = 0;
    D_803B6528 = 1;
}

void func_80393B34(void)
{
    Color4 buttonColor = {255, 255, 255, 255};
    void *tex;
    s16 texIdx;
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
        if (D_803B5F28[buttonIdx].fields[1] == -1) {
            memcpy(D_803B5F28[buttonIdx].matrix, D_8011418C, 36);
            D_803B5F28[buttonIdx].fields[1] = sign_extend_call(
                string_copy_format(D_803B5EF0[D_803B5F28[buttonIdx].fields[0]], 0, D_80140BDC - 1, 1),
                D_803B5F28[buttonIdx].matrix, -1,
                (D_803B5F28[buttonIdx].fields[0] == 0 || D_803B5F28[buttonIdx].fields[0] == 1 ||
                 D_803B5F28[buttonIdx].fields[0] == 2 || D_803B5F28[buttonIdx].fields[0] == 7 ||
                 D_803B5F28[buttonIdx].fields[0] == 8 || D_803B5F28[buttonIdx].fields[0] == 9 ||
                 D_803B5F28[buttonIdx].fields[0] == 10 || D_803B5F28[buttonIdx].fields[0] == 11) ? 0x42000 : 0);
            func_8008E06C(D_803B5F28[buttonIdx].fields[1], &buttonColor);
            model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        }
    }
    tex = func_800B24EC(D_803B8C10, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[17].fields[1], tex, -1);
    memcpy(D_803B5F28[17].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[17].matrix);
    D_803B5F28[17].position[0] = 0.0f;
    D_803B5F28[17].position[1] = 115.0f;
    D_803B5F28[17].position[2] = 200.0f;
    D_803B5F28[17].position[0] *= (1.0f / 1.0f);
    D_803B5F28[17].position[1] *= (1.0f / 1.0f);
    D_803B5F28[17].position[2] *= (1.0f / 1.0f);
    tex = func_800B24EC(D_803B8C20, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[18].fields[1], tex, -1);
    tex = func_800B24EC(D_803B8C28, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[19].fields[1], tex, -1);
    memcpy(D_803B5F28[19].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[19].matrix);
    D_803B5F28[19].position[0] = 75.1f;
    D_803B5F28[19].position[1] = 55.2f;
    D_803B5F28[19].position[2] = 100.0f;
    func_8008B32C(D_803B5F28[19].matrix, D_803B5F28[19].matrix, 0.5f);
    memcpy(D_803B5F28[20].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[20].matrix);
    func_800B5940(3.1415927f, D_803B5F28[20].matrix);
    D_803B5F28[20].position[0] = 87.1f;
    D_803B5F28[20].position[1] = 55.2f;
    D_803B5F28[20].position[2] = 100.0f;
    tex = func_800B24EC(D_803B8C2C, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[21].fields[1], tex, -1);
    memcpy(D_803B5F28[21].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[21].matrix);
    D_803B5F28[21].position[0] = -75.1f;
    D_803B5F28[21].position[1] = 55.2f;
    D_803B5F28[21].position[2] = 100.0f;
    D_803B5F28[21].position[0] *= (1.0f / 1.0f);
    D_803B5F28[21].position[1] *= (1.0f / 1.0f);
    D_803B5F28[21].position[2] *= (1.0f / 1.0f);
    func_8008B32C(D_803B5F28[21].matrix, D_803B5F28[21].matrix, 0.5f);
    memcpy(D_803B5F28[22].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[22].matrix);
    D_803B5F28[22].position[0] = -87.1f;
    D_803B5F28[22].position[1] = 55.2f;
    D_803B5F28[22].position[2] = 100.0f;
    D_803B5F28[22].position[0] *= (1.0f / 1.0f);
    D_803B5F28[22].position[1] *= (1.0f / 1.0f);
    D_803B5F28[22].position[2] *= (1.0f / 1.0f);
    D_803BA7D0 = 1;
    D_803BA7D4 = 1;
    particle_velocity_set();
}

/* func_803915E8 = updateSelectButtons@recordscr.c (MAT3 GC 0x80084f38, size 0xa60). Direct typed port.
 * Globals (N64 address = MAT3 name): D_803B5F28 buttonInfo[23] (64-byte tMenuButton: buttonType, objIdx, angle,
 * mat.uvs[3][3], mat.uvs[3] position, alpha), D_803B64E8 gButtonScale, D_803B64EC gButtonSpace, D_803B64F0 buttonX,
 * D_803B64F4 maxY, D_803B64F8 minY, D_803B64FC buttonZ, D_803B6500 cursorZ, D_803BA7D8 cursorSpeed,
 * D_803BA7DC scrollSpeed, D_803BA7D4 buttonsJustInited, D_803BA7F0 gSelectMenuVisible[11], D_803BA808
 * gSelectMenuSelect, D_803BA80C gSelectMenuClear, D_803BA84D gDeleteConfirm, D_803BA854 gDeleteNotify,
 * D_8002EB94 gFrameSeconds (volatile), D_80140BDC gNumModels (volatile), D_8011418C unitmatrix.
 * Calls: model_transform_setup = MBOX_ShowObject, model_data_load = MBOX_HideObject, func_800B24EC =
 * MBOX_FindTexture_WORK, func_8008D870 = ZOID_SetTexture, func_8008E06C = ZOID_SetColor, func_800B5940 = WRollUV,
 * func_800B5898 = WPitchUV, func_8008B32C = ScaleUV. */
extern f32 D_803B64E8, D_803B64EC, D_803B64F0, D_803B64F4, D_803B64F8, D_803B64FC, D_803B6500;
extern f32 D_803BA7D8, D_803BA7DC;
extern volatile f32 D_8002EB94;
extern f32 D_8011418C[3][3];
extern s32 D_803BA7D4, D_803BA808;
extern s8 D_803BA7F0[], D_803BA80C, D_803BA84D, D_803BA854;
extern void *memcpy(void *, const void *, u32);
extern void model_data_load(s32, s32, s32), model_transform_setup(s32, s32, s32);
extern void *func_800B24EC(char *, s16 *, s32, s8, s32);
extern void func_8008D870(s16, void *, s32);
extern void func_800B5898(f32, f32 (*)[3]), func_800B5940(f32, f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3], f32 (*)[3], f32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_803915E8(void)
{
    s32 buttonIdx;
    s32 i;
    s32 unusedA, unusedB;
    f32 prevPos[3];
    s32 gSelectMenuCount;
    s32 whichItem;
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed = 1;
    void *tex;
    s16 texIdx;
    Color4 buttonColor = {255, 255, 255, 255};

    if (D_803BA84D == 1 || D_803BA854 == 1) {
        for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
            if (buttonIdx == 17 || buttonIdx == 19 || buttonIdx == 21 || buttonIdx == 22)
                model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
            else model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        }
    } else {
        for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
            if ((buttonIdx > 11 && buttonIdx < 13) || buttonIdx == 15 || buttonIdx == 16)
                model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        }
        gSelectMenuCount = 0;
        whichItem = 0;
        for (i = 0; i < 11; i++) {
            if (D_803BA7F0[i]) {
                gSelectMenuCount++;
                if (i < D_803BA808) whichItem++;
            }
        }
        if (D_803B64E8 * ((gSelectMenuCount - 1) * D_803B64EC) > D_803B64F4 - D_803B64F8)
            selectedPos = D_803B64F4 - whichItem * (D_803B64F4 - D_803B64F8) / (gSelectMenuCount - 1);
        else selectedPos = D_803B64F4 - D_803B64E8 * (whichItem * D_803B64EC);
        memcpy(prevPos, D_803B5F28[18].position, 12);
        D_803B5F28[18].angle += 6.2831855f * D_8002EB94;
        if (D_803B5F28[18].angle > 6.2831855f) D_803B5F28[18].angle -= 6.2831855f;
        memcpy(D_803B5F28[18].matrix, D_8011418C, 36);
        D_803B5F28[18].position[0] = D_803B64F0 * D_803B6500 / D_803B64FC + -165.0f * D_803B64E8;
        D_803B5F28[18].position[1] = selectedPos * D_803B6500 / D_803B64FC;
        D_803B5F28[18].position[2] = D_803B6500;
        if (!D_803BA7D4) {
            if (fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60 * D_803B64E8 / 10 > D_803BA7D8) {
                D_803BA7D8 = fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60 * D_803B64E8 / 10;
                D_803BA7D8 = D_803BA7D8 < 540 * D_803B64E8 ? D_803BA7D8 : 540 * D_803B64E8;
                D_803BA7D8 = D_803BA7D8 > 180 * D_803B64E8 ? D_803BA7D8 : 180 * D_803B64E8;
            }
            if (D_803B5F28[18].position[1] > prevPos[1] + D_803BA7D8 * D_8002EB94)
                D_803B5F28[18].position[1] = prevPos[1] + D_803BA7D8 * D_8002EB94;
            else if (D_803B5F28[18].position[1] < prevPos[1] - D_803BA7D8 * D_8002EB94)
                D_803B5F28[18].position[1] = prevPos[1] - D_803BA7D8 * D_8002EB94;
            else D_803BA7D8 = 180 * D_803B64E8;
        }
        func_800B5940(D_803B5F28[18].angle, D_803B5F28[18].matrix);
        func_800B5898(1.5707964f, D_803B5F28[18].matrix);
        func_8008B32C(D_803B5F28[18].matrix, D_803B5F28[18].matrix, D_803B64E8 * 0.4f);
        buttonY = selectedPos + D_803B64E8 * (whichItem * D_803B64EC);
        for (buttonIdx = 0; buttonIdx < 11; buttonIdx++) {
            if (buttonIdx == D_803BA808 && !D_803BA80C) {
                if (D_803B5F28[buttonIdx].angle < 3.1415927f) D_803B5F28[buttonIdx].angle += 12.566371f * D_8002EB94;
                if (D_803B5F28[buttonIdx].angle > 3.1415927f) D_803B5F28[buttonIdx].angle = 3.1415927f;
                if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 3.1415927f;
            } else {
                if (D_803B5F28[buttonIdx].angle > 0) D_803B5F28[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B5F28[buttonIdx].angle < 0) D_803B5F28[buttonIdx].angle = 0;
                if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 0;
            }
            memcpy(prevPos, D_803B5F28[buttonIdx].position, 12);
            if (D_803B5F28[buttonIdx].fields[0] == 11) D_803B5F28[buttonIdx].position[0] = D_803B64F0 + 0 * D_803B64E8;
            else if (D_803B5F28[buttonIdx].fields[0] == 9) D_803B5F28[buttonIdx].position[0] = D_803B64F0 + -80 * D_803B64E8;
            else D_803B5F28[buttonIdx].position[0] = D_803B64F0 + -120 * D_803B64E8;
            D_803B5F28[buttonIdx].position[1] = buttonY;
            D_803B5F28[buttonIdx].position[2] = D_803B64FC;
            if (!D_803BA7D4) {
                if (fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60 / 10 > D_803BA7DC) {
                    D_803BA7DC = fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60 / 10;
                    D_803BA7DC = D_803BA7DC < 540 * D_803B64E8 ? D_803BA7DC : 540 * D_803B64E8;
                    D_803BA7DC = D_803BA7DC > 180 * D_803B64E8 ? D_803BA7DC : 180 * D_803B64E8;
                }
                if (D_803B5F28[buttonIdx].position[1] > prevPos[1] + D_803BA7DC * D_8002EB94) {
                    D_803B5F28[buttonIdx].position[1] = prevPos[1] + D_803BA7DC * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B5F28[buttonIdx].position[1] < prevPos[1] - D_803BA7DC * D_8002EB94) {
                    D_803B5F28[buttonIdx].position[1] = prevPos[1] - D_803BA7DC * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            func_8008B32C(D_8011418C, D_803B5F28[buttonIdx].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[buttonIdx].angle - 1.5707964f, D_803B5F28[buttonIdx].matrix);
            if (D_803B5F28[buttonIdx].angle > 1.5707964f)
                tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B5F28[buttonIdx].fields[1], tex, -1);
            if (D_803B5F28[buttonIdx].position[1] > D_803B64F4 + D_803B64EC * D_803B64E8 ||
                D_803B5F28[buttonIdx].position[1] < D_803B64F8 - D_803B64EC * D_803B64E8) D_803B5F28[buttonIdx].alpha = 0;
            else if (D_803B5F28[buttonIdx].position[1] > D_803B64F4)
                D_803B5F28[buttonIdx].alpha = 255.f - (D_803B5F28[buttonIdx].position[1] - D_803B64F4) * 255.0f / (D_803B64EC * D_803B64E8);
            else if (D_803B5F28[buttonIdx].position[1] < D_803B64F8)
                D_803B5F28[buttonIdx].alpha = 255.f - (D_803B64F8 - D_803B5F28[buttonIdx].position[1]) * 255.0f / (D_803B64EC * D_803B64E8);
            else D_803B5F28[buttonIdx].alpha = 255;
            if (!D_803BA7F0[buttonIdx]) D_803B5F28[buttonIdx].alpha = 0;
            buttonColor.alpha = D_803B5F28[buttonIdx].alpha;
            func_8008E06C(D_803B5F28[buttonIdx].fields[1], &buttonColor);
            if (!D_803BA7F0[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
            if (buttonIdx == 1) {
                D_803B5F28[14].position[0] = D_803B64F0 + 160 * D_803B64E8;
                D_803B5F28[14].position[1] = D_803B5F28[buttonIdx].position[1];
                D_803B5F28[14].position[2] = D_803B5F28[buttonIdx].position[2];
                D_803B5F28[14].angle = D_803B5F28[buttonIdx].angle;
                func_8008B32C(D_8011418C, D_803B5F28[14].matrix, D_803B64E8);
                func_800B5898(D_803B5F28[14].angle - 1.5707964f, D_803B5F28[14].matrix);
                func_8008D870(D_803B5F28[14].fields[1], tex, -1);
                D_803B5F28[14].alpha = D_803B5F28[buttonIdx].alpha;
                func_8008E06C(D_803B5F28[14].fields[1], &buttonColor);
                if (!D_803BA7F0[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[14].fields[1], 1, 15);
                else model_transform_setup(D_803B5F28[14].fields[1], 0, 15);
            } else if (D_803BA808 >= 3 && D_803BA808 < 11) {
                if (buttonIdx == D_803BA808) {
                    D_803B5F28[13].position[0] = D_803B64F0 + 120 * D_803B64E8;
                    D_803B5F28[13].position[1] = D_803B5F28[buttonIdx].position[1];
                    D_803B5F28[13].position[2] = D_803B5F28[buttonIdx].position[2];
                    if (D_803BA80C) {
                        if (D_803B5F28[13].angle < 3.1415927f) D_803B5F28[13].angle += 12.566371f * D_8002EB94;
                        if (D_803B5F28[13].angle > 3.1415927f) D_803B5F28[13].angle = 3.1415927f;
                        if (D_803BA7D4) D_803B5F28[13].angle = 3.1415927f;
                    } else {
                        if (D_803B5F28[13].angle > 0) D_803B5F28[13].angle -= 12.566371f * D_8002EB94;
                        if (D_803B5F28[13].angle < 0) D_803B5F28[13].angle = 0;
                        if (D_803BA7D4) D_803B5F28[13].angle = 0;
                    }
                    func_8008B32C(D_8011418C, D_803B5F28[13].matrix, D_803B64E8);
                    func_800B5898(D_803B5F28[13].angle - 1.5707964f, D_803B5F28[13].matrix);
                    if (D_803B5F28[13].angle > 1.5707964f)
                        tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
                    else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
                    func_8008D870(D_803B5F28[13].fields[1], tex, -1);
                    D_803B5F28[13].alpha = D_803B5F28[buttonIdx].alpha;
                    func_8008E06C(D_803B5F28[13].fields[1], &buttonColor);
                    if (!D_803BA7F0[buttonIdx] || !D_803B5F28[13].alpha) model_data_load(D_803B5F28[13].fields[1], 1, 15);
                    else model_transform_setup(D_803B5F28[13].fields[1], 0, 15);
                }
            } else {
                D_803B5F28[13].angle = 0;
                model_data_load(D_803B5F28[13].fields[1], 1, 15);
            }
            if (D_803BA7F0[buttonIdx] == 1) buttonY -= D_803B64EC * D_803B64E8;
        }
        for (; buttonIdx < 13; buttonIdx++) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        if (clearScrollSpeed == 1) D_803BA7DC = 0;
        D_803BA7D4 = 0;
    }
}

/* w36f (wave 36): func_803923B0 = MAT3 recordscr.c updateButtons (GC 0x80084EE8, 0x50 B) with updateViewButtons
 * (GC 0x80085998, 0x8B4 B) inlined by umerge; updateSelectButtons (GC 0x80084F38) stays the call func_803915E8.
 * Direct typed port of the MAT3 bodies; local list and ORDER from the GC DWARF (rush_d_dwarf.c: buttonIdx, i,
 * prevPos, gViewMenuCount, whichItem, buttonY, selectedPos, clearScrollSpeed, tex, texIdx, buttonColor; the MAT3 C
 * moved clearScrollSpeed up for CodeWarrior).  clearScrollSpeed keeps its initialiser, gViewMenuCount/whichItem are
 * assigned after the declarations (as in MAT3 updateSelectButtons): that statement order puts `li t7,1` in the
 * dispatch delay slot.  updateViewButtons (symbol recordscr_updateViewButtons, unique so it cannot clash with other
 * groups) is a NON-static function off `keep` (umerge inlines it; its deleted stub is retail's caller-less 0x803915E0).
 * N64 differences (from the retail displacements/constants): 23 buttons at D_803B5F28 (0x40 stride), statics
 * gButtonScale, gButtonSpace, buttonX, maxY, minY, buttonZ, cursorZ = D_803B64E8..D_803B6500, cursorSpeed/scrollSpeed
 * = D_803BA7D8/DC, buttonsInited/JustInited = D_803BA7D0/D4, gRecordState = D_803BA804, gViewMenuVisible =
 * D_803BA7FE, gViewMenuSelect = D_803BA810; the alpha fades linearly over one pitch
 * (255 - d * 255 / (space * scale)) instead of GC's flat 80; the two trailing MBOX_HideObject(buttonInfo[19/20])
 * calls are absent; associations follow the matched carscr updateButtons (`-140.0f * scale + buttonX`,
 * `scale * 0.4f`, fabsf on the right of the speed comparisons).  Callees: MBOX_HideObject = model_data_load,
 * MBOX_ShowObject = model_transform_setup, ZOID_SetTexture = func_8008D870, ZOID_SetColor = func_8008E06C,
 * MBOX_FindTexture_WORK = func_800B24EC, WPitchUV = func_800B5898, WRollUV = func_800B5940, ScaleUV = func_8008B32C.
 * SHAPING (disclosed frame residual): pad0 and pad1 are unused s32 locals of updateButtons.  Retail frame 144; the
 * same source without them 136 (every other offset already retail: prevPos 108, clearScrollSpeed 91, texIdx 82,
 * buttonColor 76, temp 52); 2 slots x 4 B = 8 B above the inlined callee's locals.  Two unused int parameters of
 * updateViewButtons give the identical object.  No pad arrays.
 */
typedef unsigned short u16;
extern void func_803915E8(void);
extern f32 D_803B64E8, D_803B64EC, D_803B64F0, D_803B64F4;
extern f32 D_803B64F8, D_803B64FC, D_803B6500;
extern f32 D_803BA7D8, D_803BA7DC;
extern volatile f32 D_8002EB94;
extern char D_803B8C60[], D_803B8C70[];
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void recordscr_updateViewButtons(void);

void func_803923B0(void)
{
    s32 pad0, pad1;
    if (D_803BA7D0) {
        switch (D_803BA804) {
        case 0: func_803915E8(); break;
        case 1: recordscr_updateViewButtons(); break;
        }
    }
}

void recordscr_updateViewButtons(void)
{
    s32 buttonIdx, i;
    f32 prevPos[3];
    s32 gViewMenuCount;
    s32 whichItem;
    f32 buttonY, selectedPos;
    s8 clearScrollSpeed = 1;
    void *tex;
    u16 texIdx;
    Color4 buttonColor = {255, 255, 255, 255};
    gViewMenuCount = 0;
    whichItem = 0;
    for (i = 0; i < 2; i++) {
        if (D_803BA7FE[i]) {
            gViewMenuCount++;
            if (i < D_803BA810) whichItem++;
        }
    }
    if ((gViewMenuCount - 1) * D_803B64EC * D_803B64E8 > D_803B64F4 - D_803B64F8)
        selectedPos = D_803B64F4 - whichItem * (D_803B64F4 - D_803B64F8) / (gViewMenuCount - 1);
    else selectedPos = D_803B64F4 - whichItem * D_803B64EC * D_803B64E8;
    memcpy(prevPos, D_803B5F28[18].position, 12);
    D_803B5F28[18].angle += 6.2831855f * D_8002EB94;
    if (D_803B5F28[18].angle > 6.2831855f) D_803B5F28[18].angle -= 6.2831855f;
    memcpy(D_803B5F28[18].matrix, D_8011418C, 36);
    D_803B5F28[18].position[0] = D_803B64F0 * D_803B6500 / D_803B64FC + -165.0f * D_803B64E8;
    D_803B5F28[18].position[1] = selectedPos * D_803B6500 / D_803B64FC;
    D_803B5F28[18].position[2] = D_803B6500;
    if (!D_803BA7D4) {
        if (D_803BA7D8 < fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60.0f * D_803B64E8 / 10.0f) {
            D_803BA7D8 = fabsf(D_803B5F28[18].position[1] - prevPos[1]) * 60.0f * D_803B64E8 / 10.0f;
            D_803BA7D8 = D_803BA7D8 < 540.0f * D_803B64E8 ? D_803BA7D8 : 540.0f * D_803B64E8;
            D_803BA7D8 = D_803BA7D8 > 180.0f * D_803B64E8 ? D_803BA7D8 : 180.0f * D_803B64E8;
        }
        if (D_803B5F28[18].position[1] > prevPos[1] + D_803BA7D8 * D_8002EB94)
            D_803B5F28[18].position[1] = prevPos[1] + D_803BA7D8 * D_8002EB94;
        else if (D_803B5F28[18].position[1] < prevPos[1] - D_803BA7D8 * D_8002EB94)
            D_803B5F28[18].position[1] = prevPos[1] - D_803BA7D8 * D_8002EB94;
        else D_803BA7D8 = 180.0f * D_803B64E8;
    }
    func_800B5940(D_803B5F28[18].angle, D_803B5F28[18].matrix);
    func_800B5898(1.5707964f, D_803B5F28[18].matrix);
    func_8008B32C(D_803B5F28[18].matrix, D_803B5F28[18].matrix, D_803B64E8 * 0.4f);
    buttonY = selectedPos + whichItem * D_803B64EC * D_803B64E8;
    for (buttonIdx = 0; buttonIdx < 11; buttonIdx++) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
    for (buttonIdx = 11; buttonIdx < 13; buttonIdx++) {
        if (buttonIdx - 11 == D_803BA810) {
            if (D_803B5F28[buttonIdx].angle < 3.1415927f) D_803B5F28[buttonIdx].angle += 12.566371f * D_8002EB94;
            if (D_803B5F28[buttonIdx].angle > 3.1415927f) D_803B5F28[buttonIdx].angle = 3.1415927f;
            if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 3.1415927f;
        } else {
            if (D_803B5F28[buttonIdx].angle > 0.0f) D_803B5F28[buttonIdx].angle -= 12.566371f * D_8002EB94;
            if (D_803B5F28[buttonIdx].angle < 0.0f) D_803B5F28[buttonIdx].angle = 0.0f;
            if (D_803BA7D4) D_803B5F28[buttonIdx].angle = 0.0f;
        }
        memcpy(prevPos, D_803B5F28[buttonIdx].position, 12);
        D_803B5F28[buttonIdx].position[0] = -140.0f * D_803B64E8 + D_803B64F0;
        D_803B5F28[buttonIdx].position[1] = buttonY;
        D_803B5F28[buttonIdx].position[2] = D_803B64FC;
        if (!D_803BA7D4) {
            if (D_803BA7DC < fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f) {
                D_803BA7DC = fabsf(D_803B5F28[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f;
                D_803BA7DC = D_803BA7DC < 540.0f * D_803B64E8 ? D_803BA7DC : 540.0f * D_803B64E8;
                D_803BA7DC = D_803BA7DC > 180.0f * D_803B64E8 ? D_803BA7DC : 180.0f * D_803B64E8;
            }
            if (D_803B5F28[buttonIdx].position[1] > prevPos[1] + D_803BA7DC * D_8002EB94) {
                D_803B5F28[buttonIdx].position[1] = prevPos[1] + D_803BA7DC * D_8002EB94;
                clearScrollSpeed = 0;
            } else if (D_803B5F28[buttonIdx].position[1] < prevPos[1] - D_803BA7DC * D_8002EB94) {
                D_803B5F28[buttonIdx].position[1] = prevPos[1] - D_803BA7DC * D_8002EB94;
                clearScrollSpeed = 0;
            }
        }
        func_8008B32C(D_8011418C, D_803B5F28[buttonIdx].matrix, D_803B64E8);
        func_800B5898(D_803B5F28[buttonIdx].angle - 1.5707964f, D_803B5F28[buttonIdx].matrix);
        if (D_803B5F28[buttonIdx].angle > 1.5707964f) tex = func_800B24EC(D_803B8C60, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        else tex = func_800B24EC(D_803B8C70, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        func_8008D870((s16)D_803B5F28[buttonIdx].fields[1], tex, -1);
        if (D_803B5F28[buttonIdx].position[1] > D_803B64F4 + D_803B64EC * D_803B64E8 || D_803B5F28[buttonIdx].position[1] < D_803B64F8 - D_803B64EC * D_803B64E8) D_803B5F28[buttonIdx].alpha = 0;
        else if (D_803B5F28[buttonIdx].position[1] > D_803B64F4) D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B5F28[buttonIdx].position[1] - D_803B64F4) * 255 / (D_803B64EC * D_803B64E8);
        else if (D_803B5F28[buttonIdx].position[1] < D_803B64F8) D_803B5F28[buttonIdx].alpha = 255.0f - (D_803B64F8 - D_803B5F28[buttonIdx].position[1]) * 255 / (D_803B64EC * D_803B64E8);
        else D_803B5F28[buttonIdx].alpha = 255;
        if (!D_803BA7FE[buttonIdx - 11]) D_803B5F28[buttonIdx].alpha = 0;
        buttonColor.alpha = D_803B5F28[buttonIdx].alpha;
        func_8008E06C((s16)D_803B5F28[buttonIdx].fields[1], &buttonColor);
        if (!D_803BA7FE[buttonIdx - 11] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        if (buttonIdx - 11 == 0) {
            D_803B5F28[15].position[0] = 100.0f * D_803B64E8 + D_803B64F0;
            D_803B5F28[15].position[1] = D_803B5F28[buttonIdx].position[1];
            D_803B5F28[15].position[2] = D_803B5F28[buttonIdx].position[2];
            D_803B5F28[15].angle = D_803B5F28[buttonIdx].angle;
            func_8008B32C(D_8011418C, D_803B5F28[15].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[15].angle - 1.5707964f, D_803B5F28[15].matrix);
            func_8008D870((s16)D_803B5F28[15].fields[1], tex, -1);
            D_803B5F28[15].alpha = D_803B5F28[buttonIdx].alpha;
            func_8008E06C((s16)D_803B5F28[15].fields[1], &buttonColor);
            if (!D_803BA7FE[buttonIdx] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[15].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[15].fields[1], 0, 15);
        } else if (buttonIdx - 11 == 1) {
            D_803B5F28[16].position[0] = 100.0f * D_803B64E8 + D_803B64F0;
            D_803B5F28[16].position[1] = D_803B5F28[buttonIdx].position[1];
            D_803B5F28[16].position[2] = D_803B5F28[buttonIdx].position[2];
            D_803B5F28[16].angle = D_803B5F28[buttonIdx].angle;
            func_8008B32C(D_8011418C, D_803B5F28[16].matrix, D_803B64E8);
            func_800B5898(D_803B5F28[16].angle - 1.5707964f, D_803B5F28[16].matrix);
            func_8008D870((s16)D_803B5F28[16].fields[1], tex, -1);
            D_803B5F28[16].alpha = D_803B5F28[buttonIdx].alpha;
            func_8008E06C((s16)D_803B5F28[16].fields[1], &buttonColor);
            if (!D_803BA7FE[buttonIdx - 11] || !D_803B5F28[buttonIdx].alpha) model_data_load(D_803B5F28[16].fields[1], 1, 15);
            else model_transform_setup(D_803B5F28[16].fields[1], 0, 15);
        }
        if (D_803BA7FE[buttonIdx - 11] == 1) buttonY -= D_803B64EC * D_803B64E8;
    }
    for (; buttonIdx < 23; buttonIdx++) {
        if (buttonIdx < 15) model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
        else model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
    }
    if (clearScrollSpeed == 1) D_803BA7DC = 0.0f;
    D_803BA7D4 = 0;
}
