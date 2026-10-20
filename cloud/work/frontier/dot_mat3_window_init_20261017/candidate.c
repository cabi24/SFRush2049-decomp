/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* ZOID_InitializeWindow (0x800A5908, 312 bytes).
 * Restore the window-record return and the donor's narrowing-before-alpha
 * packed-color expression. Keep native clip/fog and viewport behavior.
 */
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
void func_800A5560(u16 color);
extern Descriptor72 *exhaust_smoke_effect(int,f32,f32,f32,f32,f32,f32);
/* Exact SDK macro from reference/repos/ultralib/include/PR/gbi.h. */
#define GPACK_RGBA5551(r,g,b,a) ((((r)<<8)&0xf800)|(((g)<<3)&0x7c0)|(((b)>>2)&0x3e)|((a)&1))
Descriptor72 *arb_rate_set(int index,void *first,void *bounds,f32 horizontal,f32 vertical,f32 width,f32 height,f32 near_plane,f32 far_plane)
{
    D_8017A510[index].first=first;
    D_8017A510[index].bounds=bounds;
    D_8017A510[index].zoom=2.5f;
    D_8017A510[index].fog=2000.0f;
    D_80151AA0=2000.0f;
    D_8011EA30.scale[0]=(s16)(width*2);
    D_8011EA30.translate[0]=(s16)(near_plane*2);
    D_8011EA30.scale[1]=(s16)(height*2);
    D_8011EA30.translate[1]=(s16)(far_plane*2);
    exhaust_smoke_effect(index,horizontal,vertical,width,height,near_plane,far_plane);
    D_8017A510[index].range_start=1000;
    D_8017A510[index].range_end=999;
    D_8017A510[index].red=0;
    D_8017A510[index].green=0;
    D_8017A510[index].blue=0;
    D_8017A510[index].alpha=255;
    func_800A5560((u16)(((D_8017A510[index].red<<8)&0xf800)|((D_8017A510[index].green<<3)&0x7c0)|((D_8017A510[index].blue>>2)&0x3e))|1);
    return &D_8017A510[index];
}
