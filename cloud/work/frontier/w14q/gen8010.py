import itertools, random, sys, os
out = sys.argv[1]; n = int(sys.argv[2]); seed = int(sys.argv[3]) if len(sys.argv) > 3 else 1
os.makedirs(out, exist_ok=True)
head = open(os.path.join(os.path.dirname(__file__), 'func_8010FBE0/b0/b_vol.c')).read().split('void func_8010FBE0')[0]
head = head.replace('/* flags: -g0 -O3 -mips2 -G 0 -non_shared */', '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */')
stmts = {
 'next': ['D_80155238.next = 0;'],
 'msgq': ['D_80155238.msgQ = &D_80152750;'],
 'msg': ['D_80155238.msg = 0;', 'D_80155238.msg = (OSMesg)0;'],
 'flags': ['D_80155238.flags = 2;'],
 'memcpy': ['memcpy({LIST}, task, {SZ});'],
}
rng = random.Random(seed)
seen = set(); files = []
space = []
for vol in ['volatile ', '']:
  for order in itertools.permutations(['next', 'msgq', 'msg', 'flags', 'memcpy']):
    for lst in ['(void *)&D_80155238.list', '&D_80155238.list']:
      for sz in ['sizeof(OSTask)', '64']:
        for mc in ['(OSMesg)&D_80155238', '&D_80155238']:
          for msgc in [0, 1]:
            space.append((vol, order, lst, sz, mc, msgc))
rng.shuffle(space)
space = [space[0]] + [s for s in space[1:]]  # keep random
# control = b_vol order
ctrl = ('volatile ', ('next', 'msgq', 'msg', 'flags', 'memcpy'), '(void *)&D_80155238.list', 'sizeof(OSTask)', '(OSMesg)&D_80155238', 0)
space = [ctrl] + [s for s in space if s != ctrl]
for i, (vol, order, lst, sz, mc, msgc) in enumerate(space[:n]):
    body = []
    for k in order:
        if k == 'memcpy':
            body.append('    memcpy(%s, task, %s);' % (lst, sz))
        else:
            s = stmts[k][msgc if k == 'msg' and msgc < 2 else 0]
            body.append('    ' + s)
    src = head.replace('extern volatile OSScTask', 'extern %sOSScTask' % vol)
    src = src + 'void func_8010FBE0(OSTask *task)\n{\n' + '\n'.join(body) + '\n    osJamMesg(&D_8002E960, %s, 1);\n    osJamMesg(&D_8002E928, (OSMesg)670, 1);\n}\n' % mc
    open(os.path.join(out, 'v%04d.c' % i), 'w').write(src)
print(min(n, len(space)), 'variants')
