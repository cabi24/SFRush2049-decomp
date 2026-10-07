/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B: update emitted trail quads and their live source records. */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Record88 Record88;
typedef struct Trail Trail;
typedef struct Fade Fade;
struct Trail { Trail *next; u32 unknown04, flags; f32 lifetime; f32 origin[3], previous[3], lower[3], upper[3]; };
struct Fade { Fade *next; u32 unknown04; Record88 *object; f32 lifetime, interval; u8 color[4]; Trail *source; };
typedef struct FadePool { u8 unknown00[16]; Fade *active; } FadePool;
typedef struct TrailPool { u8 unknown00[16]; Trail *active; } TrailPool;
extern FadePool D_80396B80;
extern TrailPool D_80395EE8;
extern f32 D_8002EB94;
extern s16 D_8014A108;
extern u8 D_80394394[4][4];
extern void func_8008D0C0(Record88 *);
extern void func_800AFA84(void *,void *);
extern void *func_8008E3C0(void *);
extern Record88 *func_800A78BC(s32,f32 *,u16,u8 *,u16,s32);
extern void func_8008C074(Record88 *,s32,f32 *,u16,u8 *,u16,s32);

void func_8038CB20(void)
{
    Fade *fade, *next_fade, *new_fade;
    Trail *trail, *next_trail;
    f32 width;
    f32 corners[4][3];
    for (fade = D_80396B80.active; fade; fade = next_fade) {
        next_fade = fade->next;
        fade->lifetime -= D_8002EB94;
        if (fade->lifetime <= 0.0f) {
remove_fade:
            func_8008D0C0(fade->object);
            func_800AFA84(&D_80396B80, fade);
        } else {
            if (fade->source->flags & 0x1000) fade->lifetime = 0.2f;
            if (fade->lifetime <= 0.2f) {
                fade->interval -= D_8002EB94;
                if (fade->interval <= 0.0f) {
                    if (fade->source->flags & 8) fade->interval = 0.0f;
                    else fade->interval = 0.0333333f;
                    if (fade->color[3] < 23) {
                        goto remove_fade;
                    } else {
                        fade->color[3] -= 23;
                        func_8008C074(fade->object, 0, 0, 0, fade->color, 0, 0);
                    }
                }
            }
        }
    }
    for (trail = D_80395EE8.active; trail; trail = next_trail) {
        next_trail = trail->next;
        if (trail->flags & 0x1000) {
            trail->flags &= ~0x1000;
            func_800AFA84(&D_80395EE8, trail);
        } else if (trail->lifetime >= 0.0f) {
            trail->lifetime -= D_8002EB94;
            if (!(trail->flags & 0x2000) && (trail->flags & 0x10)) {
                trail->lower[0] = trail->upper[0] = trail->origin[0];
                trail->lower[1] = trail->upper[1] = trail->origin[1];
                trail->lower[2] = trail->upper[2] = trail->origin[2];
                if (trail->flags & 1) width = 0.75f;
                else if (trail->flags & 2) width = 0.5f;
                else if (trail->flags & 4) width = 0.35f;
                else width = 0.025f;
                trail->lower[1] -= width;
                trail->upper[1] += width;
            }
        } else if (trail->flags & 0x10) {
            if (!(trail->flags & 0x2000)) trail->flags |= 0x2000;
            if (D_8014A108 >= 3) trail->lifetime = 0.1f;
            else trail->lifetime = 0.0333333f;
            trail->flags &= ~0x10;
            new_fade = func_8008E3C0(&D_80396B80);
            if (new_fade) {
                if (trail->flags & 1) {
                    new_fade->lifetime = 0.75f;
                    width = 0.75f;
                    new_fade->color[0] = D_80394394[0][0];
                    new_fade->color[1] = D_80394394[0][1];
                    new_fade->color[2] = D_80394394[0][2];
                    new_fade->color[3] = D_80394394[0][3];
                } else if (trail->flags & 2) {
                    new_fade->lifetime = 0.5f;
                    width = 0.5f;
                    new_fade->color[0] = D_80394394[1][0];
                    new_fade->color[1] = D_80394394[1][1];
                    new_fade->color[2] = D_80394394[1][2];
                    new_fade->color[3] = D_80394394[1][3];
                } else if (trail->flags & 4) {
                    new_fade->lifetime = 0.25f;
                    width = 0.35f;
                    new_fade->color[0] = D_80394394[2][0];
                    new_fade->color[1] = D_80394394[2][1];
                    new_fade->color[2] = D_80394394[2][2];
                    new_fade->color[3] = D_80394394[2][3];
                } else {
                    new_fade->lifetime = 0.16f;
                    width = 0.025f;
                    new_fade->color[0] = D_80394394[3][0];
                    new_fade->color[1] = D_80394394[3][1];
                    new_fade->color[2] = D_80394394[3][2];
                    new_fade->color[3] = D_80394394[3][3];
                }
                if (D_8014A108 >= 3) new_fade->lifetime *= 0.5f;
                if (trail->flags & 8) new_fade->interval = 0.0f;
                else new_fade->interval = 0.0333333f;
                new_fade->source = trail;
                corners[0][0] = trail->lower[0];
                corners[0][1] = trail->lower[1];
                corners[0][2] = trail->lower[2];
                corners[1][0] = trail->upper[0];
                corners[1][1] = trail->upper[1];
                corners[1][2] = trail->upper[2];
                corners[2][0] = trail->origin[0];
                corners[2][1] = trail->origin[1];
                corners[2][2] = trail->origin[2];
                corners[3][0] = trail->origin[0];
                corners[3][1] = trail->origin[1];
                corners[3][2] = trail->origin[2];
                corners[2][1] += width;
                corners[3][1] -= width;
                new_fade->object = func_800A78BC(4, &corners[0][0], 0, new_fade->color, 0x120f, 0);
                if (new_fade->object) {
                    trail->upper[0] = corners[2][0];
                    trail->upper[1] = corners[2][1];
                    trail->upper[2] = corners[2][2];
                    trail->lower[0] = corners[3][0];
                    trail->lower[1] = corners[3][1];
                    trail->lower[2] = corners[3][2];
                    trail->previous[0] = trail->origin[0];
                    trail->previous[1] = trail->origin[1];
                    trail->previous[2] = trail->origin[2];
                } else {
                    func_800AFA84(&D_80396B80, new_fade);
                }
            }
        }
    }
}
