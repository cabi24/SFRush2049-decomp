/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Scale each element of a 3-by-3 matrix, preserving row-major store order. */
void func_8008B32C(float src[3][3], float dst[3][3], float scale)
{
 int i, j;
 for (i = 0; i < 3; i++) {
  for (j = 0; j < 3; j++) {
   dst[i][j] = src[i][j] * scale;
  }
 }
}
