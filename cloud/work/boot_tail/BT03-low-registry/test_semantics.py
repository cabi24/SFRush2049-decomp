"""Bounded actual-source C89 behavior; registry pointer layouts are N64-specific."""
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
PUSH=r'''
u8 D_8002C630;short D_80038390;ResourceGroup *D_80038398[128];
typedef struct GroupStorage {ResourceGroup header;u8 data[160];} GroupStorage;
static GroupStorage groups[3];static ResourceOffsets resources;static SampleRecord samples[2];static u8 address[8];
static ResourceGroup *selected;static int stage,final_calls,mode,initial_count;static u16 requested;
typedef char group_layout[sizeof(ResourceGroup)==32&&offsetof(ResourceGroup,offsets)==8?1:-1];
typedef char host_address_word[sizeof(unsigned long)==sizeof(void*)?1:-1];
static void step(u16 *p,int n){assert(stage==n&&p==(u16 *)((u8 *)selected+selected->offsets[n]+8));if(n==0)assert(D_80038398[initial_count]==selected);stage++;if(mode){if(n==0){selected->offsets[1]=128;selected->identifier=0x1234;}if(n==1)D_80038398[initial_count]=&groups[2].header;if(n==2)D_80038390=5;if(n==4){selected->type=1;selected->offsets[5]=144;}}}
void func_80014F14(u16 *p,ResourceOffsets *r){assert(r==&resources);step(p,0);}
void func_80015228(u16 *p,void *a,SampleRecord *s){assert(a==address&&s==samples);step(p,1);}
void func_80014F80(u16 *p,ResourceOffsets *r){assert(r==&resources);step(p,2);}
void func_80014FEC(u16 *p,ResourceOffsets *r){assert(r==&resources);step(p,3);}
void func_80015058(u16 *p,ResourceOffsets *r){assert(r==&resources);step(p,4);}
void func_80015318(u16 id,u16 *p){assert(stage==5&&id==requested&&p==(u16 *)((u8 *)selected+selected->offsets[5]+8));final_calls++;}
int main(void){int enabled,count,key,kind,m,i,j,result,cases;cases=0;
for(enabled=0;enabled<2;enabled++)for(count=0;count<3;count++)for(key=0;key<3;key++)for(kind=0;kind<3;kind++)for(m=0;m<2;m++){
 memset(groups,0,sizeof(groups));memset(D_80038398,0,sizeof(D_80038398));for(i=0;i<3;i++){groups[i].header.next_offset=i<2?sizeof(GroupStorage):0xFFFFFFFFU;groups[i].header.identifier=(u16)(0x8000+i);groups[i].header.type=kind;for(j=0;j<6;j++)groups[i].header.offsets[j]=32+j*16;}
 D_8002C630=enabled;D_80038390=count;initial_count=count;requested=(u16)(0x8000+key);selected=&groups[key].header;stage=final_calls=0;mode=m;
 result=func_8001536C(&groups[0].header,requested,address,samples,&resources);
 if(enabled&&key<2){assert(result==1&&stage==5&&final_calls==(m||kind==1)&&D_80038390==(m?6:count+1));assert(D_80038398[count]==(m?&groups[2].header:selected));}else assert(!result&&!stage&&!final_calls&&D_80038390==count);
 cases++;
}
D_8002C630=0;D_80038390=0;assert(func_8001536C(0,0,0,0,0)==0);
D_8002C630=1;D_80038390=128;assert(func_8001536C(0,0,0,0,0)==0);
assert(cases==108);return 0;}
'''

class Semantics(unittest.TestCase):
    def run_c(self,address,body,control=None):
        source=WORK/('func_'+address+'_NONMATCH.c') if control is None else WORK/'controls'/control
        with tempfile.TemporaryDirectory(prefix='low-registry-host-') as t:
            p=Path(t);(p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            cc=shutil.which('cc');self.assertIsNotNone(cc)
            r=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stdout+r.stderr)
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1');r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=env);self.assertEqual(r.returncode,0,r.stdout+r.stderr)
    def test_resource_push_live_fields(self):self.run_c('8001536C',PUSH)
    def test_rejected_address_word_domain(self):self.run_c('8001536C',PUSH,'func_8001536C_address_word.c')
    def test_registry_preparation(self):
        self.run_c('8001605C',r'''
int D_800385A0;RegisteredSamples D_800385A8[8];
static SampleRecord old[8][2],fresh[6];static u8 memory[16];static int phase,mode,initial_count,length;
void func_80014594(void){assert(phase==0);phase=1;if(mode==1&&initial_count<7){D_800385A8[initial_count].records=old[initial_count];D_800385A0++;}if(mode==2&&length>1)fresh[1].identifier=0xFFFF;}
void func_800145DC(void){assert(phase==1);phase=2;}
int main(void){int n,existing,len,m,i,result,slot,expected_refs,cases;SampleRecord *input;cases=0;
for(n=0;n<=8;n++)for(existing=-1;existing<n;existing++)for(len=0;len<=4;len++)for(m=0;m<3;m++){
 memset(D_800385A8,0,sizeof(D_800385A8));memset(fresh,0,sizeof(fresh));for(i=0;i<8;i++){D_800385A8[i].records=i<n?old[i]:0;D_800385A8[i].base=memory+i;D_800385A8[i].count=77;D_800385A8[i].unknown0A=0xA5A5;}
 for(i=0;i<6;i++){fresh[i].identifier=(u16)(0x8000+i);fresh[i].references=0x1234;}fresh[len].identifier=0xFFFF;
 D_800385A0=n;initial_count=n;mode=m;length=len;phase=0;input=existing>=0?old[existing]:fresh;result=func_8001605C(input,memory);
 if(existing>=0){assert(result==1&&phase==0&&D_800385A0==n);}else if(n==8){assert(result==0&&phase==0&&D_800385A0==8);}else{slot=n+(m==1&&n<7);assert(result==1&&phase==2&&D_800385A0==slot+1);assert(D_800385A8[slot].records==fresh&&D_800385A8[slot].base==memory&&D_800385A8[slot].count==len&&D_800385A8[slot].unknown0A==0xA5A5);}
 for(i=0;i<6;i++){expected_refs=existing<0&&n<8&&i<len?0:0x1234;assert(fresh[i].references==expected_refs);}
 cases++;
}assert(cases==675);return 0;}
''')
    def test_registry_removal_and_live_limit(self):
        self.run_c('800161A0',r'''
int D_800385A0;RegisteredSamples D_800385A8[8];
static SampleRecord records[9];static u8 memory[16];static int phase,mode,initial_count;
void func_80014594(void){assert(phase==0);phase=1;if(mode&&initial_count<8){D_800385A8[initial_count].records=&records[8];D_800385A8[initial_count].base=memory+8;D_800385A8[initial_count].count=88;D_800385A8[initial_count].unknown0A=0x9999;D_800385A0++;}}
void func_800145DC(void){assert(phase==1);phase=2;}
int main(void){RegisteredSamples expected[8];SampleRecord *input;int n,found,m,i,j,current,result,cases;cases=0;
for(n=0;n<=8;n++)for(found=-1;found<n;found++)for(m=0;m<2;m++){
 memset(D_800385A8,0,sizeof(D_800385A8));for(i=0;i<8;i++){D_800385A8[i].records=&records[i];D_800385A8[i].base=memory+i;D_800385A8[i].count=(u16)(10+i);D_800385A8[i].unknown0A=(u16)(0xA500+i);}memcpy(expected,D_800385A8,sizeof(expected));
 D_800385A0=n;initial_count=n;mode=m;phase=0;input=found<0?&records[8]:&records[found];current=n;
 if(found>=0){if(m&&n<8){expected[n].records=&records[8];expected[n].base=memory+8;expected[n].count=88;expected[n].unknown0A=0x9999;current++;}for(j=found+1;j<current;j++)expected[j-1]=expected[j];current--;}
 result=func_800161A0(input);assert(result==(found>=0)&&phase==(found>=0?2:0)&&D_800385A0==current);
 for(i=0;i<8;i++){assert(D_800385A8[i].records==expected[i].records&&D_800385A8[i].base==expected[i].base&&D_800385A8[i].count==expected[i].count&&D_800385A8[i].unknown0A==expected[i].unknown0A);}cases++;
}assert(cases==90);return 0;}
''')
    def test_native_width_probes_and_receipts(self):
        import sys
        sys.path.insert(0,str(ROOT))
        from tools.cloud import score
        with tempfile.TemporaryDirectory(prefix='registry-abi-') as t:
            p=Path(t)
            for file,checks in [('controls/func_8001536C_address_word.c','sizeof(unsigned long)==4 && sizeof(void*)==4 && sizeof(ResourceGroup)==32'),('func_8001605C_NONMATCH.c','sizeof(SampleRecord)==28 && sizeof(RegisteredSamples)==12')]:
                source=p/'probe.c';source.write_text('#include "'+str(WORK/file)+'"\ntypedef char abi_check[('+checks+') ? 1 : -1];\n');score.compile_single(source,'-g0 -O2 -mips2 -G 0 -non_shared',p/'probe.o')
        r=json.loads((WORK/'verification.json').read_text());rows=[x for x in r['results'] if x['purpose']=='retained' and '-O2 ' in x['flags']]
        self.assertEqual(len(rows),3);self.assertEqual(sum(x['native_bytes'] for x in rows),904)
        for x in rows:
            self.assertFalse(x['strict_match']);self.assertFalse(x['unresolved'] or x['unverified'] or x['errors'] or x['masked_relocations']);self.assertEqual(hashlib.sha256((ROOT/x['source_path']).read_bytes()).hexdigest(),x['source_sha256'])

if __name__=='__main__':unittest.main()
