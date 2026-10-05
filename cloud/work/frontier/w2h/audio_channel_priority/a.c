/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;

typedef struct Rec60 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 val[3][5];
    /* 0x40 */ u8 pad40[0x20];
} Rec60;

extern Rec60 D_80150F88[12];
extern s32 D_80151690[12][3][5];

void audio_channel_priority(s32 id) {
    s32 i;
    s32 j;
    s32 k;
    s32 m;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 5; k++) {
                if (id == D_80151690[i][j][k]) {
                    for (m = k; m < 4; m++) {
                        D_80150F88[i].val[j][m] = D_80150F88[i].val[j][m + 1];
                        D_80151690[i][j][m] = D_80151690[i][j][m + 1];
                    }
                    D_80150F88[i].val[j][m] = 0.0f;
                    D_80151690[i][j][m] = -1;
                    k--;
                }
            }
        }
    }
}
