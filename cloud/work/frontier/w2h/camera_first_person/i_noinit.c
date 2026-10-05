/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

#define AddVector(v1,v2,r)		(r[0] = v1[0]+v2[0], r[1] = v1[1]+v2[1], r[2] = v1[2]+v2[2])
#define SubVector(v1,v2,r)		(r[0] = v1[0]-v2[0], r[1] = v1[1]-v2[1], r[2] = v1[2]-v2[2])
#define vecadd(a,b,r)	{r[0]=a[0]+b[0]; r[1]=a[1]+b[1]; r[2]=a[2]+b[2];}
typedef struct Item {
    /* 0x00 */ u16 tag;
    /* 0x02 */ u16 matrix;
    /* 0x04 */ s16 rot[9];
    /* 0x16 */ u16 point;
    /* 0x18 */ s16 pos[3];
    /* 0x1E */ u16 frac;
} Item;

typedef struct OutMatrix {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 rot[9];
    /* 0x16 */ s16 pad;
} OutMatrix;

typedef struct OutPoint {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u16 frac;
} OutPoint;

extern u16 D_8015267C;
extern Item *D_801525EC;
extern OutMatrix *D_801497F8;
extern OutPoint *D_8015201C;

void func_800AD650(s16 *p, f32 *o) {
    o[0] = (f32) p[0] * 0.00006103515625f;
    o[1] = (f32) p[1] * 0.00006103515625f;
    o[2] = (f32) p[2] * 0.00006103515625f;
    o[3] = (f32) p[3] * 0.00006103515625f;
    o[4] = (f32) p[4] * 0.00006103515625f;
    o[5] = (f32) p[5] * 0.00006103515625f;
    o[6] = (f32) p[6] * 0.00006103515625f;
    o[7] = (f32) p[7] * 0.00006103515625f;
    o[8] = (f32) p[8] * 0.00006103515625f;
}

void func_800BF780(f32 m[3][3], f32 in[3][3], f32 out[3][3]);
void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);

void camera_first_person(s32 tag, f32 *origin, f32 m1[3][3], f32 m2[3][3]) {
    s32 pad0;
    s32 ipos[3];
    s32 pad1;
    s32 pad2;
    f32 pos[3];
    f32 rel[3];
    f32 mat[3][3];
    f32 tmp[3][3];
    Item *item;
    s16 *out;
    OutPoint *pt;
    u32 i;
    s32 count;

    item = D_801525EC;
    count = 0;
    for (i = 0; i < D_8015267C; i++, item++) {
        if (tag == item->tag) {
            count++;
            func_800AD650(item->rot, (f32 *) mat);
            func_800BF780(m1, mat, tmp);
            func_800BF780(m2, tmp, mat);
            out = D_801497F8[item->matrix].rot;
            out[0] = mat[0][0] * 16384.0f;
            out[1] = mat[0][1] * 16384.0f;
            out[2] = mat[0][2] * 16384.0f;
            out[3] = mat[1][0] * 16384.0f;
            out[4] = mat[1][1] * 16384.0f;
            out[5] = mat[1][2] * 16384.0f;
            out[6] = mat[2][0] * 16384.0f;
            out[7] = mat[2][1] * 16384.0f;
            out[8] = mat[2][2] * 16384.0f;
            pos[0] = ((item->pos[0] << 5) + ((item->frac & 0x7C00) >> 10)) * 0.03125f;
            pos[1] = ((item->pos[1] << 5) + ((item->frac & 0x3E0) >> 5)) * 0.03125f;
            pos[2] = ((item->pos[2] << 5) + (item->frac & 0x1F)) * 0.03125f;
            rel[0] = pos[0] - origin[0];
            rel[1] = pos[1] - origin[1];
            rel[2] = pos[2] - origin[2];
            func_8009E820(rel, pos, (f32 *) m1);
            func_8009E820(pos, rel, (f32 *) m2);
            vecadd(rel, origin, pos);
            ipos[0] = pos[0] * 32.0f;
            ipos[1] = pos[1] * 32.0f;
            ipos[2] = pos[2] * 32.0f;
            pt = &D_8015201C[item->point];
            pt->frac = (ipos[2] & 0x1F) + ((ipos[0] & 0x1F) << 10) + ((ipos[1] & 0x1F) << 5);
            pt->pos[0] = ipos[0] >> 5;
            pt->pos[1] = ipos[1] >> 5;
            pt->pos[2] = ipos[2] >> 5;
        }
    }
    if (count == 0) {
    }
}
