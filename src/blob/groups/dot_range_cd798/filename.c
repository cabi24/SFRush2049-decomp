typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct Y { u8 pad[0x38]; s32 f38; u8 name[11]; } Y;
typedef struct X { u8 p0[8]; struct C96Handle *f8; u8 p1[0x15]; u8 name[11]; Y **f2C; } X;
s32 func_800A1910(u8 *a, u8 *b, s32 n);
extern void *memcpy(void *, void *, unsigned);
u32 format_string_parse(u8 *, u32);
struct C96Handle;
void slot_state_lookup(struct C96Handle *, u8 *, u32);
void func_800CCE5C(X **arg0, u8 *arg1)
{
Y *y;
 if ((*arg0)->f2C != 0) {
 y = *(*arg0)->f2C;
 if (func_800A1910(y->name, arg1, 11) != 0) {
 memcpy((*arg0)->name, arg1, 11);
 memcpy(y->name, arg1, 11);
 y->f38 = format_string_parse(y->name, 11);
 slot_state_lookup((*arg0)->f8, (u8 *) &y->f38, 15);
 }
 }
}