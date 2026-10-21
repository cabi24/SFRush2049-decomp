/* Reference SetEnvColor contract with locked N64 field offset. */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Object68 { unsigned int flags; char middle[60]; unsigned int env; } Object68;
extern Object68 D_8012E700[];
void func_80092BC8(short objnum, Color *color) { D_8012E700[objnum].env=color->w; }
