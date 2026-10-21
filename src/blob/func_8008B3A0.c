/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8008B3A0: replacement source for the locked single (w36l, wave 36). Struct form against the
 * ObjInstanceList table D_8012E700 (MAT3 zoid.c / zoidtexture.c / visuals.c); matches standalone and in
 * the whole-program unit, and its inlined copies share one table base inside AnimateRocketFlame/AnimateFrame
 * (the m2c singles use &D_8012E708/&D_8012E73C bases, which split that web). One statement per line:
 * as1 schedules by source line, a one-line body schedules the param home store differently. */
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
