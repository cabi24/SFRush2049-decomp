/* Host-only writer instrumentation. Actual candidate body and matched game
 * encoder/copy/append C are compiled unchanged apart from one entry observer.
 * Standard %d/%s formatting is a host hook after native formatter review;
 * this is a writer-capacity test, not a native gameplay/ABI execution proof. */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *text_start, *suffix_start;
static size_t text_capacity, suffix_capacity, maximum_text, maximum_suffix;
static unsigned long calls, writes;
static void observe_buffers(char *text, size_t tn, char *suffix, size_t sn)
{
    text_start=text; suffix_start=suffix; text_capacity=tn; suffix_capacity=sn;
    ++calls;
}
#include "candidate.h"
#include "native_helpers.h"
#include "language_data.h"

Record **D_803B9BC0;
Language D_8017A4E0;
s16 D_803B9BBC,D_803B9BC6,D_803B9BC4,D_803B9E38,D_803B9E3A,D_803B9BBA;
u8 D_803BA7EB;
char D_803B88DC[]=" ",D_803B88E0[]="%d",D_803B88E4[]=".",D_803B88E8[]="%d",D_803B88EC[]="%d",D_803B88F0[]="1%s %s %d%s%s";
static Record record, *record_pointer=&record;
static int present;
static size_t encoded_bytes(const char *s)
{
    const unsigned char *p=(const unsigned char *)s;
    size_t n;
    if(*p!=255) return strlen(s)+1;
    for(n=1;p[n] || p[n+1];n+=2) assert(n<256);
    return n+2;
}
static void check(char *destination)
{
    size_t n=encoded_bytes(destination);
    ++writes;
    if(destination==text_start) {
        assert(n<=text_capacity); if(n>maximum_text) maximum_text=n;
    } else {
        assert(destination==suffix_start && n<=suffix_capacity);
        if(n>maximum_suffix) maximum_suffix=n;
    }
}
char *func_800BE6A4(char *out,const char *in)
{
    native_copy((u8 *)out,(u8 *)in);check(out);return out;
}
char *func_800BE4F0(char *out,const char *in)
{
    native_append((u8 *)out,(u8 *)in);check(out);return out;
}
void func_800B4360(char *out,const char *format,...)
{
    va_list ap; int result; va_start(ap,format);
    result=vsnprintf(out,out==text_start?text_capacity:suffix_capacity,format,ap);
    va_end(ap); assert(result>=0 && (size_t)result<(out==text_start?text_capacity:suffix_capacity));check(out);
}
s32 func_80004990(char *out,const char *format,...)
{
    va_list ap; int result; va_start(ap,format);
    result=vsnprintf(out,text_capacity,format,ap);va_end(ap);
    assert(result>=0 && (size_t)result<text_capacity);check(out);return result;
}
Record **func_80398BF0(s32 index) { (void)index;return present?&record_pointer:0; }
void func_800B42F0(s32 font) { (void)font; }
void func_800B74A0(s32 color) { (void)color; }
s16 func_800B71A0(const char *text,s16 x) { (void)text;return x; }
void func_800B71D4(s16 x,s16 y,const char *text) { (void)x;(void)y;(void)text; }

static void select_language(int kind)
{
    static TextIndices indices;
    indices.file_action=0;
    D_8017A4E0.indices=&indices;
    D_8017A4E0.labels=native_labels[kind];
    D_8017A4E0.choices=native_choices[kind];
}
static void run_case(void) { func_803A7180(); }
int main(void)
{
    u8 name[16],extension[4];
    int language, n, e, code, position, value;
    static const u32 page_values[]={0,1,255,65535,16777215};
    size_t page;
    for(language=0;language<6;++language) {
        select_language(language);
        D_803B9BC4=16;D_803B9BC6=0;D_803B9E38=0;D_803B9E3A=0;
        D_803B9BC0=&record_pointer;
        for(value=SHRT_MIN;value<=SHRT_MAX;++value) {
            D_803B9BBC=(s16)value;D_803BA7EB=(u8)value;run_case();
        }
        D_803B9BC0=0;run_case();D_803B9BC0=&record_pointer;
        D_803B9E3A=1;present=0;run_case();present=1;
        for(n=0;n<=16;++n) for(e=0;e<=4;++e) {
            memset(name,0,sizeof name);memset(extension,0,sizeof extension);
            memset(name,66,(size_t)n);memset(extension,66,(size_t)e);
            native_encode((u8 *)record.name,name,16);
            native_encode((u8 *)record.suffix,extension,4);
            assert(encoded_bytes(record.name)<=35 && encoded_bytes(record.suffix)<=11);
            for(page=0;page<sizeof page_values/sizeof page_values[0];++page) {
                record.blocks=page_values[page];run_case();
            }
        }
        for(code=0;code<256;++code) for(position=0;position<20;++position) {
            memset(name,16,sizeof name);memset(extension,16,sizeof extension);
            if(position<16)name[position]=(u8)code;else extension[position-16]=(u8)code;
            native_encode((u8 *)record.name,name,16);
            native_encode((u8 *)record.suffix,extension,4);
            run_case();
            /* The polling producer may shorten the first encoded pair. */
            if(record.suffix[0])record.suffix[1]=0;
            run_case();
        }
        /* Both selected-row branches, wrap and zero-row branches. */
        D_803B9BC6=1;D_803B9E38=15;D_803B9E3A=2;run_case();
    }
    assert(maximum_text==45 && maximum_suffix==11);
    printf("{\"calls\":%lu,\"writes\":%lu,\"maximum_text\":%zu,\"maximum_suffix\":%zu}\n",
           calls,writes,maximum_text,maximum_suffix);
    return 0;
}
