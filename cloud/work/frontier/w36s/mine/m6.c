/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
static s16 MBOX_NewObject(s32 obj,void *mat,s32 parent,u32 flags){s16 dlidx;dlidx=func_8008E26C(obj,mat,parent,flags);return dlidx;}
static void ZOID_SetColor(s16 objnum,Color *col){D_8012E700[objnum].body.view.color.word=col->word;}
static Color *ZOID_GetColor(s16 objnum){return &D_8012E700[objnum].body.view.color;}
static u32 ZOID_ObjectFlags(s16 objnum){return D_8012E700[objnum].flags;}
static void ZOID_SetObjectFlags(s32 objnum,u32 flags){D_8012E700[objnum].flags=flags;}
static void ZOID_RemoveObjectTree(s16 objnum,s32 removeChildren){entity_spawn_callback(objnum,removeChildren,0);}
static void ZOID_RemoveObject(s16 objnum){ZOID_RemoveObjectTree(objnum,0);}
void func_8010C974(Visual *v,s16 op)
{
    int i, j, k;
    Target *t;
    MineData *d;
    Color color;
    Color *color_ptr;
    f32 floor_position[3], floor_uv[3][3];
    f32 oldest;
    Link *node, *next;
    int index;
    if (!op) {
        entity_transform_apply(v, 1);
        return;
    }
    if (D_801170FC) return;
    t = v->data;
    d = t->data;
    if (!(t->flags & 1)) {
        if (D_80151528[t->owner].count == 3) {
            index = -1;
            oldest = 0.0f;
            for (i = 0; i < 4; i++) {
                if (D_80151528[t->owner].target[i] && oldest < D_80151528[t->owner].target[i]->age) {
                    index = i;
                    oldest = D_80151528[t->owner].target[i]->age;
                }
            }
            if (index != -1) D_80151528[t->owner].target[index]->age = 121.0f;
        }
        for (i = 0; i < 4; i++) {
            if (!D_80151528[t->owner].target[i]) {
                D_80151528[t->owner].target[i] = t;
                D_80151528[t->owner].count++;
                break;
            }
        }
        if (camera_trigger_check(t->position, floor_position, floor_uv)) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) t->mat[i][j] = floor_uv[i][j];
            }
        }
        d->base = MBOX_NewObject(D_80399B2C, 0, t->model, 0x2080);
        color.c.r = 255;
        color.c.g = 255;
        color.c.b = 255;
        color.c.a = 255;
        ZOID_SetColor(d->base, &color);
        ZOID_SetObjectFlags(t->model, ZOID_ObjectFlags(t->model) | 0x2000);
        ZOID_SetColor(t->model, &color);
        func_8008D870(d->base, &D_80151AE8[D_80399B00 >> 10].base[D_80399B00 & 1023], -1);
        d->phase = 0;
        d->timer = 0.0f;
        color_ptr = &color;
        color_ptr->c.r = D_80121D54[D_8012E67C[t->owner]][0];
        color_ptr->c.g = D_80121D54[D_8012E67C[t->owner]][1];
        color_ptr->c.b = D_80121D54[D_8012E67C[t->owner]][2];
        math_utility(t->mat, d->mat[0]);
        math_utility(t->mat, d->mat[1]);
        d->mat[0][3][0] = t->position[0];
        d->mat[0][3][1] = t->position[1];
        d->mat[0][3][2] = t->position[2];
        d->mat[0][3][0] = d->mat[0][3][0] + d->mat[0][1][0] * 0.75f;
        d->mat[0][3][1] = d->mat[0][3][1] + d->mat[0][1][1] * 0.75f;
        d->mat[0][3][2] = d->mat[0][3][2] + d->mat[0][1][2] * 0.75f;
        d->mat[1][3][0] = t->position[0];
        d->mat[1][3][1] = t->position[1];
        d->mat[1][3][2] = t->position[2];
        d->mat[1][3][0] = d->mat[1][3][0] + d->mat[1][1][0];
        d->mat[1][3][2] = d->mat[1][3][2] + d->mat[1][1][2];
        d->mat[1][3][1] = d->mat[1][3][1] + d->mat[1][1][1];
        d->ring[0] = MBOX_NewObject(D_80399B30, d->mat[0], -1, 0x42000);
        ZOID_SetColor(d->ring[0], &color);
        d->ring[1] = MBOX_NewObject(D_80399B30, d->mat[1], -1, 0x42000);
        ZOID_SetColor(d->ring[1], &color);
        d->alpha[0] = 255;
        d->alpha[1] = 255;
        d->step[0] = 0;
        d->step[1] = -1;
        d->scale[0] = 1.0f;
        d->scale[1] = 1.0f;
        d->elapsed[0] = 0.0f;
        d->elapsed[1] = 0.0f;
        model_data_load(d->ring[1], 0, 15);
        t->flags |= 3;
    }
    d->timer += D_8002EB94;
    if (d->timer >= 0.25f) {
        if (d->phase) {
            d->phase = 0;
            func_8008D870(d->base, &D_80151AE8[D_80399B00 >> 10].base[D_80399B00 & 1023], -1);
        } else {
            d->phase = 1;
            func_8008D870(d->base, &D_80151AE8[D_80399AF8[D_8012E67C[t->owner]] >> 10].base[D_80399AF8[D_8012E67C[t->owner]] & 1023], -1);
        }
        d->timer = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        if (d->step[i] >= 0) {
            if (d->step[i] >= 45) {
                d->scale[i] = 1.0f;
                d->alpha[i] = 255;
                color_ptr = ZOID_GetColor(d->ring[i]);
                color_ptr->c.a = 255;
                math_utility(t->mat, d->mat[i]);
                d->step[i] = -1;
                model_data_load(d->ring[i], 0, 15);
            } else {
                if (d->elapsed[i] >= D_80124954) {
                    math_utility(t->mat, d->mat[i]);
                    for (j = 0; j < 3; j++) {
                        for (k = 0; k < 3; k++) d->mat[i][j][k] *= d->scale[i];
                    }
                    d->elapsed[i] = 0.0f;
                    d->scale[i] += 0.25f;
                    d->step[i]++;
                }
                d->elapsed[i] += D_8002EB94;
                if (d->step[i] >= 20) {
                    if (d->alpha[i] >= 9) d->alpha[i] -= 8;
                    color_ptr = ZOID_GetColor(d->ring[i]);
                    color_ptr->c.a = d->alpha[i];
                }
                if (d->step[i] >= 25 && d->step[!i] < 0) {
                    d->step[!i] = 0;
                    model_transform_setup(d->ring[!i], 0, 15);
                }
            }
        }
    }
    t->age += D_8002EB94;
    if ((t->flags & 2) && !(t->flags & 4) && !(t->age > 120.0f)) return;
    for (node = D_80151AA8.active; node; node = next) {
        next = node->next;
        if (node->target == t) func_800AFA84(&D_80151AA8, node);
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
    if (d->ring[0] != -1) {
        ZOID_RemoveObject(d->ring[0]);
        d->ring[0] = -1;
    }
    if (d->ring[1] != -1) {
        ZOID_RemoveObject(d->ring[1]);
        d->ring[1] = -1;
    }
    if (d->base != -1) {
        ZOID_RemoveObject(d->base);
        d->base = -1;
    }
    d->active = 0;
    ZOID_RemoveObject(t->model);
    t->flags = 0;
    func_800AFA84(&D_80143FC8, t);
    entity_transform_apply(v, 1);
}
