/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical label only. Scrolls up to four point lists (D_80114628[slot],
 * a 20-byte header with an s16 count followed by 20-byte points whose first
 * two s16 are x and y) by the current frame's per-slot velocity
 * D_80114264[D_80151AD0 - 1].slot[slot].dx/dy * D_8002EB94 * (dimension << 5),
 * then wraps: every point votes -1 / +1 when it is beyond +/- (dimension << 5);
 * a unanimous vote (exactly -4 or +4, i.e. four points) shifts the whole list
 * by one dimension. The resulting x/y are recorded as floats in the frame
 * record (xs[i], ys[i]) and D_8011463C[D_80151AD0 - 1] is set to 1.
 * No arcade ancestor identified.
 *
 * Retail facts kept as they are: the second y test is `y <= +(height << 5)`
 * (not a negated bound), and the negations are `(-dim) << 5`.
 * Shaping: the two "no shift" stores are the int literal 0 while the
 * initialisers are 0.0f (retail keeps two separate zero registers, f0 and
 * f16). Also MATCH at -O2 with the same source. Prior attempt:
 * cloud/work/tiny_A69 (235/254; raw s16 list and byte-offset macro).
 */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef float f32;

typedef struct {
    s16 x;
    s16 y;
    char pad4[0x10];
} Point; /* 0x14 */

typedef struct {
    s16 count;
    char pad2[0x12];
    Point pts[1];
} PointList;

typedef struct {
    char pad0[0x10];
    u16 width;
    u16 height;
} Dims;

typedef struct {
    char pad0[0x10];
    f32 dx;
    f32 dy;
    f32 xs[4];
    f32 ys[4];
} Motion; /* 0x38 */

typedef struct {
    Motion slot[4];
} MotionFrame; /* 0xE0 */

extern PointList *D_80114628[4];
extern Dims *D_80114638;
extern s32 D_8011463C[];
extern s16 D_80151AD0;
extern f32 D_8002EB94;
extern MotionFrame D_80114264[];

void func_800FC9F8(void) {
    s32 slot;
    s32 i;
    PointList *list;
    f32 shift[2];

    for (slot = 0; slot < 4; slot++) {
        list = D_80114628[slot];
        if (list != 0) {
            shift[0] = 0.0f;
            shift[1] = 0.0f;
            for (i = 0; i < list->count; i++) {
                list->pts[i].x += D_80114264[D_80151AD0 - 1].slot[slot].dx * D_8002EB94 * (D_80114638->width << 5);
                list->pts[i].y += D_80114264[D_80151AD0 - 1].slot[slot].dy * D_8002EB94 * (D_80114638->height << 5);
            }
            for (i = 0; i < list->count; i++) {
                if (list->pts[i].x >= D_80114638->width << 5) {
                    shift[0] -= 1.0f;
                }
                if (list->pts[i].x <= -D_80114638->width << 5) {
                    shift[0] += 1.0f;
                }
                if (list->pts[i].y >= D_80114638->height << 5) {
                    shift[1] -= 1.0f;
                }
                if (list->pts[i].y <= D_80114638->height << 5) {
                    shift[1] += 1.0f;
                }
            }
            if (shift[0] == -4.0f) {
                shift[0] = -D_80114638->width << 5;
            } else if (shift[0] == 4.0f) {
                shift[0] = D_80114638->width << 5;
            } else {
                shift[0] = 0;
            }
            if (shift[1] == -4.0f) {
                shift[1] = -D_80114638->height << 5;
            } else if (shift[1] == 4.0f) {
                shift[1] = D_80114638->height << 5;
            } else {
                shift[1] = 0;
            }
            for (i = 0; i < list->count; i++) {
                list->pts[i].x += shift[0];
                list->pts[i].y += shift[1];
                D_80114264[D_80151AD0 - 1].slot[slot].xs[i] = list->pts[i].x;
                D_80114264[D_80151AD0 - 1].slot[slot].ys[i] = list->pts[i].y;
            }
            D_8011463C[D_80151AD0 - 1] = 1;
        }
    }
}
