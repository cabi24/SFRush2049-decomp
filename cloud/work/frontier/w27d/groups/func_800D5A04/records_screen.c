/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * records_screen() -- tear-down of the records/attract screen state (name is a historical label).
 * If the screen voice D_80154198 is live: clear the sound handles (sound_handles_clear(1)), stop the
 * voice and forget it, then for each of the D_80151AD0 rows clear its status word D_80154350[i] and
 * release the two entity slots held in the 104-byte row record at D_801541A8 (s16 ids at +0 and
 * +52; -1 = empty) through entity_spawn_callback(id, 0, 0), marking each slot empty; finally clear
 * D_80154360. Then stop and forget the second voice D_801541A0, and in gameplay modes 6 and 4 call
 * the overlay routine at 0x8039244C. No arcade ancestor identified.
 *
 * Shaping (w13f; tiny_A121's draft was 4 words off):
 *  - `neg` is a named local holding -1 (the empty-slot marker). Retail keeps -1 in s1 and compares
 *    `beq s1,v0`; with a literal -1 (either operand order) ugen emits `beq v0,s1`. The named local
 *    gives retail's operand order.
 *  - the two loop pointers are initialised on ONE source line, status first. as1 breaks scheduling
 *    ties by source line: on one line the status `lui` lands in the blez delay slot and the addiu
 *    pair comes out row-then-status as in retail (two lines give s0,s3,s0,s3 or s3,s0,s3,s0).
 *    Proven by reassembling the edited ugen listing (asm.sh) before writing the source.
 */
typedef signed short s16;
typedef int s32;
typedef unsigned char u8;

typedef struct Voice Voice;
typedef struct RecordSlot { s16 id; u8 pad2[50]; } RecordSlot;       /* 52 bytes */
typedef struct RecordRow { RecordSlot a, b; } RecordRow;              /* 104 bytes */

extern Voice *D_80154198;
extern Voice *D_801541A0;
extern s32 D_80154350[];
extern s32 D_80154360;
extern s16 D_80151AD0;
extern RecordRow D_801541A8[];
extern s32 gameplay_mode;

void sound_handles_clear(s32 arg0);
void sound_stop(Voice *voice);
void entity_spawn_callback(s16 idx, s32 freeChildren, s32 freeSiblings);
void func_8039244C(void);

void records_screen(void) {
    s32 i;
    s32 id;
    s32 neg;
    RecordRow *row;
    s32 *status;

    if (D_80154198 != 0) {
        sound_handles_clear(1);
        sound_stop(D_80154198);
        D_80154198 = 0;
        neg = -1;
        i = 0;
        if (D_80151AD0 > 0) {
            status = D_80154350; row = D_801541A8;
            do {
                id = row->a.id;
                *status = 0;
                if (neg != id) {
                    entity_spawn_callback((s16)id, 0, 0);
                    row->a.id = neg;
                }
                id = row->b.id;
                if (neg != id) {
                    entity_spawn_callback((s16)id, 0, 0);
                    row->b.id = neg;
                }
                i++;
                status++;
                row++;
            } while (i < D_80151AD0);
        }
        D_80154360 = 0;
    }
    if (D_801541A0 != 0) {
        sound_stop(D_801541A0);
        D_801541A0 = 0;
    }
    if (gameplay_mode == 6 || gameplay_mode == 4) {
        func_8039244C();
    }
}
