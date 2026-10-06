typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 handle;          /* 0x00 */
    f32 mat[3][3];       /* 0x04 */
    f32 pos[3];          /* 0x28 */
    f32 scale;           /* 0x34 */
    f32 dscale;          /* 0x38 */
    f32 yaw;             /* 0x3C */
    f32 dyaw;            /* 0x40 */
    f32 pitch;           /* 0x44 */
    f32 dpitch;          /* 0x48 */
    u8 alpha;            /* 0x4C */
    u8 fade;             /* 0x4D */
    u8 pad4E[2];
    f32 height;          /* 0x50 */
} Piece; /* 0x54 */
typedef struct {
    Piece piece[4];      /* 0x000 */
    s32 glow;            /* 0x150 */
    u8 pad154[0x178 - 0x154];
    f32 pos[3];          /* 0x178 */
    u8 alpha;            /* 0x184 */
    u8 phase;            /* 0x185 */
    u8 pad186[2];
    f32 life;            /* 0x188 */
    f32 timer;           /* 0x18C */
} Debris; /* 0x190 */
typedef struct {
    u8 pad0[8];
    s16 car;
} Obj;
typedef struct {
    u8 pad0[8];
    f32 pos[3];
    u8 pad14[952 - 0x14];
} Car;
typedef struct {
    u8 pad0[0x640];
    s8 active;           /* 0x640 */
    u8 pad641[0x6C4 - 0x641];
    s16 resurrect;       /* 0x6C4 */
    u8 pad6C6[0x808 - 0x6C6];
} Model;
typedef struct {
    u8 pad0[0xC];
    f32 glow;            /* 0x0C */
    u8 pad10[0x3C - 0x10];
    u32 color;           /* 0x3C */
    u8 pad40[4];
} Node;

extern s32 D_801170FC;
extern volatile f32 D_8002EB94;
extern Debris D_80154660[];
extern Model D_8014A250[];
extern Car D_80152818[];
extern Node D_8012E700[];
extern s8 D_80156994;
extern s8 D_8014978C;
extern s16 active_player_count;
extern s32 D_8011735C;

void entity_spawn_callback(s16 idx, s32 a, s32 b);
void entity_transform_apply(void *node, s32 unlink);
void func_80090F44(f32 angle, f32 uv[][3]);
void func_80090E9C(f32 angle, f32 uv[][3]);

void entity_update_callback(Obj *obj, s16 mode) {
    char buf[16];
    u8 color[4];
    f32 dx, dz;
    char buf2[80];
    s32 i;
    Piece *p;

    if (D_801170FC != 0)
        return;
    D_80154660[obj->car].life -= D_8002EB94;
    if (D_80154660[obj->car].life <= 0.0f || D_8014A250[obj->car].resurrect >= 0) {
        for (i = 0; i < 4; i++) {
            p = &D_80154660[obj->car].piece[i];
            if (p->handle != -1)
                entity_spawn_callback(p->handle, 0, 0);
            p->handle = -1;
        }
    }
    if (D_8014A250[obj->car].active == 0) {
        if (D_80156994 != 0 || D_8014978C >= 6) {
            if (D_80154660[obj->car].glow != -1)
                entity_spawn_callback(D_80154660[obj->car].glow, 0, 0);
            D_80154660[obj->car].glow = -1;
        }
        for (i = 0; i < 4; i++) {
            p = &D_80154660[obj->car].piece[i];
            if (p->handle != -1)
                entity_spawn_callback(p->handle, 0, 0);
            p->handle = -1;
        }
        goto done;
    }
    if (mode == 0) {
done:
        entity_transform_apply(obj, 1);
        return;
    }
    D_80154660[obj->car].pos[0] = D_80152818[obj->car].pos[0];
    D_80154660[obj->car].pos[1] = D_80152818[obj->car].pos[1];
    D_80154660[obj->car].pos[2] = D_80152818[obj->car].pos[2];
    if (obj == 0) {}
    D_80154660[obj->car].timer -= D_8002EB94;
    if (D_80154660[obj->car].timer <= 0.0f) {
    if (D_80154660[obj->car].phase == 0) {
        D_80154660[obj->car].alpha -= 31;
    } else if (D_80154660[obj->car].phase == 1) {
        D_80154660[obj->car].alpha += 31;
    } else if (D_80154660[obj->car].phase >= 2) {
        D_8011735C = D_8011735C * 1103515245 + 12345;
        D_80154660[obj->car].phase = (s32)((f32)((D_8011735C >> 16) & 0x7FFF) * 3.0f / 32768.0f) * -1;
        D_80154660[obj->car].alpha = 95;
    }
    D_80154660[obj->car].phase++;
    if (active_player_count < 4 && (D_80156994 != 0 || D_8014978C >= 6)) {
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        color[3] = D_80154660[obj->car].alpha;
        D_8012E700[(s16)D_80154660[obj->car].glow].color = *(u32 *)color;
        if (D_8012E700[D_80154660[obj->car].glow].glow < 1.0f)
            D_8012E700[D_80154660[obj->car].glow].glow += 0.05f;
    }
    for (i = 0; i < 4; i++) {
        p = &D_80154660[obj->car].piece[i];
        if (p->handle == -1)
            continue;
        p->mat[0][0] = p->mat[1][1] = p->mat[2][2] = p->scale;
        p->mat[0][1] = p->mat[0][2] = 0.0f;
        p->mat[1][0] = p->mat[1][2] = 0.0f;
        p->mat[2][0] = p->mat[2][1] = 0.0f;
        switch (i) {
        case 0:
            dx = dz = 0.0f;
            break;
        case 1:
            dz = dx = p->height * 0.5f;
            break;
        case 2:
            dz = dx = p->height * -0.5f;
            break;
        case 3:
            dx = p->height * -0.5f;
            dz = p->height * 0.5f;
            break;
        }
        p->pos[0] = D_80152818[obj->car].pos[0] + dx;
        p->pos[2] = D_80152818[obj->car].pos[2] + dz;
        p->pos[1] = D_80152818[obj->car].pos[1] + p->height - 3.0f;
        p->height += 0.2f;
        func_80090F44(p->pitch, p->mat);
        func_80090E9C(p->yaw, p->mat);
        p->scale += p->dscale;
        p->dscale -= 0.002f;
        p->yaw += p->dyaw;
        p->pitch += p->dpitch;
        if (D_80154660[obj->car].life < 0.2f) {
            color[0] = 244;
            color[1] = 205;
            color[2] = 20;
            color[3] = p->alpha;
            if (p->fade < p->alpha)
                p->alpha -= p->fade;
            D_8012E700[(s16)p->handle].color = *(u32 *)color;
        }
    }
    D_80154660[obj->car].timer = 0.0333333f;
    }
}
