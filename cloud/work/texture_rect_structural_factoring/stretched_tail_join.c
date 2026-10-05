/* Structural research control; not a matching-source claim.
 * sdk_context.h is generated from pinned SDK headers by verify.py.
 * Ordinary disjoint, nonracing domain. Full-word modular arithmetic.
 */
#include "sdk_context.h"
extern Gfx *D_80149438;
extern int D_8012E608,D_8012E60C,D_8012E610,D_8012E668,D_8012E674,D_8014A248;
void func_80087110(int x,int y,int right,int bottom,int s,int t)
{
    unsigned int height,step,offset,texture_edge;
    unsigned int tex_s = s;
    unsigned int tex_t = t;
    unsigned int ds,dt,t_fixed;
    if(x<D_8012E60C) {
        if(!(D_8012E608&4))tex_s+=(unsigned int)D_8012E60C-(unsigned int)x;
        x=D_8012E60C;
    }
    if(y<D_8012E668) {
        if(!(D_8012E608&8))tex_t+=(unsigned int)D_8012E668-(unsigned int)y;
        y=D_8012E668;
    }
    if(right>D_8012E610) {
        if(D_8012E608&4)tex_s=tex_s-D_8012E610+right;
        right=D_8012E610;
    }
    if(bottom>D_8012E674) {
        if(D_8012E608&8)tex_t=tex_t-D_8012E674+bottom;
        bottom=D_8012E674;
    }
    if(right<x || bottom<y)return;
    if(!D_8014A248) {
        if((D_8012E608&4)&&(D_8012E608&8)) {
            texture_edge=tex_s+right;tex_s=texture_edge-x;tex_t+=(unsigned int)bottom-(unsigned int)y;
            gSPTextureRectangle(D_80149438++,(unsigned int)x<<2,(unsigned int)y<<2,(unsigned int)right<<2,(unsigned int)bottom<<2,0,tex_s<<5,tex_t<<5,-4096,-1024);
        } else if(D_8012E608&4) {
            texture_edge=tex_s+right;tex_s=texture_edge-x;
            gSPTextureRectangle(D_80149438++,(unsigned int)x<<2,(unsigned int)y<<2,(unsigned int)right<<2,(unsigned int)bottom<<2,0,tex_s<<5,tex_t<<5,-4096,1024);
        } else if(D_8012E608&8) {
            tex_t+=(unsigned int)bottom-(unsigned int)y;
            gSPTextureRectangle(D_80149438++,(unsigned int)x<<2,(unsigned int)y<<2,(unsigned int)right<<2,(unsigned int)bottom<<2,0,tex_s<<5,tex_t<<5,4096,-1024);
        } else {
            gSPTextureRectangle(D_80149438++,(unsigned int)x<<2,(unsigned int)y<<2,(unsigned int)right<<2,(unsigned int)bottom<<2,0,tex_s<<5,tex_t<<5,4096,1024);
        }
    } else {
        height=(unsigned int)bottom-(unsigned int)y;
        if(D_8012E608&0x8000) {bottom=(int)((unsigned int)bottom+height+1U);step=512;offset=16;}
        else {step=1024;offset=0;}
        if((D_8012E608&4)&&(D_8012E608&8)) {
            texture_edge=tex_s+right;tex_s=texture_edge-x;tex_t+=height;
            t_fixed=(tex_t<<5)+offset;ds=0U-1024U;dt=0U-step;
        } else if(D_8012E608&4) {
            texture_edge=tex_s+right;tex_s=texture_edge-x;
            t_fixed=tex_t<<5;ds=0U-1024U;dt=step;
        } else if(D_8012E608&8) {
            tex_t+=height;
            t_fixed=(tex_t<<5)+offset;ds=1024U;dt=0U-step;
        } else {
            t_fixed=tex_t<<5;ds=1024U;dt=step;
        }
        gSPTextureRectangle(D_80149438++,(unsigned int)x<<2,(unsigned int)y<<2,
                            ((unsigned int)right+1U)<<2,((unsigned int)bottom+1U)<<2,
                            0,tex_s<<5,t_fixed,ds,dt);
    }
}
