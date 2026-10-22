/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w39b straight MAT3 port of _AnimateMine (types/helpers from the w36s draft). Base header follows: w36s NON-MATCH draft: func_8010C974 = _AnimateMine (MAT3 targets.c 0x8004D428), from Astra's #403 typed body.
 * Unit: blob_unit --tag w36s score func_8010C974 --with THIS -> FAIL 167 of 659 words (frame 376 vs 392).
 * Residuals: 16 B frame, j/index colour (a0 vs a1), ugen temp-ring rotation from the first texture lookup.
 * See cloud/work/frontier/w36s/RESULTS.md. */
/* Native-led typed Mine callback, informed by the complete MAT3 donor.
 * Research only. No match, runtime, or integration claim. The exact native
 * threshold is independently verified against 0.0333333f by the proof.
 * Allocator return width follows original signed-short contract; its complete
 * body is retained as context, with unchanged inherited debug checks. */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
typedef union Color {struct {u8 r,g,b,a;} c;u32 word;} Color;
typedef struct MineData {
 s32 base,ring[2]; f32 timer; f32 mat[2][4][3];
 f32 elapsed[2]; s8 step[2]; u8 alpha[2]; f32 scale[2]; s16 phase,active;
} MineData;
typedef struct Target {
 struct Target *next;u8 flags,pad5[7];s32 model;s16 info;u8 pad18[2];
 f32 mat[4][3],position[3];s16 kind;u8 pad82[10];s8 car;u8 pad93[3];
 s32 sound;s8 sound_state,owner;u8 pad102[2];f32 age;MineData *data;
} Target;
typedef struct Visual {struct Visual *next;s16 index,model,owner;u8 pad10[2];Target *data;f32 time;void (*callback)(struct Visual*,s16);} Visual;
typedef struct Object68 {u32 flags;s32 w4;void *owner;f32 scaleA,scaleB;u16 id;s16 link[3];union {s32 words[9];struct {s32 prefix[8];Color color;} view;} body;s32 w64;} Object68;
typedef struct Texture36 {u8 bytes[36];} Texture36;
typedef struct TextureBank {Texture36 *base;u32 other;} TextureBank;
typedef struct MineSlots {Target *target[4];s32 count;} MineSlots;
typedef struct Link {struct Link *next;Target *target;} Link;
typedef struct Pool {u8 doubly,pad1[15];Link *active,*free;} Pool;
extern s32 D_801170FC,D_80399B2C,D_80399B30;
extern volatile f32 D_8002EB94;
extern f32 D_80124954;
extern MineSlots D_80151528[];
extern Object68 D_8012E700[];
extern TextureBank D_80151AE8[];
extern u16 D_80399B00,D_80399AF8[];
extern s8 D_8012E67C[];
extern u8 D_80121D54[][3];
extern Pool D_80151AA8,D_80143FC8;
extern void entity_transform_apply(Visual *,s32);
extern void *camera_trigger_check(f32 *,f32 *,f32 (*)[3]);
extern s16 func_8008E26C(s32,void *,s16,s32);
extern void func_8008D870(s16,Texture36 *,s32);
extern void math_utility(void *,void *);
extern void model_data_load(s32,s32,u32),model_transform_setup(s32,s32,u32);
extern void func_800AFA84(Pool *,void *);
extern void func_8038D798(f32 *,f32 *,s32,f32,s32);
extern void save_write_data(f32 *,s32,f32,s32);
extern void entity_spawn_callback(s16,s32,s32);
extern s32 D_80156990,D_801569A8;
extern void render_mode_select(s16,s16);
#define DEBUG_PRINT(args)
static s16 MBOX_NewObject(s32 obj,void *mat,s32 parent,u32 flags){return func_8008E26C(obj,mat,parent,flags);}
static void ZOID_SetColor(s16 objnum,Color *col){D_8012E700[objnum].body.view.color.word=col->word;}
static Color *ZOID_GetColor(s16 objnum){return &D_8012E700[objnum].body.view.color;}
static u32 ZOID_ObjectFlags(s16 objnum){return D_8012E700[objnum].flags;}
static void ZOID_SetObjectFlags(s32 objnum,u32 flags){D_8012E700[objnum].flags=flags;}
static void ZOID_RemoveObjectTree(s16 objnum,s32 removeChildren){entity_spawn_callback(objnum,removeChildren,0);}
static void ZOID_RemoveObject(s16 objnum){ZOID_RemoveObjectTree(objnum,0);}
void func_8010C974(Visual *v, s16 op)
{
    int i;
    int j;
    int k;
    Target *t;
    MineData *m;
    Color clr;
    Color *clrPtr;
    f32 opos[3];
    f32 uvs[3][3];
    f32 oldT;
    Link *acol;
    Link *acolnext;
    if (!op) {
        entity_transform_apply(v, 1);
        return;
    }
    if (D_801170FC != 0) return;
    t = v->data;
    m = t->data;
    if (!(t->flags & 1)) {
        if (D_80151528[t->owner].count == 3) {
            oldT = 0.0f;
            j = -1;
            for (i = 0; i < 4; i++) {
                if (D_80151528[t->owner].target[i] && D_80151528[t->owner].target[i]->age > oldT) {
                    j = i;
                    oldT = D_80151528[t->owner].target[i]->age;
                }
            }
            if (j != -1) D_80151528[t->owner].target[j]->age = 121.0f;
        }
        for (i = 0; i < 4; i++) {
            if (!D_80151528[t->owner].target[i]) {
                D_80151528[t->owner].target[i] = t;
                D_80151528[t->owner].count++;
                break;
            }
        }
        if (camera_trigger_check(t->position, opos, uvs)) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) t->mat[i][j] = uvs[i][j];
            }
        }
        m->base = MBOX_NewObject(D_80399B2C, 0, t->model, 0x2080);
        clr.c.r = 255;
        clr.c.g = 255;
        clr.c.b = 255;
        clr.c.a = 255;
        ZOID_SetColor(m->base, &clr);
        ZOID_SetObjectFlags(t->model, ZOID_ObjectFlags(t->model) | 0x2000);
        ZOID_SetColor(t->model, &clr);
        func_8008D870(m->base, &D_80151AE8[D_80399AF8[4] >> 10].base[D_80399AF8[4] & 1023], -1);
        m->timer = 0.0f;
        m->phase = 0;
        clrPtr = &clr;
        clrPtr->c.r = D_80121D54[D_8012E67C[t->owner]][0];
        clrPtr->c.g = D_80121D54[D_8012E67C[t->owner]][1];
        clrPtr->c.b = D_80121D54[D_8012E67C[t->owner]][2];
        math_utility(t->mat, m->mat[0]);
        math_utility(t->mat, m->mat[1]);
        m->mat[0][3][0] = t->position[0];
        m->mat[0][3][1] = t->position[1];
        m->mat[0][3][2] = t->position[2];
        m->mat[0][3][0] = m->mat[0][3][0] + m->mat[0][1][0] * 0.75f;
        m->mat[0][3][1] = m->mat[0][3][1] + m->mat[0][1][1] * 0.75f;
        m->mat[0][3][2] = m->mat[0][3][2] + m->mat[0][1][2] * 0.75f;
        m->mat[1][3][0] = t->position[0];
        m->mat[1][3][1] = t->position[1];
        m->mat[1][3][2] = t->position[2];
        m->mat[1][3][0] += m->mat[1][1][0];
        m->mat[1][3][1] += m->mat[1][1][1];
        m->mat[1][3][2] += m->mat[1][1][2];
        m->ring[0] = MBOX_NewObject(D_80399B30, m->mat[0], -1, 0x42000);
        ZOID_SetColor(m->ring[0], &clr);
        m->ring[1] = MBOX_NewObject(D_80399B30, m->mat[1], -1, 0x42000);
        ZOID_SetColor(m->ring[1], &clr);
        m->alpha[0] = 255;
        m->alpha[1] = 255;
        m->step[0] = 0;
        m->step[1] = -1;
        m->scale[0] = 1.0f;
        m->scale[1] = 1.0f;
        m->elapsed[0] = 0.0f;
        m->elapsed[1] = 0.0f;
        model_data_load(m->ring[1], 0, 15);
        t->flags |= 3;
    }
    m->timer += D_8002EB94;
    if (m->timer >= 0.25f) {
        if (m->phase) {
            m->phase = 0;
            func_8008D870(m->base, &D_80151AE8[D_80399AF8[4] >> 10].base[D_80399AF8[4] & 1023], -1);
        } else {
            m->phase = 1;
            func_8008D870(m->base, &D_80151AE8[D_80399AF8[D_8012E67C[t->owner]] >> 10].base[D_80399AF8[D_8012E67C[t->owner]] & 1023], -1);
        }
        m->timer = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        if (m->step[i] < 0) continue;
        if (m->step[i] >= 45) {
            m->scale[i] = 1.0f;
            m->alpha[i] = 255;
            clrPtr = ZOID_GetColor(m->ring[i]);
            clrPtr->c.a = 255;
            math_utility(t->mat, m->mat[i]);
            m->step[i] = -1;
            model_data_load(m->ring[i], 0, 15);
            continue;
        }
        if (m->elapsed[i] >= 0.0333333f) {
            math_utility(t->mat, m->mat[i]);
            for (k = 0; k < 3; k++) {
                for (j = 0; j < 3; j++) m->mat[i][k][j] *= m->scale[i];
            }
            m->scale[i] += 0.25f;
            m->elapsed[i] = 0.0f;
            m->step[i]++;
        }
        m->elapsed[i] += D_8002EB94;
        if (m->step[i] >= 20) {
            if (m->alpha[i] > 8) m->alpha[i] -= 8;
            clrPtr = ZOID_GetColor(m->ring[i]);
            clrPtr->c.a = m->alpha[i];
        }
        if (m->step[i] >= 25) {
            j = !i;
            if (m->step[j] < 0) {
                m->step[j] = 0;
                model_transform_setup(m->ring[j], 0, 15);
            }
        }
    }
    t->age += D_8002EB94;
    if (!(t->flags & 2) || (t->flags & 4) || t->age > 120.0f) {
        acol = D_80151AA8.active;
        while (acol) {
            acolnext = acol->next;
            if (acol->target == t) func_800AFA84(&D_80151AA8, acol);
            acol = acolnext;
        }
        for (i = 0; i < 4; i++) {
            if (D_80151528[t->owner].target[i] == t) {
                D_80151528[t->owner].target[i] = 0;
                D_80151528[t->owner].count--;
                break;
            }
        }
        if (t->age > 120.0f) func_8038D798(t->position, t->position, t->owner, 400.0f, 1600);
        save_write_data(t->position, 0, 0.4f, 1);
        if (m->ring[0] != -1) {
            ZOID_RemoveObject(m->ring[0]);
            m->ring[0] = -1;
        }
        if (m->ring[1] != -1) {
            ZOID_RemoveObject(m->ring[1]);
            m->ring[1] = -1;
        }
        if (m->base != -1) {
            ZOID_RemoveObject(m->base);
            m->base = -1;
        }
        m->active = 0;
        ZOID_RemoveObject(t->model);
        t->flags = 0;
        func_800AFA84(&D_80143FC8, t);
        entity_transform_apply(v, 1);
    }
}
