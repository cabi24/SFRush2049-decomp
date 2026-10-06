/* Host execution includes the unchanged candidate; layout assertions are separate. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
ModelBank D_801161F4[64];
static ModelRecord storage[1026];
static ModelRecord snapshot[1026];
static unsigned long read32(const unsigned char *p)
{
    return ((unsigned long)p[0]<<24)|((unsigned long)p[1]<<16)|
           ((unsigned long)p[2]<<8)|p[3];
}
static void output(unsigned int result, ModelRecord *record)
{
    unsigned char raw[92];
    unsigned int i;
    u16 value;
    raw[0]=(unsigned char)(result>>24); raw[1]=(unsigned char)(result>>16);
    raw[2]=(unsigned char)(result>>8); raw[3]=(unsigned char)result;
    for(i=0;i<44;i++) {
        memcpy(&value, (unsigned char *)record+2*i, 2);
        raw[4+2*i]=(unsigned char)(value>>8); raw[5+2*i]=(unsigned char)value;
    }
    assert(fwrite(raw,1,sizeof(raw),stdout)==sizeof(raw));
}
int main(void)
{
    unsigned char raw[100];
    unsigned int i, j, index, bank, result;
    u16 handle, value, half;
    s16 selector;
    ModelRecord *record;
    size_t length;
    assert(sizeof(RecordSpan16)==16 && sizeof(ModelRecord)==88);
    assert(offsetof(ModelRecord,spans)==8 && offsetof(RecordSpan16,tail)==14);
    for(;;) {
        length=fread(raw,1,sizeof(raw),stdin);
        if(length==0) break;
        assert(length==sizeof(raw));
        handle=(u16)read32(raw); selector=(s16)read32(raw+4); value=(u16)read32(raw+8);
        index=handle&1023; bank=handle>>10;
        memset(storage,0xA5,sizeof(storage));
        memset(D_801161F4,0,sizeof(D_801161F4));
        D_801161F4[bank].records=&storage[1];
        D_801161F4[bank].count=1024;
        record=&storage[1+index];
        for(i=0;i<44;i++) {
            half=(u16)((raw[12+2*i]<<8)|raw[13+2*i]);
            memcpy((unsigned char *)record+2*i,&half,2);
        }
        memcpy(snapshot,storage,sizeof(storage));
        for(j=0;j<2;j++) {
            result=(unsigned int)func_8008B000(handle,selector,value);
            assert(memcmp(storage,snapshot,(1+index)*88)==0);
            assert(memcmp(&storage[2+index],&snapshot[2+index],(1024-index)*88)==0);
            assert(D_801161F4[bank].records==&storage[1] && D_801161F4[bank].count==1024);
            output(result,record);
            value=(u16)~value;
        }
    }
    assert(!ferror(stdin));
    return 0;
}
