#!/usr/bin/env python3
"""w21a: expand template with choice points  @{ alt0 @| alt1 @| ... @}  (alternatives may span lines)
into OUTDIR/v_<idx>.c (all combinations)."""
import itertools, re, sys
tpl = open(sys.argv[1]).read(); out = sys.argv[2]
parts = re.split(r'@\{(.*?)@\}', tpl, flags=re.S)
choices = [p.split('@|') for p in parts[1::2]]
n = 0
for combo in itertools.product(*[range(len(c)) for c in choices]):
    s = parts[0]
    for k, ci in enumerate(combo):
        s += choices[k][ci] + parts[2 * k + 2]
    open(f"{out}/v_{'_'.join(map(str, combo))}.c", 'w').write(s); n += 1
print(n, "variants")
