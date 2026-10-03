/* Reconstruction: default viewport and screen bounds. Names are hypotheses. */
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct Viewport Viewport;
typedef struct Bounds { unsigned short left, top, right, bottom; } Bounds;
typedef struct ViewBinding { Viewport *viewport; Bounds *bounds; } ViewBinding;
extern int D_8002AFC0, D_8002AFC4;
extern unsigned char D_80146204, D_8017A63C;
extern Viewport D_8011EA30;
extern Bounds D_80149870;
extern ViewBinding D_8017A510;
extern void *D_80154188;
extern void arb_rate_set(int, Viewport *, Bounds *, void *, float, float, float, float, float);
void func_800A5A40(void)
{
    int width = D_8002AFC0;
    int height;
    int right, bottom;
    Bounds *bounds;
    ViewBinding *binding;
    D_80146204 = 1;
    D_8017A63C = 0;
    height = D_8002AFC4;
    arb_rate_set(0, &D_8011EA30, &D_80149870, D_80154188,
                 0.0f, (float)width, (float)height,
                 (float)(width / 2), (float)(height / 2));
    bottom = (unsigned short)D_8002AFC4;
    right = (unsigned short)D_8002AFC0;
    bounds = &D_80149870;
    binding = &D_8017A510;
    bounds->left = 0;
    bounds->top = 0;
    binding->viewport = &D_8011EA30;
    binding->bounds = &D_80149870;
    bounds->bottom = bottom;
    bounds->right = right;
}
