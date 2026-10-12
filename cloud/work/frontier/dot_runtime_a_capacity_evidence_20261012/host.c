/* Exhaustive writer-path check of the actual new callback with service hooks.
 * The native formatter itself is inspected separately; snprintf is a host hook.
 * This does not prove full gameplay or native/helper behavioral equivalence. */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "groups/dot_runtime_a_seven_option_group_20261007/renderer.c"
#include "language_data.h"
Language countdown_state;
s32 D_803B4634, D_803B469C;
f32 D_803B4624[3];
Camera D_80150B70[1];
s16 D_8014A108;
Slot D_803B42F4[7];
char D_803B8854[]="%d %s", D_803B885C[]="%d %s";
Color4 D_801146BC, D_801146C0;
static int writes, max_bytes;
static const char *expected_string;
static void check(int ok) { if (!ok) abort(); }
void render_helper(f32 f) {}
s32 object_create(s32 n) { return 0; }
void dispatch_handler(s32 n) {}
void func_800B669C(u32 a,u32 b) {}
void brake_light_update(s32 a,f32 *b,Camera *c,void *d,s16 *p) { p[0]=160;p[1]=80; }
s16 sound_pitch_diff_halved(void *text,s16 x) { return x; }
s16 object_bytes_sum_global(void) { return 10; }
void state_utility(s16 x,s16 y,void *text) {}
s32 func_80399394(s32 i) { return 0; }
void func_800ED66C(f32 f) {}
void func_800BEA3C(Color4 a,Color4 b) {}
void fcvt_wrapper(char *text,char *format,...) {
    va_list args;
    int count, size;
    char *string;
    char expected[36];
    va_start(args,format);
    count=va_arg(args,int);string=va_arg(args,char *);
    va_end(args);
    check(count == D_8014A108 && string == expected_string);
    check(strcmp(format,"%d %s")==0);
    size=snprintf(text,36,format,count,string);
    check(size>=0 && size+1<=17);
    check(snprintf(expected,sizeof expected,"%d %s",count,expected_string)==size);
    check(memcmp(text,expected,(size_t)size+1)==0);
    if(size+1>max_bytes)max_bytes=size+1;
    ++writes;
}
int main(void) {
    static char *labels[41]={0}, *indexed[168]={0};
    static u16 indices[23]={0};
    int kind,count;
    labels[40]="title";indices[20]=167;
    countdown_state.text=labels;countdown_state.indices=indices;countdown_state.indexed=indexed;
    for(kind=0;kind<6;++kind) {
        labels[29]=native_strings[kind][0];indexed[167]=native_strings[kind][1];
        for(count=-32768;count<=32767;++count) {
            D_8014A108=(s16)count;
            expected_string=native_strings[kind][count==1?0:1];
            check(func_803A6B50(NULL)==1);
        }
    }
    check(writes==393216 && max_bytes==17);
    return 0;
}
