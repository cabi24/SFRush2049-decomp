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

