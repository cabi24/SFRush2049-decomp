#!/usr/bin/env python3
"""Observe stock UOPT-to-UGEN selector metadata without compiler modifications.

All intermediate code, tree listings and assembly stay temporary. The receipt
contains only compiler/source hashes, register descriptors and fidelity checks.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
GROUP=ROOT/'cloud/work/frontier/dot_runtime_a_callers_deep_20261011'


def require(ok,message):
    if not ok:raise ValueError(message)


def sha(data):return hashlib.sha256(data).hexdigest()


def run(command,cwd):
    if '>' in command:
        index=command.index('>')
        with Path(command[index+1]).open('wb') as stream:
            result=subprocess.run(command[:index],cwd=cwd,stdout=stream,stderr=subprocess.PIPE)
    else:result=subprocess.run(command,cwd=cwd,capture_output=True)
    require(result.returncode==0,('compiler stage failed',command,result.stderr))
    return result


def summarize(path,listing):
    text=path.read_text().split('Tree dump after Build',1)[1].split('Tree dump',1)[0]
    external=int(re.search(r'\bent .* external=(\d+)',text)[1])
    descriptors=[dict(kind=int(k),length=int(n),offset=int(o))
                 for k,n,o in re.findall(r'\bregs lexlev=0 blockno=(\d+) length=(\d+) offset=(\d+)',text)]
    functions=re.findall(r'^\s*\.ent\s+(\w+)',listing.read_text(),re.M)
    require(functions[0]=='func_80392F28','selector is not first diagnostic function')
    return dict(external=external,register_descriptors=descriptors)


def inspect(group=GROUP):
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('selector_metadata_score',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=ROOT/'asm/us/ovl_a'
    source=(group/'group.c').read_text()
    ido=score.ido
    common=['-mips2','-EB','-g0','-O3']
    results={}
    with tempfile.TemporaryDirectory(prefix='selector-compiler-evidence-') as td:
        temp=Path(td)
        for name in ('standalone','genuine_group'):
            work=temp/name;work.mkdir()
            if name=='genuine_group':
                (work/'group.c').write_text(source)
                (work/'keep.txt').write_text('func_80394058\n')
                steps=[
                    [ido('cc'),'-j','-g0','-O3','-mips2','-G','0','-non_shared','group.c'],
                    [ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
                    [ido('usplit'),'-mips2','-o','split','-t','st','linked'],
                    [ido('umerge'),'-Olimit','5000',*common,'split','-o','merged','-t','st'],
                    [ido('uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog']]
                ugen=[ido('ugen'),'-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp']
                as1=[ido('as1'),'-elf','-G','0','-p0',*common,'-r4300_mul','-Olimit','5000','gen','-o','stock.o','-t','st']
                st=work/'st'
            else:
                archived=ROOT/'cloud/work/frontier/dot_runtime_a_availability_wave_20261011/selector.c'
                shutil.copy(archived,work/'selector.c')
                command=[ido('cc'),'-c','-v','-g0','-O2','-mips2','-G','0','-non_shared',score.R4300_CC,'-o',str(work/'driver.o'),str(work/'selector.c')]
                result=run(command,work)
                mapping={};commands=[]
                for line in result.stderr.decode().splitlines():
                    if not line.startswith('/usr/lib/'):continue
                    argv=shlex.split(line);argv[0]=ido(Path(argv[0]).name)
                    for i,value in enumerate(argv):
                        prefix='-XS' if value.startswith('-XS/tmp/ctm') else ''
                        path=value[len(prefix):]
                        if path.startswith('/tmp/ctm'):
                            if path not in mapping:mapping[path]=str(work/('stage'+str(len(mapping))))
                            argv[i]=prefix+mapping[path]
                    commands.append(argv)
                require([Path(x[0]).name for x in commands]==['cfe','uopt','ugen','as1'],'unexpected compiler pipeline')
                steps=commands[:2];ugen=commands[2];as1=commands[3]
                as1[as1.index('-o')+1]=str(work/'stock.o')
                st=Path(ugen[ugen.index('-t')+1])
            for step in steps:run(step,work)
            shutil.copy(st,work/'saved-st')
            run(ugen,work);run(as1,work)
            shutil.copy(work/'saved-st',st)
            run(ugen+['-e',str(work/'trees'),'-l',str(work/'listing')],work)
            as1[as1.index('-o')+1]=str(work/'diagnostic.o');run(as1,work)
            stock=(work/'stock.o').read_bytes()
            require(stock==(work/'diagnostic.o').read_bytes(),'diagnostic changed emitted object')
            if name=='standalone':require(stock==(work/'driver.o').read_bytes(),'explicit pipeline changed driver object')
            else:
                score.compile_group(group,work/'canonical.o')
                require(stock==(work/'canonical.o').read_bytes(),'explicit pipeline changed canonical object')
            result=summarize(work/'trees',work/'listing')
            comparison=score.compare(work/'stock.o','func_80392F28',show=0)
            result.update(full_object_diagnostic_identical=True,full_object_canonical_identical=True,
                          differing_words=comparison.differing,total_words=comparison.total,
                          object_sha256=sha(stock))
            results[name]=result
    require(results['standalone']['external']==1 and results['standalone']['differing_words']==2,'standalone metadata drift')
    require(results['genuine_group']['external']==0 and results['genuine_group']['differing_words']==0,'group metadata drift')
    require(results['standalone']['register_descriptors'][0]==dict(kind=1,length=6,offset=2),'standalone GP reservation drift')
    require(results['genuine_group']['register_descriptors'][:3]==[
            dict(kind=1,length=12,offset=2),dict(kind=1,length=8,offset=16),dict(kind=1,length=1,offset=30)],'group GP reservation drift')
    return dict(status='PASS',source_sha256=sha(source.encode()),
                compiler_sha256={name:sha(Path(ido(name)).read_bytes()) for name in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')},
                observations=results,
                interpretation='Authentic whole-group context changes external classification and optimizer register descriptors before UGEN; the unchanged selector becomes exact.',
                limits=['No original compiler IR exists for retail; native-side allocation metadata is not inferred.',
                        'No forced register, compiler patch, pressure variable, dummy argument, synthetic caller or changed keep policy was used.',
                        'This captures stock UOPT output and fidelity, not a complete live UGEN event trace.'])


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--group',type=Path,default=GROUP);parser.add_argument('--output',type=Path)
    args=parser.parse_args();result=inspect(args.group)
    if args.output:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
