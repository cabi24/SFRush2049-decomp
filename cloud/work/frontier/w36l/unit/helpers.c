/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* ObjInstanceList (D_8012E700, 0x44-byte instances) accessors from MAT3 zoid.c /
 * zoidtexture.c, written against the instance table so that inlined copies share
 * one table base (retail AnimateRocketFlame keeps D_8012E700 in s3 for both the
 * colour stores at +0x3C and the matrix loads at +8). */
typedef union Color { unsigned char b[4]; unsigned int w; } Color;
typedef struct Node Node;
typedef struct ObjInstance {
    unsigned int flags;         /* 0x00 */
    char pad4[4];
    Node *mat;                  /* 0x08 */
    char padC[8];
    unsigned short index;       /* 0x14 */
    char pad16[0x26];
    unsigned int color;         /* 0x3C */
    unsigned int envcolor;      /* 0x40 */
} ObjInstance;
typedef struct CarParts { int frame; int extraparts[3]; int hood; int sheen; char tail[0x28]; } CarParts;
extern ObjInstance D_8012E700[];
extern CarParts D_80139320[];

/* ZOID_FindMatrix */
Node *func_8008B3A0(short objnum)
{
    return D_8012E700[objnum].mat;
}

/* ZOID_ObjectFlags */
unsigned int func_8008AE64(short objnum)
{
    return D_8012E700[objnum].flags;
}

/* ZOID_SetObjectFlags */
void func_8008AE48(int objnum, unsigned int flags)
{
    D_8012E700[objnum].flags = flags;
}

/* ZOID_SetColor */
void func_8008E06C(short objnum, Color *col)
{
    D_8012E700[objnum].color = col->w;
}

/* ZOID_SetEnvColor */
void func_80092BC8(short objnum, Color *col)
{
    ObjInstance *obj;
    obj = &D_8012E700[objnum];
    obj->envcolor = col->w;
}

/* SetSheenColor */
void func_80092BF4(short slot, Color *col, Color *env)
{
    CarParts *car;
    car = &D_80139320[slot];
    func_8008E06C(car->sheen, col);
    func_80092BC8(car->sheen, env);
}
