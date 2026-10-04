"""Actual C plus a temporary consumption-tracked copy; unsafe inputs never run plain C.

Instrumentation is host-test-only and never compiled/scored/submitted as a
candidate. It initializes tracking flags, never the five real float values.
"""
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from model import NAMES, Domain, fixtures, run, bits
WORK=Path(__file__).resolve().parent
ROOT=WORK.parents[3]
SOURCE=ROOT/'cloud/matches/boot_tail/func_8001DDE0.c'
sys.path.insert(0,str(ROOT))
from tools.cloud import score


def tracked_copy(source):
    body=source[source.index('void func_8001DDE0(void)\n{'):]
    body=body.replace('void func_8001DDE0(void)','void func_8001DDE0_tracked(void)',1)
    for i,name in enumerate(NAMES):
        assert body.count('    float '+name+';')==1
        body=body.replace('    float '+name+';', '    TrackedValue '+name+'_tracked;')
        body=body.replace('&'+name, '@OUTPUT%d@'%i)
        body=re.sub(r'\b'+name+r'\b','tracked_read(&'+name+'_tracked)',body)
        body=body.replace('@OUTPUT%d@'%i,'&'+name+'_tracked.value')
    init=''.join('    %s_tracked.initialized = 0;\n    %s_tracked.writer = -1;\n    slots[%d] = &%s_tracked;\n'%(name,name,i,name) for i,name in enumerate(NAMES))
    body=body.replace('    func_8001D928();',init+'    func_8001D928();',1)
    body=body.replace('        next = emitter->next;','        next = emitter->next;\n        tracked_current = emitter;',1)
    return body


def cfloat(x):
    s=format(x,'.9g')
    if '.' not in s and 'e' not in s:s+='.0'
    return s+'f'


class Semantics(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.configs=fixtures();cls.expected=[];cls.counts={'safe':0,'undefined_output':0,'capacity':0,'defined_carry_cases':0,'consumptions':0,'carried_consumptions':0}
        for config in cls.configs:
            try:
                result=run(config);cls.expected.append((0,result));cls.counts['safe']+=1;cls.counts['consumptions']+=result['consumed'];cls.counts['carried_consumptions']+=result['carried'];cls.counts['defined_carry_cases']+=result['carried']>0
            except Domain as error:
                cls.expected.append((1 if error.kind=='undefined_output' else 2,None));cls.counts[error.kind]+=1
        cls.source_hash=hashlib.sha256(SOURCE.read_bytes()).hexdigest()

    def test_domain_examples_and_prior_iteration_carry(self):
        self.assertGreater(self.counts['safe'],0);self.assertGreater(self.counts['undefined_output'],0);self.assertGreater(self.counts['capacity'],0);self.assertGreater(self.counts['defined_carry_cases'],0)
        bylabel={c['label']:(c,e) for c,e in zip(self.configs,self.expected)}
        self.assertEqual(bylabel['large-pool-full'][1][0],0)
        self.assertEqual(bylabel['new-group-before-large-failure'][1][0],0)
        self.assertEqual(bylabel['small-pool-overflow'][1][0],2)
        self.assertEqual(bylabel['group-overflow'][1][0],2)
        # These are source-value consumption failures, not invented defaults.
        for config in [dict(nodes=[dict(flags=0x80000,identifier=0xFFFFFFFF,group=0,fade=0.)],empty=False,fail=False),dict(nodes=[dict(flags=0x20001,identifier=0xFFFFFFFF,group=0,fade=0.)],empty=True,fail=False)]:
            with self.assertRaises(Domain):run(config)

    def test_c89_actual_and_consumption_tracked_sources(self):
        inputs=[];cases=[]
        for config,(kind,result) in zip(self.configs,self.expected):
            begin=len(inputs)
            for n in config['nodes']:
                inputs.append('{%dU,%dU,%dU,%s}'%(n['flags'],n['identifier'],n['group'],cfloat(n['fade'])))
            cases.append('{%d,%d,%d,%d,%s,%d,%dU,%d,%d}'%(begin,len(config['nodes']),config['empty'],config['fail'],cfloat(config.get('volume',.5)),kind,result['hash'] if result else 0,result['consumed'] if result else 0,result['carried'] if result else 0))
        body=r'''
#include <assert.h>
#include <float.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "SOURCE"
Emitter *D_8004FD50;
const float D_8002D90C=0.25f;
typedef struct Input {u32 flags,identifier,group;float fade;} Input;
typedef struct Case {int begin,n,empty,fail;float volume;int kind;u32 hash;int consumed,carried;} Case;
static const Input inputs[]={INPUTS};
static const Case cases[]={CASES};
static Emitter objects[40];static const Case *config;static u32 groups[32],trace_hash;
static int group_count,large_count,small_count,starts,tracking,reason,consumed,carried;
static jmp_buf escape;
typedef struct TrackedValue {float value;int initialized,writer;} TrackedValue;
static TrackedValue *slots[5];static Emitter *tracked_current;
static int index_of(Emitter *em){return em?(int)(em-objects):-1;}
static u32 float_bits(float value){u32 word;memcpy(&word,&value,4);return word;}
static void hash_word(u32 word){int i;for(i=3;i>=0;i--)trace_hash=(trace_hash^((word>>(i*8))&255U))*16777619U;}
static void event(int code,int count,...){va_list a;int i;hash_word((u32)code);hash_word((u32)count);va_start(a,count);for(i=0;i<count;i++)hash_word(va_arg(a,u32));va_end(a);}
static void reject(int kind){reason=kind;longjmp(escape,1);}
static float tracked_read(TrackedValue *value){if(!value->initialized)reject(1);consumed++;if(value->writer!=index_of(tracked_current))carried++;return value->value;}
TRACKED
static void output(float *p,float value,int writer){int i;*p=value;if(tracking){for(i=0;i<5;i++)if(p==&slots[i]->value){slots[i]->initialized=1;slots[i]->writer=writer;return;}assert(0);}}
static int group_for(u32 key,int checked){int i;for(i=0;i<group_count;i++)if(groups[i]==key)return 1;if(group_count==32){if(checked)return 0;reject(2);}groups[group_count++]=key;return 1;}
void func_8001D928(void){group_count=large_count=small_count=0;event(0,0);}
void func_8001D084(Emitter *em){event(1,1,(u32)index_of(em));if(em->next)em->next->previous=em->previous;if(em->previous)em->previous->next=em->next;else D_8004FD50=em->next;em->flags&=65535U;}
void func_8001C860(Emitter *em,float *v,float *p,float *x,float *y,float *z){int i;i=index_of(em);event(2,1,(u32)i);output(v,config->empty?0.0f:config->volume+i/64.0f,i);output(p,config->empty?1.0f:1.25f+i/64.0f,i);if(!config->empty){output(x,.125f+i/128.0f,i);output(y,-.25f+i/128.0f,i);output(z,.75f-i/128.0f,i);}}
int func_8001DA74(Emitter *em,float v,float x,float y,float z,float p){event(3,6,(u32)index_of(em),float_bits(v),float_bits(x),float_bits(y),float_bits(z),float_bits(p));if(!group_for(em->group,1)||large_count==32)return 0;large_count++;return 1;}
u32 func_8001B1D0(u16 id,u8 volume,u8 pan){event(4,3,(u32)id,(u32)volume,(u32)pan);if(config->fail)return 0xFFFFFFFFU;return 0x1000U+(u32)++starts;}
u32 func_800201D0(u32 id){event(5,1,id);return id>=0x1000U&&id<0x1100U?id:0xFFFFFFFFU;}
void func_8001D944(Emitter *em,float volume){event(6,2,(u32)index_of(em),float_bits(volume));group_for(em->group,0);if(small_count==32)reject(2);small_count++;}
int func_8001B8C4(u32 id){event(7,1,id);return 0;}
void func_8001CCDC(Emitter *em,float v,float x,float y,float z,float p){event(8,7,(u32)index_of(em),em->identifier,float_bits(v),float_bits(x),float_bits(y),float_bits(z),float_bits(p));}
void func_8001DC08(void){event(9,0);}
static void reset(const Case *c,int track){int i,j;const Input *in;config=c;tracking=track;reason=consumed=carried=starts=0;group_count=large_count=small_count=0;trace_hash=2166136261U;memset(objects,0,sizeof(objects));D_8004FD50=c->n?objects:0;
for(i=0;i<c->n;i++){in=&inputs[c->begin+i];objects[i].next=i+1<c->n?&objects[i+1]:0;objects[i].previous=i?&objects[i-1]:0;objects[i].flags=in->flags;objects[i].identifier=in->identifier;objects[i].group=in->group;objects[i].sound_id=(u16)(7+i);objects[i].counter=(u16)(0x1200+i);objects[i].fade=in->fade;for(j=0;j<40;j++)objects[i].unknown0C[j]=(u8)(0x80+(i*7+j)%128);}}
static u32 snapshot(void){int i,j;u8 *b;hash_word((u32)index_of(D_8004FD50));hash_word((u32)config->n);hash_word((u32)group_count);hash_word((u32)large_count);hash_word((u32)small_count);hash_word((u32)starts);for(i=0;i<group_count;i++)hash_word(groups[i]);
for(i=0;i<config->n;i++){hash_word((u32)index_of(objects[i].next));hash_word((u32)index_of(objects[i].previous));hash_word(objects[i].flags);hash_word(objects[i].identifier);hash_word(objects[i].group);hash_word(objects[i].sound_id);hash_word(objects[i].counter);hash_word(float_bits(objects[i].fade));for(j=0;j<40;j+=4){b=&objects[i].unknown0C[j];hash_word(((u32)b[0]<<24)|((u32)b[1]<<16)|((u32)b[2]<<8)|b[3]);}}return trace_hash;}
static unsigned int current_case;static int safe_count,bad_output,bad_capacity;
int main(void){typedef char widths[sizeof(u32)==4&&sizeof(float)==4?1:-1];(void)sizeof(widths);assert(FLT_RADIX==2&&FLT_MANT_DIG==24);
for(current_case=0;current_case<sizeof(cases)/sizeof(cases[0]);current_case++){
reset(&cases[current_case],1);
if(setjmp(escape)==0){func_8001DDE0_tracked();assert(cases[current_case].kind==0);assert(consumed==cases[current_case].consumed&&carried==cases[current_case].carried);assert(snapshot()==cases[current_case].hash);safe_count++;}
else{assert(reason==cases[current_case].kind);if(reason==1)bad_output++;else if(reason==2)bad_capacity++;continue;}
reset(&cases[current_case],0);
if(setjmp(escape)==0){func_8001DDE0();assert(snapshot()==cases[current_case].hash);}else{assert(0);}
}
printf("safe=%d undefined_output=%d helper_capacity=%d\n",safe_count,bad_output,bad_capacity);return 0;}
'''
        body=body.replace('SOURCE',str(SOURCE)).replace('INPUTS',','.join(inputs)).replace('CASES',','.join(cases)).replace('TRACKED',tracked_copy(SOURCE.read_text()))
        with tempfile.TemporaryDirectory(prefix='emitter-handle-host-') as t:
            p=Path(t);(p/'test.c').write_text(body);cc=shutil.which('cc');self.assertIsNotNone(cc)
            proc=subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-ffp-contract=off','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr)
            proc=subprocess.run([str(p/'test')],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'));self.assertEqual(proc.returncode,0,proc.stdout+proc.stderr);print(proc.stdout.strip())
        self.assertEqual(hashlib.sha256(SOURCE.read_bytes()).hexdigest(),self.source_hash)

    def test_native_layout_and_source_freeze(self):
        with tempfile.TemporaryDirectory(prefix='emitter-handle-abi-') as t:
            p=Path(t);(p/'probe.c').write_text('#define O(T,m) ((unsigned int)&(((T*)0)->m))\n#include "'+str(SOURCE)+'"\ntypedef char native_layout[sizeof(Emitter)==68&&O(Emitter,flags)==8&&O(Emitter,identifier)==52&&O(Emitter,group)==56&&O(Emitter,sound_id)==60&&O(Emitter,counter)==62&&O(Emitter,fade)==64?1:-1];\n');score.compile_single(p/'probe.c',score.DEFAULT_FLAGS,p/'probe.o')
        self.assertEqual(hashlib.sha256(SOURCE.read_bytes()).hexdigest(),self.source_hash)
        source=SOURCE.read_text()
        for name in NAMES:self.assertIn('    float '+name+';',source)
        self.assertNotIn('initialized',source[source.index('void func_8001DDE0(void)\n{'):])

    def test_strict_hash_bound_receipt(self):
        r=json.loads((WORK/'verification.json').read_text());self.assertEqual(len(r['results']),2)
        selected=r['results'][0];self.assertEqual((selected['native_bytes'],selected['elf_function_bytes'],selected['object_text_bytes']),(736,736,736))
        self.assertTrue(selected['strict_match'] and selected['relocated_full_word_equality'])
        self.assertEqual((selected['differing_words'],selected['extra_words'],selected['masked_relocations']),(0,0,0))
        self.assertFalse(selected['unresolved'] or selected['unverified'] or selected['errors'])
        self.assertEqual(selected['source_sha256'],self.source_hash)
        self.assertFalse(r['results'][1]['strict_match'])

    @classmethod
    def tearDownClass(cls):print(json.dumps(cls.counts,sort_keys=True))

if __name__=='__main__':unittest.main()
