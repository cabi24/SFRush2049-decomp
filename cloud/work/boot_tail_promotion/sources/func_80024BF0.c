/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80024BF0.c: local struct/typedef names Matrix/Vector
 * renamed SalMatrix/SalVector (rom_tu.h already defines Matrix); no other change. */
/* MusyX salApplyMatrix source lead; see BT06-math-trio/README.md. */
typedef struct SalVector { float x, y, z; } SalVector;
typedef struct SalMatrix { float m[3][3]; float t[3]; } SalMatrix;
void func_80024BF0(const SalMatrix *mat, const SalVector *in, SalVector *out)
{
    out->x = mat->m[0][0] * in->x + mat->m[0][1] * in->y + mat->m[0][2] * in->z + mat->t[0];
    out->y = mat->m[1][0] * in->x + mat->m[1][1] * in->y + mat->m[1][2] * in->z + mat->t[1];
    out->z = mat->m[2][0] * in->x + mat->m[2][1] * in->y + mat->m[2][2] * in->z + mat->t[2];
}
