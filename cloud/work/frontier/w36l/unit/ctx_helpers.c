/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* MAT3 helper forms (zoid.c ZOID_FindMatrix, zoidtexture.c SetEnvColor, visuals.c
 * SetSheenColor) against the D_8012E700 instance table / D_80139320 CarParts. */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Node Node;
typedef struct Object68 { unsigned int flags; char pad4[4]; Node *node; char padC[0x30]; unsigned int color; unsigned int env; } Object68;
typedef struct CarParts { char prefix[0x14]; int sheen; char tail[0x28]; } CarParts;
extern Object68 D_8012E700[];
extern CarParts D_80139320[];
void func_8008E06C(short objnum, Color *color);
Node *func_8008B3A0(short objnum) { return D_8012E700[objnum].node; }
void func_80092BC8(short objnum, Color *color) { Object68 *o; o = &D_8012E700[objnum]; o->env=color->w; }
void func_80092BF4(short slot, Color *col, Color *env)
{
    CarParts *car;
    car = &D_80139320[slot];
    func_8008E06C(car->sheen, col);
    func_80092BC8(car->sheen, env);
}
