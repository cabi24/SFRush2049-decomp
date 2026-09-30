/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern signed char D_8014AA3A;
extern char D_80152818;
int func_8010C448(short *arg0, float *arg1, float *arg2, float *arg3) {
float diff[3];
volatile float pos[3];
char *car;
short index;
float radius, square;
int ret;
index = *arg0;
if (*((signed char *)&D_8014AA3A + index * 0x808) == 0) return 0;
pos[0] = arg1[0];
pos[1] = arg1[1];
pos[2] = arg1[2];
car = &D_80152818 + index * 0x3B8;
diff[0] = *(float *)(car + 8) - pos[0];
diff[1] = *(float *)(car + 12) - pos[1];
diff[2] = *(float *)(car + 16) - pos[2];
radius = *arg2 + 3.5f;
square = diff[2] * diff[2] + diff[0] * diff[0];
if (arg3 != 0) *arg3 = square - radius * radius;
if (square - radius * radius > 0.0f) return 0;
ret = 0;
if (diff[1] > -2.0f && diff[1] < 18.0f) ret = 1;
return ret;
}
