/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;typedef int s32;typedef short s16;typedef signed char s8;typedef unsigned char u8;typedef unsigned int u32;

#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern s32 D_801141C8;
extern f32 D_8012388C;
extern f32 fabsf(f32),sqrtf(f32);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)
extern f32 func_8008B3C8(f32*);extern void vector_copy_scale(void*,void*);

void vector_normalize_length(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f22;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f0_2;
    f32 temp_f20;
    void *temp_a0;

    M2C_FIELD(arg1, f32 *, 0x18) = (f32) M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) M2C_FIELD(arg0, f32 *, 4);
    temp_a0 = (u8 *) arg1 + 0x18;
    M2C_FIELD(arg1, f32 *, 0x20) = M2C_FIELD(arg0, f32 *, 8);
    vector_copy_scale(temp_a0, temp_a0);
    M2C_FIELD(arg1, f32 *, 4) = 0.0f;
    M2C_FIELD(arg1, f32 *, 0) = M2C_FIELD(arg0, f32 *, 8);
    M2C_FIELD(arg1, f32 *, 8) = (f32) -M2C_FIELD(arg0, f32 *, 0);
    temp_f0 = (func_8008B3C8(arg1));
    if (temp_f0 <= D_8012388C) {
        M2C_FIELD(arg1, f32 *, 0) = M2C_FIELD(&D_801141C8, f32 *, 0);
        M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(&D_801141C8, f32 *, 4);
        M2C_FIELD(arg1, f32 *, 8) = (f32) M2C_FIELD(&D_801141C8, f32 *, 8);
    } else {
        temp_f2 = 1.0f / temp_f0;
        M2C_FIELD(arg1, f32 *, 0) = (((M2C_FIELD(arg1, f32 *, 0)) * temp_f2));
        M2C_FIELD(arg1, f32 *, 4) = (f32) (M2C_FIELD(arg1, f32 *, 4) * temp_f2);
        M2C_FIELD(arg1, f32 *, 8) = (f32) (M2C_FIELD(arg1, f32 *, 8) * temp_f2);
    }
    temp_f18 = M2C_FIELD(arg1, f32 *, 0x1C);
    temp_f12 = M2C_FIELD(arg1, f32 *, 8);
    temp_f2_2 = M2C_FIELD(arg1, f32 *, 4);
    temp_f20 = M2C_FIELD(arg1, f32 *, 0x20);
    temp_f0_2 = M2C_FIELD(arg1, f32 *, 0);
    temp_f22 = M2C_FIELD(arg1, f32 *, 0x18);
    M2C_FIELD(arg1, f32 *, 0xC) = (f32) ((temp_f18 * temp_f12) - (temp_f2_2 * (temp_f20)));
    temp_f0_3 = M2C_FIELD(arg1, f32 *, 0xC);
    temp_f14 = ((temp_f20) * (temp_f0_2)) - (temp_f12 * temp_f22);
    temp_f16 = (temp_f22 * temp_f2_2) - ((temp_f0_2) * temp_f18);
    M2C_FIELD(arg1, f32 *, 0x10) = temp_f14;
    M2C_FIELD(arg1, f32 *, 0x14) = temp_f16;
    M2C_FIELD(arg1, f32 *, 0) = (((temp_f14 * (temp_f20)) - (temp_f18 * temp_f16)));
    M2C_FIELD(arg1, f32 *, 4) = (f32) ((M2C_FIELD(arg1, f32 *, 0x14) * temp_f22) - ((temp_f20) * temp_f0_3));
    M2C_FIELD(arg1, f32 *, 8) = (f32) ((temp_f0_3 * temp_f18) - (temp_f22 * M2C_FIELD(arg1, f32 *, 0x10)));
}

extern f32 D_80123888;
extern f32 func_8008B424(f32*);

f32 func_8008B3C8(f32 *v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vector_copy_scale(void *arg0, void *arg1) {
    f32 temp_f0;

    temp_f0 = func_8008B424(arg0);
    M2C_FIELD(arg1, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0) * temp_f0);
    M2C_FIELD(arg1, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 4) * temp_f0);
    M2C_FIELD(arg1, f32 *, 8) = (f32) (M2C_FIELD(arg0, f32 *, 8) * temp_f0);
}

f32 func_8008B424(f32 *v) {
 f32 sum=v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
 if(sum<D_80123888) sum=D_80123888;
 return 1.0f/sqrtf(sum);
}

extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
 f32 x=v[0],y=v[1],z=v[2],len,inv;
 len=sqrtf(x*x+y*y+z*z);
 if(len<=D_8012394C)return 0.0f;
 inv=1.0f/len;v[0]=x*inv;v[1]=y*inv;v[2]=z*inv;return len;
}
void math_utility(f32 *source,f32 *destination) {
 destination[0]=source[0];destination[1]=source[1];destination[2]=source[2];
 destination[3]=source[3];destination[4]=source[4];destination[5]=source[5];
 destination[6]=source[6];destination[7]=source[7];destination[8]=source[8];
}
void *func_8008E3C0(void *pool) {
 void *node,*head;
 node=M2C_FIELD(pool,void **,20);
 if(node) {
  M2C_FIELD(pool,void **,20)=M2C_FIELD(node,void **,0);
  M2C_FIELD(node,void **,0)=M2C_FIELD(pool,void **,16);
  if(M2C_FIELD(pool,u8 *,0)) {
   M2C_FIELD(node,s32 *,4)=0;
   head=M2C_FIELD(pool,void **,16);
   if(head)M2C_FIELD(head,void **,4)=node;
  }
  M2C_FIELD(pool,void **,16)=node;
 }
 return node;
}
