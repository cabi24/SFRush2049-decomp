import sys, re
sys.path.insert(0, 'tools/cloud'); import score
import rabbitizer
T = score.targets()
res = {'s_first': [], 'a_first': []}
for name, words in T.items():
    ins = [rabbitizer.Instruction(w).disassemble() for w in words]
    for i, s in enumerate(ins):
        if not s.startswith('jal '): continue
        win = ins[max(0, i-3):i+2]
        ls = [k for k, x in enumerate(win) if re.match(r'lw\s+\$s[0-7], 0x\w+\(\$sp\)', x)]
        la = [k for k, x in enumerate(win) if re.match(r'(addiu|ori|li|move|or|addu)\s+\$a0, \$zero', x) or re.match(r'(li|addiu)\s+\$a0, \$zero, ', x)]
        if ls and la:
            key = 's_first' if ls[0] < la[0] else 'a_first'
            res[key].append((name, ' ; '.join(win)))
for k, v in res.items():
    print(k, len(v))
    for x in v[:12]: print('   ', x[0], '|', x[1])
