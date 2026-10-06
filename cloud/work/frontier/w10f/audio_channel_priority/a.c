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
void func_80094A4C(f32 *weights, s32 *handles, s32 slot) {
    s32 index;
    for (index = slot; index < 4; index++) {
        weights[index] = weights[index + 1];
        handles[index] = handles[index + 1];
    }
    weights[index] = 0.0f;
    handles[index] = -1;
}

void audio_channel_priority(s32 id) {
    s32 outer, row, slot;
    for (outer = 0; outer < 12; outer++) {
        for (row = 0; row < 3; row++) {
            for (slot = 0; slot < 5; slot++) {
                if (id == D_80151690[outer][row][slot]) {
                    func_80094A4C(D_80150F88[outer].weight[row], D_80151690[outer][row], slot);
                    slot--;
                }
            }
        }
    }
}
