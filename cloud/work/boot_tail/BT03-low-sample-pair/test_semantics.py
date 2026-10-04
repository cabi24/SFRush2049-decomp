"""Actual-source tests of the playback/descriptor contracts, not hardware safety."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]


class Semantics(unittest.TestCase):
    def run_c(self, address, body, match=False):
        source = ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c') if match else WORK / 'nonmatch' / ('func_' + address + '.c')
        with tempfile.TemporaryDirectory(prefix='bt03-sample-tests-') as tmp:
            p = Path(tmp)
            (p / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True)
            subprocess.run([str(p / 'test')], check=True, capture_output=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))

    def test_playback_fields_pointer_tags_and_unsigned_double(self):
        self.run_c('800146B4', r'''
AudioState *D_80038294;static AudioState first[4],second[4],expected[4];
static int use_second,calls;static u16 wanted_index;
static void initialize(AudioState *a,u16 flags) {
    a->active=0;a->enabled=1;a->flags=flags;a->gain=4096;
    memset(a->unknown18,0,sizeof(a->unknown18));memset(a->unknown40,0,8);
}
void func_80011C1C(u16 index,u16 flags) {
    assert(calls++==0&&index==wanted_index&&flags==0);
    if(use_second)D_80038294=second;
    initialize(&D_80038294[index],flags);
}
int main(void) {
    u32 offsets[5]={0,1,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};
    u32 lengths[4]={0,10,100,0xFFFFFFFFU};
    u32 starts[3]={0,1,20};u32 loops[4]={0,1,10,0xFFFFFFFFU};
    unsigned long tags[3]={0,0x80000100UL,0xA0000100UL};
    u8 resets[3]={0,1,255};
    SampleInfo sample,before;AudioState *e;u32 tail;
    int index,r,o,l,s,n,t,b,cases;
    cases=0;
    for(index=0;index<4;index++)for(r=0;r<3;r++)for(o=0;o<5;o++)for(l=0;l<4;l++)for(s=0;s<3;s++)for(n=0;n<4;n++)for(t=0;t<3;t++)for(b=0;b<2;b++){
        memset(first,0xA5,sizeof(first));memset(second,0xA5,sizeof(second));memset(expected,0xA5,sizeof(expected));
        memset(&sample,0x5A,sizeof(sample));sample.frequency=0x12345678U;sample.data=(void *)tags[t];sample.offset=offsets[o];sample.length=lengths[l];sample.loopStart=starts[s];sample.loopLength=loops[n];sample.format=255;
        before=sample;D_80038294=first;use_second=b;calls=0;wanted_index=(u16)index;
        e=&expected[index];initialize(e,0);
        if(resets[r]){e->ramp20=0;e->ramp22=0;e->scale24=1.0f;e->steps28=20;}
        e->data2C=sample.data;e->position08=(double)sample.offset;e->position10=e->position14=sample.offset;
        e->length30=sample.length;e->loopStart34=sample.loopStart;e->loopLength38=sample.loopLength;e->format5D=sample.format;
        tail=loops[n]?lengths[l]-loops[n]-starts[s]:0;if(tail<10)tail=0;e->tail3C=tail;
        if((tags[t]&0xF0000000UL)!=0x80000000UL)e->flags|=1;
        func_800146B4((u32)index,&sample,resets[r]);
        assert(calls==1&&D_80038294==(b?second:first)&&memcmp(D_80038294,expected,sizeof(expected))==0&&memcmp(&sample,&before,sizeof(sample))==0);
        assert(D_80038294[index].position08==(double)offsets[o]);cases++;
    }
    assert(cases==17280);return 0;
}
''', True)

    def test_search_descriptor_globals_and_live_bank_count(self):
        self.run_c('80016CF0', r'''
int D_800385A0;RegisteredSamples D_800385A8[8];u16 D_80042648;
SampleRecord *D_80042664;SamplePayload *D_80042668;
static SampleRecord records[8][3];static unsigned char bytes[8][3];
static u16 wanted_key;static int calls,change_count;static SampleRecord *old_found;
int func_80016CE0(const void *key,const void *item) {return (int)*(const u16 *)key-(int)((const SampleRecord *)item)->identifier;}
void *func_8001E864(const void *key,const void *base,int count,int size,int (*compare)(const void *,const void *)) {
    int i;SampleRecord *p;
    assert(key==&D_80042648&&D_80042648==wanted_key&&base==records[calls]&&count==3&&size==(int)sizeof(SampleRecord)&&compare==func_80016CE0);
    assert(D_80042664==(calls?0:old_found));
    if(calls==0&&change_count)D_800385A0=change_count;
    calls++;
    for(i=0;i<count;i++){p=(SampleRecord *)((unsigned char *)base+i*size);if(compare(key,p)==0)return p;}
    return 0;
}
static void prepare(int count,int pattern) {
    int b,j;D_800385A0=count;calls=change_count=0;
    for(b=0;b<8;b++){
        D_800385A8[b].records=records[b];D_800385A8[b].base=bytes[b];D_800385A8[b].count=3;
        for(j=0;j<3;j++){
            records[b][j].identifier=(u16)(0x1000+b*16+j);records[b][j].references=7;records[b][j].offset=0x87654321U;records[b][j].data=&bytes[b][j];
            records[b][j].descriptor.frequency=pattern?0xFFFFFFFFU:0;
            records[b][j].descriptor.lengthFormat=pattern?0xABCDEF12U:0x00123456U;
            records[b][j].descriptor.loopStart=pattern?0x80000000U:1;
            records[b][j].descriptor.loopLength=pattern?0xFFFFFFFFU:0;
        }
    }
    old_found=&records[7][2];D_80042664=old_found;D_80042668=&old_found->descriptor;
}
static void expected_info(SampleInfo *out,SampleRecord *record) {
    out->frequency=record->descriptor.frequency;out->data=record->data;out->offset=0;
    out->length=record->descriptor.lengthFormat&0xFFFFFFU;out->loopStart=record->descriptor.loopStart;out->loopLength=record->descriptor.loopLength;out->format=(u8)(record->descriptor.lengthFormat>>24);
}
int main(void) {
    SampleInfo output,expected;SampleRecord *found;int count,target,item,pattern,result,cases;
    cases=0;
    for(count=0;count<=8;count++)for(target=-1;target<count;target++)for(item=0;item<3;item++)for(pattern=0;pattern<2;pattern++){
        prepare(count,pattern);wanted_key=(u16)(target<0?65535:0x1000+target*16+item);
        memset(&output,0xA5,sizeof(output));expected=output;found=target<0?0:&records[target][item];
        if(found)expected_info(&expected,found);
        result=func_80016CF0(wanted_key,&output);
        assert(result==(found?0:-1)&&D_80042648==wanted_key&&memcmp(&output,&expected,sizeof(output))==0);
        assert(calls==(found?target+1:count));
        assert(D_80042664==(found?found:(count?0:old_found)));
        assert(D_80042668==(found?&found->descriptor:&old_found->descriptor));cases++;
    }
    assert(cases==270);
    prepare(1,1);wanted_key=records[1][2].identifier;change_count=2;memset(&output,0,sizeof(output));expected=output;expected_info(&expected,&records[1][2]);
    assert(func_80016CF0(wanted_key,&output)==0&&calls==2&&memcmp(&output,&expected,sizeof(output))==0);
    prepare(2,0);wanted_key=records[1][2].identifier;change_count=1;memset(&output,0xA5,sizeof(output));expected=output;
    assert(func_80016CF0(wanted_key,&output)==-1&&calls==1&&memcmp(&output,&expected,sizeof(output))==0&&D_80042664==0&&D_80042668==&old_found->descriptor);
    return 0;
}
''')


if __name__ == '__main__':
    unittest.main()
