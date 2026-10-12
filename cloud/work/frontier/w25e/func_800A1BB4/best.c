/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800A1BB4 (w25e, NOT a match, provisional lane): controller-pak record `index` (D_80144030, 772-byte
 * Pak772 records, byte +11) stays flagged only while some handle in list D_80144D68[index] has an enabled object
 * (+72) whose file slot byte (+133 + 40*file) is set; otherwise the flag is cleared. N64-only, no arcade ancestor.
 * INTERNAL function: retail's four-wide t6-t9 temp ring appears only with `--internal func_800A1BB4` (keep mode
 * gives t1-t5); its real caller track_data_decompress (two jal sites, result ignored) is unmatched, so this is
 * scored with the stand-in caller zz_caller_a1bb4 (NOT part of a deliverable) and --block func_800A1BB4.
 * Levers: the walk written inline (no static helper: there is no caller-less stub next to this function);
 * `next = p = head` makes the web numbers p < next < data (retail v1 / a1 / a2). No shaping devices.
 * Left (9 rows): retail's separate early `jr ra; nop` and the found-path `beqz t8; nop; b exit; nop`. */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct Sub { s8 pad0[5]; s8 on; s8 pad6[34]; } Sub;
typedef struct Rec { s8 pad0[11]; s8 active; s8 pad12[116]; Sub sub[16]; s8 pad[4]; } Rec;
typedef struct ModelLink { struct ModelData *data; } ModelLink;
typedef struct ModelData { ModelLink *next; s8 pad4[13]; u8 wheel; s8 pad18[54]; s32 enabled; } ModelData;
typedef struct ModelList { ModelLink *head; s32 pad[3]; } ModelList;
extern Rec D_80144030[];
extern ModelList D_80144D68[];


void func_800A1BB4(s32 index) {
    ModelLink *p, *next;
    ModelData *data;

    if (D_80144030[index].active == 0) return;
    next = p = D_80144D68[index].head;
    while (p != 0) {
        data = p->data;
        next = data->next;
        if (data->enabled != 0) {
            if (D_80144030[index].sub[data->wheel].on != 0) break;
        }
        p = next;
    }
    if (p == 0) D_80144030[index].active = 0;
}

void zz_caller_a1bb4(s32 i)
{
    func_800A1BB4(i);
    func_800A1BB4(i + 1);
}
