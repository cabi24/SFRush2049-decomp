/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef short s16;
typedef struct { char pad[1032]; float f; char pad2[964]; s16 out; } Obj;
extern float D_8012412C;
extern float D_80124130;
void func_800D03AC(Obj *a0) {
    a0->out = a0->f * D_8012412C * D_80124130;
}
