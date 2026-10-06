/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef unsigned char u8;
typedef struct WeightRecord {
    unsigned char prefix[4];
    f32 weight[3][5];
    unsigned char suffix[32];
} WeightRecord;
extern WeightRecord D_80150F88[12];
extern int D_80151690[12][3][5];
void audio_channel_priority(int handle) {
    int outer,row,slot,index;
    int *handles; f32 *weights;
    for (outer=0;outer<12;outer++) {
        weights = 0;
        for (row=0;row<60;row+=20) {
            handles=(int *)((u8 *)D_80151690[outer]+row);
            for (slot=0;slot<5;slot++) {
                if (handle==handles[slot]) {
                    weights=(f32 *)((u8 *)&D_80150F88[outer]+4+row);
                    for (index=slot;index<4;index++) {
                        weights[index]=weights[index+1];
                        handles[index]=handles[index+1];
                    }
                    weights[index]=0.0f;
                    handles[index]=-1;
                    slot--;
                }
            }
        }
    }
}
