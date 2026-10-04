"""Actual C89 source behavior under explicit, bounded external test contracts."""
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

class Semantics(unittest.TestCase):
    def run_c(self,address,body):
        source=ROOT/'cloud/matches/boot_tail'/('func_'+address+'.c')
        with tempfile.TemporaryDirectory(prefix='low-service-host-') as t:
            p=Path(t);(p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            cc=shutil.which('cc');self.assertIsNotNone(cc)
            r=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stdout+r.stderr)
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
            r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=env);self.assertEqual(r.returncode,0,r.stdout+r.stderr)

    def test_prepare_and_live_registration_list(self):
        self.run_c('80015228',r'''
static u16 ids[10];static SampleRecord records[5];static unsigned char input[16],mapped[16];
static int phase,enabled,calls,shorten,length;
void *func_80014CAC(void *p){assert(phase==0&&p==input);phase=1;return mapped;}
int func_8001605C(SampleRecord *p,void *b){assert(phase==1&&p==records&&b==mapped);phase=2;if(enabled&&length)ids[0]=0x8000;return enabled;}
int func_800162AC(u16 id,SampleRecord *p){assert(phase==2&&p==records);assert(id==ids[calls]);calls++;if(shorten)ids[calls]=0xFFFF;return calls&1;}
int main(void){int n,e,m,i;for(n=0;n<=8;n++)for(e=0;e<2;e++)for(m=0;m<2;m++){for(i=0;i<10;i++)ids[i]=(u16)(i*8191);ids[n]=0xFFFF;enabled=e;shorten=m;length=n;phase=calls=0;func_80015228(e?ids:0,input,records);assert(phase==2&&calls==(e?(m&&n?1:n):0));}return 0;}
''')

    def test_registered_sample_reference_and_hook(self):
        self.run_c('800162AC',r'''
int D_800385A0;RegisteredSamples D_800385A8[4];
static SampleRecord groups[4][5],before[4][5];static unsigned char payload[4][128];
static SampleRecord *selected;static int calls,mutate;static void *expected_data;
void func_80014CFC(void **descriptor,void **data){assert(calls==0&&*descriptor==selected->descriptor&&data==&selected->data&&*data==expected_data);calls++;*descriptor=payload[0];*data=payload[3]+100;if(mutate)selected->references=7;}
int main(void){int n,g,k,r,m,i,j;u16 refs[3];u16 key;unsigned int cases;refs[0]=0;refs[1]=1;refs[2]=65535;cases=0;
for(n=1;n<=4;n++)for(g=0;g<n;g++)for(k=0;k<5;k++)for(r=0;r<3;r++)for(m=0;m<2;m++){
 memset(groups,0,sizeof(groups));for(i=0;i<4;i++){D_800385A8[i].records=groups[i];D_800385A8[i].base=payload[i];D_800385A8[i].count=4;for(j=0;j<4;j++){groups[i][j].identifier=(u16)(0x8000+j);groups[i][j].references=refs[r];groups[i][j].offset=j*8;groups[i][j].data=payload[i]+j;}groups[i][4].identifier=0xFFFF;}
 memcpy(before,groups,sizeof(groups));D_800385A0=n;selected=k<4?&groups[g][k]:0;key=(u16)(0x8000+k);calls=0;mutate=m;expected_data=k<4?payload[g]+k*8:0;
 assert(func_800162AC(key,groups[g])==1);
 if(k<4){before[g][k].references=(u16)(refs[r]+1);if(refs[r]==0){before[g][k].data=payload[3]+100;before[g][k].references=m?8:1;}}
 assert(calls==(k<4&&r==0));assert(memcmp(before,groups,sizeof(groups))==0);cases++;
}assert(cases==300U);return 0;}
''')

    def test_resource_reset_then_hook(self):
        self.run_c('80017108',r'''
int D_800385A0,D_80038608,D_8003C610,D_8003CE18,D_80042228,D_8003DA20;ResourceRange D_8003DA28[512];static int calls;
typedef char layout[sizeof(ResourceRange)==4&&offsetof(ResourceRange,first)==2?1:-1];
void func_80014CF4(void){int i;assert(!D_800385A0&&!D_80038608&&!D_8003C610&&!D_8003CE18&&!D_80042228&&!D_8003DA20);for(i=0;i<512;i++)assert(!D_8003DA28[i].count&&!D_8003DA28[i].first);calls++;D_800385A0=7;}
int main(void){int n;for(n=0;n<3;n++){D_800385A0=D_80038608=D_8003C610=D_8003CE18=D_80042228=D_8003DA20=-1;memset(D_8003DA28,0x55+n,sizeof(D_8003DA28));calls=0;func_80017108();assert(calls==1&&D_800385A0==7);}return 0;}
''')

    def test_both_live_linked_heads(self):
        self.run_c('8001734C',r'''
static ContextPrefix context;static Entry first[3],second[3],replacement[2];static u32 log_ids[8];static int calls,mutate;
int func_8001FA18(u32 id){log_ids[calls++]=id;if(mutate&&calls==1){first[0].next=&first[2];context.headF7C=replacement;}return calls&1?0:-1;}
int main(void){int a,b,m,i,n;u32 expected[8];for(a=0;a<=3;a++)for(b=0;b<=3;b++)for(m=0;m<2;m++){
 memset(&context,0,sizeof(context));for(i=0;i<3;i++){first[i].identifier=10+i;second[i].identifier=20+i;first[i].next=i<a-1?&first[i+1]:0;second[i].next=i<b-1?&second[i+1]:0;}replacement[0].identifier=30;replacement[0].next=&replacement[1];replacement[1].identifier=31;replacement[1].next=0;
 context.headF78=a?first:0;context.headF7C=b?second:0;calls=0;mutate=m&&a==3;n=0;
 for(i=0;i<a;i++)if(!(mutate&&i==1))expected[n++]=10+i;
 if(mutate){expected[n++]=30;expected[n++]=31;}else for(i=0;i<b;i++)expected[n++]=20+i;
 func_8001734C(&context);assert(calls==n);for(i=0;i<n;i++)assert(log_ids[i]==expected[i]);
}return 0;}
''')

    def test_eight_record_lookup_and_flag(self):
        self.run_c('80017644',r'''
VoiceRecord D_80043EB8[8];
typedef char layout[sizeof(VoiceRecord)==4088&&offsetof(VoiceRecord,identifier)==0&&offsetof(VoiceRecord,inactiveFC1)==4033?1:-1];
int main(void){u32 values[5],query,want;int value,slot,flag,inactive,duplicate,i;unsigned int cases;values[0]=0;values[1]=1;values[2]=0x12345678U;values[3]=0x7FFFFFFEU;values[4]=0x7FFFFFFFU;cases=0;
for(value=0;value<5;value++)for(slot=-1;slot<8;slot++)for(flag=0;flag<2;flag++)for(inactive=0;inactive<3;inactive++)for(duplicate=0;duplicate<2;duplicate++){
 memset(D_80043EB8,0,sizeof(D_80043EB8));for(i=0;i<8;i++){D_80043EB8[i].identifier=0x80000000U|values[value];D_80043EB8[i].inactiveFC1=0;}
 if(slot>=0){D_80043EB8[slot].identifier=values[value];D_80043EB8[slot].inactiveFC1=inactive==2?255:inactive;if(duplicate)D_80043EB8[0].identifier=values[value];}
 query=values[value]|(flag?0x80000000U:0);want=0xFFFFFFFFU;for(i=0;i<8;i++)if(!D_80043EB8[i].inactiveFC1&&D_80043EB8[i].identifier==values[value]){want=(flag?0x80000000U:0)|i;break;}
 assert(func_80017644(query)==want);cases++;
}assert(cases==540U);return 0;}
''')

    def test_source_hashes_and_native_proofs(self):
        r=json.loads((WORK/'verification.json').read_text());rows=[x for x in r['results'] if x['purpose']=='retained' and '-O2 ' in x['flags']]
        self.assertEqual(len(rows),5);self.assertEqual(sum(x['native_bytes'] for x in rows),876)
        for x in rows:
            self.assertTrue(x['strict_match'] and x['relocated_full_word_equality']);self.assertEqual((x['differing_words'],x['extra_words']),(0,0));self.assertFalse(x['unresolved'] or x['unverified'] or x['errors'] or x['masked_relocations']);self.assertEqual(hashlib.sha256((ROOT/x['source_path']).read_bytes()).hexdigest(),x['source_sha256'])

if __name__=='__main__':unittest.main()
