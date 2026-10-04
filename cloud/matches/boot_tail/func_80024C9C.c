/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* MusyX salNormalizeVector source lead; see BT06-math-trio/README.md. */
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
