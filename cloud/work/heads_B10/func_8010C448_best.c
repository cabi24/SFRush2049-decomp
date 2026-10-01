/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern signed char D_8014AA3A;
extern char D_80152818;
typedef struct BVector { float x,y,z; } BVector;
int func_8010C448(short *arg0, float *arg1, float *arg2, float *arg3) {
BVector diff;
volatile BVector pos;
char *car;
short index;
float radius, square;
int ret;
index = *arg0;
radius = *arg2;
if (*((signed char *)&D_8014AA3A + index * 0x808) == 0) return 0;
pos.x = arg1[0];
pos.y = arg1[1];
pos.z = arg1[2];
car = &D_80152818 + index * 0x3B8;
diff.x = *(float *)(car + 8) - pos.x;
diff.y = *(float *)(car + 12) - pos.y;
diff.z = *(float *)(car + 16) - pos.z;
radius += 3.5f;
square = diff.z * diff.z + diff.x * diff.x;
if (arg3 != 0) *arg3 = square - radius * radius;
if (square - radius * radius > 0.0f) return 0;
ret = 0;
if (diff.y > -2.0f && diff.y < 18.0f) ret = 1;
return ret;
}
