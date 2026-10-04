"""Insertion semantics: canonical native, list model, host C89 and IDO controls."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from fixtures import COUNT, TABLE, CAPACITY, RANGES, PAYLOAD, NEW_PAYLOAD, PROFILES, initial, encode, fingerprint, model, cases, native_hook
from native_replay import execute
WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
FUNCTIONS = (0x80015A0C, 0x80016998)


class Semantics(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        score.ASM_DIR = ROOT / 'asm/us/boot_tail'
        cls.targets = score.targets()
        cls.expected = {}
        for function in FUNCTIONS:
            cls.expected[function] = []
            for profile, key, mode, parameter in cases(function):
                result, state = model(function, profile, key, mode, parameter)
                expected = encode(function, state)
                run = execute(cls.targets['func_%08X' % function], function, encode(function, initial(function, profile)),
                              0xABCD0000 | key, NEW_PAYLOAD, native_hook(function, key, mode), parameter=0xABCD0000 | parameter)
                if run['result'] != result or run['regions'] != expected or run['uninitialized_stack_reads']:
                    raise AssertionError(('native/model disagreement', hex(function), profile, key, mode, run['result'], result))
                if run['calls'] != [0x80014594, 0x800145DC]:
                    raise AssertionError('unexpected hook order')
                cls.expected[function].append((profile, key, mode, parameter, result, fingerprint(expected)))

    def test_all_native_model_cases(self):
        self.assertEqual([len(self.expected[f]) for f in FUNCTIONS], [672, 360])

    def test_retained_and_count_control_ido_replays(self):
        for function in FUNCTIONS:
            name = 'func_%08X' % function
            paths = [WORK / (name + '_NONMATCH.c')]
            if function == 0x80016998:
                paths.append(WORK / 'controls' / (name + '_cached_count.c'))
            for source in paths:
                with tempfile.TemporaryDirectory(prefix='insert-native-') as t:
                    obj=Path(t)/'candidate.o';score.compile_single(source,score.DEFAULT_FLAGS,obj)
                    words=score.text_words(obj);relocated,masks,unresolved,unverified,errors=score.relocate(obj,words,0,len(words)*4,score.image_symbols())
                    self.assertFalse(masks or unresolved or unverified or errors)
                    for profile,key,mode,parameter,result,expected in self.expected[function]:
                        run=execute(relocated,function,encode(function,initial(function,profile)),0xABCD0000|key,NEW_PAYLOAD,native_hook(function,key,mode),parameter=0xABCD0000|parameter)
                        self.assertEqual((run['result'],fingerprint(run['regions'])),(result,expected),(source.name,profile,key,mode))
                        self.assertFalse(run['uninitialized_stack_reads'])

    def test_c89_sanitizers(self):
        for function in FUNCTIONS:
            bucket = function == 0x80016998
            capacity = CAPACITY[function]
            name = 'func_%08X' % function
            source = WORK / (name + '_NONMATCH.c')
            count, table = ('D_8003DA20','D_8003E228') if bucket else ('D_8003CE18','D_8003CE20')
            declarations='int '+count+';ResourceEntry '+table+'[%d];\n'%capacity
            if bucket: declarations+='ResourceRange D_8003DA28[512];\n'
            rows=','.join('{%d,%d,%d,%d,%d,%dU}'%row for row in self.expected[function])
            profile_code='switch(profile){\n'
            for pi,(n,groups) in enumerate(PROFILES):
                if bucket and pi<7 or not bucket and pi>=7:continue
                profile_code+='case %d:\n'%pi
                if bucket:
                    cursor=0
                    for group,length in groups:
                        profile_code+='D_8003DA28[%d].first=%d;D_8003DA28[%d].count=%d;\n'%(group,cursor,group,length)
                        step=2 if pi==11 and group==0 else 1
                        profile_code+='for(i=0;i<%d;i++){%s[%d+i].id=(u16)(%d+i*%d);%s[%d+i].references=reference_values[i%%4];}\n'%(length,table,cursor,group*64,step,table,cursor)
                        cursor+=length
                    profile_code+=count+'=%d;break;\n'%cursor
                else:
                    profile_code+=count+'=%d;for(i=0;i<%d;i++){%s[i].id=(u16)(2*i+2);%s[i].references=(u16)(i%%3==0?65535:100+i);}break;\n'%(n,n,table,table)
            profile_code+='default:assert(0);}\n'
            reset='memset(D_8003DA28,0,sizeof(D_8003DA28));' if bucket else ''
            hash_ranges='for(i=0;i<512;i++){hash_value(D_8003DA28[i].count,2);hash_value(D_8003DA28[i].first,2);}' if bucket else ''
            hash_parameter='' if bucket else 'hash_value(TABLE[i].parameter,2);'
            hash_tail='' if bucket else 'hash_value(TABLE[i].unknown0A[0],1);hash_value(TABLE[i].unknown0A[1],1);'
            init_extra='' if bucket else 'TABLE[i].parameter=(u16)(0xB000+i);TABLE[i].unknown0A[0]=(unsigned char)((0xD000+i)>>8);TABLE[i].unknown0A[1]=(unsigned char)(0xD000+i);'
            c='#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+declarations+r'''
static const u16 reference_values[4]={1,0,2,65535};
static unsigned char payloads[2049];static int phase,mode;static u16 requested;static unsigned int digest;
static void hash_value(unsigned int value,int bytes){int i;for(i=bytes-1;i>=0;i--)digest=(digest^((value>>(i*8))&255U))*16777619U;}
void func_80014594(void){assert(phase==0);phase=1;if(mode==1){COUNT=0;RESET}}
void func_800145DC(void){int i;assert(phase==1);phase=2;if(mode==2)for(i=0;i<COUNT;i++)if(TABLE[i].id==requested){TABLE[i].references=500;break;}}
static unsigned int snapshot(void){int i;unsigned int pointer;digest=2166136261U;hash_value((unsigned int)COUNT,4);HASHRANGES
for(i=0;i<CAPACITY;i++){pointer=0x600000U+(unsigned int)((unsigned char *)TABLE[i].payload-payloads)*4U;hash_value(pointer,4);hash_value(TABLE[i].id,2);HASHPARAMETER hash_value(TABLE[i].references,2);HASHTAIL}return digest;}
typedef struct Case {int profile;u16 key;int mode;u16 parameter;int result;unsigned int hash;} Case;
static const Case cases[]={ROWS};
int main(void){unsigned int c;int i,profile,result;typedef char widths[sizeof(unsigned int)==4&&sizeof(u16)==2?1:-1];(void)sizeof(widths);(void)reference_values;
for(c=0;c<sizeof(cases)/sizeof(cases[0]);c++){for(i=0;i<CAPACITY;i++){TABLE[i].payload=&payloads[i];TABLE[i].id=(u16)(0xA500+i%256);TABLE[i].references=0xCAFE;INITEXTRA}RESET
profile=cases[c].profile;PROFILE
phase=0;mode=cases[c].mode;requested=cases[c].key;result=FUNCTION_CALL;assert(phase==2&&result==cases[c].result&&snapshot()==cases[c].hash);}return 0;}
'''
            c=c.replace('HASHPARAMETER',hash_parameter).replace('HASHTAIL',hash_tail).replace('INITEXTRA',init_extra).replace('CAPACITY',str(capacity)).replace('FUNCTION_CALL',name+'(requested)' if bucket else name+'(requested,&payloads[2048],cases[c].parameter)')
            c=c.replace('COUNT',count).replace('TABLE',table).replace('RESET',reset).replace('HASHRANGES',hash_ranges).replace('ROWS',rows).replace('PROFILE',profile_code).replace('FUNCTION',name)
            with tempfile.TemporaryDirectory(prefix='insert-host-') as t:
                p=Path(t);(p/'test.c').write_text(c)
                cc=shutil.which('cc');self.assertIsNotNone(cc)
                proc=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
                self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr)
                proc=subprocess.run([str(p/'test')],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
                self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr)

    def test_native_layout_and_receipt_hashes(self):
        for function in FUNCTIONS:
            source=WORK/('func_%08X_NONMATCH.c'%function)
            checks='sizeof(void*)==4&&sizeof(ResourceEntry)==%d&&offsetof(ResourceEntry,id)==4&&offsetof(ResourceEntry,references)==%d'%((8,6) if function==0x80016998 else (12,8))
            if function==0x80015A0C:checks+='&&offsetof(ResourceEntry,parameter)==6&&offsetof(ResourceEntry,unknown0A)==10'
            if function==0x80016998:checks+='&&sizeof(ResourceRange)==4&&offsetof(ResourceRange,first)==2'
            with tempfile.TemporaryDirectory(prefix='insert-abi-') as t:
                p=Path(t);(p/'probe.c').write_text('#define offsetof(T, m) ((unsigned int)&(((T *)0)->m))\n#include "'+str(source)+'"\ntypedef char native_check[('+checks+')?1:-1];\n');score.compile_single(p/'probe.c',score.DEFAULT_FLAGS,p/'probe.o')
        receipt=json.loads((WORK/'verification.json').read_text())
        for row in receipt['results']:
            self.assertEqual(hashlib.sha256((ROOT/row['source_path']).read_bytes()).hexdigest(),row['source_sha256']);self.assertFalse(row['strict_match'])

if __name__=='__main__':unittest.main()
