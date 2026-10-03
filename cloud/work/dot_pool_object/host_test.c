#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "func_800B3704.c"
s32 D_80149788;
PoolObject *D_80149450[200];
static PoolObject objects[200], expected;
static unsigned phase, resource, handle;
static s32 expected_x, expected_y;
static PoolObject *selected;
void func_800B362C(PoolObject *p) {
    assert(phase++==0 && p==selected);
    assert(memcmp(p,&expected,sizeof(*p))==0);
    assert(D_80149788>0);
    p->resource=(u16)resource;p->x=0x6abc;p->y=0x789a;
}
s32 func_800A79F4(u16 id,s32 a,s32 b,s32 x,s32 y,s32 c,s32 d) {
    assert(phase++==1 && id==resource && a==0 && b==0);
    assert(x==expected_x && y==expected_y && c==-1 && d==-1);
    return (s32)handle;
}
void func_80094EC8(PoolObject *p) {
    assert(phase++==2 && p==selected && (u16)p->handle==(u16)handle);
}
static uint32_t rng=0xb3704;
static uint32_t next(void) {rng=rng*1664525u+1013904223u;return rng;}
int main(void) {
    unsigned i,j;int count;u32 owner;PoolObject *result;
    assert(sizeof(PoolObject)==64 && offsetof(PoolObject,resource)==12);
    assert(offsetof(PoolObject,x)==14 && offsetof(PoolObject,y)==16);
    assert(offsetof(PoolObject,active)==27 && offsetof(PoolObject,field28)==40);
    assert(offsetof(PoolObject,handle)==52 && offsetof(PoolObject,field3c)==60);
    for(j=0;j<200;j++)D_80149450[j]=&objects[j];
    for(i=0;i<100000;i++) {
        count=(int)(next()%205);D_80149788=count;phase=0;
        selected=&objects[count<200?count:0];
        for(j=0;j<sizeof(*selected);j++)((u8*)selected)[j]=(u8)next();
        expected=*selected;owner=next();expected_x=(s32)next();expected_y=(s32)next();
        resource=next()&65535;handle=next();
        if(count<200) {
            expected.owner=owner;expected.x=(s16)expected_x;expected.y=(s16)expected_y;
            expected.field1a=0;expected.field19=0;expected.active=1;
            expected.field3c=0;expected.field28=0;expected.field2c=-1;
            expected.field30=-1;expected.field38=0;
        }
        result=func_800B3704(owner,expected_x,expected_y);
        if(count>=200) {assert(!result && phase==0 && D_80149788==count);}
        else {
            expected.resource=(u16)resource;expected.x=0x6abc;expected.y=0x789a;expected.handle=(s16)handle;
            assert(result==selected && phase==3 && D_80149788==count+1);
        }
        assert(memcmp(&expected,selected,sizeof(expected))==0);
    }
    return 0;
}
