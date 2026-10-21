/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Node Node;
typedef struct Object68 { unsigned int flags; char pad4[4]; Node *node; char padC[0x30]; unsigned int color; unsigned int env; } Object68;
extern Object68 D_8012E700[];
void func_8008E06C(short objnum, Color *color) { ((Object68 *)((char *)D_8012E700 + objnum * 0x44))->color = color->w; }
void func_80092BC8(short objnum, Color *color) { ((Object68 *)((char *)D_8012E700 + objnum * 0x44))->env = color->w; }
Node *func_8008B3A0(short objnum) { return ((Object68 *)((char *)D_8012E700 + objnum * 0x44))->node; }
unsigned int func_8008AE64(short objnum) { return ((Object68 *)((char *)D_8012E700 + objnum * 0x44))->flags; }
