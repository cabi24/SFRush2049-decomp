# Generates func_8010FBE0 variants: permutations of the four init stores, memcpy placement, and type forms.
import itertools, os, sys
out = sys.argv[1]
os.makedirs(out, exist_ok=True)
src = open(sys.argv[2]).read()
head = src[:src.index('void func_8010FBE0')]
head = head.replace('-- NOT A MATCH', '-- lane w14za variant (see RESULTS.md)')
forms_task = ['OSTask *task', 'void *task']
stores = {
 'a': ['D_80155238.next = 0;'],
 'b': ['D_80155288 = &D_80152750;'],
 'c': ['D_8015528C = 0;'],
 'd': ['D_80155240 = 2;'],
}
mcpy = ['memcpy(&D_80155248, task, sizeof(OSTask));']
jams = ['osJamMesg(&D_8002E960, (OSMesg)&D_80155238, 1);', 'osJamMesg(&D_8002E928, (OSMesg)670, 1);']
n = 0
for perm in itertools.permutations('abcd'):
    for mpos in (0, 1, 2, 3, 4):
        for tf in (0, 1):
            body = []
            seq = [stores[p][0] for p in perm]
            seq.insert(mpos, mcpy[0])
            seq += jams
            for line in seq:
                body.append('    ' + line)
            sig = 'void func_8010FBE0(%s)' % forms_task[tf]
            text = head + sig + '\n{\n' + '\n'.join(body) + '\n}\n'
            open(os.path.join(out, 'v%04d.c' % n), 'w').write(text)
            n += 1
print(n)
