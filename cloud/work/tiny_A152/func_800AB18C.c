/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { s16 x,z,radius,near_clip,far_clip,base; } Region12;
typedef struct { unsigned char fields[60]; f32 threshold;u16 near_clip,far_clip;unsigned char tail[4]; } Descriptor72;
extern s8 D_8013F1D8,D_8014978C;
extern s16 D_8014A108;
extern s16 D_8011E810[][4],D_8011E7F0[][4];
extern f32 D_8011E7A8[][4],D_80151AA0;
extern Region12 *D_8011E76C[];
extern Descriptor72 D_8017A510[];
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_800AB18C(s32 index,f32 *position)
{
    Descriptor72 *descriptor;
    Region12 *region,*nearest;
    s16 distance,closest;
    f32 dx,dz,ratio;
    if(D_8013F1D8) {
        descriptor=&D_8017A510[index];
        descriptor->far_clip=1000;
        descriptor->near_clip=D_8011E810[D_8014A108][D_8013F1D8];
        descriptor->threshold=(f32)D_8011E7F0[D_8014A108][D_8013F1D8];
        D_80151AA0=D_8011E7A8[D_8014A108][D_8013F1D8];
        region=D_8011E76C[D_8014978C];
        if(region) {
            nearest=0;
            closest=32767;
            while(region->radius) {
                dx=(f32)region->x-position[0];
                dz=(f32)region->z-position[2];
                distance=(s16)(s32)sqrtf(dx*dx+dz*dz);
                if(distance<region->radius && distance<closest) {
                    closest=distance;
                    nearest=region;
                }
                region++;
            }
            if(nearest) {
                if((f32)nearest->radius<=descriptor->threshold)
                    ratio=(f32)closest/(f32)nearest->radius;
                else {
                    if((f32)closest<descriptor->threshold-(f32)nearest->base)
                        ratio=(f32)closest/(descriptor->threshold-(f32)nearest->base);
                    else return;
                }
                descriptor->near_clip=(u32)((f32)nearest->near_clip+ratio*(f32)(descriptor->near_clip-nearest->near_clip));
                descriptor->far_clip=(u32)((f32)nearest->far_clip+ratio*(f32)(descriptor->far_clip-nearest->far_clip));
                if(descriptor->near_clip>descriptor->far_clip-4)
                    descriptor->near_clip=descriptor->far_clip-4;
                descriptor->threshold=(f32)nearest->base+ratio*(descriptor->threshold-(f32)nearest->base);
                D_80151AA0=(f32)nearest->base+ratio*(D_80151AA0-(f32)nearest->base);
            }
        }
    }
}
