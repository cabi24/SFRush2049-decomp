/* Accepted MAT3 SetSheenColor wrapper, native CarParts stride/field. */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct CarParts { char prefix[0x14]; int sheen; char tail[0x28]; } CarParts;
extern CarParts D_80139320[];
void func_8008E06C(short objnum, Color *color);
void func_80092BC8(short objnum, Color *color);
void func_80092BF4(short slot, Color *col, Color *env)
{
    CarParts *car;
    car = &D_80139320[slot];
    func_8008E06C(car->sheen, col);
    func_80092BC8(car->sheen, env);
}
