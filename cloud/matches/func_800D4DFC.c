/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 model-to-reckon snapshot, adapting arcade game/communic.c mcommunication
 * and the snapshot portions of game/reckon.c. game/vecmath.h mveccopy is kept
 * in its authentic three-component form. Donor revision 845329d7b36f5a384c5625ed9a0aef584ab46139.
 *
 * The N64 scalar is a signed halfword; its 2.72727275f coefficient is four
 * times the arcade mph conversion, suggesting quarter-mph units. Other N64 differences: copying the
 * orientation first, two four-element suspension/shadow arrays, and three
 * base acceleration/velocity/position vectors into their reckon snapshots.
 * The offsets are observed N64 layout, not an asserted complete MODELDAT.
 *
 * Natural macro expansion closes the historical final-vector FP register
 * residual. All code and the owned coefficient match at both O3 and O2.
 * No artificial locals, dummy conditions, volatile shaping or recipe changes.
 */
#define mveccopy(a,r) {r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}

typedef unsigned char u8;
typedef signed short s16;
typedef float f32;
typedef struct ModelSnapshot {
    u8 other0[748];
    f32 uv[9];
    u8 other784[472];
    f32 rear2_radius;
    u8 other1260[68];
    f32 rear2_angvel;
    u8 other1332[16];
    f32 rear3_radius;
    u8 other1352[68];
    f32 rear3_angvel;
    u8 other1424[60];
    f32 suscomp[4];
    u8 other1500[16];
    f32 airdist[4];
    u8 other1532[312];
    f32 base_vectors[9];
    s16 speed_scaled;u8 other1882[2];
    f32 reckon_suscomp[4];
    f32 reckon_airdist[4];
    f32 reckon_vectors[9];
    f32 reckon_uv[9];
} ModelSnapshot;

extern void math_utility(f32 *,f32 *);
void func_800D4DFC(ModelSnapshot *object)
{
    math_utility(object->uv,object->reckon_uv);
    object->speed_scaled=(s16)(int)(((object->rear3_radius*object->rear3_angvel+object->rear2_angvel*object->rear2_radius)*0.5f)*2.72727275f);
    {
        int i;
        for(i=0;i<4;i++) {
            object->reckon_suscomp[i]=object->suscomp[i];
            object->reckon_airdist[i]=object->airdist[i];
        }
    }
    mveccopy((&object->base_vectors[0]),(&object->reckon_vectors[0]));
    mveccopy((&object->base_vectors[3]),(&object->reckon_vectors[3]));
    mveccopy((&object->base_vectors[6]),(&object->reckon_vectors[6]));

}
