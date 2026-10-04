"""Coherent synthetic table views; no original external contents are assumed."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from native_replay import bits, f32, reference, execute
WORK=Path(__file__).resolve().parent
ROOT=WORK.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
SOURCE=WORK/'func_8001E50C_NONMATCH.c'


def profiles():
    # C840 is512 bytes after C640: overlap is coherent in both test views.
    return [[f32(.25+i/512.) for i in range(384)],
            [f32(((i*37)%257+1)/128.) for i in range(384)]]


def inputs():
    out={}
    for note in range(256):
        for key in range(256):
            out[(note,(key<<24)|22050,48000)]=(note*256+key)%31==0
    for note in [0,1,64,127,128,254,255]:
        for key in [0,1,64,127,128,254,255]:
            for low in [0,1,0x7FFFFF,0xFFFFFF]:
                for rate in [1,2,16,64,44100,48000,2147483647]:
                    out[(note,(key<<24)|low,rate)]=True
    return sorted(out.items())


def literal(value):
    text=format(value,'.9g')
    if '.' not in text and 'e' not in text:text+='.0'
    return text+'f'


class Semantics(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        score.ASM_DIR=ROOT/'asm/us/boot_tail';cls.words=score.targets()['func_8001E50C'];cls.profiles=profiles();cls.host=[];cls.total=cls.rejected=cls.high=0
        for pool in cls.profiles:
            pb=list(map(bits,pool));host=[]
            for (note,encoded,rate),selected in inputs():
                try:expected=reference(note,encoded,pool,rate)
                except ValueError:cls.rejected+=1;continue
                result,reads=execute(cls.words,note,encoded,pb,rate)
                if result!=expected:raise AssertionError(('native/model',note,encoded,rate,result,expected))
                key=(0x40005622 if encoded==0xFFFFFFFF else encoded)>>24
                address=0x8002C640+4*(note-key) if note>key else 0x8002C840+4*(key-note)
                if reads!=(set() if note==key else {address}):raise AssertionError('native lookup view/index mismatch')
                cls.total+=1;cls.high+=expected>=0x80000000
                if selected:host.append((note,encoded,rate,expected))
            cls.host.append(host)

    def test_all_byte_pair_branches_and_conversion_domain(self):
        self.assertGreaterEqual(self.total,131072)
        self.assertGreater(self.high,0)
        self.assertGreater(self.rejected,0)

    def test_retained_ido_object_semantics(self):
        with tempfile.TemporaryDirectory(prefix='rate-native-') as t:
            obj=Path(t)/'candidate.o';score.compile_single(SOURCE,score.DEFAULT_FLAGS,obj)
            words=score.text_words(obj);relocated,masks,u,uv,e=score.relocate(obj,words,0,len(words)*4,score.image_symbols());self.assertFalse(masks or u or uv or e)
            for pool,rows in zip(self.profiles,self.host):
                pb=list(map(bits,pool))
                for note,encoded,rate,expected in rows:
                    result,_=execute(relocated,note,encoded,pb,rate)
                    self.assertEqual(result,expected)

    def test_c89_sanitizers_with_native_results(self):
        for pool,rows in zip(self.profiles,self.host):
            declarations='const float D_8002C640[256]={'+','.join(map(literal,pool[:256]))+'};\nconst float D_8002C840[256]={'+','.join(map(literal,pool[128:]))+'};\nint D_8004F800;\n'
            cases=','.join('{%d,%dU,%d,%dU}'%row for row in rows)
            c='#include <assert.h>\n#include <float.h>\n#include "'+str(SOURCE)+'"\n'+declarations+r'''
typedef struct Case {u8 note;u32 encoded;int rate;u32 expected;} Case;
static const Case cases[]={CASES};
int main(void){unsigned int i;typedef char widths[sizeof(u8)==1&&sizeof(u32)==4&&sizeof(int)==4&&sizeof(float)==4?1:-1];(void)sizeof(widths);assert(FLT_RADIX==2&&FLT_MANT_DIG==24);
for(i=0;i<sizeof(cases)/sizeof(cases[0]);i++){D_8004F800=cases[i].rate;assert(func_8001E50C(cases[i].note,cases[i].encoded)==cases[i].expected);}return 0;}
'''
            c=c.replace('CASES',cases)
            with tempfile.TemporaryDirectory(prefix='rate-host-') as t:
                p=Path(t);(p/'test.c').write_text(c);cc=shutil.which('cc');self.assertIsNotNone(cc)
                proc=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-ffp-contract=off','-fsanitize=address,undefined,float-cast-overflow','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr)
                proc=subprocess.run([str(p/'test')],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'));self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr)

    def test_native_widths_and_receipts(self):
        with tempfile.TemporaryDirectory(prefix='rate-width-') as t:
            p=Path(t);(p/'probe.c').write_text('typedef char widths[sizeof(unsigned char)==1&&sizeof(unsigned int)==4&&sizeof(int)==4&&sizeof(float)==4?1:-1];\nvoid width_probe(void) {}\n');score.compile_single(p/'probe.c',score.DEFAULT_FLAGS,p/'probe.o')
        r=json.loads((WORK/'verification.json').read_text());self.assertEqual(len(r['results']),2)
        for row in r['results']:
            self.assertEqual(hashlib.sha256((ROOT/row['source_path']).read_bytes()).hexdigest(),row['source_sha256']);self.assertFalse(row['strict_match'] or row['masked_relocations'] or row['unresolved'] or row['unverified'] or row['errors'])
        self.assertEqual((r['results'][0]['differing_words'],r['results'][0]['extra_words']),(94,1))

    @classmethod
    def tearDownClass(cls):
        print('rate fixtures: native=%d host=%d ido=%d high_u32=%d excluded_casts=%d'%(cls.total,sum(map(len,cls.host)),sum(map(len,cls.host)),cls.high,cls.rejected))

if __name__=='__main__':unittest.main()
