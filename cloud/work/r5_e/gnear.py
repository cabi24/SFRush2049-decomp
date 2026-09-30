#!/usr/bin/env python3
"""gnear.py GROUP_DIR [--diff FN] : build group (as1 -r4300_mul) and print aligned-exact/shape per member; --diff prints opcode diff hunks."""
import sys, tempfile, difflib, subprocess, re, json, shlex, argparse
from pathlib import Path
ROOT=Path('/home/user/SFRush2049-decomp')
sys.path.insert(0,str(ROOT/'tools'/'cloud')); import score
ap=argparse.ArgumentParser(); ap.add_argument('g'); ap.add_argument('--diff',default=''); ap.add_argument('--max',type=int,default=60)
a=ap.parse_args(); g=Path(a.g); spec=json.loads((g/'group.json').read_text())
def build(out):
    with tempfile.TemporaryDirectory() as tmp:
        work=Path(tmp)
        for n in spec['files']: (work/n).write_text((g/n).read_text())
        (work/'keep.txt').write_text(''.join(k+'\n' for k in spec['keep']))
        units=[re.sub(r'\.c$','.u',f) for f in spec['files']]
        common=['-mips2','-EB','-g0','-O3']; ido=score.ido
        steps=[[ido('cc'),'-j',*shlex.split(spec['flags']),*spec['files']],
         [ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt',*units,'-ko','linked'],
         [ido('usplit'),'-mips2','-o','split','-t','st','linked'],
         [ido('umerge'),'-Olimit','5000',*common,'split','-o','merged','-t','st'],
         [ido('uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog'],
         [ido('ugen'),'-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp'],
         [ido('as1'),'-elf','-G','0','-p0',*common,'-r4300_mul','-Olimit','5000','gen','-o',str(out),'-t','st']]
        for s in steps:
            p=score._run(s,cwd=work)
            if p.returncode: sys.exit(Path(s[0]).name+' failed: '+(p.stderr or p.stdout)[:3000])
def shape(w):
    op=w>>26
    if op==0: return (0,w&0x3f)
    if op==1: return (1,(w>>16)&0x1f)
    if op==0x11: return (op,(w>>21)&0x1f, w&0x3f if (w>>21)&0x1f>=16 else 0)
    return (op,)
def dis(ws,t):
    p=Path(t)/'x.bin'; p.write_bytes(b''.join(w.to_bytes(4,'big') for w in ws))
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-mmips:4000','-EB',str(p)],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',l.split('\t',2)[-1].strip()) for l in out.splitlines() if '\t' in l and ':' in l.split('\t')[0]]
with tempfile.TemporaryDirectory() as t:
    obj=Path(t)/'o.o'; build(obj)
    fns=score.symbols(obj); words=score.text_words(obj)
    for n in spec['members']+spec.get('context',[]):
        want=score.targets().get(n)
        if want is None or n not in fns: print(n,'missing'); continue
        st=fns[n]; end=min([o for o in fns.values() if o>st] or [len(words)*4])
        res,masks,*_=score.relocate(obj,words,st,end,score.image_symbols())
        got=res[st//4:end//4]
        gm=[x&masks.get(st+4*i,0xFFFFFFFF) for i,x in enumerate(got)]
        wm=[x&masks.get(st+4*i,0xFFFFFFFF) for i,x in enumerate(want)]
        sm=difflib.SequenceMatcher(None,wm,gm,autojunk=False); ex=sum(b.size for b in sm.get_matching_blocks())
        s2=difflib.SequenceMatcher(None,[shape(x) for x in want],[shape(x) for x in got],autojunk=False); sh=sum(b.size for b in s2.get_matching_blocks())
        strict=sum(1 for i in range(min(len(gm),len(wm))) if gm[i]==wm[i])
        print(f'{n}: want {len(want)} got {len(got)} strict {strict} aligned-exact {ex} ({100*ex//len(want)}%) shape {sh} ({100*sh//len(want)}%)')
        if a.diff==n:
            A=dis(want,t); B=dis(got,t); sm=difflib.SequenceMatcher(None,A,B,autojunk=False); k=0
            for tag,i1,i2,j1,j2 in sm.get_opcodes():
                if tag!='equal':
                    print(f'@{i1}/{j1} {tag}\n  WANT {A[i1:i2]}\n  GOT  {B[j1:j2]}'); k+=1
                    if k>=a.max: break
