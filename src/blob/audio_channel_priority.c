/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_channel_priority (0x80094A54, 468 B): remove every occurrence of `handle` from the 12 x 3 x 5
 * handle table D_80151690, closing each gap: the later entries of the same 5-slot row shift down one place,
 * the parallel f32 weight row in the 96-byte record D_80150F88[outer] (weights at +4) shifts with them, and
 * the freed last slot becomes handle -1 / weight 0.0f. The slot index is re-tested after a removal (slot--).
 *
 * No shaping devices. The match came from the plain index form (w26e): `row` is an index 0..2 that uopt
 * strength-reduces to the byte offset row*20 (retail a0, bound 60), the parameter is used directly as the id
 * (its web is numbered at its first use, after slot -> t0), and `weights` is assigned at the top of the row
 * loop next to `handles`; uopt sinks it into the if-block, and its earlier first occurrence numbers the weights
 * web right after `outer`, so phase-2 colouring gives retail's outer v0 / weights v1 / row a0 order
 * (w2h/w4a/w12e: 11 words with a byte-offset `row` and the weights pointer formed inside the if).
 */
typedef float f32;
typedef unsigned char u8;

typedef struct WeightRecord {
    u8 prefix[4];
    f32 weight[3][5];
    u8 suffix[32];
} WeightRecord;

extern WeightRecord D_80150F88[12];
extern int D_80151690[12][3][5];

void audio_channel_priority(int handle) {
    int outer, row, slot, index;
    f32 *weights;
    int *handles;

    for (outer = 0; outer < 12; outer++) {
        for (row = 0; row < 3; row++) {
            handles = D_80151690[outer][row];
            weights = D_80150F88[outer].weight[row];
            for (slot = 0; slot < 5; slot++) {
                if (handle == handles[slot]) {
                    for (index = slot; index < 4; index++) {
                        weights[index] = weights[index + 1];
                        handles[index] = handles[index + 1];
                    }
                    weights[index] = 0.0f;
                    handles[index] = -1;
                    slot--;
                }
            }
        }
    }
}
