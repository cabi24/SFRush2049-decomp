import sys, tempfile, difflib, subprocess, struct
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
src, fn = sys.argv[1], sys.argv[2]
flags = sys.argv[3] if len(sys.argv)>3 else '-g0 -O2 -mips2 -G 0 -non_shared'
want = score.targets()[fn]
with tempfile.TemporaryDirectory() as t:
    obj = Path(t)/'o.o'; score.compile_single(src, flags, obj)
    words = score.text_words(obj); fns = score.symbols(obj); start = fns[fn]
    end = min((o for o in fns.values() if o > start), default=len(words)*4)
    res, masks, *_ = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start//4:end//4]
def dis(ws):
    p=Path('/tmp/r5a.bin'); p.write_bytes(b''.join(struct.pack('>I',w) for w in ws))
    o=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4000','-EB','-M','no-aliases,reg-names=32',str(p)],capture_output=True,text=True).stdout
    return [l.split('\t',2)[2].strip() if l.count('\t')>=2 else '' for l in o.splitlines() if l.startswith(' ') and ':' in l]
wd, gd = dis(want), dis(got)
sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
bad=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    bad+=max(i2-i1,j2-j1)
    print(f'--- want[{i1}:{i2}] got[{j1}:{j2}]')
    for k in range(max(i2-i1,j2-j1)):
        a = f'{i1+k:3d} {wd[i1+k]}' if i1+k<i2 else ''
        b = f'{j1+k:3d} {gd[j1+k]}' if j1+k<j2 else ''
        print(f'   {a:40s} | {b}')
print('aligned-diff words', bad, 'got', len(got), 'want', len(want))
