/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Screen-to-world unprojection (historical label car_lights_render).
 * Takes a viewport index, an s16 screen point, a camera (3x3 rotation
 * followed by its position) and a depth; builds the view-space vector
 * ((sx - cx) * depth / (scale_x * proj_x), (cy - sy) * depth / (scale_y * proj_y), depth),
 * mirrors it for the two mirror flags, rotates it with func_8009E820
 * (row-vector x matrix) and adds the camera position. No arcade ancestor
 * found (N64 viewport code). Also matches at -O2.
 *
 * Shaping quirks: the viewport pointer is formed as `v = D_8017A510; v += view;`
 * -- the typed `&D_8017A510[view]` gives ugen a `.noalias v,$sp` and lets as1
 * hoist the y-row loads above the in[0] stack store (6 rows off). `world` is
 * declared before `in` (stack order), and the output sums are written
 * `world[i] + cam->pos[i]`.
 */
typedef float f32;
typedef short s16;
typedef signed char s8;

typedef struct View {
    char pad0[28];
    f32 scale_x;   /* 28 */
    f32 scale_y;   /* 32 */
    f32 proj_x;    /* 36 */
    f32 proj_y;    /* 40 */
    f32 center_x;  /* 44 */
    f32 center_y;  /* 48 */
    char pad52[72 - 52];
} View;

typedef struct Camera {
    f32 mat[9];
    f32 pos[3];
} Camera;

extern View D_8017A510[];
extern s8 D_80140A04;
extern s8 D_80151AD8;
void func_8009E820(f32 *in, f32 *out, Camera *cam);

void car_lights_render(int view, s16 *screen, Camera *cam, f32 depth, f32 *out)
{
    f32 world[3];
    f32 in[3];
    View *v = D_8017A510;

    v += view;

    in[0] = ((screen[0] - v->center_x) * depth) / (v->scale_x * v->proj_x);
    in[1] = ((v->center_y - screen[1]) * depth) / (v->scale_y * v->proj_y);
    in[2] = depth;
    if (D_80140A04) {
        in[0] = -in[0];
    }
    if (D_80151AD8) {
        in[0] = -in[0];
        in[1] = -in[1];
    }
    func_8009E820(in, world, cam);
    out[0] = world[0] + cam->pos[0];
    out[1] = world[1] + cam->pos[1];
    out[2] = world[2] + cam->pos[2];
}
