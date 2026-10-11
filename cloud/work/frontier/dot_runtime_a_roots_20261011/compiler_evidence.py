#!/usr/bin/env python3
"""Stock compiler root-memory metadata with whole-object fidelity checks.

Compiler IR and listings remain temporary. No instrumentation or forced frame
is used, and these measurements cannot recover the retail compiler's IR.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]


def require(ok,detail):
    if not ok:raise ValueError(detail)


def sha(data):return hashlib.sha256(data).hexdigest()


def inspect():
    incoming_path=list(sys.path)
    try:
        return _inspect()
    finally:
        sys.path[:]=incoming_path


def _inspect():
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('roots_metadata_score',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=ROOT/'asm/us/ovl_a'
    ido=score.ido;common=['-mips2','-EB','-g0','-O3']
    with tempfile.TemporaryDirectory(prefix='roots-stock-metadata-') as td:
        work=Path(td)
        (work/'group.c').write_bytes((HERE/'group.c').read_bytes())
        (work/'keep.txt').write_text('func_80394058\n')
        steps=[
            [ido('cc'),'-j','-g0','-O3','-mips2','-G','0','-non_shared','group.c'],
            [ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
            [ido('usplit'),'-mips2','-o','split','-t','st','linked'],
            [ido('umerge'),'-Olimit','5000',*common,'split','-o','merged','-t','st'],
            [ido('uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog'],
            [ido('ugen'),'-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp','-e','trees','-l','listing'],
            [ido('as1'),'-elf','-G','0','-p0',*common,score.R4300_AS1,'-Olimit','5000','gen','-o','diagnostic.o','-t','st']]
        for command in steps:
            result=subprocess.run(command,cwd=work,capture_output=True)
            require(result.returncode==0,('stock diagnostic stage failed',Path(command[0]).name,result.stderr))
        score.compile_group(HERE,work/'canonical.o')
        canonical=(work/'canonical.o').read_bytes()
        require(canonical==(work/'diagnostic.o').read_bytes(),'diagnostic changes complete object')
        order=re.findall(r'^\s*\.ent\s+(\w+)',(work/'listing').read_text(),re.M)
        blocks=(work/'trees').read_text().split('Tree dump after Build')[1:]
        require(len(order)==len(blocks)==9,'diagnostic function enumeration')
        records={}
        for name,block in zip(order,blocks):
            block=block.split('Tree dump',1)[0]
            external=int(re.search(r'\bent .* external=(\d+)',block)[1])
            areas={kind:int(length) for kind,length in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)}
            records[name]={'external':external,'memory_areas':areas}
        require(records['func_80394058']=={'external':1,'memory_areas':{'T':16,'P':4,'M':2}},'root memory metadata drift')
        result=score.compare(work/'canonical.o','func_80394058',show=0)
        require(result.differing==2 and not any((result.unresolved,result.unverified,result.errors,result.extra_words)),'root residual drift')
        # Independent compiler-derived storage layout outside the matching TU.
        probe=work/'layout.c'
        fields=['sizeof(MenuRecord)','(u32)&((MenuRecord *)0)->fields[1]',
                '(u32)&((MenuRecord *)0)->angle','(u32)&((MenuRecord *)0)->matrix',
                '(u32)&((MenuRecord *)0)->position','(u32)&((MenuRecord *)0)->alpha',
                'sizeof(Color4)','(u32)&((Color4 *)0)->alpha']
        probe.write_text((HERE/'group.c').read_text()+'\nu32 roots_layout[] = {'+','.join(fields)+'};\n')
        score.compile_single(probe,'-g0 -O2 -mips2 -G 0 -non_shared',work/'layout.o')
        data,sections=score._elf(work/'layout.o')
        syms=[x for i,s in enumerate(sections) if s['type']==2 for x in score._symbol_table(data,sections,i)]
        symbol=next(x for x in syms if x['name']=='roots_layout')
        section=sections[symbol['section']];start=section['off']+symbol['value']
        values=[int.from_bytes(data[start+4*i:start+4*i+4],'big') for i in range(8)]
        require(values==[64,4,8,12,48,60,4,3],'native record/color layout')
    return {'status':'PASS_RESEARCH_METADATA','source_sha256':sha((HERE/'group.c').read_bytes()),
            'canonical_object_sha256':sha(canonical),'whole_object_diagnostic_identical':True,
            'compiler_sha256':{name:sha(Path(ido(name)).read_bytes()) for name in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')},
            'observations':records,'native_layout':dict(zip(fields,values)),
            'root_frame':{'native_bytes':128,'candidate_bytes':120,'saved_bytes':88,
                          'save_interval':[24,112],'byte_local_offsets':[119,118],
                          'native_unused_top_interval':[120,128]},
            'limits':['Original retail compiler IR is unavailable; no missing-local or expression-home ownership is established.',
                      'Existing expression-home allocation research is a hypothesis source, not proof of this retail frame.',
                      'Only stock UGEN diagnostic output was requested; no patched compiler, forced register, synthetic caller or padding.']}


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
    value=inspect();text=json.dumps(value,indent=2)+'\n'
    if args.output:args.output.write_text(text)
    print(text)
