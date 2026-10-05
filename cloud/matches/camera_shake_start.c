/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_shake_start @ 0x800BDAA8, 744 bytes (historical label; real semantics: per-track
 * animation resource binding). It is the set-up half of race_setup_1 (the per-frame updater over
 * the same two tables). No arcade ancestor found.
 *   - Skipped when bit 3 of state_word_a is set and D_80156994 is clear.
 *   - Palette-animation rows of the current track (D_8011A840[D_8014978C], 20-byte records ending
 *     with a NULL name): reset the delay to 0.0333333f (one 30 Hz tick) and bind the data pointer,
 *     through func_800B24EC (kind >= 9, takes its field +24) or sound_bank_load (field +20, when found).
 *   - Unless D_80114650 is set, the object-animation sequences (D_8011A31C[track], 20-byte records
 *     ending with count == 0): for every frame, look its name up with func_800B24EC (only when
 *     D_80156994 is set, the track is >= 6, or the frames are not the shared D_8011905C table), bind the
 *     frame's display list (the object's own +24 list when flag 0x08000000 is set, else a copy made by
 *     func_800BDA24), then reset the sequence delay/index to their initial values.
 *   Both lookups get the name, &id, 0, D_80140BDC - 1 and 1 (five arguments; the locked callee
 *   definitions declare four).
 *
 * Strict MATCH at -O3 (also at -O2); own .rodata (0.0333333f = 0x3D088880 at 0x80123E58) verified by the
 * scorer; EQUAL in the whole-program unit.
 *
 * Shaping facts (each needed):
 * - `if (!row) goto second;` before the row loop: with `if (row) { while ... }` uopt places the
 *   re-materialised &D_80140BDC / &id in the shared join block (13 words off); the goto gives retail's
 *   separate null-path and loop-exit copies;
 * - two unused int locals declared after `id`: retail's frame is 112 bytes with id at sp+94
 *   (quirk: they only reserve stack slots);
 * - the kind byte at +9 is read signed (`lb`, `kind >= 9`); race_setup_1 reads the same byte as an
 *   unsigned switch operand;
 * - the second-phase condition keeps the redundant `!D_80156994` test (retail tests it again).
 */
typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef signed short s16;
typedef unsigned int u32; typedef float f32;

typedef struct Resource32 { u8 opaque[20]; void *bank, *data; u32 flags; } Resource32;
typedef struct Row20 { char *name; u8 opaque[5]; s8 kind; u8 gap[2]; f32 time; void *data; } Row20;
typedef struct Item12 { char *name; Resource32 *resource; void *output; } Item12;
typedef struct Sequence20 { s16 count, maximum, unused, current; f32 time, default_time; Item12 *items; } Sequence20;

extern Row20 *D_8011A840[];
extern Sequence20 *D_8011A31C[];
extern Item12 D_8011905C[];
extern u32 state_word_a;
extern s8 D_80156994, D_80114650, D_8014978C;
extern u8 D_80140BDC;
extern Resource32 *func_800B24EC(char *, u16 *, s8, s8, int);
extern Resource32 *sound_bank_load(char *, u16 *, s8, s8, int);
extern void *func_800BDA24(void *);

void camera_shake_start(void) {
    Row20 *row;
    Sequence20 *sequence;
    Resource32 *resource;
    int i;
    u16 id;
    int unused1;
    int unused2;

    if ((state_word_a & 8) && !D_80156994) return;
    row = D_8011A840[D_8014978C];
    if (!row) goto second;
    while (row->name) {
        row->time = 0.0333333f;
        if (row->kind >= 9) row->data = func_800B24EC(row->name, &id, 0, D_80140BDC - 1, 1)->data;
        else {
            resource = sound_bank_load(row->name, &id, 0, D_80140BDC - 1, 1);
            if (resource) row->data = resource->bank;
        }
        row++;
    }
second:
    if (D_80114650) return;
    sequence = D_8011A31C[D_8014978C];
    if (!sequence) return;
    while (sequence->count) {
        for (i = 0; i < sequence->count; i++) {
            if (D_80156994 || D_8014978C >= 6) {
                sequence->items[i].resource = func_800B24EC(sequence->items[i].name, &id, 0, D_80140BDC - 1, 1);
            } else if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6 && sequence->items != D_8011905C) {
                sequence->items[i].resource = func_800B24EC(sequence->items[i].name, &id, 0, D_80140BDC - 1, 1);
            } else continue;
            resource = sequence->items[i].resource;
            if (resource->flags & 0x08000000) sequence->items[i].output = resource->data;
            else sequence->items[i].output = func_800BDA24(resource->data);
            sequence->time = sequence->default_time;
            sequence->current = sequence->maximum;
        }
        sequence++;
    }
}
