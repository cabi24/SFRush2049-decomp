/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef union tRGBA { unsigned char c[4]; unsigned int rgba; } tRGBA;
typedef struct tUVMatrix { float uvs[3][3]; float pos[3]; } tUVMatrix;
typedef struct tBlastObj {
    int mbIdx;                  /* 0x00 */
    tUVMatrix mat;              /* 0x04 */
    float scale;                /* 0x34 */
    float scaleInc;             /* 0x38 */
    float yaw;                  /* 0x3C */
    float yawInc;               /* 0x40 */
    float pitch;                /* 0x44 */
    float pitchInc;             /* 0x48 */
    unsigned char alpha;        /* 0x4C */
    unsigned char alphaInc;     /* 0x4D */
    signed short unused;        /* 0x4E */
    float ht;                   /* 0x50 */
} tBlastObj;
typedef struct tBlastStruct {
    tBlastObj blastObjs[4];     /* 0x000 */
    int glowIdx;                /* 0x150 */
    tUVMatrix mat;              /* 0x154 */
    unsigned char alpha;        /* 0x184 */
    unsigned char alphaCnt;     /* 0x185 */
    unsigned char ralpha;       /* 0x186 */
    unsigned char ralphaCnt;    /* 0x187 */
    float lifeTime;             /* 0x188 */
    float incTime;              /* 0x18C */
} tBlastStruct;
typedef struct tVisual { u8 pad0[6]; s16 objnum; s16 slot; u8 padA[0x14 - 0xA]; void *func; } tVisual;
typedef struct tCarData { u8 pad0[8]; float dr_pos[3]; u8 pad14[952 - 0x14]; } tCarData;
typedef struct tModelData { u8 pad0[0x640]; s8 crashflag; u8 pad641[0x6C4 - 0x641]; s16 moving_state; u8 pad6C6[0x808 - 0x6C6]; } tModelData;
typedef struct ObjInstance { unsigned int flags; char pad4[8]; float scale; char pad10[0x2C]; unsigned int color; unsigned int envcolor; } ObjInstance;

extern s32 D_801170FC;                  /* gPauseState */
extern f32 D_8002EB94;                  /* gFrameSeconds */
extern tBlastStruct D_80154660[];       /* gBlastObjs */
extern tCarData player_array[];         /* gGameCar */
extern tModelData D_8014A250[];         /* model */
extern ObjInstance D_8012E700[];        /* ObjInstanceList */
extern s8 D_80156994;
extern s8 D_8014978C;                   /* gTrackNum */
extern s16 active_player_count;
extern int D_8011735C;

void entity_spawn_callback(s16 objnum, s32 a, s32 b);     /* ZOID_RemoveObject */
void entity_transform_apply(tVisual *v, s32 unlink);       /* ReleaseEnviron */
void func_80090F44(float angle, float uvs[3][3]);          /* PitchUV */
void func_80090E9C(float angle, float uvs[3][3]);          /* YawUV */

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static f32 Random(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}

static void ZOID_SetColor(s16 objnum, tRGBA *col)
{
    D_8012E700[objnum].color = col->rgba;
}

void entity_update_callback(tVisual *v, s16 op)
{
    int i;
    tBlastObj *b;
    tRGBA clr;
    float x;
    float y;
    if (D_801170FC) return;
    D_80154660[v->slot].lifeTime -= D_8002EB94;
    if (D_80154660[v->slot].lifeTime <= 0.0f || D_8014A250[v->slot].moving_state >= 0) {
        for (i = 0; i < 4; i++) {
            b = &D_80154660[v->slot].blastObjs[i];
            if (b->mbIdx != -1) entity_spawn_callback(b->mbIdx, 0, 0);
            b->mbIdx = -1;
        }
    }
    if (!D_8014A250[v->slot].crashflag) {
        if (D_80156994 || D_8014978C >= 6) {
            if (D_80154660[v->slot].glowIdx != -1) {
                entity_spawn_callback(D_80154660[v->slot].glowIdx, 0, 0);
                D_80154660[v->slot].glowIdx = -1;
            }
        }
        for (i = 0; i < 4; i++) {
            b = &D_80154660[v->slot].blastObjs[i];
            if (b->mbIdx != -1) entity_spawn_callback(b->mbIdx, 0, 0);
            b->mbIdx = -1;
        }
        goto release;
    }
    if (!op) {
release:
        entity_transform_apply(v, 1);
        return;
    }
    D_80154660[v->slot].mat.pos[0] = player_array[v->slot].dr_pos[0];
    D_80154660[v->slot].mat.pos[1] = player_array[v->slot].dr_pos[1];
    D_80154660[v->slot].mat.pos[2] = player_array[v->slot].dr_pos[2];
    D_80154660[v->slot].incTime -= D_8002EB94;
    if (D_80154660[v->slot].incTime <= 0.0f) {
        if (D_80154660[v->slot].alphaCnt == 0) D_80154660[v->slot].alpha -= 31;
        else if (D_80154660[v->slot].alphaCnt == 1) D_80154660[v->slot].alpha += 31;
        else if (D_80154660[v->slot].alphaCnt > 1) {
            D_80154660[v->slot].alphaCnt = (int)Random(3.0f) * -1;
            D_80154660[v->slot].alpha = 95;
        }
        D_80154660[v->slot].alphaCnt++;
        if (active_player_count < 4 && (D_80156994 || D_8014978C >= 6)) {
            clr.c[0] = 255;
            clr.c[1] = 255;
            clr.c[2] = 255;
            clr.c[3] = D_80154660[v->slot].alpha;
            ZOID_SetColor(D_80154660[v->slot].glowIdx, &clr);
            if (D_8012E700[D_80154660[v->slot].glowIdx].scale < 1.0f)
                D_8012E700[D_80154660[v->slot].glowIdx].scale += 0.05f;
        }
        for (i = 0; i < 4; i++) {
            b = &D_80154660[v->slot].blastObjs[i];
            if (b->mbIdx != -1) {
                b->mat.uvs[0][0] = b->mat.uvs[1][1] = b->mat.uvs[2][2] = b->scale;
                b->mat.uvs[0][1] = b->mat.uvs[1][0] = b->mat.uvs[2][0] = 0.0f;
                b->mat.uvs[0][2] = b->mat.uvs[1][2] = b->mat.uvs[2][1] = 0.0f;
                switch (i) {
                    case 0: x = 0.0f; y = 0.0f; break;
                    case 1: x = 0.5f * b->ht; y = 0.5f * b->ht; break;
                    case 2: x = -0.5f * b->ht; y = -0.5f * b->ht; break;
                    case 3: x = -0.5f * b->ht; y = 0.5f * b->ht; break;
                }
                b->mat.pos[0] = x + player_array[v->slot].dr_pos[0];
                b->mat.pos[2] = y + player_array[v->slot].dr_pos[2];
                b->mat.pos[1] = b->ht + player_array[v->slot].dr_pos[1] - 3.0f;
                b->ht += 0.2f;
                func_80090F44(b->pitch, b->mat.uvs);
                func_80090E9C(b->yaw, b->mat.uvs);
                b->scale += b->scaleInc;
                b->scaleInc -= 0.002f;
                b->yaw += b->yawInc;
                b->pitch += b->pitchInc;
                if (D_80154660[v->slot].lifeTime < 0.2f) {
                    clr.c[0] = 244;
                    clr.c[1] = 205;
                    clr.c[2] = 20;
                    clr.c[3] = b->alpha;
                    if (b->alpha > b->alphaInc) b->alpha -= b->alphaInc;
                    ZOID_SetColor(b->mbIdx, &clr);
                }
            }
        }
        D_80154660[v->slot].incTime = 0.0333333f;
    }
}
