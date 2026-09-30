import sys, tempfile, json, shlex, re, difflib, subprocess
from pathlib import Path
ROOT = Path('/home/user/SFRush2049-decomp')
sys.path.insert(0, str(ROOT/'tools'/'cloud')); import score
GD = ROOT/'cloud/work/ipa-groups/render_large_objects'
def build(src_text, out, keep=None, flags=None):
    spec = json.loads((GD/'group.json').read_text())
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp); (work/'group.c').write_text(src_text)
        (work/'keep.txt').write_text(''.join(k+'\n' for k in (keep or spec['keep'])))
        ido = score.ido; common=['-mips2','-EB','-g0','-O3']
        steps=[[ido('cc'),'-j',*shlex.split(flags or spec['flags']),'group.c'],
         [ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
         [ido('usplit'),'-mips2','-o','split','-t','st','linked'],
         [ido('umerge'),'-Olimit','5000',*common,'split','-o','merged','-t','st'],
         [ido('uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog'],
         [ido('ugen'),'-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp'],
         [ido('as1'),'-elf','-G','0','-p0',*common,'-r4300_mul','-Olimit','5000','gen','-o',str(out),'-t','st']]
        for s in steps:
            p = score._run(s, cwd=work)
            if p.returncode: return 'ERR '+Path(s[0]).name+': '+(p.stderr or p.stdout)[:800]
    return None
def words_of(src_text, fn='func_800DE860'):
    with tempfile.TemporaryDirectory() as t:
        obj = Path(t)/'o.o'
        e = build(src_text, obj)
        if e: return None, e
        fns = score.symbols(obj); words = score.text_words(obj)
        st = fns[fn]; end = min([o for o in fns.values() if o>st] or [len(words)*4])
        res, masks, *_ = score.relocate(obj, words, st, end, score.image_symbols())
        got = res[st//4:end//4]
        gm=[g & masks.get(st+4*i,0xFFFFFFFF) for i,g in enumerate(got)]
        return gm, (None, [masks.get(st+4*i,0xFFFFFFFF) for i in range(len(score.targets()[fn]))])
def score_src(src_text, fn='func_800DE860'):
    gm, e = words_of(src_text, fn)
    if gm is None: return (-1, -1, e)
    e, wmask = e
    want = [w & m for w,m in zip(score.targets()[fn], wmask)]
    strict = sum(1 for i in range(min(len(gm),len(want))) if gm[i]==want[i]) if len(gm)==len(want) else 0
    sm = difflib.SequenceMatcher(None, want, gm, autojunk=False)
    ex = sum(b.size for b in sm.get_matching_blocks())
    return (ex, len(gm), strict)
if __name__=='__main__':
    src = Path(sys.argv[1]).read_text()
    print(score_src(src))
