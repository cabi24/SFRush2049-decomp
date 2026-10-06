/* Unchanged candidate source under witnessed, bounded callback contracts. */
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#define sprintf callback_format
#include "../../../matches/ovl_a/func_803AF744.c"
#undef sprintf
MenuText *D_8017A4E4;
u8 D_803B83A0;
s8 D_803B839C;
char D_803B92B4[1];
static MenuText objects[2];
static char strings[2][8][2];
static int trace[32][10], calls, mutation, height_value, length_value;
static int token(void *p) {
    int i,j;
    if (p == D_803B92B4) return 9001;
    for (i=0;i<2;i++) for (j=0;j<8;j++) if (p==strings[i][j]) return 100+i*8+j;
    abort(); return 0;
}
static void emit(int kind,int a,int b,int c,int d,int e,int f,int g,int h) {
    int *p=trace[calls++];
    p[0]=kind;p[1]=a;p[2]=b;p[3]=c;p[4]=d;p[5]=e;p[6]=f;p[7]=g;p[8]=h;p[9]=0;
    if (calls==mutation) {
        D_803B83A0 = D_803B83A0 ? 0 : 1;
        D_803B839C = D_803B839C ? 0 : -128;
        D_8017A4E4 = &objects[1];
    }
}
void render_helper(f32 v) {emit(1,(int)v,0,0,0,0,0,0,0);}
void func_800B669C(u32 a,u32 b) {emit(2,(int)a,(int)b,0,0,0,0,0,0);}
void *object_create(s32 n) {emit(3,n,0,0,0,0,0,0,0);return 0;}
void dispatch_handler(s32 n) {emit(4,n,0,0,0,0,0,0,0);}
s32 func_800A3508(s32 n) {if(n!=0x810)abort();return (n+255)>>8;}
s32 callback_format(char *out,const char *format,...) {
    va_list ap;char *a,*b,*c,*d,*f;int e,i;
    va_start(ap,format);a=va_arg(ap,char*);b=va_arg(ap,char*);c=va_arg(ap,char*);d=va_arg(ap,char*);e=va_arg(ap,int);f=va_arg(ap,char*);va_end(ap);
    for(i=0;i<length_value;i++)out[i]=(char)('A'+i%26);
    out[length_value]=0;
    emit(5,9000,token((void*)format),token(a),token(b),token(c),token(d),e,token(f));
    return length_value;
}
void camera_auto_follow(s16 a,s16 b,s16 c,s16 d,s16 e,s16 f,void *p) {
    char *s=(char*)p;int i;
    for(i=0;i<length_value;i++)if(s[i]!=(char)('A'+i%26))abort();
    if(s[length_value]!=0)abort();
    emit(6,a,b,c,d,e,f,9000,length_value);
}
s16 object_bytes_sum_global(void) {emit(7,height_value,0,0,0,0,0,0,0);return (s16)height_value;}
void state_utility(s16 x,s16 y,void *p) {emit(8,x,y,token(p),0,0,0,0,0);}
int host_run(int enabled,int flag,int height,int hook,int length,int *out) {
    int i;
    memset(trace,0,sizeof(trace));calls=0;mutation=hook;height_value=height;length_value=length;
    for(i=0;i<2;i++) {
        objects[i].text344=strings[i][0];objects[i].text352=strings[i][1];
        objects[i].text360=strings[i][2];objects[i].text608=strings[i][3];
        objects[i].text612=strings[i][4];objects[i].text616=strings[i][5];
        objects[i].text624=strings[i][6];objects[i].text944=strings[i][7];
    }
    D_8017A4E4=&objects[0];D_803B83A0=(u8)enabled;D_803B839C=(s8)flag;
    if(func_803AF744(0x12345678)!=1)abort();
    memcpy(out,trace,sizeof(trace));return calls;
}
