/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Tire load and polynomial-coefficient setup, 0x800D08E4..0x800D09E8.
 * Historical target label retained; this is not a track-preview renderer.
 * Adapted from historicalsource/rushtherock 845329d7, game/initiali.c:
 * copy_tire_info load formula and tire_constants field expressions.
 * N64 receives the combined load at model +1468 and the tire offset separately.
 * Its l2 uses addition, and its reciprocal coefficients are rounded to float
 * from the donor's double literals before multiplication. This preserves the
 * actual binary32 pool, notably 1/3.4; float-first division is one ULP wrong.
 * Natural field expressions eliminate archived decompiler scalar ownership.
 * O2 also matches. No artificial locals, helpers, volatility or compiler changes.
 * Opaque bytes describe the real 2056-byte model and 92-byte tire records.
 */
typedef float F32;
typedef unsigned char U8;
typedef struct Tire { F32 tradius,springK,rubdamp,Cstiff,Cfmax,invmi,Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4,patchy; U8 tail72[20]; } Tire;
typedef struct Model { U8 other[1072]; Tire tires[4]; U8 other1440[28]; F32 load; U8 other1472[8]; F32 wheelbase; U8 tail1484[572]; } Model;
void track_preview_handler(Model *m,Tire *tdes,F32 otw) {
 tdes->Zforce = (m->load * otw / m->wheelbase) * .5f;
 tdes->Afmax = 3*tdes->Cfmax*tdes->Zforce/tdes->Cstiff;
 tdes->k1 = tdes->Cstiff/tdes->Zforce;
 tdes->k2 = tdes->k1*tdes->k1/(3*tdes->Cfmax);
 tdes->k3 = tdes->k1*tdes->k1*tdes->k1/(27*tdes->Cfmax*tdes->Cfmax);
 tdes->l2 = tdes->k2+tdes->k2;
 tdes->l3 = tdes->k3*3;
 tdes->m1 = 4.16f/tdes->Cfmax;
 tdes->m2 = tdes->m1*tdes->m1*(F32)(1.0/3.4);
 tdes->m3 = tdes->m1*tdes->m1*tdes->m1*(F32)(1.0/46.3);
 tdes->m4 = (F32)(1.0/.000055);
 tdes->patchy = 0;
}
