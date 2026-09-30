/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef struct { u8 a; u8 b; u8 c; u8 d; } S;
extern S D_8011EACC;
extern s32 D_8012E6D0;
void func_8008A148(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800878E0(s32 arg0);

void func_8008A38C(u16 arg0) {
    if (arg0 != D_8011EACC.d) {
        D_8011EACC.d = arg0;
        D_8012E6D0 = 0;
    }
    func_8008A148(&D_8011EACC, 4, 1, 0);
    func_800878E0(32);
}
