"""C89 numeric tests use synthetic extern data, never original scalar/table values."""
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
MATCHES={'8001E688','8001E940'}

class Semantics(unittest.TestCase):
    def run_c(self,address,body):
        source=ROOT/'cloud/matches/boot_tail'/('func_'+address+'.c') if address in MATCHES else WORK/('func_'+address+'_NONMATCH.c')
        with tempfile.TemporaryDirectory(prefix='c11-numeric-host-') as t:
            p=Path(t);(p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include <float.h>\n#include "'+str(source)+'"\n'+body)
            cc=shutil.which('cc');self.assertIsNotNone(cc)
            r=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined,float-cast-overflow','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stdout+r.stderr)
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1');r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=env);self.assertEqual(r.returncode,0,r.stdout+r.stderr)

    def test_external_scale_with_synthetic_values(self):
        for coefficient in ['0.0f','0.5f','1.0f','17.5f','1000.0f','65536.0f']:
            self.run_c('8001E440','const float D_8002D924='+coefficient+';\n'+r'''
int main(void){unsigned int i,expected;float rounded;double product;typedef char widths[sizeof(float)==4&&sizeof(double)==8&&sizeof(u32)==4&&sizeof(u16)==2?1:-1];(void)sizeof(widths);assert(FLT_RADIX==2&&FLT_MANT_DIG==24&&DBL_MANT_DIG==53);
for(i=0;i<65536U;i++){product=(double)i*(double)D_8002D924;rounded=(float)product;assert((double)rounded>=0.0&&(double)rounded<4294967296.0);expected=(u32)rounded;assert(func_8001E440((u16)i)==(u16)expected);}return 0;}
''')

    def test_double_word_roundtrip_valid_domain(self):
        self.run_c('8001E688',r'''
int main(void){unsigned int i,whole;double value,result;double inputs[9]={-0.75,-0.0,0.0,0.9999,1.1,2.0,2147483647.75,2147483648.75,4294967295.75};unsigned int expected[9]={0,0,0,0,1,2,2147483647U,2147483648U,4294967295U};int j;typedef char widths[sizeof(unsigned int)==4&&sizeof(double)==8?1:-1];(void)sizeof(widths);
for(i=0;i<65536U;i++){whole=i*65537U;value=(double)whole+0.75;assert(func_8001E688(value)==(double)whole);}
for(j=0;j<9;j++){result=func_8001E688(inputs[j]);assert(result==(double)expected[j]);}return 0;}
''')

    def test_quadrants_with_synthetic_table(self):
        # Formula-generated signed samples exercise lookup/negation only; they are
        # not the game's table and are never linked into a matching submission.
        values=[-32768 if i==0 else (32767 if i==1023 else ((i*173)%65536)-32768) for i in range(1024)]
        declaration='const short D_8002CC54[1024]={'+','.join(map(str,values))+'};\n'
        self.run_c('8001E7BC',declaration+r'''
int main(void){unsigned int phase,q,index;u16 bits;typedef char width[sizeof(short)==2?1:-1];(void)sizeof(width);
for(phase=0;phase<65536U;phase++){q=(phase&4095U)/1024U;index=phase&1023U;if(q&1U)index=1023U-index;bits=(u16)D_8002CC54[index];if(q>=2U)bits=(u16)(0U-bits);assert((u16)func_8001E7BC((u16)phase)==bits);}return 0;}
''')

    def test_callback_search(self):
        self.run_c('8001E864',r'''
typedef struct Record {int key;int payload[3];} Record;
static Record records[64];static int key,calls,length;
static int compare(const void *k,const void *r){const Record *record;record=r;assert(k==&key&&record>=records&&record<records+length);calls++;return *(const int *)k-record->key;}
int main(void){int n,k,i;void *expected;for(i=0;i<64;i++){records[i].key=i*2;records[i].payload[0]=i;}
for(n=0;n<=64;n++)for(k=-1;k<=129;k++){length=n;key=k;calls=0;expected=k>=0&&k%2==0&&k/2<n?(void *)&records[k/2]:0;assert(func_8001E864(&key,records,n,sizeof(Record),compare)==expected);assert(calls<=7);}
for(i=0;i<8;i++){records[i].key=7;}length=8;key=7;calls=0;assert(func_8001E864(&key,records,8,sizeof(Record),compare)==&records[3]&&calls==1);assert(func_8001E864(0,0,0,0,0)==0);return 0;}
''')

    def test_time_conversion_after_real_pointer_call(self):
        self.run_c('8001E940',r'''
static unsigned char state[80];static u32 rate,*current;static int calls,mutate;
u32 func_80019AA8(unsigned char *p){assert(p==state);calls++;if(mutate)*current^=0xA5A5A5A5U;return rate;}
int main(void){u32 values[9]={0,1,32767,65535,65536,0x12345678U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};u32 rates[8]={1,2,3,31,32,1000,65535,0xFFFFFFFFU};u32 value,input,expected;unsigned long wide;int i,j,m;typedef char width[sizeof(unsigned long)>=8&&sizeof(u32)==4?1:-1];(void)sizeof(width);
memset(state,0,sizeof(state));state[75]=255;for(i=0;i<9;i++)for(j=0;j<8;j++)for(m=0;m<2;m++){value=values[i];current=&value;rate=rates[j];mutate=m;calls=0;input=m?values[i]^0xA5A5A5A5U:values[i];wide=((unsigned long)input<<16)&0xFFFFFFFFUL;wide/=rate;wide=(wide*1000UL)&0xFFFFFFFFUL;expected=(u32)(wide>>5);func_8001E940(&value,state);assert(calls==1&&value==expected&&state[75]==255);}return 0;}
''')

    def test_native_widths_and_receipt_hashes(self):
        import sys
        sys.path.insert(0,str(ROOT))
        from tools.cloud import score
        with tempfile.TemporaryDirectory(prefix='numeric-abi-') as t:
            p=Path(t);(p/'probe.c').write_text('typedef char abi_check[sizeof(unsigned int)==4&&sizeof(unsigned short)==2&&sizeof(float)==4&&sizeof(double)==8?1:-1];\nvoid width_probe(void) {}\n');score.compile_single(p/'probe.c','-g0 -O2 -mips2 -G 0 -non_shared',p/'probe.o')
        receipt=json.loads((WORK/'verification.json').read_text());rows=[r for r in receipt['results'] if r['purpose']=='retained' and '-O2 ' in r['flags']]
        self.assertEqual(len(rows),5);self.assertEqual(sum(r['native_bytes'] for r in rows if r['strict_match']),268)
        for r in rows:
            self.assertFalse(r['unresolved'] or r['unverified'] or r['errors'] or r['masked_relocations']);self.assertEqual(hashlib.sha256((ROOT/r['source_path']).read_bytes()).hexdigest(),r['source_sha256'])
            if r['strict_match']:self.assertTrue(r['relocated_full_word_equality']);self.assertEqual((r['differing_words'],r['extra_words']),(0,0))

if __name__=='__main__':unittest.main()
