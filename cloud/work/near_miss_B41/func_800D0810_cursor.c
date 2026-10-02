/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef struct State {
    unsigned char prefix[0xC4];
    f32 matrix[4][3];
    unsigned char gap[0x30];
    f32 first[3];
    unsigned char gap2[0x18];
    f32 second[3],third[3],fourth[3],fifth[3];
    unsigned char tail[0x228];
    int state[4];
} State;
extern int D_80124EEC;
void func_800D0810(State *p) {
    int i,j; f32 *vector;
    for (i=0;i<3;i++) {
        p->first[i]=0.0f;
        p->fourth[i]=0.0f;
        p->fifth[i]=0.0f;
        p->second[i]=0.0f;
        p->third[i]=0.0f;
    }
    for (i=0;i<4;i++) {
        p->state[i]=D_80124EEC;
        vector=p->matrix[i];
        for (j=0;j<3;j++) *vector++=0.0f;
    }
}
