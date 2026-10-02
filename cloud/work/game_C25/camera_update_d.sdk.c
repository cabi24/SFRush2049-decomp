/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
extern f32 D_80123AEC,D_80123AF0,D_80123AF4,D_80123AF8;
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
typedef struct C25Light {u8 col[3],pad1,colc[3],pad2;s8 dir[3];u8 padding[5];} C25Light;
typedef struct C25LookAt {C25Light l[2];} C25LookAt;
#define C25_FRAC8(x) (((x)*128.0f<127.0f)?((x)*128.0f):127.0f)
void camera_update_d(C25LookAt *l,f32 xEye,f32 yEye,f32 zEye,f32 xAt,f32 yAt,f32 zAt,f32 xUp,f32 yUp,f32 zUp) {
 f32 len,xLook,yLook,zLook,xRight,yRight,zRight;
 xLook=xAt-xEye;
 yLook=yAt-yEye;
 zLook=zAt-zEye;
 len=xLook*xLook+yLook*yLook+zLook*zLook;
 if(len<D_80123AEC) len=D_80123AF0;
 len=-(1.0f/sqrtf(len));
 xLook*=len;yLook*=len;zLook*=len;
 xRight=yUp*zLook-zUp*yLook;
 yRight=zUp*xLook-xUp*zLook;
 zRight=xUp*yLook-yUp*xLook;
 len=xRight*xRight+yRight*yRight+zRight*zRight;
 if(len<D_80123AF4)len=D_80123AF8;
 len=1.0f/sqrtf(len);
 xRight*=len;yRight*=len;zRight*=len;
 l->l[0].dir[0]=C25_FRAC8(xRight);
 l->l[0].dir[1]=C25_FRAC8(yRight);
 l->l[0].dir[2]=C25_FRAC8(zRight);
 xUp=yLook*zRight-zLook*yRight;
 yUp=zLook*xRight-xLook*zRight;
 zUp=xLook*yRight-yLook*xRight;
 l->l[1].dir[0]=C25_FRAC8(xUp);
 l->l[1].dir[1]=C25_FRAC8(yUp);
 l->l[1].dir[2]=C25_FRAC8(zUp);
 l->l[0].col[0]=0;l->l[0].col[1]=0;l->l[0].col[2]=0;l->l[0].pad1=0;
 l->l[0].colc[0]=0;l->l[0].colc[1]=0;l->l[0].colc[2]=0;l->l[0].pad2=0;
 l->l[1].col[0]=0;l->l[1].col[1]=0x80;l->l[1].col[2]=0;l->l[1].pad1=0;
 l->l[1].colc[0]=0;l->l[1].colc[1]=0x80;l->l[1].colc[2]=0;l->l[1].pad2=0;
}
