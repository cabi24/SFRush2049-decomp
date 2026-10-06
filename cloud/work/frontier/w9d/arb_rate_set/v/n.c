/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * arb_rate_set @ 0x800A5908: initialise a 72-byte view record and the N64
 * viewport, delegate projection setup, then install opaque black fill color.
 * Historical name is unrelated to this renderer behavior. No arcade donor
 * is present in this checkout; this is N64-native reconstruction.
 *
 * Complete NONMATCH: 312-byte ELF function, 31/78 relocated words differ.
 * Uses genuine SDK packed-color operands and integer-literal multiplication;
 * no fabricated shift work, filler declarations, or compiler-flag changes.
 * The unchanged accepted exhaust_smoke_effect source is real group context.
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
void func_800A5560(u16 color) {
    D_80124FC8 = (color << 16) | color;
}
extern Descriptor72 *exhaust_smoke_effect(int,f32,f32,f32,f32,f32,f32);
/* Exact SDK macro from reference/repos/ultralib/include/PR/gbi.h. */
#define GPACK_RGBA5551(r,g,b,a) ((((r)<<8)&0xf800)|(((g)<<3)&0x7c0)|(((b)>>2)&0x3e)|((a)&1))
static void set_color(int i, u8 r, u8 g, u8 b, u8 a) {
    D_8017A510[i].red=r;
    D_8017A510[i].green=g;
    D_8017A510[i].blue=b;
    D_8017A510[i].alpha=a;
    func_800A5560(GPACK_RGBA5551(r,g,b,1));
}
void arb_rate_set(int index,void *first,void *bounds,f32 horizontal,f32 vertical,f32 width,f32 height,f32 near_plane,f32 far_plane)
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
    set_color(index,0,0,0,255);
}
