# func_8010FBE0 round 2: all orders of the 4 stores + memcpy, times declaration/cast forms.
import itertools, os, sys
out, srcf = sys.argv[1], sys.argv[2]
os.makedirs(out, exist_ok=True)
src = open(srcf).read()
head = src[:src.index('void func_8010FBE0')].replace('-- NOT A MATCH', '-- lane w14za variant (see RESULTS.md)')
S = {
 'a': ['{A} = 0;'],
 'b': ['D_80155288 = {B};'],
 'c': ['D_8015528C = {C};'],
 'd': ['D_80155240 = {D};'],
 'm': ['memcpy(&D_80155248, task, sizeof(OSTask));'],
}
forms = list(itertools.product(
  ['D_80155238.next', 'D_80155238_next'],
  ['&D_80152750', '(OSMesgQueue *)&D_80152750'],
  ['0', '(OSMesg)0'],
  ['2', '2u'],
))
n = 0
for perm in itertools.permutations('abcdm'):
    for fi, (a, b, c, d) in enumerate(forms):
        lines = []
        for p in perm:
            for l in S[p]:
                lines.append('    ' + l.replace('{A}', a).replace('{B}', b).replace('{C}', c).replace('{D}', d))
        lines += ['    osJamMesg(&D_8002E960, (OSMesg)&D_80155238, 1);', '    osJamMesg(&D_8002E928, (OSMesg)670, 1);']
        text = head + 'void func_8010FBE0(OSTask *task)\n{\n' + '\n'.join(lines) + '\n}\n'
        if fi == 0 and False:
            pass
        open(os.path.join(out, 'v%05d.c' % n), 'w').write(text)
        n += 1
print(n)
