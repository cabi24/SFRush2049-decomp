#!/usr/bin/env python3
"""Replay the independent review fixture against unchanged actual sources."""
from pathlib import Path
import subprocess,os,hashlib,json,tempfile
p=Path(__file__).resolve().parent
repo=p.parents[3]
s=(p/'host_behavior.c').read_text()
s=s.replace('static int cache_count, callback_count;','static int cache_count, callback_count;\nstatic int mutation_mode;\nstatic unsigned short mutation_index;\nstatic AudioLoop mutation_loop;')
s=s.replace('    seen_count++;\n}', '''    seen_count++;
    if (mutation_mode) {
        assert(seen_count == 1);
        D_80038294 = states + 1;
        D_8003829C = 2;
        D_800382D0 = blocks + 1;
        D_800382D4 = &mutation_index;
        D_800382E4 = opaque[2];
        D_800382E8 = &mutation_loop;
        D_800382EC = 192;
        D_800382CE = 1;
    }
}''')
fixture='''
static void test_mutation(void)
{
    unsigned short original_index;
    memset(states, 0, sizeof(states));
    memset(blocks, 0, sizeof(blocks));
    phase = 3; mutation_mode = 1; mutation_index = 0; original_index = 0;
    mutation_loop.unknown00 = 0; mutation_loop.length = 384;
    D_80038294 = states; D_8003829C = 3; D_800382D0 = blocks;
    D_800382D4 = &original_index; D_800382CE = 2;
    D_800382E4 = opaque[0]; D_800382E8 = 0; D_800382EC = 0;
    D_80038030 = D_80038034 = opaque[3];
    blocks[0].commands[0].status = 123;
    blocks[1].commands[0].status = 456;
    expected_samples = 192; seen_count = 0;
    states[0].active = 1; states[0].changed = 1; states[0].bit_index = 1;
    states[1].changed = 1; states[1].bit_index = 2;
    states[2].changed = 1; states[2].bit_index = 7;
    func_800139D4(opaque[1], 192);
    assert(seen_count == 1 && states[0].changed == 0 && states[1].changed == 1 && states[2].changed == 0);
    assert(original_index == 0 && mutation_index == 1);
    assert(blocks[0].commands[0].status == 123 && blocks[0].commands[0].output == 0);
    assert(blocks[1].commands[0].status == 0 && blocks[1].commands[0].output == opaque[1]);
    assert(blocks[1].changed == 130U && blocks[1].work == opaque[2]);
    assert(blocks[1].loop == &mutation_loop && blocks[1].sample == 1 && D_800382EC == 0);
    assert(D_80038030 == opaque[3] && D_80038034 == opaque[3]);
    mutation_mode = 0;
}
'''
s=s.replace('int main(void)',fixture+'\nint main(void)')
s=s.replace('a = test_init(); b = test_setup(); c = test_submit();','a = test_init(); b = test_setup(); c = test_submit(); test_mutation();')
directory=tempfile.TemporaryDirectory(prefix='audio-final-peer-')
h=Path(directory.name)/'mutation.c';h.write_text(s);results=[]
for kind,flags in [('c89',['-O2']),('asan_ubsan',['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie'])]:
 exe=Path(directory.name)/kind
 cmd=['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-Wno-pointer-to-int-cast','-Wno-int-to-pointer-cast',*flags,*[str(p/'nonmatch'/('func_'+a+'.c')) for a in ['80011104','800114C0','800139D4']],str(h),'-o',str(exe)]
 subprocess.run(cmd,check=True,capture_output=True,text=True)
 out=subprocess.check_output([str(exe)],text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0',UBSAN_OPTIONS='halt_on_error=1'))
 results.append(dict(mode=kind,result='PASS',original_cases=6408,additional_mutation_cases=1,stdout=out.strip()))
receipt=dict(result='PASS',harness_sha256=hashlib.sha256(h.read_bytes()).hexdigest(),cases_per_mode=6409,results=results,scope='Independent synthetic callback mutation of current voice pointer/count, block pointer/index-pointer, work pointer, loop pointer/position and block-count boundary; original sources unchanged. Tests caller reload semantics only, not actual80012D18 implementation.')
print(json.dumps(receipt,indent=2))
directory.cleanup()
