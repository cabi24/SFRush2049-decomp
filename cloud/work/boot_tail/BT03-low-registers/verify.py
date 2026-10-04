#!/usr/bin/env python3
"""Rebuild the packet with pinned IDO and compare all relocated target words."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo',type=Path,default=HERE.parents[3])
    parser.add_argument('--ido-dir',type=Path,default=Path(os.environ.get('IDO_DIR','tools/cloud/ido')))
    parser.add_argument('--output',type=Path)
    args=parser.parse_args();root=args.repo.resolve()
    sys.path.insert(0,str(root))
    from tools.cloud import score
    score.IDO=args.ido_dir.resolve();score.ASM_DIR=root/'asm/us/boot_tail'
    pin=json.loads((HERE/'preflight.json').read_text())
    for name,want in pin['source_hashes'].items():assert sha(root/name)==want,name
    compiler={p.name:sha(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler==pin['compiler_files_sha256']
    manifest=subprocess.check_output(['sha256sum','-c','SHA256SUMS'],cwd=score.ASM_DIR,text=True).splitlines()
    rows=lambda x:{r['address']:r['size'] for r in x['functions']}
    inv=rows(json.loads((root/'specs/015-boot-tail-runtime/inventory.json').read_text()))
    ext=rows(json.loads((score.ASM_DIR/'extents.json').read_text()))
    assert inv==ext and len(inv)==439 and sum(inv.values())==99120
    results=[];layouts=[]
    expected={'8001558C':{'O2':(33,87,0),'O1':(83,87,14)},'80015C0C':{'O2':(23,87,0),'O1':(85,87,26)},'800163A8':{'O2':(0,74,0),'O1':(73,74,4)}}
    with tempfile.TemporaryDirectory(prefix='bt03-register-verify-') as tmp:
        tmp=Path(tmp)
        getter=root/'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter,score.DEFAULT_FLAGS,tmp/'getter.o')
        assert score.compare(tmp/'getter.o',getter.stem,show=0).accepted()
        for address in expected:
            name='func_'+address
            source=root/'cloud/matches/boot_tail'/f'{name}.c' if address=='800163A8' else HERE/f'{name}_NONMATCH.c'
            for opt in ['O2','O1']:
                flags='-g0 -'+opt+' -mips2 -G 0 -non_shared';obj=tmp/(address+opt+'.o')
                score.compile_single(source,flags,obj);result=score.compare(obj,name,show=0)
                actual=(result.differing,result.total,result.extra_words)
                assert actual==expected[address][opt],(address,opt,actual)
                assert not(result.unresolved or result.unverified or result.errors)
                assert result.accepted()==(address=='800163A8' and opt=='O2')
                results.append(dict(function=name,source_path=str(source.relative_to(root)),source_sha256=sha(source),target_bytes=inv['0x'+address],flags=flags,effective_flags=flags+' -Wab,-r4300_mul',differing_words=result.differing,total_words=result.total,extra_nonzero_words=result.extra_words,unresolved=result.unresolved,unverified=result.unverified,errors=result.errors,strict_match=result.accepted()))
            checks={'8001558C': [('sizeof(ResourceHeader)',40),('sizeof(Program)',132),('(u32)&((ResourceHeader *)0)->bank_a',28),('(u32)&((ResourceHeader *)0)->bank_b',32),('(u32)&((ResourceHeader *)0)->programs',36)],'80015C0C':[('sizeof(RecordFields)',12),('sizeof(RecordStorage)',12),('(unsigned int)&((RecordFields *)0)->id',4),('(unsigned int)&((RecordFields *)0)->references',8)],'800163A8':[('sizeof(Resource)',28),('sizeof(RegistryEntry)',12),('(u32)&((Resource *)0)->data',12),('(u32)&((Resource *)0)->size',4)]}[address]
            text=source.read_text()+'\n'
            for i,(expression,value) in enumerate(checks):text+='typedef char native_layout_%d[(%s == %d) ? 1 : -1];\n'%(i,expression,value)
            layout=tmp/(address+'-layout.c');layout.write_text(text)
            score.compile_single(layout,score.DEFAULT_FLAGS,tmp/(address+'-layout.o'))
            layouts.append(dict(function=name,result='PASS',checks=[dict(expression=e,value=v) for e,v in checks]))
    report=dict(schema_version=1,result='PASS',manifest=manifest,extent_functions=len(inv),extent_bytes=sum(inv.values()),pinned_compiler_files=len(compiler),getter_strict_match=True,verified_match_functions=1,verified_match_bytes=296,research_nonmatch_functions=2,research_nonmatch_bytes=696,results=results,native_layout=layouts)
    rendered=json.dumps(report,indent=2)+'\n'
    if args.output:args.output.write_text(rendered)
    else:print(rendered,end='')
if __name__=='__main__':main()
