/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef struct WeightRecord {
    unsigned char prefix[4];
    f32 weight[3][5];
    unsigned char suffix[32];
} WeightRecord;
extern WeightRecord D_80150F88[12];
extern int D_80151690[12][3][5];
void audio_channel_priority(int handle) {
    int outer,row,slot,index;
    for (outer=0;outer<12;outer++) {
        for (row=0;row<3;row++) {
            for (slot=0;slot<5;slot++) {
                if (D_80151690[outer][row][slot]==handle) {
                    for (index=slot;index<4;index++) {
                        D_80150F88[outer].weight[row][index]=D_80150F88[outer].weight[row][index+1];
                        D_80151690[outer][row][index]=D_80151690[outer][row][index+1];
                    }
                    D_80150F88[outer].weight[row][index]=0.0f;
                    D_80151690[outer][row][index]=-1;
                    slot--;
                }
            }
        }
    }
}
