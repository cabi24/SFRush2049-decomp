/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct Matrix { float m[3][3]; float t[3]; } Matrix;
void func_80024D74(Matrix *out, const Matrix *in) {
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
