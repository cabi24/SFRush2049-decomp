/* GENERATED ROM-aligned TU — segment 0x257f0 (rom/lib_257f0)
 * layout map 1f4c654a273510a3d1bc3afe375aae20d5732a86ac8c1bcf9496c5a73fb09799; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80024BF0
 * Source:   cloud/work/boot_tail_promotion/sources/func_80024BF0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80024BF0.c:func_80024BF0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SalVector { float x, y, z; } SalVector;
typedef struct SalMatrix { float m[3][3]; float t[3]; } SalMatrix;
void func_80024BF0(const SalMatrix *mat, const SalVector *in, SalVector *out)
{
    out->x = mat->m[0][0] * in->x + mat->m[0][1] * in->y + mat->m[0][2] * in->z + mat->t[0];
    out->y = mat->m[1][0] * in->x + mat->m[1][1] * in->y + mat->m[1][2] * in->z + mat->t[1];
    out->z = mat->m[2][0] * in->x + mat->m[2][1] * in->y + mat->m[2][2] * in->z + mat->t[2];
}

/* PROMOTED 2026-10-04 — func_80024C9C
 * Source:   cloud/matches/boot_tail/func_80024C9C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/matches/boot_tail/func_80024C9C.c:func_80024C9C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct Vector { float x, y, z; } Vector;
extern float sqrtf(float);
float func_80024C9C(Vector *vec)
{
    float length;
    length = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
    vec->x /= length;
    vec->y /= length;
    vec->z /= length;
    return length;
}

/* PROMOTED 2026-10-04 — func_80024D04
 * Source:   cloud/matches/boot_tail/func_80024D04.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/matches/boot_tail/func_80024D04.c:func_80024D04 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80024D04(Vector *out, const Vector *a, const Vector *b)
{
    out->x = (a->y * b->z) - (a->z * b->y);
    out->y = (a->z * b->x) - (a->x * b->z);
    out->z = (a->x * b->y) - (a->y * b->x);
}

/* PROMOTED 2026-10-04 — func_80024D74
 * Source:   cloud/work/boot_tail_promotion/sources/func_80024D74.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80024D74.c:func_80024D74 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80024D74(SalMatrix *out, const SalMatrix *in) {
  float a;
  float b;
  float c;
  float f;

  a = in->m[1][1] * in->m[2][2] - in->m[2][1] * in->m[1][2];
  b = -(in->m[1][0] * in->m[2][2] - in->m[2][0] * in->m[1][2]);
  c = in->m[1][0] * in->m[2][1] - in->m[2][0] * in->m[1][1];
  f = 1.f / (in->m[0][0] * a + in->m[0][1] * b + in->m[0][2] * c);
  out->m[0][0] = f * a;
  out->m[1][0] = f * b;
  out->m[2][0] = f * c;
  out->m[0][1] = -f * (in->m[0][1] * in->m[2][2] - in->m[2][1] * in->m[0][2]);
  out->m[1][1] = f * (in->m[0][0] * in->m[2][2] - in->m[2][0] * in->m[0][2]);
  out->m[2][1] = -f * (in->m[0][0] * in->m[2][1] - in->m[2][0] * in->m[0][1]);
  out->m[0][2] = f * (in->m[0][1] * in->m[1][2] - in->m[1][1] * in->m[0][2]);
  out->m[1][2] = -f * (in->m[0][0] * in->m[1][2] - in->m[1][0] * in->m[0][2]);
  out->m[2][2] = f * (in->m[0][0] * in->m[1][1] - in->m[1][0] * in->m[0][1]);
  out->t[0] = (-in->t[0] * out->m[0][0] - in->t[1] * out->m[0][1]) - in->t[2] * out->m[0][2];
  out->t[1] = (-in->t[0] * out->m[1][0] - in->t[1] * out->m[1][1]) - in->t[2] * out->m[1][2];
  out->t[2] = (-in->t[0] * out->m[2][0] - in->t[1] * out->m[2][1]) - in->t[2] * out->m[2][2];
}

