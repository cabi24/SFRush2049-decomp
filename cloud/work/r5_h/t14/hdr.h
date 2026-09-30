typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[8]; s16 idx; } In;
typedef struct { u8 pad[320]; u16 v[8]; } Out;
typedef struct { u32 flags; u8 pad[948]; } PF;
typedef struct { Out *o; u8 pad[20]; } PO;
extern PF D_80152900[];
extern PO D_8013FEF4[];
extern s8 D_8014978C;
extern u16 D_8011B57C[8];
extern u16 D_8011B58C[8];
extern u16 D_8011B59C[8];
#define CP(src) do { \
    o->v[0] = (src)[0]; o->v[1] = (src)[1]; o->v[2] = (src)[2]; o->v[3] = (src)[3]; \
    o->v[4] = (src)[4]; o->v[5] = (src)[5]; o->v[6] = (src)[6]; o->v[7] = (src)[7]; } while (0)
