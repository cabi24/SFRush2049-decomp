/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * menu_save_options (historical label): save-slot table initialiser.
 *   - binds the four save slots (D_80146150[i] -> D_801391F8[i] -> D_801392F0[i] -> D_8013A028[i]),
 *     resets each through func_800CCCCC and names it with sprintf(D_801211B0, i + 1);
 *   - initialises two list heads with func_800A44E8;
 *   - allocates n = (D_80156994 ? 12 : 1) * 3 44-byte records plus an index of pointers to them.
 * Whole-program facts this depends on (it cannot match alone):
 *   - func_800CCCCC is internal: its handle arrives in s5 and it leaves s0/s1/s2/s4/s6 unsaved, so this
 *     function saves every callee-saved register (and f20/f22) and spills s0/s1 around the call;
 *   - func_800A44E8(list, a, b) is inlined twice, and it in turn inlines func_800A370C(list). That nesting is
 *     what gives the 160-byte frame (24 bytes per inlined call) and the retail store order b0, b1, w8, wC, w4.
 *     Both helpers are redefined here in that natural form; each still produces its own locked body.
 * Quirk: D_80117428 / D_80117424 must be unsigned char so the final `= 1` is not commoned with the s8 `1`
 * that the inlined func_800A44E8 arguments and D_8013FECA share in a1.
 * No arcade ancestor (N64 controller-pak / options code).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef struct SaveData {
    char pad0[13];
    char name[2051];
} SaveData; /* 0x810 */

typedef struct SaveSlot {
    char pad0[20];
    char name[24];
    SaveData **data;
} SaveSlot; /* 0x30 */

typedef struct ListHead {
    s8 unk0;
    s8 unk1;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} ListHead;

typedef struct Record {
    char pad[44];
} Record;

extern SaveSlot *D_80146150[4];
extern SaveSlot D_801391F8[4];
extern SaveData *D_801392F0[4];
extern SaveData D_8013A028[4];
extern char D_801211B0[];
extern ListHead D_8012E6D8;
extern ListHead D_80152020;
extern s8 D_80156994;
extern s8 D_8013FECA;
extern Record *D_8013FEC4;
extern Record **D_8012E6F8;
extern u8 D_80117428;
extern u8 D_80117424;

void func_800CCCCC(SaveSlot **h);
s32 sprintf(char *, const char *, ...);
void func_800A473C(void *, char *);
void *audio_dma_sync(s32 arg0, s32 arg1);
void audio_loop_control(u32 arg0, s32 arg1);
void *memset(void *, s32, u32);

void func_800A370C(ListHead *list) {
    list->unk8 = 0;
    list->unkC = 0;
    list->unk4 = 0;
}

void func_800A44E8(ListHead *list, s8 a, s8 b) {
    list->unk0 = a;
    list->unk1 = b;
    func_800A370C(list);
}

void menu_save_options(void) {
    s32 i;
    s32 n;

    for (i = 0; i < 4; i++) {
        D_80146150[i] = &D_801391F8[i];
        D_801391F8[i].data = &D_801392F0[i];
        D_801392F0[i] = &D_8013A028[i];
        func_800CCCCC(&D_80146150[i]);
        sprintf(D_80146150[i]->name, D_801211B0, i + 1);
        func_800A473C((*D_80146150[i]->data)->name, D_80146150[i]->name);
    }
    func_800A44E8(&D_8012E6D8, 1, 1);
    func_800A44E8(&D_80152020, 1, 0);
    if (D_80156994 == 0) {
        D_8013FECA = 1;
    } else {
        D_8013FECA = 3;
    }
    n = (D_80156994 ? 12 : 1) * 3;
    D_8013FEC4 = audio_dma_sync(0, n * sizeof(Record));
    audio_loop_control((u32) D_8013FEC4, 0);
    memset(D_8013FEC4, 0, n * sizeof(Record));
    D_8012E6F8 = audio_dma_sync(0, n * sizeof(Record *));
    audio_loop_control((u32) D_8012E6F8, 0);
    for (i = 0; i < n; i++) {
        D_8012E6F8[i] = &D_8013FEC4[i];
    }
    D_80117428 = 0;
    D_80117424 = 1;
}
