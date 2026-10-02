/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define FIELD(p,t,o) (*(t *)((unsigned char *)(p)+(o)))
typedef struct { s16 x,z,radius,near_clip,far_clip,base; } Region12;
typedef struct { unsigned char fields[60]; f32 threshold;u16 near_clip,far_clip;unsigned char tail[4]; } Descriptor72;
extern s8 D_8013F1D8,D_8014978C;
extern s16 D_8014A108;
extern s16 D_8011E810[][4],D_8011E7F0[][4];
extern f32 D_8011E7A8[][4],D_80151AA0;
extern Region12 *D_8011E76C[];
extern unsigned char D_8017A510[];
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_800AB18C(s32 index,f32 *position)
{
    void *descriptor;
    Region12 *region,*nearest;
    s16 distance,closest;
    f32 dx,dz,ratio;
    if(D_8013F1D8) {
        descriptor=D_8017A510+index*72;
        FIELD(descriptor,u16,64)=D_8011E810[D_8014A108][D_8013F1D8];
        FIELD(descriptor,u16,66)=1000;
        FIELD(descriptor,f32,60)=(f32)D_8011E7F0[D_8014A108][D_8013F1D8];
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
                if((f32)nearest->radius<=FIELD(descriptor,f32,60))
                    ratio=(f32)closest/(f32)nearest->radius;
                else {
                    if((f32)closest<FIELD(descriptor,f32,60)-(f32)nearest->base)
                        ratio=(f32)closest/(FIELD(descriptor,f32,60)-(f32)nearest->base);
                    else return;
                }
                FIELD(descriptor,u16,64)=(u32)((f32)nearest->near_clip+ratio*(f32)(FIELD(descriptor,u16,64)-nearest->near_clip));
                FIELD(descriptor,u16,66)=(u32)((f32)nearest->far_clip+ratio*(f32)(FIELD(descriptor,u16,66)-nearest->far_clip));
                if(FIELD(descriptor,u16,64)>FIELD(descriptor,u16,66)-4)
                    FIELD(descriptor,u16,64)=FIELD(descriptor,u16,66)-4;
                FIELD(descriptor,f32,60)=(f32)nearest->base+ratio*(FIELD(descriptor,f32,60)-(f32)nearest->base);
                D_80151AA0=(f32)nearest->base+ratio*(D_80151AA0-(f32)nearest->base);
            }
        }
    }
}
