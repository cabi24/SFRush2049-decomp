typedef signed int s32;
typedef signed char s8;
typedef unsigned char u8;

extern s8 D_8011103C[];
extern s8 D_80111080[];
extern s8 D_8011119C[];
extern s8 D_80111230[];
extern s8 D_8011128C[];
extern s8 D_80111604[];
extern s8 D_80111648[];
extern s8 D_8011168C[];
extern s8 D_8011157C[];
extern s8 D_801115C0[];
extern float D_801112DC[];
extern float D_801113E0[];

extern void func_800C7578(void *p, u8 idx, u8 slot, s8 val);

void draw_number(void *p, s32 idx)
{
    func_800C7578(p, (u8)idx, 0, D_8011103C[idx]);
    func_800C7578(p, (u8)idx, 1, D_80111080[idx]);
    func_800C7578(p, (u8)idx, 2, D_8011119C[idx]);
    func_800C7578(p, (u8)idx, 3, D_80111230[idx]);
    func_800C7578(p, (u8)idx, 4, D_8011128C[idx]);
    func_800C7578(p, (u8)idx, 5, D_80111604[idx]);
    func_800C7578(p, (u8)idx, 6, D_80111648[idx]);
    func_800C7578(p, (u8)idx, 7, D_8011168C[idx]);
    func_800C7578(p, (u8)idx, 8, D_8011157C[idx]);
    func_800C7578(p, (u8)idx, 9, D_801115C0[idx]);
    func_800C7578(p, (u8)idx, 10, (s8)(D_801112DC[idx] * 100.0f - 75.0f));
    func_800C7578(p, (u8)idx, 11, (s8)(D_801113E0[idx] * 100.0f - 75.0f));
}

void caller_a(void *p, s32 i) { draw_number(p, i); draw_number(p, i + 1); }
void caller_b(void *p, s32 i) { draw_number(p, i); }
