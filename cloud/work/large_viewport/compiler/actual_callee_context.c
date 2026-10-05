/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef unsigned short u16;typedef short s16;typedef unsigned int u32;typedef float f32;
typedef struct Viewport16 {s16 scale[4],translate[4];} Viewport16;
typedef struct Descriptor72 {
    void *first,*bounds;
    int orthographic;
    f32 horizontal,vertical,tan_horizontal,tan_vertical,inverse_horizontal,inverse_vertical;
    f32 width,height,near_plane,far_plane,aspect,zoom,fog;
    s16 range_start,range_end;
    u8 red,green,blue,alpha;
} Descriptor72;
extern Descriptor72 D_8017A510[];
extern Viewport16 D_8011EA30;
extern f32 D_80151AA0;
extern u32 D_80124FC8;
extern Descriptor72 *exhaust_smoke_effect(int,f32,f32,f32,f32,f32,f32);
/* Exact SDK macro from reference/repos/ultralib/include/PR/gbi.h. */
#define GPACK_RGBA5551(r,g,b,a) ((((r)<<8)&0xf800)|(((g)<<3)&0x7c0)|(((b)>>2)&0x3e)|((a)&1))
void arb_rate_set(int index,void *first,void *bounds,f32 horizontal,f32 vertical,f32 width,f32 height,f32 near_plane,f32 far_plane)
{
    Descriptor72 *entry = &D_8017A510[index];
    u16 packed;
    entry->first=first;
    entry->bounds=bounds;
    entry->zoom=2.5f;
    entry->fog=2000.0f;
    D_80151AA0=2000.0f;
    D_8011EA30.scale[0]=(s16)(width*2.0f);
    D_8011EA30.translate[0]=(s16)(near_plane*2.0f);
    D_8011EA30.scale[1]=(s16)(height*2.0f);
    D_8011EA30.translate[1]=(s16)(far_plane*2.0f);
    exhaust_smoke_effect(index,horizontal,vertical,width,height,near_plane,far_plane);
    entry->range_start=1000;
    entry->range_end=999;
    entry->red=0;
    entry->green=0;
    entry->blue=0;
    entry->alpha=255;
    packed=GPACK_RGBA5551(entry->red,entry->green,entry->blue,entry->alpha>>7);
    D_80124FC8=(packed<<16)|packed;
}

typedef signed char s8; typedef signed int s32;
typedef struct ClipRect {s16 left,top,right,bottom;} ClipRect;
extern s32 D_8002AFC0,D_8002AFC4;
extern s8 D_80146204,D_8017A63C;
extern f32 D_80154188,D_80123BEC,D_80123BF0,D_80123BF4,D_80123BF8;
extern Viewport16 D_8011EA40,D_8011EA50,D_8011EA60,D_8011EA70,D_8011EA80,D_8011EA90;
extern ClipRect D_80149870,D_80149B00,D_80149B20,D_80149B40,D_80149B58,D_80149B68,D_80149B78;
s32 wheel_render_full(s32 arg0) {
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 temp_t1_4;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;

    if (arg0 > 0) {
        D_80146204 = arg0;
        if (arg0 == 1) {
            arb_rate_set(0, &D_8011EA30, &D_80149870, D_80154188, 0.0f, (f32) D_8002AFC0, (f32) D_8002AFC4, (f32) ((s32) D_8002AFC0 / 2), (f32) ((s32) D_8002AFC4 / 2));
            D_80149870.left = 0;
            D_80149870.top = 0;
            D_8017A510[0].first = &D_8011EA30;
            D_8017A510[0].bounds = &D_80149870;
            D_80149870.right = (s16) ((unsigned short *)&D_8002AFC0)[1];
            D_80149870.bottom = (s16) ((unsigned short *)&D_8002AFC4)[1];
            D_8017A63C = 0;
        } else if (arg0 == 2) {
            temp_lo = D_8002AFC0 * 3;
            arb_rate_set(0, &D_8011EA40, &D_80149B00, D_80154188, 0.0f, (f32) (temp_lo / 4), (f32) ((s32) D_8002AFC4 / 2), (f32) ((temp_lo / 8) + 2), (f32) ((s32) D_8002AFC4 / 4));
            temp_lo_2 = D_8002AFC0 * 3;
            D_80149B00.left = 2;
            D_80149B00.top = 0;
            D_8017A510[0].bounds = &D_80149B00;
            temp_t1 = temp_lo_2 / 4;
            temp_v1 = (s32) D_8002AFC4 / 2;
            D_80149B00.bottom = temp_v1 - 1;
            D_80149B00.right = temp_t1 + 2;
            D_8017A510[0].first = &D_8011EA40;
            arb_rate_set(1, &D_8011EA50, &D_80149B20, D_80154188, 0.0f, (f32) temp_t1, (f32) temp_v1, (f32) ((temp_lo_2 / 8) + 2), (f32) ((s32) (D_8002AFC4 * 3) / 4));
            D_80149B20.left = 2;
            D_80149B20.right = ((s32) (D_8002AFC0 * 3) / 4) + 2;
            D_80149B20.top = ((s32) D_8002AFC4 / 2) + 1;
            D_8017A510[1].first = &D_8011EA50;
            D_8017A510[1].bounds = &D_80149B20;
            D_80149B20.bottom = (s16) D_8002AFC4;
            D_8017A63C = 1;
        } else if ((arg0 == 3) || (arg0 == 4)) {
            arb_rate_set(0, &D_8011EA60, &D_80149B40, D_80154188 * D_80123BEC, 0.0f, (f32) ((s32) D_8002AFC0 / 2), (f32) ((s32) D_8002AFC4 / 2), (f32) (((s32) D_8002AFC0 / 4) + 1), (f32) (((s32) D_8002AFC4 / 4) + 1));
            temp_t1_2 = (s32) D_8002AFC0 / 2;
            temp_v1_2 = (s32) D_8002AFC4 / 2;
            D_80149B40.bottom = temp_v1_2 - 1;
            D_80149B40.right = temp_t1_2 - 1;
            D_80149B40.left = 0;
            D_80149B40.top = 0;
            D_8017A510[0].first = &D_8011EA60;
            D_8017A510[0].bounds = &D_80149B40;
            arb_rate_set(1, &D_8011EA70, &D_80149B58, D_80154188 * D_80123BF0, 0.0f, (f32) temp_t1_2, (f32) temp_v1_2, (f32) (((s32) (D_8002AFC0 * 3) / 4) - 2), (f32) (((s32) D_8002AFC4 / 4) + 1));
            temp_t1_3 = (s32) D_8002AFC0 / 2;
            temp_v1_3 = (s32) D_8002AFC4 / 2;
            D_80149B58.bottom = temp_v1_3 - 1;
            D_80149B58.right = (s16) D_8002AFC0;
            D_80149B58.left = temp_t1_3 + 1;
            D_80149B58.top = 0;
            D_8017A510[1].first = &D_8011EA70;
            D_8017A510[1].bounds = &D_80149B58;
            arb_rate_set(2, &D_8011EA80, &D_80149B68, D_80154188 * D_80123BF4, 0.0f, (f32) temp_t1_3, (f32) (temp_v1_3 + 2), (f32) (((s32) D_8002AFC0 / 4) + 1), (f32) (((s32) (D_8002AFC4 * 3) / 4) + 2));
            temp_v1_4 = (s32) D_8002AFC4 / 2;
            temp_t1_4 = (s32) D_8002AFC0 / 2;
            D_80149B68.bottom = (s16) D_8002AFC4;
            D_80149B68.right = temp_t1_4 - 1;
            D_80149B68.top = temp_v1_4 + 1;
            D_80149B68.left = 0;
            D_8017A510[2].first = &D_8011EA80;
            D_8017A510[2].bounds = &D_80149B68;
            arb_rate_set(3, &D_8011EA90, &D_80149B78, D_80154188 * D_80123BF8, 0.0f, (f32) temp_t1_4, (f32) (temp_v1_4 + 2), (f32) (((s32) (D_8002AFC0 * 3) / 4) - 2), (f32) (((s32) (D_8002AFC4 * 3) / 4) + 2));
            D_80149B78.top = ((s32) D_8002AFC4 / 2) + 1;
            D_80149B78.left = ((s32) D_8002AFC0 / 2) + 1;
            D_8017A510[3].first = &D_8011EA90;
            D_8017A510[3].bounds = &D_80149B78;
            D_80149B78.right = (s16) D_8002AFC0;
            D_80149B78.bottom = (s16) D_8002AFC4;
            D_8017A63C = 2;
        }
    }
    return D_8017A63C;
}
