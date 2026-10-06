#!/usr/bin/env python3
"""Paired C declaration experiment using frozen tools and genuine caller."""
import argparse, dataclasses, hashlib, importlib.util, json, re, subprocess, sys, tempfile
from pathlib import Path
BASE = 'f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CALLER = 'caller.c'
HERE = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--repo', type=Path, required=True)
args = parser.parse_args()
def frozen(path):
    return subprocess.check_output(['git','-C',str(args.repo),'show',BASE+':'+path])
with tempfile.TemporaryDirectory(prefix='lean-options-clock-') as tmp:
    root = Path(tmp)
    paths = ['tools/cloud/score.py','tools/cloud/owndata.py']
    for region in ['blob','blob_data']:
        manifest = 'asm/us/'+region+'/SHA256SUMS'
        paths.append(manifest)
        paths += ['asm/us/'+region+'/'+line.split()[1] for line in frozen(manifest).decode().splitlines()]
    for path in paths:
        output=root/path; output.parent.mkdir(parents=True,exist_ok=True)
        output.write_bytes(frozen(path))
    sys.path.insert(0,str(root/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('score',root/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
    rows=[]
    for name in ['ordinary','qualified_hypothesis']:
        group=root/name;group.mkdir()
        (group/'target.c').write_bytes((HERE/(name+'.c')).read_bytes())
        (group/'caller.c').write_bytes((HERE / CALLER).read_bytes())
        config=dict(files=['target.c','caller.c'],flags=FLAGS,members=['func_800DA2C0'],context=['func_800DB1E0'],keep=['func_800DB1E0'],claims=[])
        (group/'group.json').write_text(json.dumps(config))
        obj=root/(name+'.o');score.compile_group(group,obj)
        data,sections=score._elf(obj)
        symtab=next(i for i,s in enumerate(sections) if s['type']==2)
        symbols=score._symbol_table(data,sections,symtab)
        for fn in ['func_800DA2C0','func_800DB1E0']:
            symbol=next(s for s in symbols if s['name']==fn and s['type']==2)
            result=dataclasses.asdict(score.compare(obj,fn,show=0))
            result['errors']=[re.sub(r'retail [0-9a-f]+, got [0-9a-f]+','retail [redacted], got [redacted]',v) for v in result['errors']]
            words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
            frames=[0x10000-(w&65535) for w in words if w>>16==0x27bd and w&0x8000]
            rows.append(dict(mode=name,function=fn,result=result,function_bytes=symbol['size'],target_bytes=len(score.targets()[fn])*4,frame_bytes=frames))
    print(json.dumps(dict(base=BASE,flags=FLAGS,automatic_backend_flag=score.R4300_CC,caller_path=CALLER,caller_sha256=hashlib.sha256((HERE / CALLER).read_bytes()).hexdigest(),source_sha256={p:hashlib.sha256((HERE/p).read_bytes()).hexdigest() for p in ['ordinary.c','qualified_hypothesis.c']},rows=rows),indent=2))
