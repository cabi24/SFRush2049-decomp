/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef unsigned char u8;
typedef signed int s32;
typedef struct WeightRecord {
    u8 prefix[4];
    f32 weight[3][5];
    u8 suffix[32];
} WeightRecord;
extern WeightRecord D_80150F88[12];
extern s32 D_80151690[12][3][5];
void audio_channel_priority(s32 handle) {
    s32 outer, row, slot, index;
    s32 *handles;
    f32 *weights;
    s32 id;

    id = handle;
    for (outer = 0; outer < 12; outer++) {
        for (row = 0; row < 60; row += 20) {
            handles = (s32 *) ((u8 *) D_80151690[outer] + row);
            weights = (f32 *) ((u8 *) D_80150F88[outer].weight + row);
            for (slot = 0; slot < 5; slot++) {
                if (id == handles[slot]) {
                    for (index = slot; index < 4; index++) {
                        handles[index] = handles[index + 1];
                        weights[index] = weights[index + 1];
                    }
                    weights[index] = 0.0f;
                    handles[index] = -1;
                    slot--;
                }
            }
        }
    }
}
