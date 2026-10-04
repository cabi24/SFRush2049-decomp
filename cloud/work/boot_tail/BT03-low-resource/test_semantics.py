"""C89 actual-source checks under explicit synthetic external contracts."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
WORK=Path(__file__).resolve().parent
ROOT=WORK.parents[3]
HELPERS={'80014F14':('80014E64','8001671C'),'80014F80':('80014E90','80015D68'),'80014FEC':('80014EBC','80015720'),'80015058':('80014EE8','80015A0C')}
WRAPPER=r'''
static ResourceOffsets resources;
typedef struct TestRecord { unsigned int prefix[2]; u16 unknown08; u16 parameter; unsigned char payload[20]; } TestRecord;
static TestRecord records[12];
static u16 ids[12], expected_ids[12], lookup_ids[12], install_ids[12];
static int hit[12], expected_lookups, expected_installs, lookups, installs, mutation;
void *LOOKUP(u16 id, ResourceOffsets *p) {
    int i;
    i=lookups++;
    assert(i<expected_lookups && id==lookup_ids[i] && p==&resources);
    if(mutation==1) ids[i]^=0x8000;
    records[i].parameter=(u16)(records[i].parameter+1);
    return hit[i] ? &records[i] : 0;
}
int INSTALL(u16 id, void *payload EXTRA_FORMAL) {
    int i;
    i=lookups-1;
    assert(hit[i] && id==install_ids[i]);
    assert(payload==(unsigned char *)&records[i]+PAYLOAD_OFFSET);
    EXTRA_CHECK
    installs++;
    if(mutation==2) ids[i+1]=0xFFFF;
    return installs&1;
}
int main(void) {
    int n,seed,miss,mode,i;
    unsigned int count;
    typedef char parameter_offset[offsetof(TestRecord,parameter)==10?1:-1];
    (void)sizeof(parameter_offset);
    count=0;
    for(n=0;n<=8;n++) for(seed=0;seed<4;seed++) for(miss=0;miss<3;miss++) for(mode=0;mode<3;mode++) {
        for(i=0;i<12;i++) {
            ids[i]=expected_ids[i]=(u16)((seed*16381U+i*71U)%65535U);
            records[i].parameter=(u16)(seed*21845U+i);
            hit[i]=miss==0?0:(miss==1?1:i%2);
        }
        ids[n]=expected_ids[n]=0xFFFF;
        expected_lookups=expected_installs=0;
        i=0;
        while(expected_ids[i]!=0xFFFF) {
            lookup_ids[i]=expected_ids[i];
            if(mode==1) expected_ids[i]^=0x8000;
            if(hit[i]) {
                install_ids[i]=expected_ids[i];
                expected_installs++;
                if(mode==2) expected_ids[i+1]=0xFFFF;
            }
            expected_lookups++;i++;
        }
        mutation=mode;lookups=installs=0;
        FUNCTION(ids,&resources);
        assert(lookups==expected_lookups && installs==expected_installs);
        assert(memcmp(ids,expected_ids,sizeof(ids))==0);
        count++;
    }
    assert(count==324U);
    return 0;
}
'''

class Semantics(unittest.TestCase):
    def run_c(self,address,body,matching=True):
        source=ROOT/'cloud/matches/boot_tail'/('func_'+address+'.c') if matching else WORK/('func_'+address+'_NONMATCH.c')
        cc=shutil.which('cc');self.assertIsNotNone(cc)
        with tempfile.TemporaryDirectory(prefix='low-resource-host-') as t:
            p=Path(t);(p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            c=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(c.returncode,0,c.stdout+c.stderr)
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
            r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=env);self.assertEqual(r.returncode,0,r.stdout+r.stderr)
    def wrapper(self,address):
        lookup,install=HELPERS[address];extended=address=='80015058'
        b=WRAPPER.replace('LOOKUP','func_'+lookup).replace('INSTALL','func_'+install).replace('FUNCTION','func_'+address)
        b=b.replace('EXTRA_FORMAL',', u16 parameter' if extended else '').replace('EXTRA_CHECK','assert(parameter==records[i].parameter);' if extended else '').replace('PAYLOAD_OFFSET','12' if extended else '8')
        self.run_c(address,b)
    def test_resource_family_0(self):self.wrapper('80014F14')
    def test_resource_family_1(self):self.wrapper('80014F80')
    def test_resource_family_2(self):self.wrapper('80014FEC')
    def test_extended_resource(self):self.wrapper('80015058')
    def test_pop_dispatch_live_fields(self):
        self.run_c('800154A4',r'''
u8 D_8002C630; short D_80038390; ResourceGroup *D_80038398[4];
typedef struct GroupStorage { ResourceGroup header; unsigned char data[256]; } GroupStorage;
static GroupStorage storage[4];
static ResourceGroup *selected;
static int stage, mode, original_count, final_calls;
static u16 final_id;
typedef char header_offsets[offsetof(ResourceGroup,id)==4 && offsetof(ResourceGroup,type)==6 && offsetof(ResourceGroup,offsets)==8 && sizeof(ResourceGroup)==28?1:-1];
static void call_stage(u16 *p,int n) {
    assert(stage==n && p==(u16 *)((unsigned char *)selected+selected->offsets[n]+8));
    assert(D_80038390==(mode && n>=3?0:original_count-1));
    stage++;
    if(mode) {
        if(n==0) selected->offsets[1]=160;
        if(n==1) D_80038398[original_count-1]=&storage[0].header;
        if(n==2) D_80038390=0;
        if(n==3) selected->type=1;
        if(n==4) selected->id=0xBEEF;
    }
}
void func_800150C8(u16 *p){call_stage(p,0);}
void func_800152A8(u16 *p){call_stage(p,1);}
void func_80015120(u16 *p){call_stage(p,2);}
void func_80015178(u16 *p){call_stage(p,3);}
void func_800151D0(u16 *p){call_stage(p,4);}
void func_80015348(u16 id){assert(stage==5);final_calls++;final_id=id;}
int main(void) {
    int enabled,count,kind,m,i,j,result,cases;
    cases=0;
    for(enabled=0;enabled<3;enabled++) for(count=0;count<=4;count++) for(kind=0;kind<3;kind++) for(m=0;m<2;m++) {
        memset(storage,0xA5,sizeof(storage));
        for(i=0;i<4;i++) {
            D_80038398[i]=&storage[i].header;
            storage[i].header.type=kind;storage[i].header.id=0x8000+i;
            for(j=0;j<5;j++) storage[i].header.offsets[j]=32+16*j;
        }
        D_8002C630=enabled==2?255:enabled;D_80038390=count;original_count=count;
        selected=count?&storage[count-1].header:0;stage=final_calls=0;mode=m;
        result=func_800154A4();
        if(enabled && count) {
            assert(result==1 && stage==5 && final_calls==(m || kind==1));
            if(final_calls) assert(final_id==(m?0xBEEF:0x8000+count-1));
            assert(D_80038390==(m?0:count-1));
        } else {assert(result==0 && stage==0 && final_calls==0 && D_80038390==count);}
        cases++;
    }
    assert(cases==90);return 0;
}
''',False)
    def test_bound_receipts(self):
        r=json.loads((WORK/'verification.json').read_text());rows=[x for x in r['results'] if x['purpose']=='retained' and '-O2 ' in x['flags']]
        self.assertEqual(len(rows),5);self.assertEqual(sum(x['strict_match'] for x in rows),4)
        self.assertEqual(sum(x['native_bytes'] for x in rows if x['strict_match']),436)
        for x in rows:
            self.assertEqual(hashlib.sha256((ROOT/x['source_path']).read_bytes()).hexdigest(),x['source_sha256'])
            self.assertFalse(x['unresolved'] or x['unverified'] or x['errors'] or x['masked_relocations'])
            if x['strict_match']:self.assertTrue(x['relocated_full_word_equality'])

if __name__=='__main__':unittest.main()
