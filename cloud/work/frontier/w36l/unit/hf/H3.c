/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Node Node;
typedef struct Object68 { unsigned int flags; char pad4[4]; Node *node; char padC[0x30]; unsigned int color; unsigned int env; } Object68;
extern int D_8012E73C;
extern int D_8012E700;
#define D_8012E7xx ((Object68 *)&D_8012E700)
void func_8008E06C(short objnum, Color *color) { D_8012E7xx[objnum].color = color->w; }
