/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* MusyX salCrossProduct source lead; see BT06-math-trio/README.md. */
typedef struct Vector { float x, y, z; } Vector;
void func_80024D04(Vector *out, const Vector *a, const Vector *b)
{
    out->x = (a->y * b->z) - (a->z * b->y);
    out->y = (a->z * b->x) - (a->x * b->z);
    out->z = (a->x * b->y) - (a->y * b->x);
}
