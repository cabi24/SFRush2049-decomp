/* ZOID_SetEnvColor (MAT3 zoidtexture.c), N64 func_80092BC8: the store goes through an instance pointer
 * local; inlined twice into AnimateFrame (via SetSheenColor) it gives retail's base-register store and
 * frame slots. Matches the locked single standalone at -O2 and in the whole-program unit (w36l). */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Object68 { unsigned int flags; char middle[60]; unsigned int env; } Object68;
extern Object68 D_8012E700[];
void func_80092BC8(short objnum, Color *color)
{
    Object68 *o;
    o = &D_8012E700[objnum];
    o->env = color->w;
}
